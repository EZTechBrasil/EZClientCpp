//-----------------------------------------------------------------------------
//
//  Empresa     :  EzTech Tecnologia e Automação Ltda
//        				 http://www.eztech.ind.br/
//
//
//	Created     : 24/08/2012
//
//  Descricao   : Classe para carga e acesso da biblioteca EZClient.dll
//
//  Observacoes :
//
//-----------------------------------------------------------------------------
#ifndef __EZ_CLIENT_MAIN__
#define __EZ_CLIENT_MAIN__

//-----------------------------------------------------------------------------
#define WriteMessage printf

//-----------------------------------------------------------------------------
extern bool runEvents;
extern bool isConnected;
extern char EZServerAddr[MAX_PATH];

extern int  appPump;
extern int  lastStatus[MAX_PATH];

//-----------------------------------------------------------------------------
void MainLoop();
void HelpText(int level);

void CompanyID(short HoseNumber, short PumpNumber, char *cid);
static wchar_t* charToWChar(const char* text);
PBSTR MakeBSTR( wchar_t* pStr , int Len );

void CheckLogin();
void PumpsStatus();
void PumpAuthorizeCancel();
void ChangePrice();
void PumpPreset();
void ReadAllDeliveries();
void ReadTotals();

void ListGrades();
void ListTanks();
void ListPumps();
void ListHoses();

void CheckEvents();

void EventPump();
void EventDelivery();
void EventCardRead();
void EventDbLogETotals();
void EventServer();
void EventClient();
void EventZB2G();
void EventLog();
void EventTank();

//-----------------------------------------------------------------------------
#endif // __EZ_CLIENT_MAIN__

