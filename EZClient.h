/*****************************************************************
 *
 * Copyright (C) 2009-2010 EZTech Tecnologia e Automação Ltda. 
 * All rights reserved.
 *
 * Created 01/06/2005
 * 
 * File: EZClient.h 
 * Desc: EZClient.DLL API definitions header file  
 *
 * Visual SourceSafe details 
 * =========================
 *
 * $Header: /EZController.root/EZController/EZClientDLL/EZClient.h 1     17/04/07 13:55 Kelvin Ussher $
 * $History: EZClient.h $
 * 
 * *****************  Version 1  *****************
 * User: Kelvin Ussher Date: 17/04/07   Time: 13:55
 * Created in $/EZController.root/EZController/EZClientDLL
 * 
 * 
 ******************************************************************/

#ifndef __EZ_CLIENT_INTERFACE__
#define __EZ_CLIENT_INTERFACE__

//-----------------------------------------------------------------------------
//#if defined( _LINUX_ )
#if defined(__linux__)

typedef time_t					DATE ;
typedef unsigned char			BYTE ;
typedef long long 				__int64 ;
typedef unsigned long long  	__uint64 ;
typedef wchar_t					BSTR ;
typedef wchar_t*				PBSTR ;
typedef wchar_t					WCHAR ;
typedef short					SHORT ;
typedef unsigned short			USHORT ;
typedef long					LONG ;
typedef unsigned long       	ULONG ;
typedef unsigned long       	LPARAM ;
typedef unsigned long			DWORD ;
typedef int						SOCKET ;
typedef void*					HANDLE ;
typedef HANDLE					HWND ;
typedef HANDLE					HMODULE ;
typedef HANDLE					HINSTANCE ;
typedef struct sockaddr			SOCKADDR ;
typedef void*					PVOID;
#define INVALID_HANDLE_VALUE	((HANDLE)(-1))
#define INVALID_SOCKET  		(SOCKET)(~0)
#define SOCKET_ERROR            (-1)
#define Sleep( s )				usleep( ( s ) * 1000 )
#define FALSE   				0
#define TRUE    				1

// OVJ - 31/10/2012
#define INT64  __int64

#define WINAPI
#define MAX_PATH 260

#define stricmp strcasecmp

#define LPTSTR char*

#define EZ_LIB_NAME "libezclient.so"

// OVJ - 19/10/2012 - Funcoes de carga dinamica de bibliotecas
#define  LoadLibrary(a)		dlopen(a, RTLD_LAZY)	// Carrega bibliote em memoria
#define  GetProcAddress		dlsym					// Encontra apontador para funcao
#define  FreeLibrary		dlclose					// Libera biblioteca da memoria

//-----------------------------------------------------------------------------
#elif defined(_WIN32)

typedef BSTR*			PBSTR ;

#define EZ_LIB_NAME "EZClient.dll"

#endif

	//--------------------------------------------------------------------------------------//

	// Declaracao das funcoes da API EZForecourt

	//--------------------------------- Connection -----------------------------------------//

typedef long (WINAPI *typeClientLogon) ( long ClientID , short ClientType , HANDLE Event , HWND hWnd, unsigned int wMsg) ;

typedef long (WINAPI *typeClientLogonEx) ( long ClientID , short ClientType , BSTR ServerName , unsigned short CallPortNo , unsigned short EventsPortNo , long CallTimeout, HANDLE Event , HWND hWnd, unsigned int wMsg) ;

typedef long (WINAPI *typeDllVersion) ( PBSTR pName ) ;

typedef long (WINAPI *typeServerVersion) ( PBSTR pName ) ;

typedef long (WINAPI *typeClientLogoff) () ;

typedef long (WINAPI *typeClientStatus) ( short* pPumpsReserved , short* pdeliveriesLocked ) ;

typedef long (WINAPI *typeTestConnection) () ;

typedef long (WINAPI *typeLicenseStatus) () ;

typedef long (WINAPI *typeGetLicenseType) ( short* pLicenseType ) ;

typedef long (WINAPI *typeGetIniValue) ( BSTR Section , BSTR Key , PBSTR pValue ) ; 

typedef long (WINAPI *typeSetIniValue) ( BSTR Section , BSTR Key , BSTR Value ) ; 

typedef long (WINAPI *typeGetClientsCount) ( long* pCount ) ;

typedef long (WINAPI *typeSetDateTime) ( DATE DateTime ) ;

typedef long (WINAPI *typeGetDateTime) ( DATE* pDateTime ) ;

typedef BSTR (WINAPI *typeResultString) ( long Res ) ;

typedef long (WINAPI *typeCheckSocketClosed) ( LPARAM lParam ) ;

	//--------------------------------- Events --------------------------------------------//

typedef long (WINAPI *typeProcessEvents) () ;

typedef long (WINAPI *typeGetEventsCount) ( long* pCount ) ;

typedef long (WINAPI *typeGetNextEventType) ( short* pType ) ;

typedef long (WINAPI *typeDiscardNextEvent) () ;

typedef long (WINAPI *typeGetNextPumpEvent) ( long* pPumpID, short* pPumpNumber, short* pState, short* pReservedFor, long* pReservedBy, long* pHoseID ,
							   short* pHoseNumber, long* pGradeID, PBSTR pGradeName , PBSTR pShortGradeName, short* pPriceLevel , double* pPrice, double* pVolume, 
							   double* pValue, short* pStackSize ) ;

typedef long (WINAPI *typeGetNextPumpEventEx) ( long* pPumpID, short* pPumpNumber, short* pState, short* pReservedFor, long* pReservedBy, long* pHoseID ,
							    short* pHoseNumber,long* pGradeID, PBSTR pGradeName , PBSTR pShortGradeName, short* pPriceLevel , double* pPrice, double* pVolume, 
							    double* pValue, short* pStackSize , PBSTR pPumpName , short* pPhysicalNumber , short* pSide , short* pAddress, 
								short* pPriceLevel1 , short* pPriceLevel2 , short* pType , long* pPortID , short* pAuthMode , short* pStackMode ,  short* pPrepayAllowed, 
								short* pPreauthAllowed ,  short* pPriceFormat , short* pValueFormat , short* pVolumeFormat  ) ;

typedef long (WINAPI *typeGetNextPumpEventEx2) ( long* pPumpID, long* pPumpNumber, short* pState, short* pReservedFor, long* pReservedBy, long* pHoseID ,
							    long* pHoseNumber,long* pHosePhysicalNumber,long* pGradeID,long* pGradeNumber, PBSTR pGradeName , PBSTR pShortGradeName, short* pPriceLevel , double* pPrice, double* pVolume, 
							    double* pValue, short* pStackSize , PBSTR pPumpName , long* pPhysicalNumber , short* pSide , short* pAddress, 
								short* pPriceLevel1 , short* pPriceLevel2 , short* pType , long* pPortID , short* pAuthMode , short* pStackMode ,  short* pPrepayAllowed, 
								short* pPreauthAllowed ,  short* pPriceFormat , short* pValueFormat , short* pVolumeFormat , __int64* pTag , 
							    long* pAttendantID , long* pAttendantNumber , PBSTR pAttendantName, __int64* pAttendantTag ,
							    long* pCardClientID , long* pCardClientNumber , PBSTR pCardClientName, __int64* pCardClientTag  ) ;

typedef long (WINAPI *typeGetNextPumpEventEx3) ( long* pPumpID, long* pPumpNumber, short* pState, short* pReservedFor, long* pReservedBy, long* pHoseID ,
							    long* pHoseNumber,long* pHosePhysicalNumber,long* pGradeID,long* pGradeNumber, PBSTR pGradeName , PBSTR pShortGradeName, short* pPriceLevel , double* pPrice, double* pVolume, 
							    double* pValue, short* pStackSize , PBSTR pPumpName , long* pPhysicalNumber , short* pSide , short* pAddress, 
								short* pPriceLevel1 , short* pPriceLevel2 , short* pType , long* pPortID , short* pAuthMode , short* pStackMode ,  short* pPrepayAllowed, 
								short* pPreauthAllowed ,  short* pPriceFormat , short* pValueFormat , short* pVolumeFormat , __int64* pTag , 
							    long* pAttendantID , long* pAttendantNumber , PBSTR pAttendantName, __int64* pAttendantTag ,
							    long* pCardClientID , long* pCardClientNumber , PBSTR pCardClientName, __int64* pCardClientTag , 
								double* pCurFlowRate , double* pPeakFlowRate ) ;

typedef long (WINAPI *typeGetNextDeliveryEvent) ( long* pDeliveryID , long* pPumpID, short* pPumpNumber, long* pHoseID, short* pHoseNumber , 
								long* pGradeID, PBSTR pGradeName ,PBSTR pShortGradeName, short* pPriceLevel, double* pPrice, double* pVolume, double* pValue, 
								short* pDeliveryState , short* pDeliveryType, long* pLockedBy, long* pReservedBy, long* pAge , DATE* pCompletedDT , long* pAttendantID ) ;

