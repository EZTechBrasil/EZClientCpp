//-----------------------------------------------------------------------------
//
//  Company     : EZTech Technology and Automation Ltda
//        				 http://www.eztech.ind.br/
//
//
//	Created     : 08/2012
//  Updated		: 07/2017
//
//
//  Description   : Class for library loading and access EZClient.dll
//
//	Parameters  :
//
//		EZClientCpp [/s addrEZServer] [/e]
//
//			/s : Passes the server's network address (default localhost)
//			/e : Processes communication by events (default by pooling)
//
//  Comments :
//
//		*. To compile with GCC in Windows environment and the installation is necessary
//			of the MinGW package (http://www.mingw.org/)
//
//		*. To compile with GCC for Windows or Linux use the Makefile script
//			use the following commands:
//
//				> make clean      (Clean old objects)
//				> make libinstall (Install libraries, Linux only)
//				> make linux      (Linux 32bits platform)
//				> make windows    (WIN32 platform)
//
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#if defined(_WIN32)

#include <conio.h>
#include <windows.h>
#include <time.h>
#include <atlcomtime.h>

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
#include "Version.h"

//-----------------------------------------------------------------------------

bool	isConnected = false ;						// Not connected to Server

// License Modules Enabled
bool	isEZServerModule = false ;
bool	isEZHOModule = false ;
bool	isZigBeeModule = false ;
bool	isEZMonitorModule = false ;
bool	isTankManagementModule = false ;
bool	isDevelopmentModule = false ;
bool	isXPertModule = false ;

char	EZServerAddr[MAX_PATH] = "192.168.1.111" ;	// Server address
BSTR	AttEZServerAddr ;
char	versionCompatible[MAX_PATH] = "2,3,0,1" ;	// Version of EZServer compatible with this program
int		currentStatus[MAX_PATH] ;					// Current Pump Status

//-----------------------------------------------------------------------------

CEZClient EZInterface ;							// Class with all declarations needed to run the API

//-----------------------------------------------------------------------------

int main( int argc, char *argv[] )
{
	CreateAcess() ;

	MainLoop() ;

	return(0) ;
}

//-----------------------------------------------------------------------------

void CreateAcess()
{
	WriteMessage("\n Enter the EZServer access IP number: ") ;
	fflush(stdin) ;
	scanf ("%s", EZServerAddr ) ;

	AttEZServerAddr = CharToBstr( EZServerAddr ) ;
}

//-----------------------------------------------------------------------------

void MainLoop()
{
	bool shutdown = false ;
	char chosenType = '?' ;

	while( ! shutdown )
	{
		CheckLogin() ;

		if ( isConnected )
		{			
			chosenType = ShowMenu(1) ;

			CheckLogin();
			if( ! isConnected )
				continue ;

			switch( chosenType )
			{
				case 'A': case 'a':
					BasicGoodPractice() ;
					break ;

				case 'B': case 'b':
					DeliveriesReading() ;
					break ;

				case 'C': case 'c':
					PumpBasicControl() ;
					break ;

				case 'D': case 'd':
					PumpControlExtended() ;
					break ;

				case 'E': case 'e':
					PumpPriceControl() ;
					break ;

				case 'F': case 'f':
					ReadingCards() ;
					break ;

				case 'G': case 'g':
					RegisterAttendant() ;
					break ;

				case 'H': case 'h': 
					RegisterClient() ;
					break ;

				case 'I': case 'i': 
					TanksSensorsReading() ;
					break ;

				case 'J': case 'j': 
					GeneralConfigurationFacility() ;
					break ;

				case '\033':	// ESC
					WriteMessage( "\n Shutdown!" ) ;
					shutdown = true ;
					break ;

				default:		// Unknown Commands
					WriteMessage("\nUnknown command!") ;
					break ;
			}

		}

		Sleep( TIME_RECONECT ) ; //Auto-connect to EZTech after XXX milliseconds ( Basic Required )
	}

	WriteMessage( "\n" ) ;
}

//-----------------------------------------------------------------------------
char ShowMenu( int menuLevel )
{
	char chosenType;

	switch( menuLevel )
	{
		case MAIN_MENU:
			WriteMessage( "\nEZForecourt - EZClient.dll - C\\C++ Demo!         Version: %s", VERSION ) ;
			WriteMessage( "\n" ) ;
			WriteMessage( "\n\t	A - Basic Good Practice") ;
			WriteMessage( "\n\t	B - Deliveries Reading") ;
			WriteMessage( "\n\t	C - Pump Basic Control") ;
			WriteMessage( "\n\t	D - Pump Control Extended") ;
			WriteMessage( "\n\t	E - Pump Price Control" ) ;
			WriteMessage( "\n\t	F - Reading Cards" ) ;
			WriteMessage( "\n\t	G - Register Attendant") ;
			WriteMessage( "\n\t	H - Register Client") ;
			WriteMessage( "\n\t	I - Tanks and Sensors Reading") ;
			WriteMessage( "\n\t	J - General Configuration Facility") ;
			break ;

		case BASIC_GOOD_PRATICE_MENU: 
			WriteMessage( "\n\n Which functionality do you want to test?") ;
			WriteMessage( "\n\t	A - Date / Time Update") ;
			WriteMessage( "\n\t	B - Version Compatibility") ;
			WriteMessage( "\n\t	C - License Verification") ;
			WriteMessage( "\n\t	D - Disconnection with EZTech") ;
			break ;

		case DELIVERY_READING_MENU: 
			WriteMessage( "\n\n Which functionality do you want to test?") ;
			WriteMessage( "\n\t	A - Reading of Pending Deliveries") ;
			break ;

		case PUMP_BASIC_CONTROL_MENU:  
			WriteMessage( "\n\n Which functionality do you want to test?") ;
			WriteMessage( "\n\t	A - Temporary Stop") ;
			WriteMessage( "\n\t	B - Reauthorize Delivery") ;
			WriteMessage( "\n\t	C - Disable Hose") ;
			WriteMessage( "\n\t	D - Activation Hose") ;
			WriteMessage( "\n\t	E - Pumps Status") ;
			WriteMessage( "\n\t F - Authorize Delivery" ) ;
			WriteMessage( "\n\t G - Cancel Authorize" ) ;			
			break ;

		case PUMP_CONTROL_EXTENDED_MENU: 
			WriteMessage( "\n\n Which functionality do you want to test?") ;
			WriteMessage( "\n\t	A - Scheduled Delivery") ;
			WriteMessage( "\n\t	B - Preset Delivery") ;			
			break ;

		case PUMP_PRICE_CONTROL_MENU:
			WriteMessage( "\n\n Which functionality do you want to test?") ;
			WriteMessage( "\n\t	A - Price Change per Hose") ;
			WriteMessage( "\n\t	B - Price Change per Grade") ;	
			break ;

		case REGISTER_ATTENDANT_MENU: 
			WriteMessage( "\n\n Which functionality do you want to test?") ;
			WriteMessage( "\n\t	A - Attendant Registration") ;
			WriteMessage( "\n\t	B - Delete Attendant") ;	
			break ;

		case REGISTER_CLIENT_MENU: 
			WriteMessage( "\n\n Which functionality do you want to test?") ;
			WriteMessage( "\n\t	A - Client Registration") ;
			WriteMessage( "\n\t	B - Delete Client") ;	
			break ;

		case TANKS_SENSORS_READING_MENU: 
			WriteMessage( "\n\n Which functionality do you want to test?") ;
			WriteMessage( "\n\t	A - Tank Reading") ;
			WriteMessage( "\n\t	B - Monitoring Alarm") ;	
			break ;

		case GENERAL_CONFIGURATION_FACILITY_MENU: 
			WriteMessage( "\n\n Which functionality do you want to test?") ;
			WriteMessage( "\n\t	A - Delivery Position Register") ;
			WriteMessage( "\n\t	B - Delete of Delivery Position") ;	
			WriteMessage( "\n\t	C - Hose Registration") ;
			WriteMessage( "\n\t	D - Delete Hose") ;
			WriteMessage( "\n\t	E - Grade Register") ;
			WriteMessage( "\n\t	F - Delete Grade") ;
			WriteMessage( "\n\t	G - Tank Registration") ;
			WriteMessage( "\n\t	H - Delete Tank") ;
			WriteMessage( "\n\t	I - Zigbee Device Registration") ;
			WriteMessage( "\n\t	J - Delete Zigbee Device") ;
			break ;
	}

	WriteMessage( "\n\nSelect Option Test: ") ;

	fflush(stdin);
	scanf ("%c", &chosenType ) ;
	system("cls") ;	

	return chosenType;
}

void EndMessage() 
{
	WriteMessage("\n\n");
	system("PAUSE");
	system("cls") ;
}

//----------------------------------------------------------------------------
// Evaluates return of APIS EZForecourt and generates error message
bool GoodResult( int result )
{
	if( result == OK_RESULT )	
		return true ;

	BSTR resultString = EZInterface.ResultString(result);

	WriteMessage( "\nError: %d %S", result, resultString ) ;

	FREE_SYS_STR( resultString ) ;

	if( ( result < OK_RESULT && result != INVALID_OBJECT_ID ) || result == NOT_LOGGED_ON_RESULT )
	{
		EZInterface.ClientLogoff();
		isConnected = false ;
	}
	return false ;
}

