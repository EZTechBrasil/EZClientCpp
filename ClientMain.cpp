//-----------------------------------------------------------------------------
//
//  Empresa     : EzTech Tecnologia e Automação Ltda
//        				 http://www.eztech.ind.br/
//
//
//	Created     : 24/08/2012
//
//  Descricao   : Classe para carga e acesso da biblioteca EZClient.dll
//
//	Parametros  :
//
//		EZClientCpp [/s addrEZServer] [/e]
//
//			/s : passa o endereco de rede do Servidor (padrao localhost)
//			/e : processa comunicacao por eventos (padrao por pooling)
//
//  Observacoes :
//
//		*. para compilacao com GCC em ambiente Windows e necessario a instalacao
//		   do pacote MinGW (http://www.mingw.org/)
//
//		*. para compilar com GCC para Windows ou Linux utilize o script Makefile
//		   utilize os seguintes comandos:
//
//				> make clean      (limpa objetos antigos)
//				> make libinstall (instala bibliotecas, somente para Linux)
//				> make linux      (plataforma LINUX 32bits)
//				> make windows    (plataforma WIN32)
//
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#if defined(_WIN32)

#include <conio.h>
#include <windows.h>

#elif defined(__linux__)

#include <unistd.h>
#include <termios.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include <sys/signal.h>

#endif

#include "EZDefinitions.h"
#include "EZClient.h"

#include "ClientMain.h"

//-----------------------------------------------------------------------------
bool runEvents   = false;					// Processando por Pooling
bool isConnected = false;					// Não conectado ao Servidor
char EZServerAddr[MAX_PATH] = "127.0.0.1";	// Endereco padrao do servidor
//char EZServerAddr[MAX_PATH] = "EZServerCE";	// Endereco padrao do servidor

int  appPump = 1;							// Bomba em uso
int  lastStatus[MAX_PATH];					// Status atual das bombas

//-----------------------------------------------------------------------------
CEZClient EZInterface;

//-----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	int ct;

	memset(lastStatus, 0, sizeof(lastStatus));

	WriteMessage("\nEZForecourt - EZClient.dll - C\\C++ Demonstracao!");

	if(argc>=2)
	{
		for(ct=1; ct<argc; ct++)
		{
			if( !stricmp(argv[ct], "/events") || !stricmp(argv[ct], "-events") || !stricmp(argv[ct], "-e") )
			{
				// Configura processamento por Eventos
				runEvents = true;
			}
			else if( !stricmp(argv[ct], "/server") || !stricmp(argv[ct], "-server") || !stricmp(argv[ct], "-s") )
			{
				ct++; //  pega o proximo parametro da lista
				if( strlen(argv[ct])>0 )
					strcpy(EZServerAddr, argv[ct]);
				else
				{
					HelpText(0);
					exit(0);
				}
			}
			else if( !stricmp(argv[ct], "/?") || !stricmp(argv[ct], "-?") || !stricmp(argv[ct], "-h") )
			{
				HelpText(0);
				exit(0);
			}
			else
			{
				WriteMessage("\n*** Parametro desconhecido: %s\n", argv[ct]);
				exit(-1);
			}
		}
	}

	MainLoop();

	return(0);
}

//-----------------------------------------------------------------------------
void MainLoop()
{
	bool ExitLoop = false;
	char CC = '?';

	if( runEvents )
		WriteMessage("\nProcessando Eventos!");

	while( !ExitLoop )
	{
		CheckLogin();

		if( !runEvents )
			PumpsStatus();
		else
			CheckEvents();

		switch( CC )
		{
			case '0': case '1': case '2':
			case '3': case '4': case '5':
			case '6': case '7': case '8':
			case '9':  // Seleciona bomba
				appPump = ((CC==0x30)?10:(CC-0x30));
				WriteMessage("\n>>> Boma %d - selecionada", appPump );
				break;

			case 'A': case 'a':   // Autoriza/Desautoza bomba
				PumpAuthorizeCancel();
				break;

			case 'P': case 'p':   // Troca precos bomba
				ChangePrice();
				break;

			case 'S': case 's':   // Preset
				PumpPreset();
				break;

			case 'T': case 't':   // Totalizadores (Encerrantes)
				ReadTotals();
				break;

			case 'U': case 'u':   // Abastecimentos
				ReadAllDeliveries();
				break;

			case 'I': case 'i':   // Le Configuracao
				ListGrades();
				ListTanks();
				ListPumps();
				ListHoses();
				break;

			case '?':		// Ajuda
				HelpText(1);
				break;

			case '\033':	// ESC
				WriteMessage("\nFinalizando!");
				ExitLoop = true;
				break;

			default:		// Comandos desconhecidos

				if( CC!=0 )
					WriteMessage("\nComando desconhecido!");

				break;
		}

		// Verifica teclado
		if( EZInterface.KBhit() )
			CC = EZInterface.KBget();
		else
			CC = 0;

		Sleep(500);
	}

	WriteMessage("\n");
}

//-----------------------------------------------------------------------------
void HelpText(int level)
{

	switch(level)
	{
		case 0:  // ajuda de linha de comando
			WriteMessage("\n\nSintexe:");
			WriteMessage("\n\t/server ou -server ou -s addrEZServer : Ajusta endereco do EZServer.");
			WriteMessage("\n\t             /events ou -events ou -e : Processamento por eventos.");
			WriteMessage("\n\t                       /? ou -? ou -h : Imprime ajuda.");
			WriteMessage("\n\n");
			break;

		case 1:  // ajuda para tempo de execucao
			WriteMessage("\nMenu:");
			WriteMessage("\n\t 0 a 9 - Seleciona bomba ( 0 = Bomba 10)");
			WriteMessage("\n\t     A - Autoriza/Desautoriza");
			WriteMessage("\n\t     P - Troca Preco");
			WriteMessage("\n\t     S - Envia Preset");
			WriteMessage("\n\t     T - Le Totalizadores (Encerrantes)");
			WriteMessage("\n\t     U - Le Abastecimentos");
			WriteMessage("\n\t     I - Lista Configuracao");
			WriteMessage("\n\t     ? - Exibe menu;");
			WriteMessage("\n\t   ESC - Sair");
			WriteMessage("\n");
			break;
	}
}

//----------------------------------------------------------------------------
// Avalia retorno das APIS EZForecourt e gera mensagem de erro
bool GoodResult( int res )
{
	if( !res )
		return true;

	WriteMessage("\n**** ERROR: %d", res);

	return false;
}

//------------------------------------------------------------------------------
// Calcula o codigo referente ao ID do bico nos concentradores CBC da Companytec.
//    HoseNumber: numero fisico do bico na bomba
//    PumpNumber: numero da bomba
//		     cid: condigo do bico HEXA em ASCII
//
void CompanyID(short HoseNumber, short PumpNumber, char *cid)
{
	long Offset;

	switch(HoseNumber)
	{
	    case  2: Offset = 0x44;
	    case  3: Offset = 0x84;
	    case  4: Offset = 0xC4;
		default: // Outros valores são tratados como Bico 1
			Offset = 0x04;
			break;
	}

	sprintf(cid, "%02X", ((PumpNumber-1)+Offset) );
}

//-----------------------------------------------------------------------------
// Converte char array para Wide-Char
static wchar_t* charToWChar(const char* text)
{
    size_t size = strlen(text) + 1;
	wchar_t* wa = new wchar_t[size];

    mbstowcs(wa,text,size);

    return wa;
}

//-----------------------------------------------------------------------------
PBSTR MakeBSTR( wchar_t* pStr , int Len )
{
	for ( int i = 0 ; i+1 < Len ; pStr[i++] = ' ' ) ;
	pStr[ Len - 1 ] = 0 ;
	return ( PBSTR ) pStr ;
}

//-----------------------------------------------------------------------------
// Executa Login
void CheckLogin()
{

	if( !GoodResult( EZInterface.TestConnection() ) )
	{

		if( GoodResult( EZInterface.ClientLogonEx(15,
												  (runEvents?7:1),
												 (BSTR)charToWChar(EZServerAddr),
												  5123, 5124, 10000, 0, 0, 0) ) )
		{
			WriteMessage("\nConectado: %s", EZServerAddr);
			isConnected = true;
		}
		else
		{
			WriteMessage("\nERRO conectando: %s", EZServerAddr);
			isConnected = false;
		}
	}
}