typedef long (WINAPI *typeGetNextDeliveryEventEx) ( long* pDeliveryID , long* pPumpID, short* pPumpNumber, long* pHoseID, short* pHoseNumber , 
								    long* pGradeID, PBSTR pGradeName , PBSTR pShortGradeName, short* pPriceLevel, double* pPrice, 
									double* pVolume, double* pValue, short* pDeliveryState , 
								    short* pDeliveryType, long* pLockedBy, long* pReservedBy, long* pAge , DATE* pCompletedDT , 
									long* pAttendantID, double* pVolumeETot , double* pVolume2ETot , double* pValueETot , __int64* pTag) ;

typedef long (WINAPI *typeGetNextDeliveryEventEx2) ( long* pDeliveryID , long* pPumpID, short* pPumpNumber, long* pHoseID, short* pHoseNumber , 
								     long* pGradeID, PBSTR pGradeName , PBSTR pShortGradeName, short* pPriceLevel, double* pPrice, 
									 double* pVolume, double* pValue, short* pDeliveryState , 
								     short* pDeliveryType, long* pLockedBy, long* pReservedBy, long* pAge , DATE* pCompletedDT , 
									 long* pAttendantID, 
									 double* pOldVolumeETot , double* pOldVolume2ETot , double* pOldValueETot , 
									 double* pNewVolumeETot , double* pNewVolume2ETot , double* pNewValueETot , 
									 __int64* pTag , long* pDuration ) ;

typedef long (WINAPI *typeGetNextDeliveryEventEx3) ( long* pDeliveryID, 
									    long* pHoseID ,long* pHoseNumber, long* pHosePhysicalNumber,
	  									long* pPumpID, long* pPumpNumber, PBSTR pPumpName,
										long* pTankID, long* pTankNumber, PBSTR pTankName,
										long* pGradeID , long* pGradeNumber, PBSTR pGradeName, PBSTR pGradeShortName, PBSTR pGradeCode,
										short* pState ,short* pType ,double* pVolume ,short* pPriceLevel ,
										double* pPrice ,double* pValue ,double* pVolume2 , DATE* pCompletedDT , long* pLockedBy, 
										long* pReservedBy , long* pAttendantID , long* pAge , DATE* pClearedDT , 
										double* pOldVolumeETot, double* pOldVolume2ETot, double* pOldValueETot , 
										double* pNewVolumeETot, double* pNewVolume2ETot, double* pNewValueETot , 
										__int64* pTag , long* pDuration , long* pAttendantNumber , PBSTR AttendantName, __int64* pAttendantTag ,
										long* pCardClientID , long* pCardClientNumber , PBSTR CardClientName, __int64* pCardClientTag ) ; 

typedef long (WINAPI *typeGetNextDeliveryEventEx4) ( long* pDeliveryID, long* pHoseID ,long* pHoseNumber, long* pHosePhysicalNumber,
	  									long* pPumpID, long* pPumpNumber, PBSTR pPumpName,
										long* pTankID, long* pTankNumber, PBSTR pTankName,
										long* pGradeID , long* pGradeNumber, PBSTR pGradeName, PBSTR pGradeShortName, PBSTR pGradeCode,
										short* pState ,short* pType ,double* pVolume ,short* pPriceLevel ,
										double* pPrice ,double* pValue ,double* pVolume2 , DATE* pCompletedDT , long* pLockedBy, 
										long* pReservedBy , long* pAttendantID , long* pAge , DATE* pClearedDT , 
										double* pOldVolumeETot, double* pOldVolume2ETot, double* pOldValueETot , 
										double* pNewVolumeETot, double* pNewVolume2ETot, double* pNewValueETot , 
										__int64* pTag , long* pDuration , long* pAttendantNumber , PBSTR AttendantName, __int64* pAttendantTag ,
										long* pCardClientID , long* pCardClientNumber , PBSTR CardClientName, __int64* pCardClientTag , double* pPeakFlowRate) ;

typedef long (WINAPI *typeGetNextServerEvent) ( long* pEventID, PBSTR pEventText ) ;

typedef long (WINAPI *typeGetNextClientEvent) ( long* pClientID, long* pEventID, PBSTR pEventText ) ;

typedef long (WINAPI *typeFireClientEvent) ( long EventID , BSTR EventStr ) ;

typedef long (WINAPI *typeGetNextDBLogEvent) ( long* pEventType , long* pDeviceID , DATE* pEventDT , PBSTR pEventText ) ;

typedef long (WINAPI *typeGetNextDBLogDeliveryEvent) ( long* pDeliveryID, long* pHoseID, short* pDeliveryState, short* pDeliveryType, double* pVolume, short* pPriceLevel, 
							 double* pPrice, double* pValue, double* pVolume2, long* pReservedBy , long* pAttendantID, DATE* pDeliveryDT ) ;

typedef long (WINAPI *typeGetNextDBClearDeliveryEvent) ( long* pDeliveryID, short* pDeliveryType, long* pClearedBY, DATE* pClearedDT, long* pAttendantID ) ;

typedef long (WINAPI *typeGetNextZB2GStatusEvent) ( long* pPortID , __int64* pZBAddress, short* pLQI , short* pRSSI , __int64*  pParZBAddress, short* pZBChannel , short* pMemBlocks , short* pMemFree ) ;

typedef long (WINAPI *typeGetNextDBStackDeliveryEvent) ( long* pDeliveryID ) ;

typedef long (WINAPI *typeGetNextDBHoseETotalsEvent) ( long* pHoseID , double* pVolume , double* pValue , double* pVolumeETot , double* pValueETot ) ;

typedef long (WINAPI *typeGetNextDBHoseETotalsEventEx) ( long* pHoseID , double* pVolume , double* pValue , double* pVolumeETot , double* pValueETot , 
										 long* pHoseNumber , long* pHosePhysicalNumber , long* pPumpID , long* pPumpNumber , PBSTR pPumpName , 
										 long* pTankID , long* pTankNumber , PBSTR pTankName , long* pGradeID , PBSTR pGradeName ) ;

typedef long (WINAPI *typeGetNextDBTriggerEvent) ( long* pTableID ,long* pRowID ) ;

typedef long (WINAPI *typeGetNextDBAttendantLogonEvent) ( long* pAttendantID ,long* pPumpID ) ;

typedef long (WINAPI *typeGetNextDBAttendantLogoffEvent) ( long* pAttendantID ) ;

typedef long (WINAPI *typeGetNextDBTankStatusEvent) ( long* pTankID , double* pGaugeVolume , double* pGaugeTCVolume, double* pGaugeUllage, double* pGaugeTemperature , 
							          double* pGaugeLevel , double* pGaugeWaterVolume, double* pGaugeWaterLevel ) ;

typedef long (WINAPI *typeGetNextDBTankStatusEventEx) ( long* pTankID , double* pGaugeVolume , double* pGaugeTCVolume, double* pGaugeUllage, 
									    double* pGaugeTemperature , double* pGaugeLevel , double* pGaugeWaterVolume, double* pGaugeWaterLevel , 
									    long* pTankNumber , PBSTR pTankName , long* pGradeID , PBSTR pGradeName , short*pType  , 
										double* pCapacity , double* pDiameter , long* pGaugeID , short* pProbeNo) ;

typedef long (WINAPI *typeGetNextDBTankStatusEventEx2) ( long* pTankID , double* pGaugeVolume , double* pGaugeTCVolume, double* pGaugeUllage, 
									    double* pGaugeTemperature , double* pGaugeLevel , double* pGaugeWaterVolume, double* pGaugeWaterLevel , 
									    long* pTankNumber , PBSTR pTankName , long* pGradeID , PBSTR pGradeName , short*pType  , double* pCapacity , 
										double* pDiameter , long* pGaugeID , short* pProbeNo , short* pState , long* pAlarmsMask ) ;

typedef long (WINAPI *typeGetNextCardReadEvent) ( long* pCardReadID , long* pNumber, PBSTR pName , long* pPumpID , short* pType , long* pParentID , __int64* pTag , DATE* pTimeStamp) ;

typedef long (WINAPI *typeGetNextLogEventEvent) ( long* pLogEventID , short* pDeviceType , long* pDeviceID , long* pDeviceNumber ,	PBSTR pDeviceName , short* pEventLevel ,
								     short* pEventType , PBSTR pEventDesc , DATE* pGeneratedDT , DATE* pClearedDT , long* pClearedBy , long* pAckedBy , double* pVolume ,
								     double* pValue , double* pProductVolume , double* pProductLevel , double* pWaterLevel , double* pTemperature) ;

typedef long (WINAPI *typeGetNextZeroDeliveryEvent) ( long* pPumpID , long* pPumpNumber , long* pHoseID , long* pHoseNumber ) ;

	//--------------------------------- Pumps ---------------------------------------------//

typedef long (WINAPI *typeGetPumpsCount) ( long* pCount ) ;

typedef long (WINAPI *typeGetPumpByNumber) ( long Number , long* pID ) ;

typedef long (WINAPI *typeGetPumpByName) ( BSTR Name , long* pID ) ;

typedef long (WINAPI *typeGetPumpByOrdinal) ( long Index , long* pID ) ;

typedef long (WINAPI *typeGetPumpProperties) (long ID , long* pNumber, PBSTR pName, short* pPhysicalNumber, short* pSide, short* pAddress, short* pPriceLevel1,
						      short* pPriceLevel2, short* pPriceDspFormat, short* pVolumeDspFormat, short* pValueDspFormat, short* pType,  
						      long* pPortID, long* pAttendantID, short* pAuthMode, short* pStackMode, short* pPrepayAllowed, short* pPreauthAllowed ) ;