//-----------------------------------------------------------------------------
void CheckLogin()
{
	if( GoodResult( EZInterface.ClientLogonEx(	LOGIN_ID , 
												CLIENT_TYPE_CALL ,
												AttEZServerAddr ,
												CALL_PORT , 
												EVENT_PORT , 
												CALL_TIMEOUT , 
												EVENT_HANDLE , 
												HANDLE_WINDOWS , 
												WINDOWS_MSG ) ) )
	{		
		system( "cls" );
		WriteMessage( "\nConnected to EZServer" ) ;
		WriteMessage( "\n" ) ;
		isConnected = true ;
	}
	else
	{
		WriteMessage( "\nERROR to connect EZServer..." ) ;		
		isConnected = false;
	}
}

//------------------------ Basic Good Practice ---------------------------------
void BasicGoodPractice()
{
	char chosenType ;
		
	chosenType = ShowMenu( BASIC_GOOD_PRATICE_MENU ) ;

	CheckLogin();
	if( ! isConnected )
		return ;

	switch ( chosenType )
	{
		case 'A': case 'a':
			SetDateTime() ;
			break ;
		case 'B': case 'b':
			CheckServerVersion() ;
			break ;
		case 'C': case 'c':
			CheckLicenseStatus() ;
			break ;
		case 'D': case 'd':
			ClientLogoff() ;
			break ;
		default:
			WriteMessage("\n Unknown command!") ;
	}
}

// Changes the date/time of the hub to the date/time of the computer to which the management system is running
void SetDateTime()
{
	SYSTEMTIME st ;
	GetLocalTime( & st ) ;
	COleDateTime eztime( st ) ;

	if( st.wHour <= 24 &&
		st.wMinute <= 60 &&
		st.wSecond <= 60 &&
		GoodResult( EZInterface.SetDateTime( eztime ) ) )
	{
		WriteMessage( "\nDateTime EZServer Updated: %04d-%02d-%02d %02d:%02d:%02d", st.wYear, 
																 st.wMonth, 
																 st.wDay, 
																 st.wHour, 
																 st.wMinute, 
																 st.wSecond ) ;
	}

	EndMessage() ;
}

// Checks if the EZTech version is compatible with the deployed version
void CheckServerVersion()
{

#if defined( _WIN32 )
    BSTR   ezserverVersion = NULL ;
#elif defined( __linux__ )
    BSTR   ezserverVersion[MAX_PATH] ;
#endif

#if defined( _WIN32 )
	if( ! GoodResult( EZInterface.ServerVersion( &ezserverVersion ) ) ) 
		return ;
#elif defined( __linux__ )
	if( ! GoodResult( EZInterface.ServerVersion( MakeBSTR( ezserverVersion, sizeof( ezserverVersion ) / sizeof( wchar_t ) ) ) ) ) 
		return ;
#endif

	int stringLength = 0 ;
	int index = 0 ;
	
	stringLength = ( int ) SysStringLen( ezserverVersion ) ;
	char versionPart[MAX_PATH] = "" ;

	for ( int i = stringLength - 7 ; i < stringLength ; i++ )
	{
		versionPart[ index ] = ezserverVersion[ i ] ;
		index++ ;
	}

	WriteMessage("\nEZServer Version: %S ", ezserverVersion ) ;

	if ( ( strcmp ( versionPart , versionCompatible ) >= 0 ) )
	{		
		WriteMessage("\n\nVersion Compatible " ) ;
	}
	else 
	{
		WriteMessage("\nIncompatible Version " ) ;
	}

	FREE_SYS_STR( ezserverVersion ) ;

	EndMessage() ;
}

// Checks the license modules enabled in EZTech
void CheckLicenseStatus()
{
	short licenseType = 0 ;

	if( GoodResult( EZInterface.GetLicenseType( &licenseType ) ) )
	{
		WriteMessage("\nActive License Modules: " ) ;

		if ( licenseType & EZSERVER_LICENSE_MASK )
		{
			WriteMessage("\nEZSERVER ENABLED " ) ;
			isEZServerModule = true ;
		}

		if ( licenseType & EZHO_LICENSE_MASK )
		{
			WriteMessage("\nEZHO ENABLED " ) ;
			isEZHOModule = true ;
		}

		if ( ( licenseType & ZIGBEE_LICENSE_MASK ) == 0 )
		{
			WriteMessage("\nZIGBEE ENABLED " ) ;
			isZigBeeModule = true ;
		}

		if ( ( licenseType & EZMONITOR_LICENSE_MASK ) == 0 )
		{
			WriteMessage("\nEZMONITOR ENABLED " ) ;
			isEZMonitorModule = true ;
		}

		if ( licenseType & TANK_MANAGEMENT_LICENSE_MASK )
		{
			WriteMessage("\nTANK MANAGEMENT ENABLED " ) ;
			isTankManagementModule = true ;
		}

		if ( licenseType & DEVELOPMENT_LICENSE_MASK )
		{
			WriteMessage("\nDEVELOPMENT ENABLED " ) ;
			isDevelopmentModule = true ;
		}

		if ( licenseType & XPERT_LICENSE_MASK )
		{		
			WriteMessage("\nXPERT ENABLED " ) ;
			isXPertModule = true ;
		}
	}

	EndMessage() ;
}

void ClientLogoff()
{
	if( GoodResult( EZInterface.ClientLogoff() ) )
	{
		WriteMessage("\nThanks for using EZClientCpp " ) ;
		Sleep( 10000 ) ;
		exit( -1 ) ;
	}
}

//------------------------ Deliveries Reading ---------------------------------
void DeliveriesReading()
{
	char chosenType ;
	
	chosenType = ShowMenu( DELIVERY_READING_MENU ) ;

	CheckLogin();
	if( ! isConnected )
		return ;

	switch ( chosenType )
	{
		case 'A': case 'a':
			ReadAllDeliveries() ;
			break ;
		default:
			WriteMessage("\n Unknown command!") ;
	}
}

// Read all pending delivery data ( Tirar dúvida com Kelvin, ClearDate )
void ReadAllDeliveries()
{
    int		idx ;
    long	deliveryCount ;
    long	deliveryID ;
    long	hoseID ;
    short	deliveryState ;
    short	deliveryType ;
    double	volume ;
    short	priceLevel ;
    double	price ;
    double	value ;
    double	volume2 ;
    DATE	completedDate ;
    long	lockedBy ;
    long	reservedBy ;
    long	attendantID ;
    long	age ;
    DATE	clearedDate ;
    double	oldVolumeETot ;
    double	oldVolume2ETot ;
    double	oldvalueETot ;
    double	newVolumeETot ;
    double	newVolume2ETot ;
    double	newvalueETot ;
    INT64	tag ;
    long	duration ;
    long	clientID ;
	double	peakFlowRate ;

    COleDateTime *oleCompletedDate = NULL ;
	COleDateTime *oleClearedDate = NULL ;

    // Read the number of configured products
    if( !GoodResult( EZInterface.GetDeliveriesCount( &deliveryCount ) ) )
      return ;

	WriteMessage( "\n [ Count Deliveries %03d ] --------------------------------------------------- ", deliveryCount ) ;

    for( idx = deliveryCount ; idx > 0 ; idx-- )
	{

       if( !GoodResult( EZInterface.GetDeliveryByOrdinal( idx, &deliveryID ) ) )
          return;

       if( GoodResult( EZInterface.GetDeliveryPropertiesEx4(	deliveryID, &hoseID, &deliveryState, &deliveryType,
                                                                &volume, &priceLevel, &price,
                                                                &value, &volume2, &completedDate,
                                                                &lockedBy, &reservedBy, &attendantID,
                                                                &age, &clearedDate, &oldVolumeETot,
                                                                &oldVolume2ETot, &oldvalueETot,
                                                                &newVolumeETot, &newVolume2ETot,
                                                                &newvalueETot, &tag, &duration, &clientID, &peakFlowRate ) ) )
	   {


		  oleCompletedDate = new COleDateTime( completedDate ) ;
		  oleClearedDate = new COleDateTime( clearedDate ) ;

          WriteMessage( "\n------ Delivery: (%03d) %d",    idx, deliveryID ) ;

		  WriteMessage( "\nHose ID: %d  Delivery State: %d  Type: %d",
										hoseID, deliveryState, deliveryType ) ;

		  WriteMessage( "\nVolume: %.03f  Price Level: %d  Price: %.03f  Value: %.03f",
										volume, priceLevel, price, value ) ;

		  WriteMessage( "\nVolume2: %.03f  Complete DaTe: %04d-%02d-%02d %02d:%02d:%02d ",
										volume2, oleCompletedDate->GetYear(), oleCompletedDate->GetMonth(), 
										oleCompletedDate->GetDay(), oleCompletedDate->GetHour(), oleCompletedDate->GetMinute(), 
										oleCompletedDate->GetSecond()  ) ;

		  WriteMessage( "\nAttendant ID: %d  Age: %d  Cleared Date: %04d-%02d-%02d %02d:%02d:%02d ",
										attendantID, age, oleClearedDate->GetYear(), oleClearedDate->GetMonth(), 
										oleClearedDate->GetDay(), oleClearedDate->GetHour(), oleClearedDate->GetMinute(), 
										oleClearedDate->GetSecond() ) ;

		  WriteMessage( "\nOld Volume ETotal: %.03f  Old Volume2 ETotal: %.03f  Old Value ETotal: %.03f",
										oldVolumeETot, oldVolume2ETot, oldvalueETot) ;

		  WriteMessage( "\nNew Volume ETotal: %.03f  New Volume2 ETotal: %.03f  New Value ETotal: %.03f",
										newVolumeETot, newVolume2ETot, newvalueETot) ;

		  WriteMessage( "\nTAG: %d Duraction: %d ", tag, duration ) ;

		  WriteMessage( "\n" ) ;

          if( lockedBy != NOBODY )
			continue ;

          if( GoodResult( EZInterface.LockDelivery( deliveryID ) ) )
			lockedBy = LOGIN_ID ;

          else
            continue ;

          if( ( lockedBy == LOGIN_ID ) && ( deliveryState != CLEARED ) )
            GoodResult( EZInterface.ClearDelivery( deliveryID , deliveryType ) ) ;

	   }
	}

	WriteMessage("\n------------------------------------------------------------------------") ;

	EndMessage() ;
}