//----------------------------------------------------------------------------
// Le o esta das bombas usando GetAllPumpStatuses
void PumpsStatus()
{
#if defined(_WIN32)
	BSTR    PumpStates;
	BSTR    CurrentHose;
	BSTR    DeliveriesCount;
#elif defined(__linux__)
	BSTR    PumpStates[MAX_PATH];
	BSTR    CurrentHose[MAX_PATH];
	BSTR    DeliveriesCount[MAX_PATH];
#endif

	long    PumpsCount;
	int     Idx;
	int     CurStatus;
	int     CurHose;
	int     CurDelv;
	char    StrStatus[MAX_PATH];
	//char    stmp[MAX_PATH];

	// Verifica se esta conectado ao servidor
	if( isConnected )
	{
	  // Verifica se esta conectado ao servidor
	  if( !GoodResult( EZInterface.GetPumpsCount(&PumpsCount)) )
		  return;

	  // Le o estado de todas as bombas configuradas
#if defined(_WIN32)
	  if( !GoodResult( EZInterface.GetAllPumpStatuses( &PumpStates, &CurrentHose, &DeliveriesCount ) ) )
#elif defined(__linux__)
	  if( !GoodResult( EZInterface.GetAllPumpStatuses(
										  MakeBSTR(PumpStates,      sizeof(PumpStates)/sizeof(wchar_t)),
										  MakeBSTR(CurrentHose,     sizeof(CurrentHose)/sizeof(wchar_t)),
										  MakeBSTR(DeliveriesCount, sizeof(DeliveriesCount)/sizeof(wchar_t)) ) ) )
#endif
		  return;

	  for( Idx=0; Idx<PumpsCount; Idx++ )
	  {
		CurStatus = PumpStates[Idx]-'0';
		CurHose   = CurrentHose[Idx]-'0';
		CurDelv   = CurrentHose[Idx]-'0';

		switch( CurStatus ) 																											// PAM1000
		{
    		case INVALID_PUMP_STATE           : strcpy( StrStatus, "estado invalido.");											break;	// 0 - OFFLINE
    		case NOT_INSTALLED_PUMP_STATE     : strcpy( StrStatus, "nao instalada.");											break;	// 6 - CLOSE
    		case NOT_RESPONDING_1_PUMP_STATE  : strcpy( StrStatus, "Bomba nao responde.");										break;	// 0 - OFFLINE
    		case IDLE_PUMP_STATE              : strcpy( StrStatus, "em espera (desocupada).");									break;	// 1 - IDLE
    		case PRICE_CHANGE_STATE           : strcpy( StrStatus, "troca de preco.");											break;	// 1 - IDLE
    		case AUTHED_PUMP_STATE            : strcpy( StrStatus, "Bomba Autorizada");											break;	// 9 - AUTHORIZED
    		case CALLING_PUMP_STATE           : strcpy( StrStatus, "esperando autorizacao.");									break;	// 5 - CALL
    		case DELIVERY_STARTING_PUMP_STATE : strcpy( StrStatus, "abastecimeneto iniciando.");								break;	// 2 - BUSY
    		case DELIVERING_PUMP_STATE        : strcpy( StrStatus, "abastecendo.");												break;	// 2 - BUSY
    		case TEMP_STOPPED_PUMP_STATE      : strcpy( StrStatus, "parada temporaria (no meio de uma abastecimento) (STOP).");	break;	// 8 - STOP
    		case DELIVERY_FINISHING_PUMP_STATE: strcpy( StrStatus, "abastecimento finalizando (fluxo de produto diminuindo).");	break;	// 2 - BUSY
    		case DELIVERY_FINISHED_PUMP_STATE : strcpy( StrStatus, "abastecimento finalizado (parou de sair combustivel).");	break;	// 2 - BUSY
    		case DELIVERY_TIMEOUT_PUMP_STATE  : strcpy( StrStatus, "abastecimento excedeu tempo maximo.");						break;	// 1 - IDLE
    		case HOSE_OUT_PUMP_STATE          : strcpy( StrStatus, "bico fora do guarda-bico (CALL)");							break;	// 5 - CALL
    		case PREPAY_REFUND_TIMEOUT_STATE  : strcpy( StrStatus, "prazo de pre-determinacao esgotado.");						break;	// 1 - IDLE
    		case DELIVERY_TERMINATED_STATE    : strcpy( StrStatus, "abastecimento terminado (EOT)");							break;	// 3 - EOT
    		case ERROR_PUMP_STATE             : strcpy( StrStatus, "Erro (resposta de erro da bomba)");							break;	// 0 - OFFLINE
    		case NOT_RESPONDING_2_PUMP_STATE  : strcpy( StrStatus, "EZID nao responde");										break;
			case LAST_PUMP_STATE              : strcpy( StrStatus, "Ultimo estado da bomba?");									break;
			default							  : sprintf(StrStatus, "estado desconhecido = %d", CurStatus);						break;
		}

		if( lastStatus[Idx] != CurStatus )
		{
		  WriteMessage("\n Bomba %02d, Bico %02d, Pendentes %d, Status(%02d): %s", Idx+1, CurHose, CurDelv, CurStatus, StrStatus);
		  lastStatus[Idx] = CurStatus;
		}
	  }
	}
}

//----------------------------------------------------------------------------
void PumpAuthorizeCancel()
{
	int  Bomba;
	long IdBomba;

	Bomba = appPump;   // Le o numero da bomba no combo

	// Verifica conexao
	if( isConnected )
	{
		// Pega Id da Bomba escolhida
		if( !GoodResult( EZInterface.GetPumpByOrdinal(Bomba, &IdBomba) ) )
		  return;

		switch( lastStatus[Bomba-1] ) 				// PAM1000
		{
    		case AUTHED_PUMP_STATE            :		// 9 - AUTHORIZED

				// Envia bloqueio (desautorizacao) para bomba
				if( !GoodResult( EZInterface.CancelAuthorise(IdBomba) ) )
					WriteMessage("\n Bomba %02d Desautorizada!", Bomba);

				break;

			case IDLE_PUMP_STATE              :		// 1 - IDLE
    		case PRICE_CHANGE_STATE           :		// 1 - IDLE
    		case DELIVERY_TIMEOUT_PUMP_STATE  :		// 1 - IDLE
    		case PREPAY_REFUND_TIMEOUT_STATE  :		// 1 - IDLE
    		case CALLING_PUMP_STATE           :		// 5 - CALL
    		case HOSE_OUT_PUMP_STATE          :		// 5 - CALL
    		case TEMP_STOPPED_PUMP_STATE      :		// 8 - STOP

				// Envia Autorizacao para bomba
				if( !GoodResult( EZInterface.Authorise(IdBomba) ) )
					WriteMessage("\n Bomba %02d Autorizada!", Bomba);

				break;

			case DELIVERY_TERMINATED_STATE    :		// 3 - EOT
    		case DELIVERY_FINISHING_PUMP_STATE:		// 2 - BUSY
    		case DELIVERY_FINISHED_PUMP_STATE :		// 2 - BUSY
    		case DELIVERY_STARTING_PUMP_STATE :		// 2 - BUSY
    		case DELIVERING_PUMP_STATE        :		// 2 - BUSY
			case ERROR_PUMP_STATE             :		// 0 - OFFLINE
    		case NOT_RESPONDING_2_PUMP_STATE  :
			case LAST_PUMP_STATE              :
    		case INVALID_PUMP_STATE           :		// 0 - OFFLINE
    		case NOT_INSTALLED_PUMP_STATE     :		// 6 - CLOSE
    		case NOT_RESPONDING_1_PUMP_STATE  :		// 0 - OFFLINE
			default							  :
				WriteMessage("\n Bomba %02d nao pode mudar de estado!", Bomba);
				break;
		}
	}
}


