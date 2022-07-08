//------------------------------------------------------------------------------------
//		Copyrigth (r) BLD France   
//
//
//    Define a structure that contains all declarations of math functions.
//
//------------------------------------------------------------------------------------
//
//
//
//------------------------------------------------------------------------------------

#ifndef SPE_SPECIFIC_H
#define SPE_SPECIFIC_H

//------------------------------------------------------------------------------------
// Includes.
#include "stdafx.h"



//------------------------------------------------------------------------------------
// Bussiness.
#define HMI_HV_BUSINESS_NUMBER             2009                // 2001 : Business Number: Calvert line 2.

#define DIM_SQL_REQUEST                   10        // Table for SQL request interface.

//------------------------------------------------------------------------------------
// Specific Coil measurements by section concerning strip structure.
//struct SPE_coil_data_section_stripTemperature
//{
//  double stripUpper ;    // Strip temperarure upper tolerance.
//  double stripTarget;    // Strip temperarure target from section.
//  double stripLower ;    // Strip temperarure lower tolerance.
//};
//
////------------------------------------------------------------------------------------
//// Specific application heat cycles.
//struct SPE_parameter_heat_cycle
//{
//  long                                          cycleNumber          ;
//  struct SPE_coil_data_section_stripTemperature section[DIM_SECTIONS];
//};

//------------------------------------------------------------------------------------
// Specific application parameters.
struct SPE_parameter
{
  //struct SPE_parameter_heat_cycle   heatCycles[DIM_HEAT_CYCLES];
  //long long                           sqlPrimaryKey;
};

//------------------------------------------------------------------------------------
// Specific Line measurements.
struct SPE_measures_Line
{
};

//------------------------------------------------------------------------------------
// Specific section measurements.
struct SPE_measurement_section
{
  double  CONSUMPTION_ELECTRICAL;
  double  CONSUMPTION_FUEL;
  double  CONSUMPTION_NITROGEN;
  double  CONSUMPTION_HYDROGEN;
  double  AIR_TEMPERATURE_ALL;
};

//------------------------------------------------------------------------------------
// Specific RTF section measurements.
struct SPE_measures_RTF_section
{
  
};

//------------------------------------------------------------------------------------
// Specific RTF zone measurements.
struct SPE_measures_RTF_zone
{
  
};

//------------------------------------------------------------------------------------
// Specific coil data.
struct SPE_coils_data
{
  char                              combinaisonIdentifier       [DIM_NAMES + 1] ;           // .
  char                              heatID                      [DIM_NAMES + 1] ;           // .
  long                              partNumber                                  ;           // .
  long                              coilCounter                                 ;           // .
  double                            weight                                      ;           // .
  double                            tension                                     ;           // .
  double                            yield                                       ;           // .
};

//------------------------------------------------------------------------------------
// Specific coil data.
struct SPE_coils_calcul
{
  
};

//------------------------------------------------------------------------------------
// Specific measurements.
struct SPE_measures_zones
{
  long      COIL_L2_IDENTIFIER;
  long      COIL_PART_NUMBER;
  double    STRIP_SAMPLE_POSITION;
  double    ANALYSIS_CO;
  double    ANALYSIS_CO2;
  double    ANALYSIS_H2;
  double    ANALYSIS_O2;
  double    ANALYSIS_DP;
  double    MAX_FLOW;
  double    MIN_FLOW;
  double    MAX_HD;
  double    MIN_HD;
  double    READY;
  double    PZH_TEMP;
  double    SPLIT_RANGE;
  
  double    h2_rate;
};

//------------------------------------------------------------------------------------
// Specific measurements.
struct SPE_measures
{
  //long    maNouvelleMesure1;
  //double  maNouvelleMesure2;
  //char    maNouvelleMesure3[DIM_NAMES];

  struct SPE_measures_zones   zones[DIM_ZONES];
  //struct SPE_measurement_section  sections[DIM_SECTIONS ];
  double SPEED_SET_OFFSET;
};

//------------------------------------------------------------------------------------
// Specific setpoints.
struct SPE_setpoints
{
  long    maNouvelleConsigne1;              // Use for Mistracking prevention.
  double  maNouvelleConsigne2;
  char    maNouvelleConsigne3[DIM_NAMES];

};