//------------------------ Pump Basic Control ---------------------------------
void PumpBasicControl()
{
	char chosenType ;
		
	chosenType = ShowMenu( PUMP_BASIC_CONTROL_MENU ) ;

	CheckLogin();
	if( ! isConnected )
		return ;

	switch ( chosenType )
	{
		case 'A': case 'a':
			TemporaryStop() ;
			break ;
		case 'B': case 'b':
			ReAuthoriseDelivery() ;
			break ;
		case 'C': case 'c':
			DisablePump() ;
			break ;
		case 'D': case 'd':
			EnablePump() ;
			break ;
		case 'E': case 'e':
			PumpsStatus() ;
			break ;
		case 'F': case 'f':
			AuthoriseDelivery() ;
			break ;
		case 'G': case 'g':
			CancelAuthorise() ;
			break ;
		default:
			WriteMessage("\n Unknown command!") ;
	}
}

void TemporaryStop()
{
	long	pumpID ;
	long	pumpNumber ;
	
	WriteMessage("\n Enter the number of the pump you want block: ") ;
	fflush(stdin) ;
	scanf ("%d", &pumpNumber ) ;

	if( !GoodResult( EZInterface.GetPumpByNumber( pumpNumber, &pumpID ) ) )
		return ;

	if( GoodResult( EZInterface.TempStop( pumpID ) ) ) 
		return ;

	WriteMessage( "\n Pump %d Temp Stoped", pumpNumber ) ;

	EndMessage() ;

}

void ReAuthoriseDelivery()
{
	long	pumpID ;
	long	pumpNumber ;
	
	WriteMessage("\n Enter the number of the pump you want reauthorize: ") ;
	fflush(stdin) ;
	scanf ("%d", &pumpNumber ) ;

	if( !GoodResult( EZInterface.GetPumpByNumber( pumpNumber, &pumpID ) ) )
		return ;

	if( GoodResult( EZInterface.ReAuthorise( pumpID ) ) ) 
		return ;

	WriteMessage( "\n Pump %d Temp Stoped", pumpNumber ) ;

	EndMessage() ;
}

void DisablePump()
{
	long	pumpID ;
	long	pumpNumber ;
	
	WriteMessage("\n Enter the number of the pump you want disable: ") ;
	fflush(stdin) ;
	scanf ("%d", &pumpNumber ) ;

	if( !GoodResult( EZInterface.GetPumpByNumber( pumpNumber, &pumpID ) ) )
		return ;

	if( GoodResult( EZInterface.DisablePump( pumpID ) ) ) 
		return ;

	WriteMessage( "\n Pump %d Disable Pump", pumpNumber ) ;

	EndMessage() ;
}

void EnablePump()
{
	long	pumpID ;
	long	pumpNumber ;
	
	WriteMessage("\n Enter the number of the pump you want enable: ") ;
	fflush(stdin) ;
	scanf ("%d", &pumpNumber ) ;

	if( !GoodResult( EZInterface.GetPumpByNumber( pumpNumber, &pumpID ) ) )
		return ;

	if( GoodResult( EZInterface.EnablePump( pumpID ) ) ) 
		return ;

	WriteMessage( "\n Pump %d Enable Pump", pumpNumber ) ;

	EndMessage() ;
}

void PumpsStatus()
{

#if defined( _WIN32 )
	BSTR    pumpStatuses ;
	BSTR    currentHose ;
	BSTR    deliveriesCount ;

#elif defined( __linux__ )
	BSTR    pumpstates[MAX_PATH] ;
	BSTR    currentHose[MAX_PATH] ;
	BSTR    deliveriesCount[MAX_PATH] ;

#endif

	long    pumpsCount ;
	int     idx ;
	int     curStatus ;
	int     curHose ;
	int     curDelv ;
	char    strStatus[MAX_PATH] ;

  // Receive the quantity of pumps registered
  if( !GoodResult( EZInterface.GetPumpsCount( &pumpsCount ) ) )
	  return ;

	// Read the status of all configured pumps
#if defined(_WIN32)
	  if( !GoodResult( EZInterface.GetAllPumpStatuses( &pumpStatuses, &currentHose, &deliveriesCount ) ) )

#elif defined(__linux__)
	  if( !GoodResult( EZInterface.GetAllPumpStatuses(
										  MakeBSTR( pumpstates,      sizeof(pumpstates) / sizeof(wchar_t) ),
										  MakeBSTR( currentHose,     sizeof(currentHose) / sizeof(wchar_t) ),
										  MakeBSTR( deliveriesCount, sizeof(deliveriesCount)/sizeof(wchar_t)) ) ) )
#endif
		  return;

	  for( idx = 0 ; idx < pumpsCount ; idx++ )
	  {
		curStatus = pumpStatuses[idx] - '0' ;
		curHose   = currentHose[idx] - '0' ;
		curDelv   = currentHose[idx] - '0' ;

		switch( curStatus ) 																											// PAM1000
		{
    		case INVALID_PUMP_STATE           : strcpy( strStatus, "Invalid status.") ;											break;	// 0 - OFFLINE
    		case NOT_INSTALLED_PUMP_STATE     : strcpy( strStatus, "Not installed.") ;											break;	// 6 - CLOSE
    		case NOT_RESPONDING_1_PUMP_STATE  : strcpy( strStatus, "Pump does not respond.") ;									break;	// 0 - OFFLINE
    		case IDLE_PUMP_STATE              : strcpy( strStatus, "On hold ( unoccupied ).") ;									break;	// 1 - IDLE
    		case PRICE_CHANGE_STATE           : strcpy( strStatus, "Price change.") ;											break;	// 1 - IDLE
    		case AUTHED_PUMP_STATE            : strcpy( strStatus, "Authorized Pump.") ;										break;	// 9 - AUTHORIZED
    		case CALLING_PUMP_STATE           : strcpy( strStatus, "Waiting for authorization.") ;								break;	// 5 - CALL
    		case DELIVERY_STARTING_PUMP_STATE : strcpy( strStatus, "Starting delivery.") ;										break;	// 2 - BUSY
    		case DELIVERING_PUMP_STATE        : strcpy( strStatus, "Fueling.") ;												break;	// 2 - BUSY
    		case TEMP_STOPPED_PUMP_STATE      : strcpy( strStatus, "Temporary stop ( in the middle of a delivery ).") ;			break;	// 8 - STOP
    		case DELIVERY_FINISHING_PUMP_STATE: strcpy( strStatus, "Finishing delivery ( decreasing product flow ).") ;			break;	// 2 - BUSY
    		case DELIVERY_FINISHED_PUMP_STATE : strcpy( strStatus, "Finished fuel ( stopped fuel ).") ;							break;	// 2 - BUSY
    		case DELIVERY_TIMEOUT_PUMP_STATE  : strcpy( strStatus, "Delivery exceeded maximum time.") ;							break;	// 1 - IDLE
    		case HOSE_OUT_PUMP_STATE          : strcpy( strStatus, "Hose out of the pump.") ;									break;	// 5 - CALL
    		case PREPAY_REFUND_TIMEOUT_STATE  : strcpy( strStatus, "Pre-determination timeout.") ;								break;	// 1 - IDLE
    		case DELIVERY_TERMINATED_STATE    : strcpy( strStatus, "Finished delivery.") ;										break;	// 3 - EOT
    		case ERROR_PUMP_STATE             : strcpy( strStatus, "Error ( pump error response ).") ;							break;	// 0 - OFFLINE
    		case NOT_RESPONDING_2_PUMP_STATE  : strcpy( strStatus, "EZID does not respond.") ;									break;
			case LAST_PUMP_STATE              : strcpy( strStatus, "Last state of the pump.") ;									break;
			default							  : sprintf(strStatus, "Unknown state = %d", curStatus) ;							break;
		}


		  WriteMessage("\n Pump %02d, Hose %02d, On Hold %d, Status(%02d): %s", idx +1 , curHose , curDelv , curStatus , strStatus ) ;
		  currentStatus[idx] = curStatus ;

		  FREE_SYS_STR( pumpStatuses ) ;
		  FREE_SYS_STR( currentHose ) ;
		  FREE_SYS_STR( deliveriesCount ) ;

	  }

	  EndMessage() ;
}

void AuthoriseDelivery()
{
	long	pumpID ;
	long	pumpNumber ;
	
	WriteMessage("\n Enter the number of the pump you want Authorise: ") ;
	fflush(stdin) ;
	scanf ("%d", &pumpNumber ) ;

	if( !GoodResult( EZInterface.GetPumpByNumber( pumpNumber, &pumpID ) ) )
		return ;

	if( GoodResult( EZInterface.Authorise( pumpID ) ) ) 
		return ;

	WriteMessage( "\n Pump %d Authorise Pump", pumpNumber ) ;

	EndMessage() ;
}