//----------------------------------------------------------------------------
void ChangePrice()
{
    int    Bomba;
    int    Bico;
    long   IdBico;
    short  Duracao;
    short  Tipo;
    double Valor1;
    double Valor2;

    int    Index;
    long   Bicos;

    long   HNumber;
    long   PhysicalNumber;
    long   PumpID;
    long   PumpNumber;
    long   TankID;
    long   TankNumber;
    long   GradeID;
    long   GradeNumber;
    double MtrTheoValue;
    double MtrTheoVolume;
    double trElecValue;
    double MtrElecVolume;
    double Price1;
    double Price2;
    short  HEnabled;

#if defined(_WIN32)
    BSTR   PumpName;
    BSTR   TankName;
    BSTR   GradeName;
    BSTR   GradeShortName;
    BSTR   GradeCode;
#elif defined(__linux__)
    BSTR   PumpName[MAX_PATH];
    BSTR   TankName[MAX_PATH];
    BSTR   GradeName[MAX_PATH];
    BSTR   GradeShortName[MAX_PATH];
    BSTR   GradeCode[MAX_PATH];
#endif

    Duracao      = MULTIPLE_DURATION_TYPE; // Duracao do preco (Multipos abastecimentos)
    Tipo         = FIXED_PRICE_TYPE;  // Tipo de preco (Fixo)
    Bomba        = appPump;   // Le o numero da bomba

    // Verifica se esta conectado ao servidor
    if( !isConnected )
      return;

	WriteMessage("\n Bomba %02d - Troca de precos", Bomba);

	WriteMessage("\n\t  Bico (1 a 6): "); scanf("%d", &Bico);
	WriteMessage("\n\t Preco nivel 1:");  scanf("%lf", &Valor1);
	WriteMessage("\n\t Preco nivel 2:");  scanf("%lf", &Valor2);

    // Le o numero de bicos cadastrados
    if( !GoodResult( EZInterface.GetHosesCount(&Bicos) ) )
      return;

    for(Index=1; Index<Bicos; Index++)
	{

      // Pega o ID do bico
      if( !GoodResult( EZInterface.GetHoseByOrdinal(Index, &IdBico) ) )
        return;

#if defined(_WIN32)

      // Pega os dados do bico
      if( GoodResult( EZInterface.GetHoseSummaryEx(IdBico, &HNumber,        &PhysicalNumber,
                                                           &PumpID,         &PumpNumber,  &PumpName,
                                                           &TankID,         &TankNumber,  &TankName,
                                                           &GradeID,        &GradeNumber, &GradeName,
                                                           &GradeShortName, &GradeCode,
                                                           &MtrTheoValue,   &MtrTheoVolume,
                                                           &trElecValue,    &MtrElecVolume, &Price1,
                                                           &Price2,         &HEnabled) ) )

#elif defined(__linux__)

		// Pega os dados do bico
		if( GoodResult( EZInterface.GetHoseSummaryEx(IdBico, &HNumber,        &PhysicalNumber,
															 &PumpID,         &PumpNumber,
															 MakeBSTR(PumpName, sizeof(PumpName)/sizeof(wchar_t)),
															 &TankID,         &TankNumber,
															 MakeBSTR(TankName, sizeof(TankName)/sizeof(wchar_t)),
															 &GradeID,        &GradeNumber,
															 MakeBSTR(GradeName, sizeof(GradeName)/sizeof(wchar_t)),
															 MakeBSTR(GradeShortName, sizeof(GradeShortName)/sizeof(wchar_t)),
															 MakeBSTR(GradeCode, sizeof(GradeCode)/sizeof(wchar_t)),
															 &MtrTheoValue,   &MtrTheoVolume,
															 &trElecValue,    &MtrElecVolume, &Price1,
															 &Price2,         &HEnabled) ) )
#endif
      {

        // Verifica se o ID co bico pertence o escolhido
        if( (Bomba==PumpNumber) && (Bico==HNumber) )
		{

			WriteMessage("\n\t\t Precos Atual: Bomba %02d Bico %02d Preco1 R$%.03f  Preco2 R$%.03f",
																PumpNumber, HNumber, Price1, Price2);

            break;
		}
	  }
	}

    // Faz ajuste do preço na bomba
    if( GoodResult( EZInterface.SetHosePrices( IdBico, Duracao, Tipo, Valor1, Valor2) ) )
	{
		WriteMessage("\n\tPreco Novo: Bomba %02d Bico %02d Preco1 R$%.03f Preco2 R$%.03f (Duraticao %d Tipo %d)",
																		Bomba, Bico, Valor1, Valor2, Duracao, Tipo );
	}
}

//----------------------------------------------------------------------------
void PumpPreset()
{
    int    Bomba;
    int    Bico;
    long   IdBomba;
    long   IdBico;
    short  LType;
    double PsValue;

    Bomba        = appPump;   // Le o numero da bomba

	WriteMessage("\n Bomba %02d - Preset", Bomba);

	WriteMessage("\n\t               Bico (1 a 6): "); scanf("%d",  &Bico);
	WriteMessage("\n\t Tipo (1=Dinheiro 2=Volume): "); scanf("%d",  &LType);
	WriteMessage("\n\t                      Valor: "); scanf("%lf", &PsValue);

	LType = (LType==1?DOLLAR_PRESET_TYPE:VOLUME_PRESET_TYPE);

    // Verifica conexao
    if( !isConnected )
      return;

    // Pega Id da Bomba escolhida
    if( !GoodResult( EZInterface.GetPumpByOrdinal(Bomba, &IdBomba) ) )
      return;

    IdBico = (1<<(Bico-1));  // Calcula ID do bico escolhido

    // Envia preset para bomba
    if( GoodResult( EZInterface.LoadPreset(IdBomba, LType, PsValue, ( short ) IdBico, 1) ) )
	{
		WriteMessage("\n\t Preset Enviado: Bomba %02d Bico %02d Tipo %d Valor %.03f Nivel 1",
							Bomba, Bico, LType, PsValue);
	}
}

//----------------------------------------------------------------------------
void ReadAllDeliveries()
{
    int  Idx;
    long Ct;
    long Id;

    long   HoseID;
    short  State;
    short  DType;
    double Volume;
    short  PriceLevel;
    double Price;
    double Value;
    double Volume2;
    DATE   CompletedDT;
    long   LockedBy;
    long   ReservedBy;
    long   AttendantID;
    long   Age;
    DATE   ClearedDT;
    double OldVolumeETot;
    double OldVolume2ETot;
    double OldValueETot;
    double NewVolumeETot;
    double NewVolume2ETot;
    double NewValueETot;
    INT64  Tag;
    long   Duration;
    long   ClientID;
	double PeakFlowRate;

	//char   cbcID[MAX_PATH];

    // Verifica conexao
    if( !isConnected )
      return;

    //--------------------------------------------------------------------
    // Le o numero de produtos configurados
    if( !GoodResult( EZInterface.GetDeliveriesCount( &Ct ) ) )
      return;

	WriteMessage("\n[Abastecimentos %03d]---------------------------------------------------", Ct);

    for(Idx=Ct; Idx>0; Idx--)
	{

       if( !GoodResult( EZInterface.GetDeliveryByOrdinal( Idx, &Id ) ) )
          return;

 /*
       if( GoodResult( EZInterface.GetDeliveryPropertiesEx2(Id, &HoseID, &State, &DType,
                                                                &Volume, &PriceLevel, &Price,
                                                                &Value, &Volume2, &CompletedDT,
                                                                &LockedBy, &ReservedBy, &AttendantID,
                                                                &Age, &ClearedDT, &OldVolumeETot,
                                                                &OldVolume2ETot, &OldValueETot,
                                                                &NewVolumeETot, &NewVolume2ETot,
                                                                &NewValueETot, &Tag, &Duration ) ) )
*/
       if( GoodResult( EZInterface.GetDeliveryPropertiesEx4(Id, &HoseID, &State, &DType,
                                                                &Volume, &PriceLevel, &Price,
                                                                &Value, &Volume2, &CompletedDT,
                                                                &LockedBy, &ReservedBy, &AttendantID,
                                                                &Age, &ClearedDT, &OldVolumeETot,
                                                                &OldVolume2ETot, &OldValueETot,
                                                                &NewVolumeETot, &NewVolume2ETot,
                                                                &NewValueETot, &Tag, &Duration, &ClientID, &PeakFlowRate ) ) )
	   {
          WriteMessage( "\n------ Abastecimento: (%03d) %d",    Idx, Id);
          WriteMessage( "\n           HoseID %d  State %d  Type %d",
										HoseID, State, DType);

          WriteMessage( "\n           Volume %.03f  PriceLevel %d  Price %.03f  Value %.03f",
										Volume, PriceLevel, Price, Value);

		  WriteMessage( "\n           Volume2 %.03f  CompleteDT %d  LockedBy %d  ReservedBy %d",
										Volume2, CompletedDT, LockedBy, ReservedBy);

		  WriteMessage( "\n           AttendantID %d  Age %d  ClearedDT %d",
										AttendantID, Age, ClearedDT);

          WriteMessage( "\n           OldVolumeETot %.03f  OldVolume2ETot %.03f  OldvalueETot %.03f",
										OldVolumeETot, OldVolume2ETot, OldValueETot);

		  WriteMessage( "\n           NewVolumeETot %.03f  NewVolume2ETot %.03f  NewValueETot %.03f",
										NewVolumeETot, NewVolume2ETot, NewValueETot);

		  WriteMessage( "\n           Tag %d  Duraction %d   ClientID %d   PeakFlowRate %.03f",
										Tag, Duration, ClientID, PeakFlowRate );

		  WriteMessage( "\n");

          if( LockedBy != -1 )
            continue;

          if( GoodResult( EZInterface.LockDelivery( Id ) ) )
			LockedBy = 1;
          else
            continue;

          if( (LockedBy==1) && ( State!=CLEARED ) )
            GoodResult( EZInterface.ClearDelivery( Id , DType ) ) ;

	   }
	}

	WriteMessage("\n------------------------------------------------------------------------");

}