typedef long (WINAPI *typeGetPumpPropertiesEx) (long ID , long* pNumber, PBSTR pName, short* pPhysicalNumber, short* pSide, short* pAddress, short* pPriceLevel1,
						      short* pPriceLevel2, short* pPriceDspFormat, short* pVolumeDspFormat, short* pValueDspFormat, short* pType, 
						      long* pPortID, long* pAttendantID, short* pAuthMode, short* pStackMode, short* pPrepayAllowed, short* pPreauthAllowed ,
							  long* pSlotZigBeeID , long* pMuxSlotZigBeeID , short* pPriceControl , short* pHasPreset) ;

typedef long (WINAPI *typeSetPumpProperties) (long ID , long Number, BSTR Name, short PhysicalNumber, short Side, short Address, short PriceLevel1,
						      short PriceLevel2, short PriceDspFormat, short VolumeDspFormat, short ValueDspFormat, short Type, 
						      long PortID, long AttendantID, short AuthMode, short StackMode, short PrepayAllowed, short PreauthAllowed ) ;

typedef long (WINAPI *typeSetPumpPropertiesEx) (long ID , long Number, BSTR Name, short PhysicalNumber, short Side, short Address, short PriceLevel1,
						        short PriceLevel2, short PriceDspFormat, short VolumeDspFormat, short ValueDspFormat, short Type, 
						        long PortID, long AttendantID, short AuthMode, short StackMode, short PrepayAllowed, short PreauthAllowed , 
								long SlotZigBeeID , long MuxSlotZigBeeID , short PriceControl , short HasPreset ) ;

typedef long (WINAPI *typeDeletePump) ( long ID ) ;

typedef long (WINAPI *typeGetPumpHosesCount) ( long ID , long* pCount ) ;

typedef long (WINAPI *typeGetPumpHoseByNumber) ( long ID , long Number , long* pHoseID ) ;

typedef long (WINAPI *typeGetPumpStatus) ( long ID , short* pState, short* pReservedFor, long* pReservedBy, long* pHoseID, short* pHoseNumber , 
						   long* pGradeID, PBSTR pGradeName ,PBSTR pShortGradeName, short* pPriceLevel , double* pPrice , double* pVolume , double* pValue , 
						   short* pStackSize ) ;

typedef long (WINAPI *typeGetPumpStatusEx) ( long ID , long* pPumpNumber, PBSTR pPumpName, long* pPhysicalNumber, short* pState, short* pReservedFor, long* pReservedBy, long* pHoseID, long* pHoseNumber , long* pHosePhysicalNumber , long* pGradeID , 
						     long* pGradeNumber , PBSTR pGradeName, PBSTR pShortGradeName, short* pPriceLevel , double* pPrice , double* pVolume , double* pValue , 
						     short* pStackSize , __int64* pTag , 
						     long* pAttendantID , long* pAttendantNumber , PBSTR pAttendantName, __int64* pAttendantTag ,
						     long* pCardClientID , long* pCardClientNumber , PBSTR pCardClientName, __int64* pCardClientTag ) ;

typedef long (WINAPI *typeGetPumpStatusEx2) ( long ID , long* pPumpNumber, PBSTR pPumpName, long* pPhysicalNumber, short* pState, short* pReservedFor, long* pReservedBy, long* pHoseID, long* pHoseNumber , long* pHosePhysicalNumber , long* pGradeID , 
							  long* pGradeNumber , PBSTR pGradeName, PBSTR pShortGradeName, short* pPriceLevel , double* pPrice , double* pVolume , double* pValue , short* pStackSize , __int64* pTag , 
							  long* pAttendantID , long* pAttendantNumber , PBSTR pAttendantName, __int64* pAttendantTag ,
							  long* pCardClientID , long* pCardClientNumber , PBSTR pCardClientName, __int64* pCardClientTag , 
							  double* pCurFlowRate , double* pPeakFlowRate ) ;

typedef BSTR (WINAPI *typePumpStateString) ( short State ) ;

typedef long (WINAPI *typeEnablePump) ( long ID ) ;

typedef long (WINAPI *typeDisablePump) ( long ID ) ;

typedef long (WINAPI *typeSetPumpDefaultPriceLevel) ( long ID , short Level ) ;

typedef long (WINAPI *typeGetDensity) ( long ID , double* pDensity) ;

typedef long (WINAPI *typeScheduleBeep) ( long ID , short Pitch1 , short Duration1 , short Pitch2 , short Duration2, short Pitch3 , short Duration3, short Pitch4 , short Duration4, short Pitch5 , short Duration5  ) ;

typedef long (WINAPI *typeFlashLEDS) ( long ID , short Side , short PeriodMs , short Cycles )  ;

	//--------------------------- Pump prepay deliveries ----------------------------------//

typedef long (WINAPI *typePrepayReserve) ( long ID ) ;

typedef long (WINAPI *typePrepayCancel) ( long ID  ) ;

typedef long (WINAPI *typePrepayAuthorise) ( long ID , short LimitType , double Value , short Hose , short PriceLevel ) ;

	//--------------------------- Pump preauth deliveries ---------------------------------//

typedef long (WINAPI *typePreauthReserve) ( long ID ) ;

typedef long (WINAPI *typePreauthCancel) ( long ID ) ;

typedef long (WINAPI *typePreauthAuthorise) ( long ID , short LimitType , double Value , short Hose , short PriceLevel ) ;

	//--------------------------- Pump payment deliveries ---------------------------------//

typedef long (WINAPI *typePaymentReserve) ( long ID , long TermID , BSTR TermHash ) ;

typedef long (WINAPI *typePaymentCancel) ( long ID , long TermID , BSTR TermHash ) ;

typedef long (WINAPI *typePaymentAuthorise) ( long ID , long TermID , BSTR TermHash ,
							  long AttendantID , __int64 AttendantTag , 
							  long CardClientID , __int64 CardClientTag , 
							  short AuthType , __int64 ExtTag ,
							  short GradeType , short PriceType  , short PriceLevel , double Price , 
							  short PresetType , double Value , short Hose ,
							  double Odometer , double Odometer2 , BSTR Plate , 
							  BSTR ExtTransactionID , BSTR DriverID , BSTR AuthorisationID ) ;

	//------------------------------ Pump authorization ------------------------------------//

typedef long (WINAPI *typeAttendantAuthorise) ( long ID , long AttendantID ) ;

typedef long (WINAPI *typeAuthorise) ( long ID ) ;

typedef long (WINAPI *typeCancelAuthorise) ( long ID ) ;

typedef long (WINAPI *typeTempStop) ( long ID ) ;

typedef long (WINAPI *typeReAuthorise) ( long ID ) ;

typedef long (WINAPI *typeTerminateDelivery) ( long ID ) ;

typedef long (WINAPI *typeLoadPreset) ( long ID , short LimitType , double Value , short Hose , short PriceLevel ) ;

typedef long (WINAPI *typeLoadPresetWithPrice) ( long ID , short LimitType , double Value , short Hose , short PriceLevel , double Price ) ;

typedef long (WINAPI *typeTagAuthorise) ( long ID , __int64 Tag , short LimitType , double Value , short Hose , short PriceLevel ) ;

	//-------------------------------- Global functions ------------------------------------//

typedef long (WINAPI *typeAllStop) () ;

typedef long (WINAPI *typeAllAuthorise) () ;

typedef long (WINAPI *typeAllReAuthorise) () ;

typedef long (WINAPI *typeAllStopIfIdle) () ;

typedef long (WINAPI *typeReadAllTanks) () ;

typedef long (WINAPI *typeGetAllPumpStatuses) ( PBSTR pStates , PBSTR pCurrentHoses , PBSTR pDeliveriesCount ) ; 

	//------------------------------------ Deliveries --------------------------------------//

typedef long (WINAPI *typeGetDeliveriesCount) ( long* pCount ) ;

typedef long (WINAPI *typeGetDeliveryByOrdinal) ( long Index , long* pID ) ;

typedef long (WINAPI *typeGetDeliveryProperties) ( long ID , long* pHoseID ,short* pState ,short* pType ,double* pVolume ,
								   short* pPriceLevel ,double* pPrice ,double* pValue ,double* pVolume2 , 
								   DATE* pCompletedDT ,long* pLockedBy , long* pReservedBy , long* pAttendantID , long* pAge ) ;

typedef long (WINAPI *typeGetDeliveryPropertiesEx) ( long ID , long* pHoseID ,short* pState ,short* pType ,double* pVolume ,short* pPriceLevel ,
									 double* pPrice ,double* pValue ,double* pVolume2 , DATE* pCompletedDT ,long* pLockedBy, long* pReservedBy , long* pAttendantID , long* pAge , 
									 DATE* pClearedDT ,double* pVolumeETot , double* pVolume2ETot , double* pValueETot , __int64* pTag ) ;