//------------------------------------------------------------------------------------
// Definition of the dynamic function structure input  for calculation.
struct  MOD_HV_TRS_In_specific_parameters
{
  double                                      rampUp                        [DIM_SECTIONS];    // Entry: Heat demand variation upper ramp.
  double                                      rampDw                        [DIM_SECTIONS];    // Entry: Heat demand variation upper ramp.
                           
};

//------------------------------------------------------------------------------------
// Definition of the dynamic function structure input  for calculation.
struct  SPE_specific_on_live_parameters
{
  
  double PERCENTAGE_RED_SPEED_TRS   ;
  double PREDICTED_SECTION_TEMP_TIME;
                           
};

//------------------------------------------------------------------------------------
// We store heat cycle .
struct SPE_CoilHeatCycleTable
{
  long internalCode;
  long heatCycleCode;
};

/* ATTENTION extern est très important sinon PB au link : undefsymb sur CMICOM */
/* note de JAB : l'extern était au dessus --> descendu pour compilation visual*/

struct p_BLD_specific
{
	
  double                            FFStripTempOffset       = 0.;
  double                            TTStripTempOffset       = 0.;
  double                            HHStripTempOffset       = 0.;
  double                            CCStripTempOffset       = 0.;
  double                            HBStripTempOffset       = 0.;
  double                            DDStripTempOffset       = 0.;
  long                              topCounterWeldEvent     = 0;
  long                              topCounterWeldEventLast = 0;
  double                            roofTempHBSetpoint      = 0.;
};


struct com_globalSection
{
	char HVM_GLOBALSECTION_NAME[50 + 1];
	char HVM_GLOBALSECTION_FILE_NAME[50 + 1];
	struct p_BLD_specific *           pSpe;                     // Pointer to GEN global section.
	HANDLE                            HandleMutexAccess;        // Handle of global section. 
	HANDLE                            handleGlobalSection;      // Handle of global section. 
	bool                              newGlobalSection;         // New global section.

	// This function create the mutex.
	void createOrOpenMutex(bool OwnerShipOnCreation)   // LPCTSTR mutexName
	{
		WCHAR mutexName[50 + 1] = { 0 };

		// Access mutex.
		swprintf(mutexName, 50, L"HVM_GLB_MUTEX_SIMU");
		this->HandleMutexAccess = CreateMutex(NULL, OwnerShipOnCreation, mutexName);
	}

	// This function lock access to the mutex to all other thread. 
	// If another thread try to lock the mutex, it will wait INFINITE TIME.
	void lockAccess()
	{
		if (NULL != HandleMutexAccess)  WaitForSingleObject(HandleMutexAccess, 250L);
		else                            createOrOpenMutex(true);
	}

	// This function unlock the mutex.
	void unLockAccess()
	{
		if (NULL != HandleMutexAccess)  ReleaseMutex(HandleMutexAccess);
		else                            createOrOpenMutex(false);
	}

	// This function Destroy the mutex.
	void DestroyMutex()
	{
		// Simulation Structure mutex.
		if (NULL != this->HandleMutexAccess)
		{
			CloseHandle(this->HandleMutexAccess);
			this->HandleMutexAccess = NULL;
		}
	}