//----------------------------------------------------------------------------
void ReadTotals()
{
    long   IdBomba;
    long   IdBico;

    long   Bomba;
    long   Bico;

    long   Number;
    long   PumpID;
    long   TankID;
    long   PhysicalNumber;
    double MtrTheoValue;
    double MtrTheoVolume;
    double MtrElecValue;
    double MtrElecVolume;
    short  UVEAntenna;
    double Price1;
    double Price2;
    short  Enabled;

    Bomba = appPump;   // Le o numero da bomba
    //Bico         := cbHose.ItemIndex+1;   // Le o numero da bico no combo

    // Verifica conexao
    if( !isConnected )
      return;

    // Pega Id da Bomba escolhida
    if( !GoodResult( EZInterface.GetPumpByOrdinal(Bomba, &IdBomba) ) )
        return;

	WriteMessage("\n Encerrantes:  Bomba %02d", Bomba);

	for(Bico=1; Bico<7; Bico++)
	{
		// Le Id do Bico (sem GoodResult() para evitar mensagens no final)
		if( EZInterface.GetPumpHoseByNumber(IdBomba, Bico, &IdBico)!=0 )
		  return;

		// Le dados do Bico
		if( GoodResult( EZInterface.GetHosePropertiesEx2(IdBico, &Number, &PumpID, &TankID,
																 &PhysicalNumber, &MtrTheoValue,
																 &MtrTheoVolume, &MtrElecValue,
																 &MtrElecVolume, &UVEAntenna,
																 &Price1, &Price2, &Enabled ) ) )
		{
			WriteMessage("\n    Bico %02d  EncVolume %.02f  EncDInheiro %.02f  Preco1 %.03f  Preco2 %.03f",
								Bico, MtrElecVolume, MtrElecValue, Price1, Price2 );

			WriteMessage("\n    [ Number %02d  PumpId %03d  TankID %03d  PhisicalNumber %02d ]",
								Number, PumpID, TankID, PhysicalNumber);

			WriteMessage("\n    [ MtrTheoValue %.02f  MtrTheoVolume %.02f  UVAntenna %02d  Enabled %02d ]",
								MtrTheoValue, MtrTheoVolume, UVEAntenna, Enabled);

			WriteMessage("\n");
		}
	}
}

//----------------------------------------------------------------------------
// Lista configuracao de produtos
void ListGrades()
{
    long Idx;
    long Ct;
    long Id;
    long Number;
	short Type;

#if defined(_WIN32)
    BSTR Name;
    BSTR ShortName;
    BSTR Code;
#elif defined(__linux__)
    BSTR Name[MAX_PATH];
    BSTR ShortName[MAX_PATH];
    BSTR Code[MAX_PATH];
#endif

    // Verifica conexao
    if( !isConnected )
      return;

    //--------------------------------------------------------------------
    // Le o numero de produtos configurados
    if( !GoodResult( EZInterface.GetGradesCount( &Ct ) ) )
      return;

    WriteMessage( "\n[Produtos %d]---------------------------------------------------", Ct);

    for(Idx=0; Idx<Ct+1; Idx++ )
	{

       if( EZInterface.GetGradeByOrdinal( Idx+1, &Id )!=0 )
          return;

#if defined(_WIN32)
       if( GoodResult( EZInterface.GetGradePropertiesEx(Id, &Number, &Name, &ShortName, &Code, &Type) ) )
#elif defined(__linux__)
       if( GoodResult( EZInterface.GetGradeProperties(Id, &Number,
    		                                          MakeBSTR(Name,      sizeof(Name)/sizeof(wchar_t)),
    		                                          MakeBSTR(ShortName, sizeof(ShortName)/sizeof(wchar_t)),
    		                                          MakeBSTR(Code,      sizeof(Code)/sizeof(wchar_t)) )))
#endif
													  WriteMessage( "\n  Grade: %d  Nome: %S  Abreviado: %S  Codigo: %S  Type: %d",
							Number, Name, ShortName, Code, Type);
	}

	WriteMessage( "\n");
}

//----------------------------------------------------------------------------
void ListTanks()
{
    long   Idx;
    long   Ct;
    long   Id;
    long   Number;
    long   GradeID;
    short  TType;
    double Capacity;
    double Diameter;
    double TheoVolume;
    double GaugeVolume;
    double GaugeTCVolume;
    double GaugeUllage;
    double GaugeTemperature;
    double GaugeLevel;
    double GaugeWaterVolume;
    double GaugeWaterLevel;
    long   GaugeID;
    short  ProbeNo;
	long   GaugeAlarmsMask;

#if defined(_WIN32)
    BSTR   Name;
#elif defined(__linux__)
    BSTR   Name[MAX_PATH];
#endif

    // Verifica conexao
    if( !isConnected)
      return;

    //--------------------------------------------------------------------
    // Le o numero de produtos configurados
    if( !GoodResult( EZInterface.GetTanksCount( &Ct ) ) )
      return;

	WriteMessage("\n[Tanques %d]---------------------------------------------------", Ct);

    for(Idx=0; Idx<Ct; Idx++)
	{
       if( EZInterface.GetTankByOrdinal( Idx, &Id )!=0 )
          return;

#if defined(_WIN32)
       if( GoodResult( EZInterface.GetTankPropertiesEx(Id, &Number, &Name, &GradeID,
                                                         &TType, &Capacity, &Diameter,
                                                         &TheoVolume, &GaugeVolume,
                                                         &GaugeTCVolume, &GaugeUllage,
                                                         &GaugeTemperature, &GaugeLevel,
                                                         &GaugeWaterVolume, &GaugeWaterLevel,
                                                         &GaugeID, &ProbeNo, &GaugeAlarmsMask ) ) )
#elif defined(__linux__)
	   if( GoodResult( EZInterface.GetTankProperties(Id, &Number,
														 MakeBSTR(Name, sizeof(Name)/sizeof(wchar_t)),
														 &GradeID,
														 &TType, &Capacity, &Diameter,
														 &TheoVolume, &GaugeVolume,
														 &GaugeTCVolume, &GaugeUllage,
														 &GaugeTemperature, &GaugeLevel,
														 &GaugeWaterVolume, &GaugeWaterLevel,
														 &GaugeID, &ProbeNo ) ) )
#endif


	   {
          WriteMessage("\n  Tanque: %02d  Nome: %S  Produto: %02d  Tipo: %02d",
								Number, Name, GradeID, TType);

          WriteMessage("\n     Capacidade: %.02f  Diametro: %.02f",
								Capacity, Diameter );

          WriteMessage("\n     TheoVolume: %.02f  GaugeVolume: %.02f  GaugeTCVolume: %.02f",
												TheoVolume, GaugeVolume, GaugeTCVolume);

          WriteMessage("\n     GaugeUllage: %.02f  GaugeTemperature:  %.02f  GaugeLevel: %.02f",
												GaugeUllage, GaugeTemperature, GaugeLevel );

		  WriteMessage("\n     GaugeWaterVolume: %.02f  GaugeWaterLevel: %.02f  GaugeID: %02d  ProbeNo: %02d  GaugeAlarmsMask: %02d",
								GaugeWaterVolume, GaugeWaterLevel, GaugeID, ProbeNo, GaugeAlarmsMask);
	   }

	}

    WriteMessage("\n");
}