typedef long (WINAPI *typeGetDeliveryPropertiesEx2) ( long ID , long* pHoseID ,short* pState ,short* pType ,double* pVolume ,short* pPriceLevel ,
									  double* pPrice ,double* pValue ,double* pVolume2 , DATE* pCompletedDT ,long* pLockedBy, long* pReservedBy , 
									  long* pAttendantID , long* pAge , DATE* pClearedDT ,
									  double* pOldVolumeETot , double* pOldVolume2ETot , double* pOldValueETot , 
									  double* pNewVolumeETot , double* pNewVolume2ETot , double* pNewValueETot , 
									  __int64* pTag , long * pDuration ) ;

typedef long (WINAPI *typeGetDeliveryPropertiesEx3) ( long ID , long* pHoseID ,short* pState ,short* pType ,double* pVolume ,short* pPriceLevel ,
									  double* pPrice ,double* pValue ,double* pVolume2 , DATE* pCompletedDT ,long* pLockedBy, long* pReservedBy , 
									  long* pAttendantID , long* pAge , DATE* pClearedDT ,
									  double* pOldVolumeETot , double* pOldVolume2ETot , double* pOldValueETot , 
									  double* pNewVolumeETot , double* pNewVolume2ETot , double* pNewValueETot , 
									  __int64* pTag , long * pDuration , long* pCardClientID ) ;

typedef long (WINAPI *typeGetDeliveryPropertiesEx4) ( long ID , long* pHoseID ,short* pState ,short* pType ,double* pVolume ,short* pPriceLevel ,
									  double* pPrice ,double* pValue ,double* pVolume2 , DATE* pCompletedDT ,long* pLockedBy, long* pReservedBy , 
									  long* pAttendantID , long* pAge , DATE* pClearedDT ,
									  double* pOldVolumeETot , double* pOldVolume2ETot , double* pOldValueETot , 
									  double* pNewVolumeETot , double* pNewVolume2ETot , double* pNewValueETot , 
									  __int64* pTag , long * pDuration , long* pCardClientID , double* pPeakFlowRate ) ;

typedef long (WINAPI *typeSetDeliveryProperties) ( long ID , long HoseID ,short State ,short Type ,double Volume ,short PriceLevel ,
								   double Price ,double Value ,double Volume2, DATE CompletedDT ,long LockedBy, long ReservedBy , 
								   long AttendantID , long Age ) ;

typedef long (WINAPI *typeSetDeliveryPropertiesEx) ( long ID , long HoseID ,short State ,short Type ,double Volume ,short PriceLevel ,
									 double Price ,double Value ,double Volume2 , DATE CompletedDT ,long LockedBy, 
									 long ReservedBy , long AttendantID , long Age , DATE ClearedDT , double VolumeETot , double Volume2ETot , double ValueETot , __int64 Tag ) ;

typedef long (WINAPI *typeSetDeliveryPropertiesEx2) ( long ID , long HoseID ,short State ,short Type ,double Volume ,short PriceLevel ,
									  double Price ,double Value ,double Volume2 , DATE CompletedDT ,long LockedBy, 
									  long ReservedBy , long AttendantID , long Age , DATE ClearedDT , 
								      double OldVolumeETot , double OldVolume2ETot , double OldValueETot , 
								      double NewVolumeETot , double NewVolume2ETot , double NewValueETot , 
								      __int64 Tag , long Duration ) ;

typedef long (WINAPI *typeSetDeliveryPropertiesEx3) ( long ID , long HoseID ,short State ,short Type ,double Volume ,short PriceLevel ,
								      double Price ,double Value ,double Volume2 , DATE CompletedDT ,long LockedBy, 
								      long ReservedBy , long AttendantID , long Age , DATE ClearedDT , 
								      double OldVolumeETot , double OldVolume2ETot , double OldValueETot , 
								      double NewVolumeETot , double NewVolume2ETot , double NewValueETot , 
								      __int64 Tag , long Duration , long CardClientID ) ; 

typedef long (WINAPI *typeSetDeliveryPropertiesEx4) ( long ID , long HoseID ,short State ,short Type ,double Volume ,short PriceLevel ,
									  double Price ,double Value ,double Volume2 , DATE CompletedDT ,long LockedBy, 
									  long ReservedBy , long AttendantID , long Age , DATE ClearedDT , 
									  double OldVolumeETot , double OldVolume2ETot , double OldValueETot , 
									  double NewVolumeETot , double NewVolume2ETot , double NewValueETot , 
									  __int64 Tag , long Duration , long CardClientID, double PeakFlowRate ) ;

typedef long (WINAPI *typeLockDelivery) ( long ID ) ;

typedef long (WINAPI *typeUnlockDelivery) ( long ID ) ;

typedef long (WINAPI *typeClearDelivery) ( long ID , short Type ) ;

typedef long (WINAPI *typeLockAndClearDelivery) ( long ID , short Type ) ;

typedef long (WINAPI *typeSetNextDeliveryID) ( long ID ) ;

typedef long (WINAPI *typeAckDeliveryDBLog) ( long ID ) ; 

typedef long (WINAPI *typeGetDeliveryIDByOrdinalNotLogged) ( long Ordinal , long* pID ) ;

typedef long (WINAPI *typeGetDeliveriesCountNotLogged) ( long* pCount ) ;

typedef long (WINAPI *typeAckDeliveryVolLog) ( long ID ) ; 

typedef long (WINAPI *typeGetDeliveryIDByOrdinalNotVolLogged) ( long Ordinal , long* pID ) ;

typedef long (WINAPI *typeGetDeliveriesCountNotVolLogged) ( long* pCount ) ;

typedef long (WINAPI *typeGetAllDeliveriesCount) ( long* pCount )  ;

typedef long (WINAPI *typeGetAllDeliveryByOrdinal) ( long Index , long* pID ) ;

typedef long (WINAPI *typeGetDeliverySummary) ( long ID , long* pHoseID ,long* pHoseNumber, long* pHosePhysicalNumber,
	  							long* pPumpID, long* pPumpNumber, PBSTR  pPumpName,
								long* pTankID, long* pTankNumber, PBSTR  pTankName,
								long* pGradeID , long* pGradeNumber, PBSTR  pGradeName, PBSTR  pGradeShortName, PBSTR  pGradeCode,
							    short* pState ,short* pType ,double* pVolume ,short* pPriceLevel ,
								double* pPrice ,double* pValue ,double* pVolume2 , DATE* pCompletedDT ,long* pLockedBy, 
								long* pReservedBy , long* pAttendantID , long* pAge ,DATE* pClearedDT ,
								double* pVolumeETot , double* pVolume2ETot , double* pValueETot , __int64* pTag) ;

typedef long (WINAPI *typeGetDeliverySummaryEx) ( long ID , long* pHoseID ,long* pHoseNumber, long* pHosePhysicalNumber,
	  							long* pPumpID, long* pPumpNumber, PBSTR  pPumpName,
								long* pTankID, long* pTankNumber, PBSTR  pTankName,
								long* pGradeID , long* pGradeNumber, PBSTR  pGradeName, PBSTR  pGradeShortName, PBSTR  pGradeCode,
							    short* pState ,short* pType ,double* pVolume ,short* pPriceLevel ,
		                        double* pPrice ,double* pValue ,double* pVolume2 , DATE* pCompletedDT ,long* pLockedBy, 
								long* pReservedBy , long* pAttendantID , long* pAge ,DATE* pClearedDT ,
								double* pOldVolumeETot , double* pOldVolume2ETot , double* pOldValueETot , 
								double* pNewVolumeETot , double* pNewVolume2ETot , double* pNewValueETot , 
								__int64* pTag , long * pDuration ) ;

typedef long (WINAPI *typeGetDeliverySummaryEx2) ( long ID , long* pHoseID ,long* pHoseNumber, long* pHosePhysicalNumber,
	  							long* pPumpID, long* pPumpNumber, PBSTR  pPumpName,
								long* pTankID, long* pTankNumber, PBSTR  pTankName,
								long* pGradeID , long* pGradeNumber, PBSTR  pGradeName, PBSTR  pGradeShortName, PBSTR  pGradeCode,
							    short* pState ,short* pType ,double* pVolume ,short* pPriceLevel ,
		                        double* pPrice ,double* pValue ,double* pVolume2 , DATE* pCompletedDT ,long* pLockedBy, 
								long* pReservedBy , long* pAttendantID , long* pAge ,DATE* pClearedDT ,
								double* pOldVolumeETot , double* pOldVolume2ETot , double* pOldValueETot , 
								double* pNewVolumeETot , double* pNewVolume2ETot , double* pNewValueETot , 
								__int64* pTag , long * pDuration , long* pAttendantNumber , PBSTR pAttendantName, __int64* pAttendantTag ,
								long* pCardClientID , long* pCardClientNumber , PBSTR pCardClientName, __int64* pCardClientTag ) ;