	long map( long        actionType                       // Entry: Type of action: opening, creation.
		      , long        maxSizeMapObject                 // Entry: The low-order  DWORD of the maximum size of the file mapping object.
		      , char      * globalSectionName                // Entry: Global section name.
		      , char      * globalSectionFileName            // Entry: Global section filename.
		      , bool      * newGlobalSection                 // Exit: Creation indicator.
		      , void    * * globalSectionMapAdress           // Exit: Adress of the global section.
		      , HANDLE    * handle_globalSectionFile         // Exit or Entry: Handle of file.
		      )
	{
		char      fileName[500 + 1] = "[utl.globalSection]";   // File Name for traces
		char      traceTempo[500 + 1] = { 0 };                    // Trace tempory chain.
		long      status = 0;                                      // Function status.

		strncpy_s(this->HVM_GLOBALSECTION_NAME, 50, "SPE", strlen("SPE"));
		strncpy_s(this->HVM_GLOBALSECTION_FILE_NAME, 50, "SPE_GlobalSection_", strlen("SPE_GlobalSection_"));

		// Pointer description.
		LPVOID                pointerMappedLocation = nullptr;     // Pointer to adress mapped.
		SECURITY_ATTRIBUTES   pointerSecurityAttributes;              // Pointer to security attributes.
		PSECURITY_DESCRIPTOR  pointerSecurityDescriptor;              // Security descriptor pointer.
		PACL                  pointerAcessControlList;              // Pointer to access control list.

		// Handle description.
		HANDLE                handleFileCreationMapped = nullptr;     // Handler of file maping.
		HANDLE                handleFileCreation = nullptr;     // Handle of associated file.

		// Local variable.
		WCHAR    tempo_globalSectionFileName[500 + 5] = { 0 };
		WCHAR    tempo_globalSectionName[500 + 5] = { 0 };
		long     length;
		long     localCreationType;

		// function status.
		status = 1;

		// local management depending of action type.
		if (actionType == TYPE_CREATION)
		{
			localCreationType = OPEN_ALWAYS;

			// Initialize exit parameters of function.
			*handle_globalSectionFile = nullptr;
			*globalSectionMapAdress = nullptr;
		}
		else if (actionType == TYPE_MAPPED)
		{
			localCreationType = OPEN_EXISTING;
		}

		*newGlobalSection = false;

		// Initialize local handle.
		handleFileCreation = nullptr;
		handleFileCreationMapped = nullptr;

		// Initializations.
		pointerSecurityDescriptor = (PSECURITY_DESCRIPTOR)LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
		pointerAcessControlList = nullptr;
		InitializeSecurityDescriptor(pointerSecurityDescriptor, SECURITY_DESCRIPTOR_REVISION);
		SetSecurityDescriptorDacl(pointerSecurityDescriptor, TRUE, pointerAcessControlList, FALSE);
		pointerSecurityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
		pointerSecurityAttributes.lpSecurityDescriptor = pointerSecurityDescriptor;
		pointerSecurityAttributes.bInheritHandle = FALSE;

		length = strlen(globalSectionFileName);
		MultiByteToWideChar(0, 0, globalSectionFileName, length + 1, tempo_globalSectionFileName, length + 1);

		if (actionType == TYPE_MAPPED)
		{
			// Try to map the file
			handleFileCreation = OpenFileMapping(FILE_MAP_READ                    // Entry: Read/write access.
				, FALSE                            // Entry: Do not inherit the name.
				, tempo_globalSectionFileName);    // Entry: Name of mapping object.

			if (handleFileCreation == nullptr)
			{

			}
		}
		else if (actionType == TYPE_CREATION)
		{
			// Map or create the global section.
			handleFileCreation = CreateFile(tempo_globalSectionFileName			       // Global section file Name pointer.
				, GENERIC_READ | GENERIC_WRITE	            // Define acess mode.
				, FILE_SHARE_READ | FILE_SHARE_WRITE        // Define share mode.
				, &pointerSecurityAttributes							  // Pointer to security attributes.
				, localCreationType  		      			        // Define type of opening.
				// If the specified file exists, the function succeeds and the last-error code is set to ERROR_ALREADY_EXISTS (183).
				// If the specified file does not exist and is a valid path to a writable location, the function creates a file and the last - error code is set to zero.
				, FILE_ATTRIBUTE_NORMAL                     // The file or device attributes and flags, FILE_ATTRIBUTE_NORMAL being the most common default value for files.
				, NULL                                      // A valid handle to a template file with the GENERIC_READ access right. 
				// The template file supplies file attributes and extended attributes for the file that is being created.
				// This parameter can be NULL.
				// When opening an existing file, CreateFile ignores this parameter.
				);

			// We test the result of the above function, to determine if the global section has been created. 
			if (GetLastError() != ERROR_ALREADY_EXISTS)
			{
				*newGlobalSection = true;
			}
		}

		if (handleFileCreation == INVALID_HANDLE_VALUE)
		{
			status = -1;
		}

		// File has been created. We map the file. 
		if (status == 1)
		{
			length = strlen(globalSectionName);
			MultiByteToWideChar(0, 0, globalSectionName, length + 1, tempo_globalSectionName, length + 1);
			handleFileCreationMapped =
				CreateFileMapping(handleFileCreation		          // Handle of associated file.
				, &pointerSecurityAttributes		  // Optional security attributes.
				, PAGE_READWRITE      	          // Protection for mapping object.
				, 0                              // The high-order DWORD of the maximum size of the file mapping object.
				, maxSizeMapObject               // The low-order  DWORD of the maximum size of the file mapping object.
				// If this parameter and dwMaximumSizeHigh are 0 (zero), the maximum size of the file mapping object 
				// is equal to the current size of the file that hFile identifies.
				, tempo_globalSectionName);		  // Name of the global section. 

			// Verification that we have mapped the global section.
			if (handleFileCreationMapped == nullptr)
			{
				status = -1;
			}
		}

		// Map global section file.
		if (status == 1)
		{
			pointerMappedLocation =
				MapViewOfFile(handleFileCreationMapped	    // A handle to a file mapping object. The CreateFileMapping and OpenFileMapping functions return this handle.
				, FILE_MAP_ALL_ACCESS          // The type of access to a file mapping object, which determines the protection of the pages.
				, 0                            // A high-order DWORD of the file offset where the view begins.
				, 0                            // A low-order  DWORD of the file offset where the view is to begin.
				, 0                            // The number of bytes of a file mapping to map to the view.All bytes must be within the maximum size specified 
				// by CreateFileMapping.If this parameter is 0 (zero), the mapping extends from the specified offset to 
				// the end of the file mapping.
				);
			// Verification.
			if (pointerMappedLocation == nullptr)
			{
				status = -1;
			}
		}

		// In case some creation or mapping does work properly<
		// We need to destroy the handle we have created. 
		if (status == 0)
		{
			if ((handleFileCreation == INVALID_HANDLE_VALUE) ||
				(handleFileCreation == nullptr)
				)
			{
				CloseHandle(handleFileCreation);
				handleFileCreation = nullptr;
			}

			if (handleFileCreationMapped == nullptr)
			{
				CloseHandle(handleFileCreationMapped);
				handleFileCreationMapped = nullptr;
			}
		}

		// Filled function exit parameters with calculetd ones.
		if (status == 1)
		{
			if (TYPE_CREATION)
			{
				*handle_globalSectionFile = handleFileCreation;
			}
			*globalSectionMapAdress = pointerMappedLocation;
			status = 1;
		}

		return status;
	};

};