//----------------------------------------------------------------------------
void ListPumps()
{
    long  Idx;
    long  Ct;
    long  Id;
    long  Number;
    short PhysicalNumber;
    short Side;
    short Address;
    short PriceLevel1;
    short PriceLevel2;
    short PriceDspFormat;
    short VolumeDspFormat;
    short ValueDspFormat;
    short PType;
    long  PortID;
    long  AttendantID;
    short AuthMode;
    short StackMode;
    short PrepayAllowed;
    short PreauthAllowed;
    long  SlotZigBeeID;
    long  MuxSlotZigBeeID;
    short PriceControl;
    short HasPreset;

#if defined(_WIN32)
    BSTR   Name;
#elif defined(__linux__)
    BSTR   Name[MAX_PATH];
#endif

	// Verifica conexao
    if( !isConnected )
      return;

    //--------------------------------------------------------------------
    // Le o numero de produtos configurados
    if( !GoodResult( EZInterface.GetPumpsCount( &Ct ) ) )
      return;

	WriteMessage( "\n[Bombas %d]---------------------------------------------------", Ct);

    for(Idx=1; Idx<Ct++; Idx++)
	{

       if( EZInterface.GetPumpByOrdinal( Idx, &Id )!=0 )
          return;

#if defined(_WIN32)
       if( GoodResult( EZInterface.GetPumpPropertiesEx(Id, &Number,          &Name,           &PhysicalNumber,
                                                           &Side,            &Address,        &PriceLevel1,
                                                           &PriceLevel2,     &PriceDspFormat, &VolumeDspFormat,
														   &ValueDspFormat,  &PType,          &PortID,
														   &AttendantID,     &AuthMode,       &StackMode,
														   &PrepayAllowed,   &PreauthAllowed, &SlotZigBeeID,
                                                           &MuxSlotZigBeeID, &PriceControl,   &HasPreset) ) )
#elif defined(__linux__)
	   if( GoodResult( EZInterface.GetPumpPropertiesEx(Id, &Number,
														   MakeBSTR(Name, sizeof(Name)/sizeof(wchar_t)),
														   &PhysicalNumber,
														   &Side,            &Address,        &PriceLevel1,
														   &PriceLevel2,     &PriceDspFormat, &VolumeDspFormat,
														   &ValueDspFormat,  &PType,          &PortID,
														   &AttendantID,     &AuthMode,       &StackMode,
														   &PrepayAllowed,   &PreauthAllowed, &SlotZigBeeID,
														   &MuxSlotZigBeeID, &PriceControl,   &HasPreset) ) )
#endif
	   {
          WriteMessage("\n  Bomba: %02d  Nome: %S  PhicalNumber: %d  Side: %d  Address: %d",
								Number, Name, PhysicalNumber, Side, Address);

          WriteMessage("\n     PriceLevel1: %d  PriceLevel2: %d  PriceDspFormat: %d",
						        PriceLevel1, PriceLevel2, PriceDspFormat);

          WriteMessage("\n     PTipe: %d  PortID: %d  AttendantID: %d  AutoMode: %d  StackMode: %d",
 							    PType, PortID, AttendantID, AuthMode, StackMode);

          WriteMessage("\n     PrepayAllwed: %d  PreauthAllowed: %d  SlotZigBeeID: %d  MuxSlotZigBeeID: %d",
							    PrepayAllowed, PreauthAllowed, SlotZigBeeID, MuxSlotZigBeeID);

          WriteMessage("\n     VolumeDspFormat: %d  ValueDspFormat: %d  PriceControl: %d  HasPreset: %d",
						        VolumeDspFormat, ValueDspFormat, PriceControl, HasPreset);
	   }
	}

	WriteMessage("\n");
}

//----------------------------------------------------------------------------
void ListHoses()
{
    long   Idx;
    long   Ct;
    long   Id;
    long   Number;
    long   PumpID;
    long   TankID;
    long   PhysicalNumber;
    double MtrTheoValue;
    double MtrTheoVolume;
    double MtrElecValue;
    double MtrElecVolume;
    short  UVEAntenna;
    double Price1;
    double Price2;
    short  Enabled;

    // Verifica conexao
    if( !isConnected )
      return;

    //--------------------------------------------------------------------
    // Le o numero de produtos configurados
    if( !GoodResult( EZInterface.GetHosesCount( &Ct ) ) )
      return;

    WriteMessage("\n[Bicos %d]---------------------------------------------------", Ct);

    for(Idx=1; Idx<Ct+1; Idx++)
	{

       if( EZInterface.GetHoseByOrdinal( Idx, &Id )!=0 )
          return;

       if( GoodResult( EZInterface.GetHosePropertiesEx2( Id, &Number, &PumpID, &TankID,
                                                        &PhysicalNumber, &MtrTheoValue,
                                                        &MtrTheoVolume, &MtrElecValue,
                                                        &MtrElecVolume, &UVEAntenna,
                                                        &Price1, &Price2,
                                                        &Enabled ) ) )
	   {
          WriteMessage( "\n    Bico: %02d  PumpID: %d  TankID: %d  PhisicalNumber: %d",
										Number, PumpID, TankID, PhysicalNumber);

          WriteMessage( "\n        MtrTheoValue: %.03f  MtrTheoVolume: %.03f",
										MtrTheoValue, MtrTheoVolume);

          WriteMessage( "\n        MtrElecValue: %.03f  MtrElecVolume: %.03f",
										MtrElecValue, MtrElecVolume);

          WriteMessage( "\n        UVEAntena: %d  Price1: %.03f  Price2: %.03f  Enables: %d",
										UVEAntenna, Price1, Price2, Enabled);
	   }
	}
}

//-----------------------------------------------------------------------------
void CheckEvents()
{
    long  EvtCt;
    short EvtType;

    // Verifica se esta conectado ao servidor
    if( !isConnected )
      return;

    // Inicia processamento de eventos
    if( !GoodResult( EZInterface.ProcessEvents() ) )
      return;

    // Le numero de eventos disponiveis
    if( !GoodResult( EZInterface.GetEventsCount( &EvtCt ) ) )
      return;

	while (true)
    {
		// Le o proximo evento
		if( !GoodResult( EZInterface.GetNextEventType( &EvtType ) ) )
		  return;

		switch( EvtType )
		{
			//---------------------------------------------------------------------
			case PUMP_EVENT :   // Trata Eventos das Bombas
				EventPump();
				break;

			//---------------------------------------------------------------------
			case DELIVERY_EVENT : // Eventos de abastecimento
				EventDelivery();
				break;

			//---------------------------------------------------------------------
			case CARD_READ_EVENT:  // Eventos de leitores de cartoes
				EventCardRead();
				break;

			//---------------------------------------------------------------------
			case DB_LOG_ETOTALS:   // Evento de mudanca de encerrantes
				EventDbLogETotals();
				break;

			//---------------------------------------------------------------------
			case SERVER_EVENT:  // Eventos do servidor
				EventServer();
				break;

			//---------------------------------------------------------------------
			case CLIENT_EVENT:  // Eventos de POS (client)
				EventClient();
				break;

			//---------------------------------------------------------------------
			case ZB2G_STATUS_EVENT: // Eventos de Zigbee
				EventZB2G();
				break ;

			//---------------------------------------------------------------------
			case LOG_EVENT_EVENT: // Log eventos
				EventLog();
				break;

			//---------------------------------------------------------------------
			case DB_TANK_STATUS: // eventos de Tanque
				EventTank();
				break;

			//---------------------------------------------------------------------
			case NO_CLIENT_EVENT :  // Trata Eventos do Cliente
				return;

			default:
				GoodResult( EZInterface.DiscardNextEvent() ) ;
				break;
		}
	}
}