typedef long (WINAPI *typeGetDeliverySummaryEx3) ( long ID , long* pHoseID ,long* pHoseNumber, long* pHosePhysicalNumber,
	  							long* pPumpID, long* pPumpNumber, PBSTR  pPumpName,
								long* pTankID, long* pTankNumber, PBSTR  pTankName,
								long* pGradeID , long* pGradeNumber, PBSTR  pGradeName, PBSTR  pGradeShortName, PBSTR  pGradeCode,
							    short* pState ,short* pType ,double* pVolume ,short* pPriceLevel ,
		                        double* pPrice ,double* pValue ,double* pVolume2 , DATE* pCompletedDT ,long* pLockedBy, 
								long* pReservedBy , long* pAttendantID , long* pAge ,DATE* pClearedDT ,
								double* pOldVolumeETot , double* pOldVolume2ETot , double* pOldValueETot , 
								double* pNewVolumeETot , double* pNewVolume2ETot , double* pNewValueETot , 
								__int64* pTag , long * pDuration , long* pAttendantNumber , PBSTR pAttendantName, __int64* pAttendantTag ,
								long* pCardClientID , long* pCardClientNumber , PBSTR pCardClientName, __int64* pCardClientTag  , double* pPeakFlowRate ) ;

typedef long (WINAPI *typeSetDeliveryExt) ( long ID , BSTR Plate , double Odometer , double Odometer2 , BSTR ExtTransactionID , BSTR DriverID , BSTR AuthID, short AuthType ) ;

typedef long (WINAPI *typeGetDeliveryExt) ( long ID , PBSTR pPlate , double* pOdometer , double* pOdometer2 , PBSTR pExtTransactionID , PBSTR pDriverID , PBSTR pAuthID, short* pAuthType ) ;

typedef long (WINAPI *typeGetPumpDeliveryProperties) ( long ID , short Index , long* pDeliveryID , short* pType , short* pState, long* pHoseID, 
									   short* pHoseNum, long* pGradeID , PBSTR pGradeName ,PBSTR pShortGradeName, short* pPriceLevel, double* pPrice , double* pVolume , 
									   double* pValue , long* pLockedBy , long* pReservedBy , long* pAge , DATE* pCompletedDT , long* pAttendantID  ) ;

typedef long (WINAPI *typeGetPumpDeliveryPropertiesEx) ( long ID , short Index , long* pDeliveryID , short* pType , short* pState, 
										long* pHoseID, short* pHoseNum, 
										long* pGradeID, PBSTR pGradeName, PBSTR pShortGradeName , 
								        short* pPriceLevel, double* pPrice , double* pVolume , double* pValue , long* pLockedBy , 
									    long* pReservedBy , long* pAge , DATE* pCompletedDT , long* pAttendantID , 
										double* pVolumeETot , double* pVolume2ETot , double* pValueETot , __int64* pTag ) ;

typedef long (WINAPI *typeGetPumpDeliveryPropertiesEx2) ( long ID , short Index , long* pDeliveryID , short* pType , short* pState, long* pHoseID, short* pHoseNum, long* pGradeID, PBSTR pGradeName, PBSTR pShortGradeName , 
								        short* pPriceLevel, double* pPrice , double* pVolume , double* pValue , long* pLockedBy , 
									    long* pReservedBy , long* pAge , DATE* pCompletedDT , long* pAttendantID , 
										double* pOldVolumeETot , double* pOldVolume2ETot , double* pOldValueETot , 
										double* pNewVolumeETot , double* pNewVolume2ETot , double* pNewValueETot , 
										__int64* pTag , long * pDuration ) ;

typedef long (WINAPI *typeGetPumpDeliveryPropertiesEx3) ( long ID , short Index , long* pDeliveryID , short* pType , short* pState, 
										 long* pHoseID, short* pHoseNum, 
										 long* pGradeID, PBSTR pGradeName, PBSTR pShortGradeName , 
								        short* pPriceLevel, double* pPrice , double* pVolume , double* pValue , long* pLockedBy , 
									    long* pReservedBy , long* pAge , DATE* pCompletedDT , long* pAttendantID , 
										double* pOldVolumeETot , double* pOldVolume2ETot , double* pOldValueETot , 
										double* pNewVolumeETot , double* pNewVolume2ETot , double* pNewValueETot , 
										__int64* pTag , long * pDuration , long* pCardClientID ) ;

typedef long (WINAPI *typeGetPumpDeliveryPropertiesEx4) ( long ID , short Index , long* pDeliveryID , short* pType , short* pState, long* pHoseID, short* pHoseNum, long* pGradeID, PBSTR pGradeName, PBSTR pShortGradeName , 
								        short* pPriceLevel, double* pPrice , double* pVolume , double* pValue , long* pLockedBy , 
									    long* pReservedBy , long* pAge , DATE* pCompletedDT , long* pAttendantID , 
										double* pOldVolumeETot , double* pOldVolume2ETot , double* pOldValueETot , 
										double* pNewVolumeETot , double* pNewVolume2ETot , double* pNewValueETot , 
										__int64* pTag , long * pDuration , long* pCardClientID , double* pPeakFlowRate  ) ;

typedef BSTR (WINAPI *typeReserveTypeString) ( short Type ) ;

typedef long (WINAPI *typeGetDuration) ( long ID , long* pDuration ) ;

typedef long (WINAPI *typeStackCurrentDelivery) ( long ID ) ;

typedef BSTR (WINAPI *typeDeliveryStateString) ( short State ) ;

typedef BSTR (WINAPI *typeDeliveryTypeString) ( short Type ) ;

	//-------------------------------------- Hoses -----------------------------------------//

typedef long (WINAPI *typeGetHosesCount) ( long* pCount ) ;

typedef long (WINAPI *typeGetHoseByOrdinal) ( long Index , long* pID ) ;

typedef long (WINAPI *typeGetHoseProperties) (long ID , long* pNumber, long* pPumpID ,long* pTankID , long* pPhysicalNumber, 
		                      double* pMtrTheoValue , double* pMtrTheoVolume ,double* pMtrElecValue ,
				              double* pMtrElecVolume ) ;

typedef long (WINAPI *typeGetHosePropertiesEx) (long ID , long* pNumber, long* pPumpID ,long* pTankID , long* pPhysicalNumber, 
		                      double* pMtrTheoValue , double* pMtrTheoVolume ,double* pMtrElecValue ,
				              double* pMtrElecVolume , short* pUVEAntenna ) ;

typedef long (WINAPI *typeGetHosePropertiesEx2) (long ID , long* pNumber, long* pPumpID ,long* pTankID , long* pPhysicalNumber, 
		                      double* pMtrTheoValue , double* pMtrTheoVolume ,double* pMtrElecValue ,
				              double* pMtrElecVolume , short* pUVEAntenna , double* pPrice1 , double* pPrice2 , short* pEnabled ) ;

typedef long (WINAPI *typeSetHoseProperties) (long ID , long Number,long PumpID, long TankID , long PhysicalNumber, 
		                      double MtrTheoValue , double MtrTheoVolume ,double MtrElecValue ,
				              double MtrElecVolume ) ;

typedef long (WINAPI *typeSetHosePropertiesEx) (long ID , long Number,long PumpID, long TankID , long PhysicalNumber, 
		                      double MtrTheoValue , double MtrTheoVolume ,double MtrElecValue ,
				              double MtrElecVolume , short UVEAntenna );

typedef long (WINAPI *typeSetHosePropertiesEx2) (long ID , long Number,long PumpID, long TankID , long PhysicalNumber, 
		                      double MtrTheoValue , double MtrTheoVolume ,double MtrElecValue ,
				              double MtrElecVolume , short UVEAntenna , double Price1 , double Price2 , short Enabled ) ;

typedef long (WINAPI *typeDeleteHose) ( long ID ) ;

typedef long (WINAPI *typeGetHosePrices) ( long ID , short* pDurationType , short* pPriceType , double* pPrice1 , double* pPrice2 ) ;

typedef long (WINAPI *typeGetHoseSummary) (	long ID , long* pNumber, long* pPhysicalNumber,
							long* pPumpID, long* pPumpNumber, PBSTR pPumpName,
							long* pTankID, long* pTankNumber, PBSTR pTankName,
							long* pGradeID , long* pGradeNumber, PBSTR pGradeName, PBSTR pGradeShortName, PBSTR pGradeCode,
		                    double* pMtrTheoValue , double* pMtrTheoVolume ,double* pMtrElecValue , double* pMtrElecVolume ) ;

typedef long (WINAPI *typeGetHoseSummaryEx) (	long ID , long* pNumber, long* pPhysicalNumber,
							long* pPumpID, long* pPumpNumber, PBSTR pPumpName,
							long* pTankID, long* pTankNumber, PBSTR pTankName,
							long* pGradeID , long* pGradeNumber, PBSTR pGradeName, PBSTR pGradeShortName, PBSTR pGradeCode,
		                    double* pMtrTheoValue , double* pMtrTheoVolume ,double* pMtrElecValue , double* pMtrElecVolume ,
							double* pPrice1 , double* pPrice2 , short* pEnabled) ;

typedef long (WINAPI *typeSetHoseETotals) ( long ID , double Volume , double Value ) ;

typedef long (WINAPI *typeSetHosePrices) ( long ID , short DurationType , short PriceType , double Price1 , double Price2 ) ;

	//-------------------------------------- Grades ---------------------------------------//

typedef long (WINAPI *typeGetGradesCount) ( long* pCount ) ;

typedef long (WINAPI *typeGetGradeByNumber) ( long Number , long* pID ) ;

typedef long (WINAPI *typeGetGradeByName) ( BSTR Name , long* pID ) ;