struct SPE_specific 
{
  struct MOD_utility * pMOD = nullptr;
  double wallThermalLosses[DIM_SECTIONS] = {0};

  //------------------------------------------------------------------------------------
  // Application name.
  char                                    AppName       [DIM_NAMES + 1]     = "ARV_CRE_HDGL2__"       ;           // Application name.
  char                                    heatCycleFile [DIM_NAMES + 1]     = "Heat Cycles Arv HDGL2" ;           // Heat Cycles.
  char                                    heatCycleName [DIM_NAMES + 1]     = "Heat cycles HDGL2"     ;           // "Heat cycles"
  long                                    setPointCounter                   = 0;
  bool                                    hasCoilQueueIntegrated            = false;  
  bool                                    hasTrackingIntegrated             = false;
  double                                  onLiveDynModelDFFpyroTemp         = 0.;
  bool                                    useAdaptOnEmisElseThLosses        = true;
  bool                                    useSpeedJumpForTransitionning     = false;
  bool                                    hasSimulationMode                 = false;
  bool                                    doFullMPCCalculation              = false;
  DWORD                                   simulationSleepingTime            = 950;
  bool                                    isUsingDFFCascadeMode             = false;
  double                                  splitRangeDFFMax[DIM_ZONES]       = {0};
  double                                  splitRangeDFFMin[DIM_ZONES]       = {0};
  double                                  nextZoneHDCutDFF[DIM_ZONES]       = {0};
  bool                                    simulationCombineTransfer         = false;
  double                                  coefTuningFixDFFEmissivity        = 0.;
  bool                                    hasCommunicationExecutable        = true;
  
  //------------------------------------------------------------------------------------
  // Standard Specific Data.
  long                                    computerNumber                    = 1;     // Indicate the computer number: server 1 or server 2.
  bool                                    useFixedDFFEmissivity             = false;
  bool                                    hasRecievedAllmessages [7]        = {0} ;
  double                                  rollExchangeCoefficient           = 0.85;     // Strip / roll exchnage correction coefficient.
  double                                  rollConductivityCoefficient       = 1.  ;     // Roll steel conductivity correction coefficient.
  bool                                    isTrackingSeparatedFromSpeed      = false;