//-----------------------------------------------------------------------------
void EventPump()
{
    long   PumpID;
    long  PumpNumber;
    short  State;
    short  ReservedFor;
    long   ReservedBy;
    long   HoseID;
    long   HoseNumber;
    long   HosePhysicalNumber;
    long   GradeID;
    long   GradeNumber;
    short  PriceLevel;
    double Price;
    double Volume;
    double Value;
    short  StackSize;
    long   PhysicalNumber;
    short  Side;
    short  Address;
    short  PriceLevel1;
    short  PriceLevel2;
    short  PumpType;
    long   PortID;
    short  AuthMode;
    short  StackMode;
    short  PrepayAllowed;
    short  PreauthAllowed;
    short  PriceFormat;
    short  ValueFormat;
    short  VolumeFormat;
    INT64  Tag;
    long   AttendantID;
    long   AttendantNumber;
    INT64  AttendantTag;
    long   CardClientID;
    long   CardClientNumber;
    INT64  CardClientTag;
	double CurFlowRate;
	double PeakFlowRate;

	char cbcID[MAX_PATH];

#if defined(_WIN32)
    BSTR   GradeName;
    BSTR   ShortGradeName;
    BSTR   PumpName;
    BSTR   AttendantName;
    BSTR   CardClientName;
#elif defined(__linux__)
    BSTR   GradeName[MAX_PATH];
    BSTR   ShortGradeName[MAX_PATH];
    BSTR   PumpName[MAX_PATH];
    BSTR   AttendantName[MAX_PATH];
    BSTR   CardClientName[MAX_PATH];
#endif

    // Verifica se esta conectado ao servidor
    if( !isConnected )
      return;

#if defined(_WIN32)
	if( GoodResult( EZInterface.GetNextPumpEventEx3(&PumpID,           &PumpNumber,         &State,
													&ReservedFor,      &ReservedBy,         &HoseID,
													&HoseNumber,       &HosePhysicalNumber,
													&GradeID,          &GradeNumber,        &GradeName,
													&ShortGradeName,   &PriceLevel,         &Price,
													&Volume,           &Value,              &StackSize,
													&PumpName,         &PhysicalNumber,     &Side,
													&Address,          &PriceLevel1,        &PriceLevel2,
													&PumpType,         &PortID,             &AuthMode,
													&StackMode,        &PrepayAllowed,      &PreauthAllowed,
													&PriceFormat,      &ValueFormat,        &VolumeFormat,
													&Tag,              &AttendantID,        &AttendantNumber,
													&AttendantName,    &AttendantTag,       &CardClientID,
													&CardClientNumber, &CardClientName,		&CardClientTag, 
													&CurFlowRate,	   &PeakFlowRate ) ) )
#elif defined(__linux__)
		if( GoodResult( EZInterface.GetNextPumpEventEx2(&PumpID,           &PumpNumber,         &State,
														&ReservedFor,      &ReservedBy,         &HoseID,
														&HoseNumber,       &HosePhysicalNumber,
														&GradeID,          &GradeNumber,
														MakeBSTR(GradeName, sizeof(GradeName)/sizeof(wchar_t)),
														MakeBSTR(ShortGradeName, sizeof(ShortGradeName)/sizeof(wchar_t)),
														&PriceLevel,         &Price,
														&Volume,           &Value,              &StackSize,
														MakeBSTR(PumpName, sizeof(PumpName)/sizeof(wchar_t)),
														&PhysicalNumber,     &Side,
														&Address,          &PriceLevel1,        &PriceLevel2,
														&PumpType,         &PortID,             &AuthMode,
														&StackMode,        &PrepayAllowed,      &PreauthAllowed,
														&PriceFormat,      &ValueFormat,        &VolumeFormat,
														&Tag,              &AttendantID,        &AttendantNumber,
														MakeBSTR(AttendantName, sizeof(AttendantName)/sizeof(wchar_t)),
														&AttendantTag,       &CardClientID,
														&CardClientNumber,
														MakeBSTR(CardClientName, sizeof(CardClientName)/sizeof(wchar_t)),
														&CardClientTag) ) )
#endif
	{

		WriteMessage("\n------ PumpEvent:  PumpID %d  PumpNumber %d  State %d  ReservedFor %d",
									PumpID, PumpNumber, State, ReservedFor);

		WriteMessage("\n           ReservedBy %d  HoseID %d  HoseNumber %d  HosePhisicalNumber %d  GradeID %d",
									ReservedBy, HoseID, HoseNumber, HosePhysicalNumber, GradeID);

		WriteMessage("\n           GradeName %S  GradeNumber %d  ShortGradeName %S  PriceLevel %d",
									GradeName, GradeNumber, ShortGradeName, PriceLevel);

		WriteMessage("\n           Price %.03f  Volume %.03f  Value %.03f  StackSize %d",
									Price, Volume, Value, StackSize);

		WriteMessage("\n           PumpName %S  PhysicalNumber %d  Side %d Address %d  PriceLevel1 %d  PriceLevel2 %d",
									PumpName, PhysicalNumber, Side, Address, PriceLevel1, PriceLevel2);

		WriteMessage("\n           PumpType %d  PortID %d  AuthMode %d  StackMode %d  PrepayAllowed %d",
									PumpType, PortID, AuthMode, StackMode, PrepayAllowed);

		WriteMessage("\n           PreauthAllowed %d  PriceFormat %d  ValueFormat %d  VolumeFormat %d  Tag %d",
								    PreauthAllowed, PriceFormat, ValueFormat, VolumeFormat, Tag);

		WriteMessage("\n           AttendantID %d  AttendantNumber %d  AttendantName %S  AttendantTag %d",
									AttendantID, AttendantNumber, AttendantName, AttendantTag);

		WriteMessage("\n           CardClientID= %d  CardClientNumber %d  CardClientName %S  CardClientTag %d",
									CardClientID, CardClientNumber, CardClientName, CardClientTag);

		WriteMessage("\n           CurFlowRate= %.03f  PeakFlowRate= %.03f",
									CurFlowRate, PeakFlowRate);

		CompanyID(( short )HoseNumber, ( short ) PumpNumber, cbcID);
		WriteMessage("\n           Bico Equivalente CBC: %s", cbcID);
	}
}