void CancelAuthorise()
{
	long	pumpID ;
	long	pumpNumber ;
	
	WriteMessage("\n Enter the number of the pump you want Cancel Authorise: ") ;
	fflush(stdin) ;
	scanf ("%d", &pumpNumber ) ;

	if( !GoodResult( EZInterface.GetPumpByNumber( pumpNumber, &pumpID ) ) )
		return ;

	if( GoodResult( EZInterface.CancelAuthorise( pumpID ) ) ) 
		return ;

	WriteMessage( "\n Pump %d Cancel Authorise Pump", pumpNumber ) ;

	EndMessage() ;
}

//------------------------ Pump Control Extended --------------------------------- 
void PumpControlExtended()
{
	char chosenType ;
		
	chosenType = ShowMenu( PUMP_CONTROL_EXTENDED_MENU ) ;

	CheckLogin();
	if( ! isConnected )
		return ;

	switch ( chosenType )
	{
		case 'A': case 'a':
			ScheduledDelivery() ;
			break ;
		case 'B': case 'b':
			PresetDelivery() ;
			break ;
		default:
			WriteMessage("\n Unknown command!") ;
	}
}

// Program a complete delivery, you can define: attendant, client, hose, value...
void ScheduledDelivery()
{
    long	pumpID ;
	long	pumpNumber ;
    long	termID = LOGIN_ID ;
    long	attendantID ;
	INT64	attendantTag ;
	long	cardClientID = NULL ;
	INT64	cardClientTag = NOBODY ;
	short	authType = NOBODY;
	INT64	extTag = NOBODY ;
	short	priceLevel = UNKNOWN_PRICE_CONTROL;
	double	price ;
	short	gradeType = ALLGRADE ;
	short	priceType = FIXED_PRICE_TYPE ;
	short	presetType = DOLLAR_LIMIT_TYPE ;
	double	value ;
	short	hose = ALL_HOSES ;
	double	odometer = NULL ;
	double	odometer2 = NULL ;

	// GetAttendantProperties
	long	attendantNumber ;
	short	shiftAStart ;
	short	shiftAEnd ;
	short	shiftBStart ;
	short	shiftBEnd ;
	short	attendantType ;

#if defined( _WIN32 )
	BSTR    termHash = NULL ;
	BSTR    plate = NULL ;
	BSTR    extTransactionID = NULL ;
	BSTR	driverID = NULL ;
	BSTR	authorisation = NULL ;

	// GetAttendantProperties
	BSTR	name = NULL ;
	BSTR	shortName = NULL ;
	BSTR	password = NULL ;
	BSTR	tag = NULL ;

#elif defined( __linux__ )
	BSTR    termHash [MAX_PATH] ;
	BSTR    plate [MAX_PATH] ;
	BSTR    extTransactionID [MAX_PATH] ;
	BSTR	driverID [MAX_PATH] ;
	BSTR	authorisation [MAX_PATH] ;

	// GetAttendantProperties
	BSTR	name [MAX_PATH] ;
	BSTR	shortName [MAX_PATH] ;
	BSTR	password [MAX_PATH] ;
	BSTR	tag [MAX_PATH] ;

#endif

	WriteMessage( "\n In this example we will make an authorization with a choice of pump, attendant, price and limit." ) ;
	WriteMessage( "\n" ) ;
	
	WriteMessage( "\n Enter the Pump Number you wish to authorize: " ) ;
    fflush(stdin) ;
    scanf ("%d", &pumpNumber ) ;

	if( !GoodResult( EZInterface.GetPortByNumber(	pumpNumber, &pumpID ) ) )
		return ;

	WriteMessage( "\n Enter the Attendant number that can authorize: " ) ;
    fflush(stdin) ;
    scanf ("%d", &attendantNumber ) ;

	if( !GoodResult( EZInterface.GetAttendantByNumber(	attendantNumber, &attendantID ) ) )
		return ;

#if defined( _WIN32 )
	if( !GoodResult( EZInterface.GetAttendantPropertiesEx(	attendantID, &attendantNumber, &name, &shortName, &password, 
															&tag, &shiftAStart, &shiftAEnd, &shiftBStart, &shiftBEnd, &attendantType ) ) )
		return ;

#elif defined( __linux__ )
	if( !GoodResult( EZInterface.GetAttendantPropertiesEx(	attendantID, &attendantNumber, 
															MakeBSTR( name, sizeof( name ) / sizeof(wchar_t) ), 
															MakeBSTR( shortName, sizeof( shortName ) / sizeof(wchar_t) ), 
															MakeBSTR( password, sizeof( password ) / sizeof(wchar_t) ), 
															MakeBSTR( tag, sizeof( tag ) / sizeof(wchar_t) ),
															&shiftAStart, &shiftAEnd, &shiftBStart, shiftBEnd, attendantType ) ) )
		return ;

#endif

	attendantTag = ( INT64 ) tag ;

	WriteMessage( "\n Enter the Price ( Double ) which will be used ( Example 1.98 ): " ) ;
    fflush(stdin) ;
    scanf ("%lf", &price ) ;

	WriteMessage( "\n Enter the volume limit ( Double ) which will be used: " ) ;
    fflush(stdin) ;
    scanf ("%lf", &value ) ;

#if defined( _WIN32 )

	if( !GoodResult( EZInterface.PaymentReserve( pumpID, termID, termHash ) ) )
		return ;

	if( !GoodResult( EZInterface.PaymentAuthorise(	pumpID, termID, termHash, attendantID,
													attendantTag, cardClientID, cardClientTag,
													authType, extTag, gradeType, priceType,
													priceLevel, price, presetType, value,
													hose, odometer, odometer2, plate,
													extTransactionID, driverID, authorisation ) ) )
		return ;

#elif defined( __linux__ )
	if( !GoodResult( EZInterface.PaymentAuthorise(	pumpID, termID, 
													MakeBSTR( termHash, sizeof( termHash ) / sizeof(wchar_t) ),
													attendantID,
													attendantTag, cardClientID, cardClientTag,
													authType, extTag, gradeType, priceType,
													priceLevel, price, presetType, value,
													hose, odometer, odometer2, 
													MakeBSTR( plate, sizeof( plate ) / sizeof(wchar_t) ),
													MakeBSTR( extTransactionID, sizeof( extTransactionID ) / sizeof(wchar_t) ),
													MakeBSTR( driverID, sizeof( driverID ) / sizeof(wchar_t) ),
													MakeBSTR( authorisation, sizeof( authorisation ) / sizeof(wchar_t) ) )
		return ;

#endif

	FREE_SYS_STR( termHash ) ;
	FREE_SYS_STR( plate ) ;
	FREE_SYS_STR( extTransactionID ) ;
	FREE_SYS_STR( driverID ) ;
	FREE_SYS_STR( authorisation ) ;
	FREE_SYS_STR( name ) ;
	FREE_SYS_STR( shortName ) ;
	FREE_SYS_STR( password ) ;
	FREE_SYS_STR( tag ) ;

	WriteMessage( "\n Pump Reserved " ) ;

	EndMessage() ;
}

// Program a delivery with value limit or volume
void PresetDelivery()
{
	
	long		pumpID ;
	long		pumpNumber ;
	long		limitType ;
	double		value ;
	short		hose ;
	short		priceLevel ;

	WriteMessage( "\n Enter the pump number which will be used: " ) ;
    fflush(stdin) ;
    scanf ("%d", &pumpNumber ) ;

	if( !GoodResult( EZInterface.GetPumpByNumber( pumpNumber, &pumpID ) ) ) 
		return ;

	WriteMessage( "\n 1 - NO PRESET TYPE " ) ;
	WriteMessage( "\n 2 - DOLLAR PRESET TYPE " ) ;
	WriteMessage( "\n 3 - VOLUME PRESET TYPE " ) ;
	WriteMessage( "\n" ) ;
	WriteMessage( "\n Enter the limit type which will be used: " ) ;
    fflush(stdin) ;
    scanf ("%d", &limitType ) ;

	WriteMessage( "\n Enter the limit value ( example 25.000 ): " ) ;
    fflush(stdin) ;
    scanf ("%lf", &value ) ;

	hose = ALL_HOSES ;

	priceLevel = UNKNOWN_PRICE_CONTROL ;

	if( !GoodResult( EZInterface.LoadPreset ( pumpID, ( short ) limitType, value, hose, priceLevel ) ) ) 
		return ;

	WriteMessage( "\n Load Preset loaded successfully" ) ;

	EndMessage() ;

}

//------------------------ Pump Price Control --------------------------------- 
void PumpPriceControl()
{
	char chosenType ;
		
	chosenType = ShowMenu( PUMP_PRICE_CONTROL_MENU ) ;

	CheckLogin();
	if( ! isConnected )
		return ;

	switch ( chosenType )
	{
		case 'A': case 'a':
			ChangeHosePrice() ;
			break ;
		case 'B': case 'b':
			ChangeGradePrice() ;
			break ;
		default:
			WriteMessage("\n Unknown command!") ;
	}
}

// Changes the price of the hose
void ChangeHosePrice()
{
	long	hoseID ;
	long	hoseNumber ;
	long	pumpID ;
	long	pumpNumber ;
	long	durationType ;
	long	priceType ;
	double	price1 ;
	double	price2 ;

	WriteMessage( "\n Enter the Pump number you want to change the price: " ) ;
    fflush( stdin ) ;
    scanf ("%d", &pumpNumber ) ;

	if( !GoodResult( EZInterface.GetPumpByNumber( pumpNumber, &pumpID ) ) ) 
		return ;

	WriteMessage( "\n Enter the Hose number you want to change the price(the hose number this pump): " ) ;
    fflush( stdin ) ;
    scanf ("%d", &hoseNumber ) ;

	if( !GoodResult( EZInterface.GetPumpHoseByNumber( pumpID, hoseNumber, &hoseID ) ) ) 
		return ;

	WriteMessage( "\n 1 - SINGLE DURATION TYPE " ) ;
	WriteMessage( "\n 2 - MULTIPLE DURATION TYPE " ) ;
	WriteMessage( "\n" ) ;
	WriteMessage( "\n Enter the duration type: " ) ;
    fflush( stdin ) ;
    scanf ("%d", &durationType ) ;

	WriteMessage( "\n 1 - FIXED PRICE TYPE " ) ;
	WriteMessage( "\n 2 - DISCOUNT PRICE TYPE " ) ;
	WriteMessage( "\n 3 - SURCHARGE PRICE TYPE " ) ;
	WriteMessage( "\n" ) ;
	WriteMessage( "\n Enter the price type: " ) ;
    fflush( stdin ) ;
    scanf ("%d", &priceType ) ;

	WriteMessage( "\n The price for level 1 or zero to leave unchanged: " ) ;
    fflush( stdin ) ;
    scanf ("%lf", &price1 ) ;

	WriteMessage( "\n The price for level 2 or zero to leave unchanged: " ) ;
    fflush( stdin ) ;
    scanf ("%lf", &price2 ) ;

	if( !GoodResult( EZInterface.SetHosePrices ( hoseID, ( short ) durationType, ( short ) priceType, price1, price2 ) ) ) 
		return ;

	WriteMessage( "\n Price changed successfully" ) ;

	EndMessage() ;
}

// Changes the price of the grade
void ChangeGradePrice()
{
	long	gradeID ; 
	long	gradeNumber ;
	long	level ;
	double	price ;

	WriteMessage( "\n Enter the Grade Number you want to change the price: " ) ;
    fflush( stdin ) ;
    scanf ( "%d", &gradeNumber ) ;

	if( !GoodResult( EZInterface.GetGradeByNumber ( gradeNumber, &gradeID ) ) ) 
		return ;

	WriteMessage( "\n Enter the price level you want to change, value between 1 and 8: " ) ;
    fflush( stdin ) ;
    scanf ("%d", &level ) ;

	WriteMessage( "\n Enter the price: " ) ;
    fflush( stdin ) ;
    scanf ("%lf", &price ) ;

	if( !GoodResult( EZInterface.SetGradePrice ( gradeID, ( short ) level, price ) ) ) 
		return ;

	WriteMessage( "\n Price changed successfully" ) ;

	EndMessage() ;
}

//------------------------ Reading Cards --------------------------------- 
void ReadingCards()
{
	long	idx ;
	long	cardCount ;
	long	cardID ;
	long	cardNumber ;
	long	pumpID ;
	short	cardType ;
	long	parentID ;
	INT64	tag ;
	DATE	timeStamp ;

	COleDateTime *oleTimeStamp = NULL ;

#if defined( _WIN32 )
	BSTR    cardName ;

#elif defined( __linux__ )
	BSTR    cardName[MAX_PATH] ;

#endif

	if( !GoodResult( EZInterface.GetCardReadsCount( &cardCount ) ) )
		return ;

	WriteMessage( "\n [ Count Card %03d ] --------------------------------------------------- ", cardCount ) ;
	
	for( idx = 0 ; idx < cardCount ; idx++ )
	{
		if( !GoodResult( EZInterface.GetCardReadByOrdinal( idx, &cardID ) ) )
			return ;

#if defined( _WIN32 )
		if( !GoodResult( EZInterface.GetCardReadProperties(	cardID, &cardNumber, &cardName, &pumpID,
															&cardType, &parentID, &tag, &timeStamp ) ) )
			return ;

#elif defined( __linux__ )
		if( GoodResult( EZInterface.GetCardReadProperties(	cardID, &cardNumber, 
															MakeBSTR( cardName, sizeof( cardName ) / sizeof(wchar_t) ), 
															&pumpID, &cardType, &parentID, &tag, &timeStamp ) ) )
			return ;
#endif

		oleTimeStamp = new COleDateTime( timeStamp ) ;

		WriteMessage( "\n------ Card: (%03d) %d",    idx, cardID ) ;

		WriteMessage( "\nCard Number: %d  Card Name: %S  Pump ID: %d",
							cardNumber, cardName, pumpID ) ;

		WriteMessage( "\nCard Type: %d  Parent ID: %d Time Stamp: %04d-%02d-%02d %02d:%02d:%02d  TAG: %ull",
							cardType, parentID, oleTimeStamp->GetYear(), oleTimeStamp->GetMonth(), 
							oleTimeStamp->GetDay(), oleTimeStamp->GetHour(), oleTimeStamp->GetMinute(), 
							oleTimeStamp->GetSecond(), tag  ) ;

		FREE_SYS_STR( cardName ) ;
	}

	WriteMessage("\n------------------------------------------------------------------------") ;

	EndMessage() ;
}

//------------------------ Register Attendant ---------------------------------
void RegisterAttendant() 
{
	char chosenType ;
		
	chosenType = ShowMenu( REGISTER_ATTENDANT_MENU ) ;

	CheckLogin();
	if( ! isConnected )
		return ;

	switch ( chosenType )
	{
		case 'A': case 'a':
			AttendantRegistration() ;
			break ;
		case 'B': case 'b':
			DeleteAttendant() ;
			break ;
		default:
			WriteMessage("\n Unknown command!") ;
	}
}

void AttendantRegistration()
{
	long	idx ;
	long	attendantCount ;
	long	attendantID ;
	long	newAttendantID = 0 ;
	long	attendantNumber ;	
	long	shiftAStart ;
	long	shiftAEnd ;
	long	shiftBStart ;
	long	shiftBEnd ;
	short	attendantType = ENABLED_ATTSTATE ;
	char	attendantName[MAX_PATH] ;
	char	attendantShortName[MAX_PATH] ;
	char	password[MAX_PATH] ;
	char	tag[MAX_PATH] ;

	WriteMessage( "\n Enter the attendant number: " ) ;
    fflush( stdin ) ;
    scanf ("%d", &attendantNumber ) ;

	WriteMessage( "\n Enter the attendant name: " ) ;
    fflush( stdin ) ;
    scanf ("%s", attendantName ) ;

	WriteMessage( "\n Enter the attendant short name: " ) ;
    fflush( stdin ) ;
    scanf ("%s", attendantShortName ) ;

	WriteMessage( "\n Enter the password: " ) ;
    fflush( stdin ) ;
    scanf ("%s", password ) ;

	WriteMessage( "\n Enter the card tag: " ) ;
    fflush( stdin ) ;
    scanf ("%s", tag ) ;

	//The time that the first shift for this attendant begins, 
	//it is in minutes from midnight, for example, 00:00 am = 0, 12:00pm = 720, 23:59 = 1439.
	WriteMessage( "\n Enter the ShiftAStart: " ) ;
    fflush( stdin ) ;
    scanf ("%d", &shiftAStart ) ;

	WriteMessage( "\n Enter the ShiftAEnd: " ) ;
    fflush( stdin ) ;
    scanf ("%d", &shiftAEnd ) ;

	WriteMessage( "\n Enter the ShiftBStart: " ) ;
    fflush( stdin ) ;
    scanf ("%d", &shiftBStart ) ;

	WriteMessage( "\n Enter the ShiftBEnd: " ) ;
    fflush( stdin ) ;
    scanf ("%d", &shiftBEnd ) ;

	if( !GoodResult( EZInterface.GetAttendantsCount( &attendantCount ) ) )
		return ;

	for( idx = attendantCount ; idx > 0 ; idx-- )
	{
		if( !GoodResult( EZInterface.GetAttendantByOrdinal( idx, &attendantID ) ) )
			return ;

		if( newAttendantID < attendantID )
			newAttendantID = attendantID ;
	}

	newAttendantID++ ;

	BSTR AttName = CharToBstr( attendantName ) ; 
	BSTR AttShortName = CharToBstr( attendantShortName ) ; 
	BSTR AttPassword = CharToBstr( password ) ; 
	BSTR AttTag = CharToBstr( tag ) ; 

	if( !GoodResult( EZInterface.SetAttendantPropertiesEx(	newAttendantID, attendantNumber, AttName,
															AttShortName, AttPassword, AttTag,
															( short ) shiftAStart, ( short ) shiftAEnd, ( short ) shiftBStart,
															( short ) shiftBEnd, attendantType ) ) )
		return ;

	FREE_SYS_STR( AttName ) ; 
	FREE_SYS_STR( AttShortName ) ; 
	FREE_SYS_STR( AttPassword ) ; 
	FREE_SYS_STR( AttTag ) ; 

	WriteMessage( "\n Attendant register successfully" ) ;

	EndMessage() ;
}

void DeleteAttendant()
{
	long attendantNumber ;
	long attendantID ;

	WriteMessage( "\n Enter the Attdendant Number you wish to delete: " ) ;
	fflush( stdin ) ;
    scanf ("%d", &attendantNumber ) ;

	if( !GoodResult( EZInterface.GetAttendantByNumber( attendantNumber, &attendantID ) ) ) 
		return ;

	if( !GoodResult( EZInterface.DeleteAttendant( attendantID ) ) ) 
		return ;

	WriteMessage( "\n Attendant delete successfully" ) ;

	EndMessage() ;
}

//------------------------ Register Client ---------------------------------
void RegisterClient() 
{
	char chosenType ;
		
	chosenType = ShowMenu( REGISTER_CLIENT_MENU ) ;

	CheckLogin();
	if( ! isConnected )
		return ;

	switch ( chosenType )
	{
		case 'A': case 'a':
			ClientRegistration() ;
			break ;
		case 'B': case 'b':
			DeleteClient() ;
			break ;
		default:
			WriteMessage("\n Unknown command!") ;
	}
}

void ClientRegistration()
{
	SYSTEMTIME st ;
	GetLocalTime( & st ) ;
	st.wYear = st.wYear + 1000 ;
	COleDateTime eztime( st ) ;

	long	idx ;
	long	clientCount ;
	long	clientID ;
	long	newClientID = 0 ;
	long	clientNumber ;
	char	clientName[MAX_PATH] ;
	char	tag[MAX_PATH] ;
	short	isEnable = true ;
	long	priceLevel ;
	short	gradeType = ALLGRADE ;
	short	cardType = ATTENDANT_CARD_TYPE ;
	short	limitType = DOLLAR_PRESET_TYPE ;
	double	limit ;
	short	entryType = NO_ENTRY_TYPE ;
	long	parentID = -1 ;

#if defined( _WIN32 )
	BSTR	plate = NULL ;

#elif defined( __linux__ )
	BSTR    plate[MAX_PATH] = NULL ;

#endif

	WriteMessage( "\n Enter the client number: " ) ;
    fflush( stdin ) ;
    scanf ("%d", &clientNumber ) ;

	WriteMessage( "\n Enter the client name: " ) ;
    fflush( stdin ) ;
    scanf ("%s", clientName ) ;

	WriteMessage( "\n Enter the card tag: " ) ;
    fflush( stdin ) ;
    scanf ("%s", tag ) ;

	BSTR AttClientName =	CharToBstr( clientName ) ;  
	BSTR AttTag =			CharToBstr( tag ) ;

	WriteMessage( "\n Enter the price level: " ) ;
    fflush( stdin ) ;
    scanf ("%d", &priceLevel ) ;

	WriteMessage( "\n Enter the limit $$ that this card can delivery: " ) ;
    fflush( stdin ) ;
    scanf ("%lf", &limit ) ;

	if( !GoodResult( EZInterface.GetCardClientsCount( &clientCount ) ) )
		return ;

	for( idx = clientCount ; idx > 0 ; idx-- )
	{
		if( !GoodResult( EZInterface.GetCardClientByOrdinal( idx, &clientID ) ) )
			return ;

		if( newClientID < clientID )
			newClientID = clientID ;
	}

	newClientID++ ;

	if( !GoodResult( EZInterface.SetCardClientPropertiesEx2( newClientID, clientNumber, 
															AttClientName, AttTag,
															isEnable, ( short ) priceLevel,
															plate, gradeType, cardType, limitType,
															limit, entryType, eztime, parentID ) ) )
		return ;

	FREE_SYS_STR( AttClientName ) ; 
	FREE_SYS_STR( AttTag ) ; 

	WriteMessage( "\n Client register successfully" ) ;

	EndMessage() ;
}

void DeleteClient()
{
	long	clientNumber ;
	long	clientID ;

	WriteMessage( "\n Enter the Client Number you wish to delete: " ) ;
	fflush( stdin ) ;
    scanf ("%d", &clientNumber ) ;

	if( !GoodResult( EZInterface.GetCardClientByNumber( clientNumber, &clientID ) ) ) 
		return ;

	if( !GoodResult( EZInterface.DeleteCardClient( clientID ) ) ) 
		return ;

	WriteMessage( "\n Client delete successfully" ) ;

	EndMessage() ;
}

//------------------------ Tanks Sensors Reading ---------------------------------
void TanksSensorsReading() 
{
	char chosenType ;
		
	chosenType = ShowMenu( TANKS_SENSORS_READING_MENU ) ;

	CheckLogin();
	if( ! isConnected )
		return ;

	switch ( chosenType )
	{
		case 'A': case 'a':
			TankReading() ;
			break ;
		case 'B': case 'b':
			MonitoringSensor() ;
			break ;
		default:
			WriteMessage("\n Unknown command!") ;
	}
}

void TankReading() 
{
	long	idx ;
	long	tankCount ;
	long	tankID ;
	long	tankNumber ;
	long	gradeID ;
	short	type ;	
    double	capacity ;	
    double	diameter ;	
	double	theoVolume ;
    double	gaugeVolume ;
    double	gaugeTCvolume ;
    double	gaugeUllage ;
    double	gaugeTemperature ;
    double	gaugeLevel ;
    double	gaugeWaterVolume ;
    double	gaugeWaterLevel ;
    long	gaugeID ;	
    short	probeNo ;
    long	gaugeAlarmsMask ;

#if defined(_WIN32)
	BSTR	tankName ;

#elif defined(__linux__)
	BSTR	tankName[MAX_PATH] ;

#endif

	if( !GoodResult( EZInterface.GetTanksCount( &tankCount ) ) )
      return ;

	WriteMessage( "\n [ Count Tanks %03d ] ------------------------------ ", tankCount ) ;

    for( idx = tankCount ; idx > 0 ; idx-- )
	{

		if( !GoodResult( EZInterface.GetTankByOrdinal( idx, &tankID ) ) )
          return;

#if defined( _WIN32 )
		if( GoodResult( EZInterface.GetTankPropertiesEx(		tankID,				&tankNumber,		&tankName,
																&gradeID,			&type,				&capacity,			
																&diameter,			&theoVolume,		&gaugeVolume,		
																&gaugeTCvolume,		&gaugeUllage,		&gaugeTemperature,	
																&gaugeLevel,		&gaugeWaterVolume,	&gaugeWaterLevel,	
																&gaugeID,			&probeNo,			&gaugeAlarmsMask		) ) )

#elif defined( __linux__ )
		if( GoodResult( EZInterface.GetTankPropertiesEx(		tankID,				&tankNumber,		
																MakeBSTR( tankName, sizeof( tankName ) / sizeof(wchar_t) ), 
																&gradeID,			&type,				&capacity,			
																&diameter,			&theoVolume,		&gaugeVolume,		
																&gaugeTCvolume,		&gaugeUllage,		&gaugeTemperature,	
																&gaugeLevel,		&gaugeWaterVolume,	&gaugeWaterLevel,	
																&gaugeID,			&probeNo,			&gaugeAlarmsMask		) ) )
#endif

		WriteMessage("\n\n\tTankID: %d   TankNumber: %d  TankName: %S ",
											tankID, tankNumber, tankName );
		WriteMessage("\n\tGradeID: %d  Type: %d  Capacity: %0.3f   ",
											gradeID, type, capacity );
		WriteMessage("\n\tDiameter: %0.3f  theoVolume %0.3f  gaugeVolume %d",
											diameter, theoVolume, gaugeVolume );
		WriteMessage("\n\tGaugeTCvolume: %0.3f  GaugeUllage: %0.3f  GaugeTemperature: %0.3f" ,
											gaugeTCvolume, gaugeUllage, gaugeTemperature );
		WriteMessage("\n\tGaugeLevel: %0.3f  GaugeWaterVolume: %0.3f  GaugeWaterLevel: %0.3f ",
											gaugeLevel, gaugeWaterVolume, gaugeWaterLevel );
		WriteMessage("\n\tGaugeID: %d  ProbeNo: %d  GaugeAlarmsMask: %d",
											gaugeID, probeNo, gaugeAlarmsMask );

		FREE_SYS_STR( tankName ) ; 
	}

	EndMessage() ;
}

void MonitoringSensor()
{
	long	idx ;
	long	sensorCount ;
	long	sensorID ;
	long	sensorNumber ;
	long	portID ;
	short	sensorType ;
	short	sensorAddress ;
	short	sensorNo ;

#if defined(_WIN32)
	BSTR	sensorName ;

#elif defined(__linux__)
	BSTR	sensorName[MAX_PATH] ;

#endif

	if( !GoodResult( EZInterface.GetSensorsCount( &sensorCount ) ) )
      return ;

	WriteMessage( "\n [ Count Sensors %03d ] ------------------------------ ", sensorCount ) ;

    for( idx = sensorCount ; idx > 0 ; idx-- )
	{

		if( !GoodResult( EZInterface.GetSensorByOrdinal( idx, &sensorID ) ) )
          return;

#if defined( _WIN32 )
		if( GoodResult( EZInterface.GetSensorProperties(		sensorID,			&sensorNumber,		&sensorName,
																&portID,			&sensorType,		&sensorAddress,			
																&sensorNo		) ) )

#elif defined( __linux__ )
		if( GoodResult( EZInterface.GetSensorProperties(		sensorID,			&sensorNumber,		
																MakeBSTR( sensorName, sizeof( sensorName ) / sizeof(wchar_t) ),
																&portID,			&sensorType,		&sensorAddress,			
																&sensorNo		) ) )
#endif

		WriteMessage("\n\n\t SensorID: %d   SensorNumber: %d  SensorName: %S ",
											sensorID, sensorNumber, sensorName );
		WriteMessage("\n\t PortID: %d  SensorType: %d  SensorAddress: %d   ",
											portID, sensorType, sensorAddress );
		WriteMessage("\n\t SensorNo: %d ", sensorNo );

		FREE_SYS_STR( sensorName ) ; 
	}

	EndMessage() ;
}

//------------------------ General Configuration Facility ---------------------------------
void GeneralConfigurationFacility() 
{
	char chosenType ;
		
	chosenType = ShowMenu( GENERAL_CONFIGURATION_FACILITY_MENU ) ;

	CheckLogin();
	if( ! isConnected )
		return ;

	switch ( chosenType )
	{
		case 'A': case 'a':
			DeliveryPositionRegister() ;
			break ;
		case 'B': case 'b':
			DeleteDeliveryPosition() ;
			break ;
		case 'C': case 'c':
			HoseRegistration() ;
			break ;
		case 'D': case 'd':
			DeleteHose() ;
			break ;
		case 'E': case 'e':
			GradeRegister() ;
			break ;
		case 'F': case 'f':
			DeleteGrade() ;
		case 'G': case 'g':
			TankRegistration() ;
			break ;
		case 'H': case 'h':
			DeleteTank() ;
			break ;
		case 'I': case 'i':
			ZigbeeDeviceRegistration() ;
			break ;
		case 'J': case 'j':
			DeleteZigbeeDevice() ;
			break ;
		default:
			WriteMessage("\n Unknown command!") ;
	}
}

void DeliveryPositionRegister()
{
	long	idx ;
	long	pumpCount ;
	long	pumpID ;
	long	newPumpID = 0 ;
	long	pumpNumber ;
	long	pumpPhysicalNumber ;
	long	pumpSide ;
	long	pumpAddress ;
	long	priceLevel1 ;
	long	priceLevel2 ;
	short	priceDspFormat = PUMP_DISPLAY_4_2 ;
	short	volumeDspFormat = PUMP_DISPLAY_6_2 ;
	short	valueDspFormat = PUMP_DISPLAY_6_2 ;
	short	pumpType = 0x04 ;					//EZSim, Details, see PumpType of Pumps at EZDrive.ini			        
	long	portID = 0x01 ;
	long	attendantID = NOBODY ;
	short	authMode = AUTO_AUTH ;
	short	stackMode = STACK_AUTO ;
	short	prepayAllowed = NULL ;
	short	preauthAllowed = NULL ;
	long	slotZigBeeID ;
	long	muxSlotZigBeeID = NOBODY ;
	short	priceControl = REMOTE_PRICE_CONTROL ;
	short	hasPreset = 0x01 ;
	char	pumpName[MAX_PATH] ;

	WriteMessage( "\n Enter the PumpNumber: " ) ;
    fflush( stdin ) ;
    scanf ("%d", &pumpNumber ) ;

	WriteMessage( "\n Enter the PumpName: " ) ;
    fflush( stdin ) ;
    scanf ("%s", pumpName ) ;

	BSTR AttPumpName =	CharToBstr( pumpName ) ;  

	WriteMessage( "\n Enter the PhysicalNumber: " ) ;
    fflush( stdin ) ;
    scanf ("%d", &pumpPhysicalNumber ) ;

	WriteMessage( "\n Enter the PumpSide: " ) ;
    fflush( stdin ) ;
    scanf ("%d", &pumpSide ) ;

	WriteMessage( "\n Enter the PumpAddress: " ) ;
    fflush( stdin ) ;
    scanf ("%d", &pumpAddress ) ;

	WriteMessage( "\n Enter the PriceLevel1: " ) ;
    fflush( stdin ) ;
    scanf ("%d", &priceLevel1 ) ;

	WriteMessage( "\n Enter the PriceLevel2: " ) ;
    fflush( stdin ) ;
    scanf ("%d", &priceLevel2 ) ;

	WriteMessage( "\n Enter the Slot ( 1 - 4 ) or ZigBeeID ( 5> ): " ) ;
    fflush( stdin ) ;
    scanf ("%d", &slotZigBeeID ) ;

	if( !GoodResult( EZInterface.GetPumpsCount( &pumpCount ) ) )
		return ;

	for( idx = pumpCount ; idx > 0 ; idx-- )
	{
		if( !GoodResult( EZInterface.GetPumpByOrdinal( idx, &pumpID ) ) )
			return ;

		if( newPumpID < pumpID )
			newPumpID = pumpID ;
	}

	newPumpID++ ;

	if( !GoodResult( EZInterface.SetPumpPropertiesEx(		newPumpID, pumpNumber, AttPumpName, ( short ) pumpPhysicalNumber,
															( short ) pumpSide, ( short ) pumpAddress, ( short ) priceLevel1, 
															( short ) priceLevel2, ( short ) priceDspFormat, ( short ) volumeDspFormat,
															( short ) valueDspFormat, pumpType, portID, attendantID,
															authMode, stackMode, prepayAllowed, preauthAllowed,
															slotZigBeeID, muxSlotZigBeeID, priceControl, hasPreset ) ) )
		return ;

	FREE_SYS_STR( AttPumpName ) ; 

	WriteMessage( "\n Pump register successfully" ) ;

	EndMessage() ;
}

void DeleteDeliveryPosition()
{
	long	pumpNumber ;
	long	pumpID ;

	WriteMessage( "\n Enter the Position Number you wish to delete: " ) ;
	fflush( stdin ) ;
    scanf ("%d", &pumpNumber ) ;

	if( !GoodResult( EZInterface.GetPumpByNumber( pumpNumber, &pumpID ) ) ) 
		return ;

	if( !GoodResult( EZInterface.DeletePump( pumpID ) ) ) 
		return ;

	WriteMessage( "\n Delivery Position delete successfully" ) ;

	EndMessage() ;
}

void HoseRegistration()
{
	long	idx ;
	long	hoseCount ;
	long	hoseID ;
	long	newHoseID = 0 ;
	long	hoseNumber ;
	long	pumpID ;
	long	pumpNumber ;
	long	tankID ;
	long	tankNumber ;
	long	hosePhysicalNumber ;
	double	mtrTheoValue = 0 ;
	double	mtrTheoVolume = 0 ;
	double	mtrElecValue = 0 ;
	double	mtrElecVolume = 0 ;
	short	UVEAtenna = FALSE ;
	double	price1 = UNKNOWN_PRICE_CONTROL ;
	double	price2 = UNKNOWN_PRICE_CONTROL ;
	short	isEnabled = TRUE ;

	WriteMessage( "\n Enter the Hose Number: " ) ;
    fflush( stdin ) ;
    scanf ("%d", &hoseNumber ) ;

	WriteMessage( "\n Enter the Pump Number: " ) ;
    fflush( stdin ) ;
    scanf ("%d", &pumpNumber ) ;

	WriteMessage( "\n Enter the Tank Number: " ) ;
    fflush( stdin ) ;
    scanf ("%d", &tankNumber ) ;

	WriteMessage( "\n Enter the Hose Physical Number: " ) ;
    fflush( stdin ) ;
    scanf ("%d", &hosePhysicalNumber ) ;

	if( !GoodResult( EZInterface.GetHosesCount( &hoseCount ) ) )
		return ;

	for( idx = hoseCount ; idx > 0 ; idx-- )
	{
		if( !GoodResult( EZInterface.GetHoseByOrdinal( idx, &hoseID ) ) )
			return ;

		if( newHoseID < hoseID )
			newHoseID = hoseID ;
	}

	newHoseID++ ;

	if( !GoodResult( EZInterface.GetPumpByNumber( pumpNumber, &pumpID ) ) ) 
		return ;

	if( !GoodResult( EZInterface.GetTankByNumber( tankNumber, &tankID ) ) ) 
		return ;

	if( !GoodResult( EZInterface.SetHosePropertiesEx2(		newHoseID, hoseNumber, pumpID, tankID, hosePhysicalNumber,
															mtrTheoValue, mtrTheoVolume, mtrElecValue, mtrElecVolume,
															UVEAtenna, price1, price2, isEnabled ) ) )
		return ;

	WriteMessage( "\n Hose register successfully" ) ;

	EndMessage() ;
}

void DeleteHose()
{
	long	hoseNumber ;
	long	pumpNumber ;
	long	hoseID ;
	long	pumpID ;

	WriteMessage( "\n Enter the Pump Number related to the hose: " ) ;
	fflush( stdin ) ;
    scanf ("%d", &pumpNumber ) ;

	WriteMessage( "\n Enter the Hose Number you wish to delete: " ) ;
	fflush( stdin ) ;
    scanf ("%d", &hoseNumber ) ;

	if( !GoodResult( EZInterface.GetPumpByNumber( pumpNumber, &pumpID ) ) ) 
		return ;

	if( !GoodResult( EZInterface.GetPumpHoseByNumber( pumpID, hoseNumber, &hoseID ) ) ) 
		return ;

	if( !GoodResult( EZInterface.DeleteHose( hoseID ) ) ) 
		return ;

	WriteMessage( "\n Hose delete successfully" ) ;

	EndMessage() ;
}

void GradeRegister()
{
	long	idx ;
	long	gradeCount ;
	long	gradeID ;
	long	newGradeID = 0 ;
	long	gradeNumber ;
	char	gradeName[MAX_PATH] ;
	char	gradeShortName[MAX_PATH] ;
	char	gradeCode[MAX_PATH] ;
	short	gradeType = 0x00 ;					//Other, Details, see GradeType at EZATG.ini file		

	WriteMessage( "\n Enter the Grade Number: " ) ;
    fflush( stdin ) ;
    scanf ("%d", &gradeNumber ) ;

	WriteMessage( "\n Enter the Grade Name: " ) ;
    fflush( stdin ) ;
    scanf ("%s", gradeName ) ;

	WriteMessage( "\n Enter the Grade ShortName: " ) ;
    fflush( stdin ) ;
    scanf ("%s", gradeShortName ) ;

	WriteMessage( "\n Enter the Grade Code: " ) ;
    fflush( stdin ) ;
    scanf ("%s", gradeCode ) ;

	BSTR	AttGradeName =	CharToBstr( gradeName ) ;
	BSTR	AttGradeShortName = CharToBstr( gradeShortName ) ;
	BSTR	AttGradeCode =	CharToBstr( gradeCode ) ;

	if( !GoodResult( EZInterface.GetGradesCount( &gradeCount ) ) )
		return ;

	for( idx = gradeCount ; idx > 0 ; idx-- )
	{
		if( !GoodResult( EZInterface.GetGradeByOrdinal( idx, &gradeID ) ) )
			return ;

		if( newGradeID < gradeID )
			newGradeID = gradeID ;
	}

	newGradeID++ ;

	if( !GoodResult( EZInterface.SetGradePropertiesEx(		newGradeID , gradeNumber, AttGradeName , 
															AttGradeShortName , AttGradeCode  , gradeType ) ) )
		return ;

	FREE_SYS_STR( AttGradeName ) ; 
	FREE_SYS_STR( AttGradeShortName ) ; 
	FREE_SYS_STR( AttGradeCode ) ; 

	WriteMessage( "\n Grade register successfully" ) ;

	EndMessage() ;
}

void DeleteGrade()
{
	long	gradeNumber ;
	long	gradeID ;

	WriteMessage( "\n Enter the grade number you wish to delete: " ) ;
	fflush( stdin ) ;
    scanf ("%d", &gradeNumber ) ;

	if( !GoodResult( EZInterface.GetGradeByNumber( gradeNumber, &gradeID ) ) ) 
		return ;

	if( !GoodResult( EZInterface.DeleteGrade( gradeID ) ) ) 
		return ;

	WriteMessage( "\n Grade delete successfully" ) ;

	EndMessage() ;
}

void TankRegistration()
{
	long	idx ;
	long	tankCount ;
	long	tankID ;
	long	newTankID = 0 ;
	long	tankNumber ;
	char	tankName[MAX_PATH] ;
	long	gradeID ;
	long	gradeNumber ;
	short	tankType = GAUGED_TANK_TYPE ;
	double  capacity ;
	double	diameter ; 					  
	double  theoVolume = 0 ; 
	double  gaugeVolume = 0 ; 
	double  gaugeTCVolume = 0 ; 
	double  gaugeUllage = 0 ;
	double  gaugeTemperature = 0 ; 			  
	double  gaugeLevel = 0 ;
	double  gaugeWaterVolume = 0 ;
	double  gaugeWaterLevel = 0 ;
	long	gaugeID = 5 ;
	long	probeNo ; 
	long	gaugeAlarmsMask = TANK_RESPONDING_BIT ;

	WriteMessage( "\n Enter the Tank Number: " ) ;
    fflush( stdin ) ;
    scanf ("%d", &tankNumber ) ;

	WriteMessage( "\n Enter the Tank Name: " ) ;
    fflush( stdin ) ;
    scanf ("%s", tankName ) ;

	WriteMessage( "\n Enter the Grade Number: " ) ;
    fflush( stdin ) ;
    scanf ("%d", &gradeNumber ) ;

	WriteMessage( "\n Enter the Capacity: " ) ;
    fflush( stdin ) ;
    scanf ("%lf", &capacity ) ;

	WriteMessage( "\n Enter the Diameter: " ) ;
    fflush( stdin ) ;
    scanf ("%lf", &diameter ) ;

	WriteMessage( "\n Enter the ProbeNo: " ) ;
    fflush( stdin ) ;
    scanf ("%d", &probeNo ) ;

	BSTR	AttTankName =	CharToBstr( tankName ) ;

	if( !GoodResult( EZInterface.GetTanksCount( &tankCount ) ) )
		return ;

	for( idx = tankCount ; idx > 0 ; idx-- )
	{
		if( !GoodResult( EZInterface.GetTankByOrdinal( idx, &tankID ) ) )
			return ;

		if( newTankID < tankID )
			newTankID = tankID ;
	}

	newTankID++ ;

	if( !GoodResult( EZInterface.GetGradeByNumber( gradeNumber, &gradeID ) ) )
		return ;

	if( !GoodResult( EZInterface.SetTankPropertiesEx(		newTankID , tankNumber, AttTankName, 
															gradeID , tankType , capacity , diameter , 
															theoVolume , gaugeVolume , gaugeTCVolume, 
															gaugeUllage, gaugeTemperature , gaugeLevel ,
															gaugeWaterVolume, gaugeWaterLevel, gaugeID, 
															( short ) probeNo , gaugeAlarmsMask ) ) )
		return ;

	FREE_SYS_STR( AttTankName ) ; 

	WriteMessage( "\n Tank register successfully" ) ;

	EndMessage() ;
}

void DeleteTank()
{
	long	tankNumber ;
	long	tankID ;

	WriteMessage( "\n Enter the Tank number you wish to delete: " ) ;
	fflush( stdin ) ;
    scanf ("%d", &tankNumber ) ;

	if( !GoodResult( EZInterface.GetTankByNumber( tankNumber, &tankID ) ) ) 
		return ;

	if( !GoodResult( EZInterface.DeleteGrade( tankID ) ) ) 
		return ;

	WriteMessage( "\n Tank delete successfully" ) ;

	EndMessage() ;
}

void ZigbeeDeviceRegistration() 
{
	long	idx ;
	long	ZBCount ;
	long	ZBID ; 
	long	newZBID = 0 ;
	long	ZBNumber ;
	char	ZBName[ MAX_PATH ] ;
	long	deviceType ;
	char	serialNumber[ MAX_PATH ] ;
	BSTR	nodeIdentifier = NULL ;
	long	portID = USB1 ;

	WriteMessage( "\n Enter the ZigBee Device Number: " ) ;
    fflush( stdin ) ;
    scanf ("%d", &ZBNumber ) ;

	WriteMessage( "\n Enter the ZigBee Device Name: " ) ;
    fflush( stdin ) ;
    scanf ("%s", ZBName ) ;

	WriteMessage( "\n 5 - EZRemote connected wirelessly. " ) ;
	WriteMessage( "\n 6 - 9  EZRemote 6(Slot1), 7(Slot2)..." ) ;
	WriteMessage( "\n Enter the ZigBee Device Type: " ) ;
    fflush( stdin ) ;
    scanf ("%d", &deviceType ) ;

	WriteMessage( "\n Enter the ZigBee Device Serial Number: " ) ;
    fflush( stdin ) ;
    scanf ("%s", serialNumber ) ;

	BSTR	AttZBName =	CharToBstr( ZBName ) ;
	BSTR	AttSerialNumber = CharToBstr( serialNumber ) ;

	if( !GoodResult( EZInterface.GetZigBeeCount( &ZBCount ) ) )
		return ;

	for( idx = ZBCount ; idx > 0 ; idx-- )
	{
		if( !GoodResult( EZInterface.GetZigBeeByOrdinal( idx, &ZBID ) ) )
			return ;

		if( newZBID < ZBID )
			newZBID = ZBID ;
	}

	newZBID++ ;

	if( !GoodResult( EZInterface.SetZigBeeProperties(		newZBID , ZBNumber , AttZBName , ( short ) deviceType ,
															AttSerialNumber , nodeIdentifier , portID ) ) )
		return ;

	FREE_SYS_STR( AttSerialNumber ) ;
	FREE_SYS_STR( AttZBName ) ; 

	WriteMessage( "\n ZigBee Device register successfully" ) ;

	EndMessage() ;
}

void DeleteZigbeeDevice()
{
	long	ZBNumber ;
	long	ZBID ;

	WriteMessage( "\n Enter the Zigbee Device number you wish to delete: " ) ;
	fflush( stdin ) ;
    scanf ("%d", &ZBNumber ) ;

	if( !GoodResult( EZInterface.GetZigBeeByNumber( ZBNumber, &ZBID ) ) ) 
		return ;

	if( !GoodResult( EZInterface.DeleteZigBee( ZBID ) ) ) 
		return ;

	WriteMessage( "\n Zigbee Device delete successfully" ) ;

	EndMessage() ;
}

//-----------------------------------------------------------------------------
// Functions converts 

static wchar_t* charToWChar( const char* text )
{
    size_t size = strlen( text ) + 1;
	wchar_t* wa = new wchar_t[size];

    mbstowcs( wa , text , size );

    return wa ;
}

PBSTR MakeBSTR( wchar_t* pStr , int Len )
{
	for ( int i = 0 ; i+1 < Len ; pStr[i++] = ' ' ) ;
	pStr[ Len - 1 ] = 0 ;
	return ( PBSTR ) pStr ;
}

BSTR CharToBstr(  const char* text )
{
    size_t size = strlen( text ) + 1;
	BSTR res =  SysAllocStringByteLen( NULL , size * sizeof( wchar_t ) ) ;
    mbstowcs( res , text , size );
	return res ;
}