typedef long (WINAPI *typeGetGradeByOrdinal) ( long Index , long* pID ) ;

typedef long (WINAPI *typeGetGradeProperties) (long ID , long* pNumber, PBSTR pName , PBSTR pShortName , PBSTR pCode ) ;

typedef long (WINAPI *typeGetGradePropertiesEx) (long ID , long* pNumber, PBSTR pName , PBSTR pShortName , PBSTR pCode , short* pType ) ;

typedef long (WINAPI *typeSetGradeProperties) (long ID , long Number, BSTR Name , BSTR ShortName , BSTR Code  ) ;

typedef long (WINAPI *typeSetGradePropertiesEx) (long ID , long Number, BSTR Name , BSTR ShortName , BSTR Code  , short Type ) ;

typedef long (WINAPI *typeDeleteGrade) ( long ID ) ;

typedef long (WINAPI *typeSetGradePrice) ( long ID , short Level , double Price ) ;

typedef long (WINAPI *typeGetGradePrice) ( long ID , short Level , double* pPrice ) ;

	//-------------------------------------- Tanks ----------------------------------------//

typedef long (WINAPI *typeGetTanksCount) ( long* pCount ) ;

typedef long (WINAPI *typeGetTankByNumber) ( long Number , long* pID ) ;

typedef long (WINAPI *typeGetTankByName) ( BSTR Name , long* pID ) ;

typedef long (WINAPI *typeGetTankByOrdinal) ( long Index , long* pID ) ;

typedef long (WINAPI *typeGetTankProperties) (long ID , long* pNumber, PBSTR pName , long* pGradeID , short* pType , double* pCapacity , double* pDiameter , 
							  double* pTheoVolume , double* pGaugeVolume , double* pGaugeTCVolume, double* pGaugeUllage, double* pGaugeTemperature , 
							  double* pGaugeLevel , double* pGaugeWaterVolume, double* pGaugeWaterLevel, long* pGaugeID, short* pProbeNo ) ;

typedef long (WINAPI *typeGetTankPropertiesEx) (long ID , long* pNumber, PBSTR pName , long* pGradeID , short* pType , double* pCapacity , double* pDiameter , 
							  double* pTheoVolume , double* pGaugeVolume , double* pGaugeTCVolume, double* pGaugeUllage, double* pGaugeTemperature , 
							  double* pGaugeLevel , double* pGaugeWaterVolume, double* pGaugeWaterLevel, long* pGaugeID, short* pProbeNo , long* pGaugeAlarmsMask ) ;

typedef long (WINAPI *typeSetTankProperties) (long ID , long Number, BSTR Name , long GradeID , short Type , double Capacity , double Diameter , double TheoVolume , 
		                      double GaugeVolume , double GaugeTCVolume ,double GaugeUllage ,double GaugeTemperature , double GaugeLevel , 
							  double GaugeWaterVolume, double GaugeWaterLevel, long GaugeID , short ProbeNo ) ;

typedef long (WINAPI *typeSetTankPropertiesEx) (long ID , long Number, BSTR Name , long GradeID , short Type , double Capacity , double Diameter , double TheoVolume , 
		                      double GaugeVolume , double GaugeTCVolume ,double GaugeUllage ,double GaugeTemperature , double GaugeLevel , 
							  double GaugeWaterVolume, double GaugeWaterLevel, long GaugeID , short ProbeNo , long GaugeAlarmsMask ) ;

typedef long (WINAPI *typeDeleteTank) ( long ID ) ;

typedef long (WINAPI *typeGetTankSummary) (long ID , long* pNumber, PBSTR pName , long* pGradeID , 
						   long* pGradeNumber, PBSTR pGradeName, PBSTR pGradeShortName, PBSTR pGradeCode, 
						   short* pType , double* pCapacity , double* pDiameter , 
						   double* pTheoVolume , double* pGaugeVolume , double* pGaugeTCVolume, double* pGaugeUllage, double* pGaugeTemperature , 
						   double* pGaugeLevel , double* pGaugeWaterVolume, double* pGaugeWaterLevel, long* pGaugeID, short* pProbeNo ) ;

typedef long (WINAPI *typeGetTankSummaryEx) (long ID , long* pNumber, PBSTR pName , long* pGradeID , 
						   long* pGradeNumber, PBSTR pGradeName, PBSTR pGradeShortName, PBSTR pGradeCode, 
						   short* pType , double* pCapacity , double* pDiameter , 
						   double* pTheoVolume , double* pGaugeVolume , double* pGaugeTCVolume, double* pGaugeUllage, double* pGaugeTemperature , 
						   double* pGaugeLevel , double* pGaugeWaterVolume, double* pGaugeWaterLevel, long* pGaugeID, short* pProbeNo , 
						   short* pState , long* pGaugeAlarmsMask ) ;

	//-------------------------------------- Ports ----------------------------------------//

typedef long (WINAPI *typeGetPortsCount) ( long* pCount ) ;

typedef long (WINAPI *typeGetPortByNumber) ( long Number , long* pID ) ;

typedef long (WINAPI *typeGetPortByName) ( BSTR Name , long* pID ) ;

typedef long (WINAPI *typeGetPortByOrdinal) ( long Index , long* pID ) ;

typedef long (WINAPI *typeGetPortProperties) (long ID , long* pNumber, PBSTR pName , long* pProtocolID , short* pDeviceType , PBSTR pSerialNo ) ;

typedef long (WINAPI *typeSetPortProperties) (long ID , long Number, BSTR Name, long ProtocolID , short DeviceType , BSTR SerialNo ) ;

typedef long (WINAPI *typeRemovePort) ( long ID ) ;

typedef long (WINAPI *typeGetZB2GConfig) ( long ID , __int64* pPanID , long* pChannels , __int64* pKeyA , __int64* pKeyB ) ;

typedef long (WINAPI *typeGetSerialNo) ( long ID , PBSTR pSerialNo ) ; 

typedef long (WINAPI *typeGetDeviceDetails) ( long ID , long ZBID , PBSTR pSerialNo , PBSTR pBootVersion , PBSTR pFirmwareVersion ) ;

typedef long (WINAPI *typeResetDevice) ( long ID , long ZBID ) ;

typedef long (WINAPI *typeRequestVersion) ( long ID , long ZBID ) ;

	//-------------------------------------- Attendants -----------------------------------//

typedef long (WINAPI *typeGetAttendantsCount) ( long* pCount ) ;

typedef long (WINAPI *typeGetAttendantByNumber) ( long Number , long* pID ) ;

typedef long (WINAPI *typeGetAttendantByName) ( BSTR Name , long* pID ) ;

typedef long (WINAPI *typeGetAttendantByOrdinal) ( long Index , long* pID ) ;

typedef long (WINAPI *typeGetAttendantProperties) (long ID , long* pNumber, PBSTR pName , PBSTR pShortName , PBSTR pPassword , PBSTR pTag ) ; 

typedef long (WINAPI *typeSetAttendantProperties) (long ID , long Number, BSTR Name , BSTR ShortName , BSTR Password , BSTR Tag ) ; 

typedef long (WINAPI *typeGetAttendantPropertiesEx) (long ID , long* pNumber, PBSTR pName , PBSTR pShortName , PBSTR pPassword , PBSTR pTag , 
									 short* pShiftAStart , short* pShiftAEnd , short* pShiftBStart , short* pShiftBEnd , short* pType )  ;

typedef long (WINAPI *typeSetAttendantPropertiesEx) (long ID , long Number, BSTR Name , BSTR ShortName , BSTR Password , BSTR Tag,
									 short ShiftAStart , short ShiftAEnd , short ShiftBStart , short ShiftBEnd , short Type )  ;

typedef long (WINAPI *typeGetAttendantState) ( long ID , short* pType , short* pLoggedOn ) ;

typedef long (WINAPI *typeDeleteAttendant) ( long ID ) ;

typedef long (WINAPI *typeAttendantLogon) ( long ID , long PumpID ) ;

typedef long (WINAPI *typeAttendantLogoff) ( long ID ) ;

	//------------------------------ Card Clients -----------------------------------------//

typedef long (WINAPI *typeGetCardClientsCount) ( long* pCount ) ;

typedef long (WINAPI *typeGetCardClientByNumber) ( long Number , long* pID ) ;

typedef long (WINAPI *typeGetCardClientByName) ( BSTR Name , long* pID ) ;

typedef long (WINAPI *typeGetCardClientByOrdinal) ( long Index , long* pID ) ;

typedef long (WINAPI *typeGetCardClientProperties) (long ID , long* pNumber, PBSTR pName , PBSTR pTag , short* pEnabled )  ;

typedef long (WINAPI *typeSetCardClientProperties) (long ID , long Number, BSTR Name , BSTR Tag , short Enabled )  ;

typedef long (WINAPI *typeGetCardClientPropertiesEx) (long ID , long* pNumber, PBSTR pName , PBSTR pTag , short* pEnabled , short* pPriceLevel, PBSTR pPlate  )  ;

typedef long (WINAPI *typeSetCardClientPropertiesEx) (long ID , long Number, BSTR Name , BSTR Tag , short Enabled , short PriceLevel , BSTR Plate ) ;