  long                                    nbColors          = 8;
  long                                    colorNumbers[8]  = { 255165000 ,  // Orange.
                                                                127255000 ,  // Chartreuse.
                                                                255       ,  // Blue.
                                                                255215000 ,  // Gold.
                                                                    255000 ,  // Lime.
                                                                199021133 ,  // MediumVioletRed.
                                                                    255255 ,  // Aqua.
                                                                138043226    // BlueViolet.
                                                              };
  char                                    entryIdentities   [10][DIM_NAMES + 1] = {0} ;             // Coil identification at line entry.
  long                                    colorCodes        [10]                = {0} ; // DIM_COILS equal mandatory
  long                                    nuNextColorIndex                      = 0   ;
 
  // Local variable.
  bool                                    coilBufferStackInitialize             = false;
  bool                                    coilStackInitialize                   = false;
  char                                    entryIdentityLast   [DIM_NAMES + 1]   = {0};
  double                                  lastWeldPosition                      = 9999.;
  char                                    refCoilID           [DIM_NAMES + 1]   = {0};
  char                                    refCoilIDLine       [DIM_NAMES + 1]   = {0};
  bool                                    lineWeldPositionAvailableForUse       = false;
  char                                    entrySectionsIdentities [DIM_SECTIONS][DIM_NAMES + 1] = {0};
  time_t                                  lastUnPack                            = 0;
  double                                  lastRTFZoneHeatDemand[DIM_ZONES][200] = {0};
  bool                                    twoFirstIDDifferent                   = false;
  long                                    hdIndex              [DIM_ZONES]      = {0};
  long                                    nbBurnerInDefaultRTF [DIM_ZONES]      = {0};
  long                                    nbHdSmothering                        = 5;
  bool                                    zoneOperationJET      [10]            = {0};
  double                                  HDDoubleZoneJET[4]                    = {0};
  struct UTL_pid_objects                  pidErrorRTF                           = {0};
  struct UTL_pid_objects                  pidErrorRTS                           = {0};
  double                                  soakingTemperatureMeasurement         = 0.;
  double                                  soakingTemperatureSetpoint            = 0.;
  double                                  rtfZonesFumesTemperature [DIM_ZONES]  = {0};    
  double                                  alliationMaxSpeed                     = 0.; 
  struct UTL_pid_objects                  pidSoakingStripTemp                   = {0};  
  double                                  tractionOffset                        = 0.;
  struct com_globalSection                gblSec ;                       // Extern declaration of HVM global section.
  long                                    lScenarioIDCurrent                    = -1;
  time_t                                  recieveTimeScenarioID                 = 0;
  bool                                    bIsScenarioCalculationAUthorized      = true;
  long                                    nuStageScenario                       = 0;
  long                                    nuSectionScenario                     = 0;
  long                                    nuZoneScenario                        = 0;
  long                                    nuCoilScenario                        = 0;
  struct MOD_HV_scenarioData*             pScenarioDataInput                    = {0};
  double                                  specificConsumption[2]                = {0};
  double                                  efficiency         [2]                = {0};
  struct MOD_HV_GLB_parameter_heat_cycle* pHeatCycles        [20]               = {0};
  long                                    lNbHeatCycleStored                    = -1; 
  long                                    lNbHeatCycleToBeStored                = -1; 
  long                                    lIndexHeatCycle                       = -1; 
  long                                    nuSectionHeatCycle                    = 0;
  time_t                                  lastRecievedFirstHCMessage            = 0;                // Time we have recieved last HC table.
  struct SPE_CoilHeatCycleTable           heatCycleCodeTable[10]                = {0};
  