//-----------------------------------------------------------------------------
void EventDelivery()
{
	long   DeliveryID;
    long   HosePhysicalNumber;
    long   TankID;
    long   TankNumber;
    short  DeliveryState;
    short  DeliveryType;
    double Volume2;
    DATE   CompletedDT;
    long   LockedBy;
    long   Age;
    DATE   ClearedDT;
    double OldVolumeETot;
    double OldVolume2ETot;
    double OldValueETot;
    double NewVolumeETot;
    double NewVolume2ETot;
    double NewValueETot;
    long   Duration;

    long   PumpID;
    long   PumpNumber;
    long   HoseID;
    long   HoseNumber;
    // short  HosePhisicalNumber;
    long   GradeID;
    long   GradeNumber;

	short  PriceLevel;
    double Price;
    double Volume;
    double Value;
	long   ReservedBy;

	INT64  Tag;
    long   AttendantID;
    long   AttendantNumber;
    INT64  AttendantTag;
    long   CardClientID;
    long   CardClientNumber;
    INT64  CardClientTag;
	double PeakFlowRate;

	char cbcID[MAX_PATH];

#if defined(_WIN32)
    BSTR   TankName;
    BSTR   GradeShortName;
    BSTR   GradeCode;
	BSTR   PumpName;
    BSTR   GradeName;
    BSTR   AttendantName;
    BSTR   CardClientName;
#elif defined(__linux__)
	BSTR   PumpName[MAX_PATH];
    BSTR   TankName[MAX_PATH];
    BSTR   GradeName[MAX_PATH];
    BSTR   GradeCode[MAX_PATH];
    BSTR   GradeShortName[MAX_PATH];
    BSTR   AttendantName[MAX_PATH];
    BSTR   CardClientName[MAX_PATH];
#endif

    // Verifica se esta conectado ao servidor
    if( !isConnected )
      return;

#if defined(_WIN32)
	if( GoodResult( EZInterface.GetNextDeliveryEventEx4(&DeliveryID,         &HoseID,           &HoseNumber,
													    &HosePhysicalNumber, &PumpID,           &PumpNumber,
													    &PumpName,           &TankID,           &TankNumber,    &TankName,
													    &GradeID,            &GradeNumber,      &GradeName,     &GradeShortName,
													    &GradeCode,          &DeliveryState,    &DeliveryType,  &Volume,
													    &PriceLevel,         &Price,            &Value,         &Volume2, &CompletedDT,
													    &LockedBy,           &ReservedBy,       &AttendantID,   &Age,     &ClearedDT,
													    &OldVolumeETot,      &OldVolume2ETot,   &OldValueETot,
													    &NewVolumeETot,      &NewVolume2ETot,   &NewValueETot,  &Tag,
													    &Duration,           &AttendantNumber,  &AttendantName, &AttendantTag,
													    &CardClientID,       &CardClientNumber, &CardClientName,
													    &CardClientTag,		 &PeakFlowRate) ) )
#elif defined(__linux__)
	if( GoodResult( EZInterface.GetNextDeliveryEventEx3(&DeliveryID,         &HoseID,           &HoseNumber,
														&HosePhysicalNumber, &PumpID,           &PumpNumber,
														MakeBSTR(PumpName, sizeof(PumpName)/sizeof(wchar_t)),
														&TankID,           &TankNumber,
														MakeBSTR(TankName, sizeof(TankName)/sizeof(wchar_t)),
														&GradeID,            &GradeNumber,
														MakeBSTR(GradeName, sizeof(GradeName)/sizeof(wchar_t)),
														MakeBSTR(GradeShortName, sizeof(GradeShortName)/sizeof(wchar_t)),
														MakeBSTR(GradeCode, sizeof(GradeCode)/sizeof(wchar_t)),
														&DeliveryState,      &DeliveryType,     &Volume,
														&PriceLevel,         &Price,            &Value,         &Volume2, &CompletedDT,
														&LockedBy,           &ReservedBy,       &AttendantID,   &Age,     &ClearedDT,
														&OldVolumeETot,      &OldVolume2ETot,   &OldValueETot,
														&NewVolumeETot,      &NewVolume2ETot,   &NewValueETot,  &Tag,
														&Duration,           &AttendantNumber,
														MakeBSTR(AttendantName, sizeof(AttendantName)/sizeof(wchar_t)),
														&AttendantTag,
														&CardClientID,       &CardClientNumber,
														MakeBSTR(CardClientName, sizeof(CardClientName)/sizeof(wchar_t)),
														&CardClientTag) ) )
#endif
	{

		// Primeiro abastecimento pode ser invalido
		if( DeliveryID>0 )
		{
			WriteMessage("\n------ DeliveryEvent:  DeliveryID %d  HoseID %d  HoseNumber %d  HosePhysicalNumber %d",
										DeliveryID, HoseID, HoseNumber, HosePhysicalNumber);

			WriteMessage("\n           PumpID %d  PumpNumber %d  PumpName %S  TankID %d",
										PumpID, PumpNumber,  PumpName, TankID);

			WriteMessage("\n           TankNumber %d  TankName %S  GradeID %d  GradeNumber %d",
										TankNumber, TankName, GradeID, GradeNumber);

			WriteMessage("\n           GradeName %S  GradeShortName %S  GradeCode %S  DeliveryState %d",
										GradeName, GradeShortName, GradeCode, DeliveryState);

			WriteMessage("\n           DeliveryType %d  Volume %.03f  PriceLevel %d  Price %.03f",
										DeliveryType, Volume, PriceLevel, Price);

			WriteMessage("\n           Value %.03f  Volume2 %.30f  CompletedDT %d  LockedBy %d",
										Value, Volume2, CompletedDT, LockedBy);

			WriteMessage("\n           ReservedBy %d  AttendantID %d  Age %d  ClearedDT %d",
										ReservedBy, AttendantID, Age, ClearedDT);

			WriteMessage("\n           OldVolumeETot %.03f  OldVolume2ETot %.03f  OldValueETot %.03f  NewVolumeETot %.03f",
										OldVolumeETot, OldVolume2ETot, OldValueETot, NewVolumeETot);

			WriteMessage("\n           NewVolume2ETot %.03f  NewValueETot %.03f  Tag %d  Duration %d",
										NewVolume2ETot, NewValueETot, Tag, Duration);

			WriteMessage("\n           AttendantNumber %d  AttendantName %S  AttendantTag %d  CardClientID %d",
										AttendantNumber, AttendantName, AttendantTag, CardClientID);

			WriteMessage("\n           CardClientNumber %d  CardClientName %S  CardClientTag %d PeakFlowRate %.03f",
										CardClientNumber, CardClientName, CardClientTag, PeakFlowRate);

			CompanyID(( short )HoseNumber, ( short )PumpNumber, cbcID);
			WriteMessage("\n           Bico Equivalente CBC: %s", cbcID);

			if( LockedBy==-1 )
			{
				if( GoodResult( EZInterface.LockDelivery( DeliveryID ) ) )
					LockedBy=1;

				if( (LockedBy==1) && (DeliveryState!=CLEARED) )
				  GoodResult( EZInterface.ClearDelivery( DeliveryID , DeliveryType ) ) ;

			}
		}
	}
}

//-----------------------------------------------------------------------------
void EventCardRead()
{
    long  CardReadID;
    long  Number;
    short CardType;
    long  ParentID;
    DATE  TimeStamp;
	INT64 Tag;
	long  PumpID;

#if defined(_WIN32)
	BSTR  Name;
#elif defined(__linux__)
	BSTR  Name[MAX_PATH];
#endif

    // Verifica se esta conectado ao servidor
    if( !isConnected )
      return;

#if defined(_WIN32)
	if( GoodResult( EZInterface.GetNextCardReadEvent(&CardReadID, &Number, &Name,
												     &PumpID,     &CardType, &ParentID,
												     &Tag,        &TimeStamp) ) )
#elif defined(__linux__)
	if( GoodResult( EZInterface.GetNextCardReadEvent(&CardReadID, &Number,
													 MakeBSTR(Name, sizeof(Name)/sizeof(wchar_t)),
													 &PumpID,     &CardType, &ParentID,
													 &Tag,        &TimeStamp) ) )
#endif
	{

		WriteMessage("\n------ CardReadEvent:  CardReadID %d  Number %d  Name %S  PumpID %d",
									CardReadID, Number, Name, PumpID);

		WriteMessage("\n           CardType %d  ParentID %d  Tag %d  TimeStamp %d",
							        CardType, ParentID, Tag, TimeStamp );

		switch(CardType )
		{
		  case ATTENDANT_TAG_TYPE:
			WriteMessage("\n           Attendant: %S  Tag %d", Name, Tag);
		    break;

		  case BLOCKED_ATTENDANT_TAG_TYPE:
			WriteMessage("\n           Blocked attendant: %S  Tag %d", Name, Tag);
		    break;

		  case WRONG_SHIFT_ATTENDANT_TAG_TYPE:
			WriteMessage("\n           Wrong shift attendant: %S  Tag %d", Name, Tag);
			break;

		  case CLIENT_TAG_TYPE:
		  	WriteMessage("\n           Client: %S  Tag %d", Name, Tag);
			break;

		  case BLOCKED_CLIENT_TAG_TYPE:
			WriteMessage("\n           Blocked Client: %S  Tag %d", Name, Tag);
		    break;

		  case UNKNOWN_TAG_TYPE:
			WriteMessage("\n           Unknown Tag read: %d", Tag);
		    break;

		  default:
			WriteMessage("\n           Unknown Tag type: %d  Tag %d", CardType, Tag);
			break;
		}

		GoodResult( EZInterface.DeleteCardRead( CardReadID )) ;
	}
}

//-----------------------------------------------------------------------------
void EventDbLogETotals()
{
	long   HoseID;
	double Volume;
    double Value;
    double VolumeETot;
    double ValueETot;
    long   HoseNumber;
    long   HosePhysicalNumber;
    long   PumpID;
    long   PumpNumber;
	long   TankID;
    long   TankNumber;
    long   GradeID;

#if defined(_WIN32)
    BSTR   PumpName;
    BSTR   TankName;
    BSTR   GradeName;
#elif defined(__linux__)
    BSTR   PumpName[MAX_PATH];
    BSTR   TankName[MAX_PATH];
    BSTR   GradeName[MAX_PATH];
#endif

    // Verifica se esta conectado ao servidor
    if( !isConnected )
      return;

#if defined(_WIN32)
	if( GoodResult( EZInterface.GetNextDBHoseETotalsEventEx(&HoseID,     &Volume,     &Value,
   													        &VolumeETot, &ValueETot,
													        &HoseNumber, &HosePhysicalNumber,
													        &PumpID,     &PumpNumber, &PumpName,
													        &TankID,     &TankNumber, &TankName,
													        &GradeID,    &GradeName) ) )
#elif defined(__linux__)
	if( GoodResult( EZInterface.GetNextDBHoseETotalsEventEx(&HoseID,     &Volume,     &Value,
															&VolumeETot, &ValueETot,
															&HoseNumber, &HosePhysicalNumber,
															&PumpID,     &PumpNumber,
															MakeBSTR(PumpName, sizeof(PumpName)/sizeof(wchar_t)),
															&TankID,     &TankNumber,
															MakeBSTR(TankName, sizeof(TankName)/sizeof(wchar_t)),
															&GradeID,
															MakeBSTR(TankName, sizeof(TankName)/sizeof(wchar_t))) ))
#endif
	{
		WriteMessage("\n------ HoseETotalEvent:  HoseID %d  Volume %.03f  Value %.03f",
											HoseID, Volume, Value);

		WriteMessage("\n             VolumeETot %.03f  ValueETot %.03f",
							                VolumeETot, ValueETot);

		WriteMessage("\n             HoseNumber %d  HosePhysicalNumber %d",
							                HoseNumber, HosePhysicalNumber);

		WriteMessage("\n             PumpID %d  PumpNumber %d  PumpName %S",
											PumpID, PumpNumber, PumpName);

		WriteMessage("\n             TankID %d  TankNumber %d TankName %S",
											TankID, TankNumber, TankName);

		WriteMessage("\n             GradeID %d  GradeName %S",
											GradeID, GradeName);

	}
}

