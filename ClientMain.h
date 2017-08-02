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

#ifndef __EZ_CLIENT_MAIN__
#define __EZ_CLIENT_MAIN__

#define TIME_RECONECT		1500

// Login in EZTech
#define LOGIN_ID		15
#define	EVENT_PORT		5124
#define	CALL_PORT		5123
#define CALL_TIMEOUT	10000
#define EVENT_HANDLE	0
#define HANDLE_WINDOWS	0
#define WINDOWS_MSG		0

// Menu options
typedef enum TMenuType
{
	HELP_MENU,
	MAIN_MENU,
	BASIC_GOOD_PRATICE_MENU,
	DELIVERY_READING_MENU,
	PUMP_BASIC_CONTROL_MENU,
	PUMP_CONTROL_EXTENDED_MENU,
	PUMP_PRICE_CONTROL_MENU,
	REGISTER_ATTENDANT_MENU,
	REGISTER_CLIENT_MENU,
	TANKS_SENSORS_READING_MENU,
	GENERAL_CONFIGURATION_FACILITY_MENU
} TMenuType ;

//-----------------------------------------------------------------------------

#define WriteMessage printf
#define FREE_SYS_STR( b ) { if( b != NULL ) { SysFreeString( b ) ; b = NULL ; } ; }

//-----------------------------------------------------------------------------

extern bool runEvents ;
extern bool isConnected ;
extern char EZServerAddr[MAX_PATH] ;

extern int  appPump;
extern int  lastStatus[MAX_PATH] ;

//-----------------------------------------------------------------------------
void CreateAcess() ;
void MainLoop() ;
void CheckLogin() ;
char ShowMenu(int menuLevel) ;

static wchar_t* charToWChar(const char* text);
PBSTR MakeBSTR( wchar_t* pStr , int len );
BSTR CharToBstr(  const char* text ) ;

// Basic Good Practice
void BasicGoodPractice();

void SetDateTime() ;
void CheckServerVersion() ;
void CheckLicenseStatus() ;
void ClientLogoff() ;

// Deliveries Reading
void DeliveriesReading() ;

void ReadAllDeliveries() ;

// Pump Basic Control
void PumpBasicControl() ;

void TemporaryStop() ;
void ReAuthoriseDelivery() ;
void DisablePump() ;
void EnablePump() ;
void PumpsStatus() ;
void AuthoriseDelivery() ;
void CancelAuthorise() ;

// Pump Control Extended
void PumpControlExtended() ;

void ScheduledDelivery() ;
void PresetDelivery() ;

// Pump Price Control
void PumpPriceControl() ;

void ChangeHosePrice() ;
void ChangeGradePrice() ;

// Reading Cards Attendant and(or) Client
void ReadingCards() ;

// Register Attendant
void RegisterAttendant() ;

void AttendantRegistration() ;
void DeleteAttendant() ;

// Register Client
void RegisterClient() ;

void ClientRegistration() ;
void DeleteClient() ;

// Tanks and Sensors Reading
void TanksSensorsReading() ;

void TankReading() ;
void MonitoringSensor() ;

// General Configuration Facility
void GeneralConfigurationFacility() ;

void DeliveryPositionRegister() ;
void DeleteDeliveryPosition() ;
void HoseRegistration() ;
void DeleteHose() ;
void GradeRegister() ;
void DeleteGrade() ;
void TankRegistration() ;
void DeleteTank() ;
void ZigbeeDeviceRegistration() ;
void DeleteZigbeeDevice() ;

//-----------------------------------------------------------------------------
#endif // __EZ_CLIENT_MAIN__