  //------------------------------------------------------------------------------------
  // Functions.
  void    applicationInitialization               ( struct GEN_utility *  pGEN                                        );  // Specific application initilization.
  void    finalExecutionActions                   (                                                                   );
  void    applicationConfigurationFinalization    ( bool    hasSimulationMode, struct GEN_utility *  pGEN             );  // Specific application configuration.
  long    selectConfigurationFile                 ( FILE **traceFilePointer, char configurationFileName[], long size  );  // Return the configuration file name of the project.
  void    storeMessageRecievedInformation         ( long messageId, struct MOD_HV_measures             *pMeasures     );  // Store information on the message just recieved.
  void    storeMessageRecievedInformationSIM      ( long messageId, struct MOD_HV_measures_simulation  *pMeasures     );  // Store information on the message just recieved.
  bool    messagesRecieved                        ( MSG *pMessage );    // Specific actions on messages recieved.
  bool    messagesToSend                          ( MSG *pMessage );    // Specific actions on messages to send.
  void    storeValueInMeasurementStructure        ( struct UTL_message_element_values * pValue, struct EQP_interface_message_elements * pInter, struct MOD_HV_measures  *           pMea );
  void    storeValueInMeasurementStructure        ( struct UTL_message_element_values * pValue, struct EQP_interface_message_elements * pInter, struct MOD_HV_measures_simulation * pMea );
  // Store the value in the right place.
  bool    messageMeasurementsTreatment            ( bool hasSimulationMode, MOD_HV_measures * pMessageRecieve, bool & hasRecievedAllmessages, bool & authorizedToContinue, struct GEN_utility *  pGEN );
  void    extractValuesFromSetpointStructure      ( struct UTL_message_element_values * pValue, struct EQP_interface_message_elements * pInter, struct MOD_HV_setpoints *             pSpt );
  void    storeValueInMeasurementStructure        ( struct UTL_message_element_values * pValue, struct EQP_interface_message_elements * pInter, struct MOD_HV_setpoints_simulation *  pSpt );
  // Extract the value in the right place from the setpoint structure.
  void    treatmentOfSpxMeaMessage                ( struct MOD_HV_measures &messageRecieve                                            );  // Specifc actions to be done on reception of message in Masn thread.
  void    productLineNumber                       ( FILE **traceFilePointer   , bool    hasSimulationMode, struct GEN_utility *  pGEN );  // Set product line number based on any parameters desired.
  void    productCalculs                          ( FILE **traceFilePointer   , bool    hasSimulationMode, struct GEN_utility *  pGEN );  // Realized all calculs on products.
  void    productCalculsALL                       ( FILE **traceFilePointer		, bool    hasSimulationMode, struct MOD_HV_coil *  pCoil, struct GEN_utility *  pGEN  );
  void    measurementTreatement                   ( FILE ** ppTraceFilePointer, bool    hasSimulationMode, struct GEN_utility *  pGEN );  // Specific measurement treatment actions.
  void    simulationTreatment                     ( struct GEN_utility *  pGEN              );   // Specific simulation  treatment actions.
  void    simulationTreatmentOnLinePLCRecieve     (                                         );   // Specific simulation treatment actions on reception of a line PLC message.
  void    simulationTreatmentOnFurnacePLCRecieve  ( struct GEN_utility *  pGEN              );   // Specific simulation treatment actions on reception of PLC message setpoint.
  void    coilMessageRecieved                     ( FILE **traceFilePointer, struct MOD_HV_measures *pMessageRecieve, struct GEN_utility *  pGEN );   // Specific Treatment of a coil message recieved.
  void    simulationMessagesToSend                ( long typeOfMessageTosend, struct MOD_HV_setpoints_simulation *pSetpoints  );  // Set ID and number of messages to be send by simulation by categories of messages. 
  void    simulationCoilsCharacteristics          ( struct GEN_utility *  pGEN                                                );  // Realize some specific coils characteristics selection.
  void    simulationInitialization                ( struct GEN_utility *  pGEN                                                );  // Specific simulation initialization.
  bool    coilQueueAndTrackingManagement          ( FILE **traceFilePointer, bool    hasSimulationMode         // Entry: Has the model a simulation mode?
                                                  , double  weldsPosition        []   // Out: Weld position of reference coil regarding reference section.
                                                  , char    referenceCoilIdentity[]   // Out: Reference coil identity: first coil necessary.
                                                  , long   &nbWelds                   // Out: Index of reference section.
                                                  , struct GEN_utility *  pGEN
                                                  );
  void    onLiveManagementBasicSection            ( long      nuSectionInLine         // Entry: Section index in Line definition
                                                  , double    timeSpan                // Entry: TYime span since last management.
                                                  , struct GEN_utility *  pGEN
                                                  );
  void    dynamicModelParameterInitialisation     ( long nuLine, struct MOD_HV_TRS_In_specific_parameters * pDynSpecific );
  void    onLiveControlParameterInitialisation    ( SPE_specific_on_live_parameters * parameters );
  double  stripRollExchangeCoefficient            ( );
  bool    jetOnLiveControl                        ( long nuSectionInType, struct UTL_pid_objects * pPidJET, struct MOD_HV_pointers_section_line *  pSectionMSC, struct MOD_HV_coil_calcul_stage_section * pStageSection, double * pHD, struct MOD_HV_TRS_profil * pProfil, double * coolDemandMeasure );
  bool    jetSaturationCalculation                ( long nuSectionInType, struct UTL_pid_objects * pPidJET, struct MOD_HV_pointers_section_line *  pSectionMSC, struct MOD_HV_coil_calcul_stage_section * pStageSection, double * pHD, struct MOD_HV_TRS_profil * pProfil, double * coolDemandMeasure, bool &isSaturatedMax, bool &isSaturatedMin );
  bool    jetOnLiveNotUnderControl                ( long nuSectionInType, double cv[], double modelSp[], UTL_pid_objects * pPidJET, struct MOD_HV_TRS_profil * pProfil );
  void    finalSpecificActions                    ( struct GEN_utility *  pGEN );
  void    onLiveCalculationBegins                 ( FILE **traceFilePointer, double timeSpan, struct GEN_utility *  pGEN );
  void    pyrometerTransfertInOnLive              ( struct MOD_HV_pointers_section_line *         pSectionMSC,
                                                    struct MOD_HV_coil_calcul_stage_section *     pStageSection );
  void    lastActionsBeforeSetpointSending        ( FILE ** ppTraceFilePointer, struct MOD_HV_TRS_profil * pProfil, struct MOD_HV_TRS_profil * pProfilDeviation, struct GEN_utility *  pGEN );
  double  wallThermalLossesHFD                    ( double    wallTemperature         // Entry: Wall temperature.
                                                  , double    correctionFactor        // Entry: hfd correction factor.
                                                  , double    stripWidth              // Entry: strip width.
                                                  );
  double  RTFwallThermalLossesHFD                 ( long nuSectionInGlobal, double wallTemperature, double stripwidth, bool isSideWall );
  bool    onLiveAllowedToReduceSpeed              ( double stripThickness );
  bool    staticModelInputLimitation              ( double &modelInputOnZone, double maximumTemperature, double thickness, double width );
  double  getPotTemperature                       ( );
  void    changeJETConfiguration                  ( struct zone_jet * pZoneJet, struct JET_model * pEczJET, long nuZone, long nuECZ );
  bool    dffZoneMaximumPower                     ( long nuSectionInLine, long nuZone, double width, double &maximumPower );
  bool    dffZoneFixedHD                          ( long nuSectionInLine, long nuZone, double refparameterModelInput, double & FixedHD, bool & Imposed );
  void    DFFSetpointCorrection                   ( double &heatDemand, long nuZone );
  void    copyHeatCycle                           ( void * pPointer );
  void    storeHeatCycleInSpecific                ( );
  void    errorOnRTFCorrection                    ( long nuSectionInLine, double &errorOnPyrometerExit, bool control, struct GEN_utility *  pGEN );
  bool    rulesForSelectingHangingThBack          ( long nuZoneRTFGlobal, long nuTubeInZone, long nbTubeInZone, long nuHangingIndex, long &nuHangingTubeIndexInZone, long &nuHangingTubeIndexGlobal );
  bool    staticModelInput                        ( struct struct_section  *pModel, double * modelInputOnZones, double refparameterModelInput
                                                  , double    firstWidth                  // Entry: First  coil width.
                                                  , double    firstThickness              // Entry: First  coil thickness.
                                                  , double    secondWidth                 // Entry: Second coil width     (transient situation).
                                                  , double    secondThickness             // Entry: Second coil thickness (transient situation). 
                                                  );
  void    readSpecificCommunication               ( FILE **traceFilePointer, void *   pMeasures , struct GEN_utility *  pGEN, bool isComExe, long lType );
  void    writeSpecificCommunication              ( FILE **traceFilePointer, void *   pSetpoints, struct GEN_utility *  pGEN, bool isComExe, long lType );
  double  calculateStripEntryTemperature          ( double * zoneHD, bool isHDElseTubeTemp );
  double  calculateStripEntryTemperature          ( double   zoneHDAvg );
};

//extern struct SPE_specific   specific;      // Declaration of specific structure.

#endif