//-----------------------------------------------------------------------------
void EventServer()
{
	long EventID;

#if defined(_WIN32)
	BSTR EventText;
#elif defined(__linux__)
	BSTR EventText[MAX_PATH];
#endif

    // Verifica se esta conectado ao servidor
    if( !isConnected )
      return;

#if defined(_WIN32)
	if( GoodResult( EZInterface.GetNextServerEvent(&EventID, &EventText) ) )
#elif defined(__linux__)
	if( GoodResult( EZInterface.GetNextServerEvent(&EventID, MakeBSTR(EventText, sizeof(EventText)/sizeof(wchar_t))) ))
#endif
		WriteMessage("\n------ ServerEvent:   EventID %d  EventText %S",
										EventID, EventText);

}

//-----------------------------------------------------------------------------
void EventClient()
{
	long EventID;
	long ClientID;

#if defined(_WIN32)
	BSTR EventText;
#elif defined(__linux__)
	BSTR EventText[MAX_PATH];
#endif

#if defined(_WIN32)
	if( GoodResult( EZInterface.GetNextClientEvent(&ClientID, &EventID, &EventText) ) )
#elif defined(__linux__)
	if( GoodResult( EZInterface.GetNextClientEvent(&ClientID, &EventID, MakeBSTR(EventText, sizeof(EventText)/sizeof(wchar_t))) ))
#endif
		WriteMessage("\n------ ClientEvent: ClientID %d  EventID %d  EventText %S",
								 ClientID, EventID, EventText);

}

//-----------------------------------------------------------------------------

void EventZB2G()
{
	long PortID = 0; 
    INT64 ZBAddress = 0; 
    short LQI = 0;
    short RSSI = 0;
    INT64 ParZBAddress = 0; 
    short ZBChannel = 0;
    short MemBlocks = 0;
    short MemFree = 0;

#if defined(_WIN32)
	if( GoodResult( EZInterface.GetNextZB2GStatusEvent( &PortID,     &ZBAddress,     &LQI,
												        &RSSI,		 &ParZBAddress,	 &ZBChannel,
												        &MemBlocks,  &MemFree ) ) )
#elif defined(__linux__)
	BSTR EventText[MAX_PATH];

#endif
		WriteMessage("\n------ ZigBeeEvent: PortID %d  ZBAddress %d  LQI %d  RSSI %d",
								 PortID, ZBAddress, LQI, RSSI);
		WriteMessage("\n              ParZBAddress %d  ZBChannel %d  MemBlocks %d  MemFree %d",
								 ParZBAddress, ZBChannel, MemBlocks, MemFree);

}

//-----------------------------------------------------------------------------

void EventLog()
{
	long LogEventID = 0;
    short DeviceType = 0;
    long DeviceID = 0;
	long DeviceNumber = 0;
	BSTR DeviceName;
    short EventLevel = 0;
	short EventType = 0;
	BSTR EventDesc;
	DATE GeneratedDT ;
	DATE ClearedDT ;
	long ClearedBy = 0;
	long AckedBy = 0;
	double Volume = 0;
    double Value = 0;
	double ProductVolume = 0;
	double ProductLevel = 0;
	double WaterLevel = 0;
	double Temperature = 0;

#if defined(_WIN32)
	if( GoodResult( EZInterface.GetNextLogEventEvent(	&LogEventID,	&DeviceType,	&DeviceID,
												        &DeviceNumber,	&DeviceName,	&EventLevel,
												        &EventType,     &EventDesc,		
														&GeneratedDT,	&ClearedDT,		&ClearedBy,
														&AckedBy,		&Volume,		&Value,
														&ProductVolume,	&ProductLevel,	&WaterLevel,
														&Temperature ) ) )
#elif defined(__linux__)
	BSTR EventText[MAX_PATH];

#endif
		WriteMessage("\n------ LogEvent: LogEventID %d  DeviceType %d  DeviceID %d  ",
										 LogEventID, DeviceType, DeviceID );
		WriteMessage("\n				 DeviceNumber %d  DeviceName %S EventLevel %d  ",
										 DeviceNumber, DeviceName, EventLevel );
		WriteMessage("\n				 EventType %d  EventDesc %S ",
										 EventType, EventDesc );
		WriteMessage("\n				 GeneratedDT %d  ClearedDT %d  ClearedBy %d  ",
										 GeneratedDT, ClearedDT, ClearedBy );
		WriteMessage("\n				 AckedBy %d  Volume %0.3f  Value %0.3f  ",
										 AckedBy, Volume, Value );
		WriteMessage("\n				 ProductVolume %0.3f  ProductLevel %0.3f  WaterLevel %0.3f  Temperature %0.3f ",
										 ProductVolume, ProductLevel, WaterLevel, Temperature );

}

//-----------------------------------------------------------------------------

void EventTank()
{
	long TankID = 0;
    double GaugeVolume = 0;
    double GaugeTCVolume = 0;
    double GaugeUllage = 0;
    double GaugeTemperature = 0;
    double GaugeLevel = 0;
    double GaugeWaterVolume = 0;
    double GaugeWaterLevel = 0;
    long TankNumber = 0;	
    BSTR TankName;
    long GradeID = 0;
    BSTR GradeName;	
    short Type = 0;	
    double Capacity = 0;	
    double Diameter = 0;	
    long GaugeID = 0;	
    short ProbeNo = 0;	
    short State = 0;	
    long AlarmsMask = 0;

#if defined(_WIN32)
	// BSTR EventText;
#elif defined(__linux__)
	BSTR EventText[MAX_PATH];
#endif

    // Verifica se esta conectado ao servidor
    if( !isConnected )
      return;

#if defined(_WIN32)
	if( GoodResult( EZInterface.GetNextDBTankStatusEventEx2(	&TankID,			&GaugeVolume,		&GaugeTCVolume,
																&GaugeUllage,		&GaugeTemperature,	&GaugeLevel,
																&GaugeWaterVolume,	&GaugeWaterLevel,	&TankNumber,
																&TankName,			&GradeID,			&GradeName,
																&Type,				&Capacity,			&Diameter,
																&GaugeID,			&ProbeNo,			&State,
																&AlarmsMask		) ) )
#elif defined(__linux__)
	if( GoodResult( EZInterface.GetNextClientEvent(&ClientID, &EventID, MakeBSTR(EventText, sizeof(EventText)/sizeof(wchar_t))) ))
#endif
		WriteMessage("\n------ TankEvent:	TankID %d  GaugeVolume %0.3f  GaugeTCVolume %0.3f",
											TankID, GaugeVolume, GaugeTCVolume );
		WriteMessage("\n					GaugeUllage %0.3f  GaugeTemperature %0.3f  GaugeLevel %0.3f",
											GaugeUllage, GaugeTemperature, GaugeLevel);
		WriteMessage("\n					GaugeWaterVolume %0.3f  GaugeWaterLevel %0.3f  TankNumber %d",
											GaugeWaterVolume, GaugeWaterLevel, TankNumber);
		WriteMessage("\n					TankName %S  GradeID %d  GradeName %S",
											TankName, GradeID, GradeName);
		WriteMessage("\n					Type %d  Capacity %0.3f  Diameter %0.3f",
											Type, Capacity, Diameter);
		WriteMessage("\n					GaugeID %d  ProbeNo %d  State %d",
											GaugeID, ProbeNo, State);
		WriteMessage("\n					AlarmsMask %d ",
											AlarmsMask );

}

//-----------------------------------------------------------------------------