typedef long (WINAPI *typeSetCardClientPropertiesEx2) (long ID , long Number, BSTR Name , BSTR Tag , short Enabled , short PriceLevel , BSTR Plate , short GradeType )  ;

typedef long (WINAPI *typeGetCardClientPropertiesEx2) (long ID , long* pNumber, PBSTR pName , PBSTR pTag , short* pEnabled , short* pPriceLevel , PBSTR pPlate , short* pGradeType)  ;

typedef long (WINAPI *typeDeleteCardClient) ( long ID ) ;

	//------------------------------ Card Reads -----------------------------------------//

typedef long (WINAPI *typeGetCardReadsCount) ( long* pCount ) ;

typedef long (WINAPI *typeGetCardReadByNumber) ( long Number , long* pID ) ;

typedef long (WINAPI *typeGetCardReadByName) ( BSTR Name , long* pID ) ;

typedef long (WINAPI *typeGetCardReadByOrdinal) ( long Index , long* pID ) ;

typedef long (WINAPI *typeGetCardReadProperties) (long ID , long* pNumber, PBSTR pName , long* pPumpID , short* pType , long* pParentID , __int64* pTag , DATE* pTimeStamp )  ;

typedef long (WINAPI *typeSetCardReadProperties) (long ID , long Number, BSTR Name , long PumpID , short Type , long ParentID , __int64 Tag , DATE TimeStamp)  ;

typedef long (WINAPI *typeDeleteCardRead) ( long ID ) ;

typedef long (WINAPI *typeGetCardType) ( BSTR Tag , short* pTagType , long* pID, PBSTR pName, long* pNumber  ) ;

	//------------------------------ ZigBee devices ---------------------------------------//

typedef long (WINAPI *typeGetZigBeeCount) ( long* pCount ) ;

typedef long (WINAPI *typeGetZigBeeByNumber) ( long Number , long* pID ) ;

typedef long (WINAPI *typeGetZigBeeByName) ( BSTR Name , long* pID ) ;

typedef long (WINAPI *typeGetZigBeeByOrdinal) ( long Index , long* pID ) ;

typedef long (WINAPI *typeGetZigBeeProperties) (long ID , long* pNumber, PBSTR pName , short* pDeviceType , PBSTR pSerialNumber , PBSTR pNodeIdentifier , long* pPortID ) ;

typedef long (WINAPI *typeSetZigBeeProperties) (long ID , long Number, BSTR Name , short DeviceType , BSTR SerialNumber , BSTR NodeIdentifier , long PortID ) ;

typedef long (WINAPI *typeDeleteZigBee) ( long ID ) ;

	//------------------------------ Sensors ----------------------------------------------//

typedef long (WINAPI *typeGetSensorsCount) ( long* pCount ) ;

typedef long (WINAPI *typeGetSensorByNumber) ( long Number , long* pID ) ;

typedef long (WINAPI *typeGetSensorByName) ( BSTR Name , long* pID ) ;

typedef long (WINAPI *typeGetSensorByOrdinal) ( long Index , long* pID ) ;

typedef long (WINAPI *typeGetSensorProperties) (long ID , long* pNumber, PBSTR pName , long* pPortID , short* pType , short* pAddress , short* pSensorNo ) ;

typedef long (WINAPI *typeSetSensorProperties) (long ID , long Number, BSTR Name , long PortID , short Type, short Address , short SensorNo ) ;

typedef long (WINAPI *typeSetSensorStatus) (long ID , short State , short IsResponding ) ;

typedef long (WINAPI *typeGetSensorStatus) (long ID , short* pState , short* pIsResponding ) ;

typedef long (WINAPI *typeDeleteSensor) ( long ID ) ;

	//------------------------------ Logged events -----------------------------------------//

typedef long (WINAPI *typeGetLogEventCount) ( long* pCount , short DeviceType , long DeviceID , short EventLevel , short EventType , long ClearedBy, long  AckedBy ) ;

typedef long (WINAPI *typeGetLogEventByOrdinal) ( long Index , long* pID , short DeviceType , long DeviceID , short EventLevel , short EventType ,long  ClearedBy, long  AckedBy ) ;

typedef long (WINAPI *typeGetLogEventProperties) ( long ID , short* pDeviceType , long* pDeviceID , long* pDeviceNumber ,	PBSTR pDeviceName , short* pEventLevel ,
								   short* pEventType , PBSTR pEventDesc , DATE* pGeneratedDT , DATE* pClearedDT , long* pClearedBy , long* pAckedBy , double* pVolume ,
								   double* pValue , double* pProductVolume , double* pProductLevel , double* pWaterLevel , double* pTemperature  );

typedef long (WINAPI *typeSetLogEventProperties) (long ID , short DeviceType , long DeviceID , long DeviceNumber , BSTR DeviceName , short EventLevel ,
								  short EventType , BSTR  EventDesc , DATE GeneratedDT , DATE ClearedDT , long  ClearedBy , long AckedBy , double  Volume ,
								  double  Value , double  ProductVolume ,	double  ProductLevel , double  WaterLevel , double  Temperature );

typedef long (WINAPI *typeDeleteLogEvent) ( long ID ) ;

typedef long (WINAPI *typeClearLogEvent) ( long ID  , long ClientID ) ;

typedef long (WINAPI *typeAckLogEvent) ( long ID  , long ClientID ) ;

class CEZClient
{
public:
	CEZClient();

	CEZClient(char* dllpath);

	~CEZClient();

	bool isLoaded();

	void GetLastErrorText(DWORD erro, LPTSTR lpszBuffer, DWORD dwSize);
	char KBget();
	int  KBhit();

protected:
	bool	dllLoaded;
	HMODULE	dllHandle;
	void	Initializer();
	int		LoadAPIs(char* dllpath);

#if defined(__linux__)
	termios oldTerm;
#endif

//-----------------------------------------------------------------------------
public:  // Declaracao dos ponteiros de pontos de entrada
	typeDllVersion							DllVersion;
	typeServerVersion						ServerVersion;
	typeClientLogon							ClientLogon;
	typeClientLogonEx						ClientLogonEx;
	typeClientLogoff						ClientLogoff;
	typeClientStatus						ClientStatus;
	typeGetLicenseType						GetLicenseType;
	typeFireClientEvent						FireClientEvent;
	typeTestConnection						TestConnection;
	typeProcessEvents						ProcessEvents;
	typeGetEventsCount						GetEventsCount;
	typeGetNextEventType					GetNextEventType;
	typeDiscardNextEvent					DiscardNextEvent;
	typeGetNextPumpEvent					GetNextPumpEvent;
	typeGetNextDeliveryEvent				GetNextDeliveryEvent;
	typeGetNextServerEvent					GetNextServerEvent;
	typeGetNextClientEvent					GetNextClientEvent;
	typeGetNextDBLogEvent					GetNextDBLogEvent;
	typeGetNextDBLogDeliveryEvent			GetNextDBLogDeliveryEvent;
	typeGetNextDBClearDeliveryEvent			GetNextDBClearDeliveryEvent;
	typeGetNextDBStackDeliveryEvent			GetNextDBStackDeliveryEvent;
	typeGetNextDBHoseETotalsEvent			GetNextDBHoseETotalsEvent;
	typeGetNextDBTriggerEvent				GetNextDBTriggerEvent;
	typeGetNextDBAttendantLogonEvent		GetNextDBAttendantLogonEvent;
	typeGetNextDBAttendantLogoffEvent		GetNextDBAttendantLogoffEvent;
	typeGetNextDBTankStatusEvent			GetNextDBTankStatusEvent;
	typeGetPumpsCount						GetPumpsCount;
	typeGetPumpByNumber						GetPumpByNumber;
	typeGetPumpByName						GetPumpByName;
	typeGetPumpByOrdinal					GetPumpByOrdinal;
	typeGetPumpProperties					GetPumpProperties;
	typeSetPumpProperties					SetPumpProperties;
	typeDeletePump							DeletePump;
	typeGetPumpHosesCount					GetPumpHosesCount;
	typeGetPumpHoseByNumber					GetPumpHoseByNumber;
	typeGetPumpStatus						GetPumpStatus;
	typeGetPumpStatusEx						GetPumpStatusEx;
	typeGetPumpDeliveryProperties			GetPumpDeliveryProperties;
	typeGetPumpDeliveryPropertiesEx			GetPumpDeliveryPropertiesEx;
	typeGetPumpDeliveryPropertiesEx2		GetPumpDeliveryPropertiesEx2;
	typeGetPumpDeliveryPropertiesEx3		GetPumpDeliveryPropertiesEx3;
	typePrepayReserve						PrepayReserve;
	typePrepayCancel						PrepayCancel;
	typePrepayAuthorise						PrepayAuthorise;
	typePreauthReserve						PreauthReserve;
	typePreauthCancel						PreauthCancel;
	typePreauthAuthorise					PreauthAuthorise;
	typeLoadPreset							LoadPreset;
	typeLoadPresetWithPrice					LoadPresetWithPrice;
	typeTagAuthorise						TagAuthorise;
	typeAttendantAuthorise					AttendantAuthorise;
	typeAuthorise							Authorise;
	typeCancelAuthorise						CancelAuthorise;
	typeTempStop							TempStop;
	typeReAuthorise							ReAuthorise;
	typeTerminateDelivery					TerminateDelivery;
	typeStackCurrentDelivery				StackCurrentDelivery;
	typeGetDensity							GetDensity;
	typeEnablePump							EnablePump;
	typeDisablePump							DisablePump;
	typeGetHosesCount						GetHosesCount;
	typeGetHoseByOrdinal					GetHoseByOrdinal;
	typeGetHoseProperties					GetHoseProperties;
	typeSetHoseProperties					SetHoseProperties;
	typeGetHoseSummary						GetHoseSummary;
	typeGetHoseSummaryEx					GetHoseSummaryEx;
	typeDeleteHose							DeleteHose;
	typeGetDeliveriesCount					GetDeliveriesCount;
	typeGetDeliveryByOrdinal				GetDeliveryByOrdinal;
	typeGetAllDeliveriesCount				GetAllDeliveriesCount;
	typeGetAllDeliveryByOrdinal				GetAllDeliveryByOrdinal;
	typeAckDeliveryDBLog					AckDeliveryDBLog;
	typeGetDeliveryIDByOrdinalNotLogged		GetDeliveryIDByOrdinalNotLogged;
	typeGetDeliveriesCountNotLogged			GetDeliveriesCountNotLogged;
	typeAckDeliveryVolLog					AckDeliveryVolLog;
	typeGetDeliveryIDByOrdinalNotVolLogged	GetDeliveryIDByOrdinalNotVolLogged;
	typeGetDeliveriesCountNotVolLogged		GetDeliveriesCountNotVolLogged;
	typeGetDeliveryProperties				GetDeliveryProperties;
	typeGetDeliveryPropertiesEx				GetDeliveryPropertiesEx;
	typeGetDeliveryPropertiesEx2			GetDeliveryPropertiesEx2;
	typeGetDeliveryPropertiesEx3			GetDeliveryPropertiesEx3;
	typeGetDeliveryPropertiesEx4			GetDeliveryPropertiesEx4;
	typeSetDeliveryProperties				SetDeliveryProperties;
	typeSetDeliveryPropertiesEx				SetDeliveryPropertiesEx;
	typeSetDeliveryPropertiesEx2			SetDeliveryPropertiesEx2;
	typeSetDeliveryPropertiesEx3			SetDeliveryPropertiesEx3;
	typeGetDeliverySummary					GetDeliverySummary;
	typeGetDeliverySummaryEx				GetDeliverySummaryEx;
	typeGetDeliverySummaryEx2				GetDeliverySummaryEx2;
	typeLockDelivery						LockDelivery;
	typeUnlockDelivery						UnlockDelivery;
	typeClearDelivery						ClearDelivery;
	typeLockAndClearDelivery				LockAndClearDelivery;
	typeGetDuration							GetDuration;
	typeGetGradesCount						GetGradesCount;
	typeGetGradeByNumber					GetGradeByNumber;
	typeGetGradeByName						GetGradeByName;
	typeGetGradeByOrdinal					GetGradeByOrdinal;
	typeGetGradeProperties					GetGradeProperties;
	typeGetGradePropertiesEx				GetGradePropertiesEx;
	typeSetGradeProperties					SetGradeProperties;
	typeDeleteGrade							DeleteGrade;
	typeSetGradePrice						SetGradePrice;
	typeGetGradePrice						GetGradePrice;
	typeGetTanksCount						GetTanksCount;
	typeGetTankByNumber						GetTankByNumber;
	typeGetTankByName						GetTankByName;
	typeGetTankByOrdinal					GetTankByOrdinal;
	typeGetTankProperties					GetTankProperties;
	typeGetTankPropertiesEx					GetTankPropertiesEx;
	typeSetTankProperties					SetTankProperties;
	typeGetTankSummary						GetTankSummary;
	typeDeleteTank							DeleteTank;
	typeGetPortsCount						GetPortsCount;
	typeGetPortByNumber						GetPortByNumber;
	typeGetPortByName						GetPortByName;
	typeGetPortByOrdinal					GetPortByOrdinal;
	typeGetPortProperties					GetPortProperties;
	typeSetPortProperties					SetPortProperties;
	typeGetAttendantsCount					GetAttendantsCount;
	typeGetAttendantByNumber				GetAttendantByNumber;
	typeGetAttendantByName					GetAttendantByName;
	typeGetAttendantByOrdinal				GetAttendantByOrdinal;
	typeGetAttendantProperties				GetAttendantProperties;
	typeSetAttendantProperties				SetAttendantProperties;
	typeGetAttendantPropertiesEx			GetAttendantPropertiesEx;
	typeSetAttendantPropertiesEx			SetAttendantPropertiesEx;
	typeDeleteAttendant						DeleteAttendant;
	typeAttendantLogon						AttendantLogon;
	typeAttendantLogoff						AttendantLogoff;
	typeGetCardReadsCount					GetCardReadsCount;
	typeGetCardReadByNumber					GetCardReadByNumber;
	typeGetCardReadByName					GetCardReadByName;
	typeGetCardReadByOrdinal				GetCardReadByOrdinal;
	typeGetCardReadProperties				GetCardReadProperties;
	typeSetCardReadProperties				SetCardReadProperties;
	typeDeleteCardRead						DeleteCardRead;
	typeGetCardClientsCount					GetCardClientsCount;
	typeGetCardClientByNumber				GetCardClientByNumber;
	typeGetCardClientByName					GetCardClientByName;
	typeGetCardClientByOrdinal				GetCardClientByOrdinal;
	typeGetCardClientProperties				GetCardClientProperties;
	typeSetCardClientProperties				SetCardClientProperties;
	typeDeleteCardClient					DeleteCardClient;
	typeAllStop								AllStop;
	typeAllAuthorise						AllAuthorise;
	typeAllReAuthorise						AllReAuthorise;
	typeAllStopIfIdle						AllStopIfIdle;
	typeReadAllTanks						ReadAllTanks;
	typeGetAllPumpStatuses					GetAllPumpStatuses;
	typeGetIniValue							GetIniValue;
	typeSetIniValue							SetIniValue;
	typeSetNextDeliveryID					SetNextDeliveryID;
	typeRemovePort							RemovePort;
	typeLicenseStatus						LicenseStatus;
	typeCheckSocketClosed					CheckSocketClosed;
	typeResultString						ResultString;
	typePumpStateString						PumpStateString;
	typeDeliveryStateString					DeliveryStateString;
	typeDeliveryTypeString					DeliveryTypeString;
	typeReserveTypeString					ReserveTypeString;
	typeGetNextPumpEventEx					GetNextPumpEventEx;
	typeGetNextPumpEventEx2					GetNextPumpEventEx2;
	typeGetNextPumpEventEx3					GetNextPumpEventEx3;
	typeGetNextDeliveryEventEx				GetNextDeliveryEventEx;
	typeGetNextDeliveryEventEx2				GetNextDeliveryEventEx2;
	typeGetNextDeliveryEventEx3				GetNextDeliveryEventEx3;
	typeGetNextDeliveryEventEx4				GetNextDeliveryEventEx4;
	typeGetNextCardReadEvent				GetNextCardReadEvent;
	typeGetNextDBHoseETotalsEventEx			GetNextDBHoseETotalsEventEx;
	typeGetNextDBTankStatusEventEx			GetNextDBTankStatusEventEx;
	typeGetZigBeeCount						GetZigBeeCount;
	typeGetZigBeeByNumber					GetZigBeeByNumber;
	typeGetZigBeeByName						GetZigBeeByName;
	typeGetZigBeeByOrdinal					GetZigBeeByOrdinal;
	typeGetZigBeeProperties					GetZigBeeProperties;
	typeSetZigBeeProperties					SetZigBeeProperties;
	typeDeleteZigBee						DeleteZigBee;
	typeGetHosePropertiesEx					GetHosePropertiesEx;
	typeSetHosePropertiesEx					SetHosePropertiesEx;
	typeGetHosePropertiesEx2				GetHosePropertiesEx2;
	typeSetHosePropertiesEx2				SetHosePropertiesEx2;
	typeSetPumpPropertiesEx					SetPumpPropertiesEx;
	typeGetPumpPropertiesEx					GetPumpPropertiesEx;
	typeGetSerialNo							GetSerialNo;
	typeGetDeviceDetails					GetDeviceDetails;
	typeResetDevice							ResetDevice;
	typeRequestVersion						RequestVersion;
	typeSetHoseETotals						SetHoseETotals;
	typeGetNextZeroDeliveryEvent			GetNextZeroDeliveryEvent;
	typeSetHosePrices						SetHosePrices;
	typeGetHosePrices						GetHosePrices;
	typeSetPumpDefaultPriceLevel			SetPumpDefaultPriceLevel;
	typeSetDateTime							SetDateTime;
	typeGetNextZB2GStatusEvent				GetNextZB2GStatusEvent;
	typeGetNextLogEventEvent				GetNextLogEventEvent;
	typeGetNextDBTankStatusEventEx2			GetNextDBTankStatusEventEx2;
	//typeIsValidMACLicense					IsValidMACLicense;
};

//-----------------------------------------------------------------------------
#endif // __EZ_CLIENT_INTERFACE__

