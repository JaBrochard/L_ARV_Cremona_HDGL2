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
#define HMI_HV_BUSINESS_NUMBER             2007                // 2001 : Business Number: Calvert line 2.

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
  //double    maxSpeedPot;
  //double    minSpeedCoil;
  //double    minSpeedDFF;
  //double    offsetTempEE;
  //double    upperToleranceP13;
  //double    stripTargetP13;
  //double    lowerToleranceP13;
  char    sGiOrGa[DIM_NAMES + 1];
  bool    isGAElseGi;
  double  alliationMaxSpeed;
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
// Specific GALMA 1 file for JET max rotation speed by QT.
struct oneQT
{
  char    QT[16];
  double  zoneMaxDemand[3];
};

struct jetMaxRotationSpeedPerQt
{
  struct oneQT    DefaultValues;
  struct oneQT    QTs          [1000];
  long            nbQt;
};






struct fichbob
{
	char      nobob[8+1]; 				  /* Numero de bobine */
	char	    res3[2];   						/* reserve pour alignement */                        
	float	    larg;	    					  /* Largeur bande en mm */
	float	    epais;	    					/* Epaisseur bande en mm */
	float	    longueur;						  /* Longueur bande en m */
	float	    posqb_sormain;				/* Position queue de bande/sortie maintien */
	float	    vmax;    						  /* vitesse max en m/mn */
	char      typeacier[5+1];			  /* code acier */
	char	    res1[2];    					/* Reserve pour allignement */
	float	    tbvischa;     				/* Temp de bande visee chauffe */
	float	    tbmaxcha;    				  /* Temp bd visee limit sup chauffe */
	float	    tbmincha;    				  /* Temp bd visee limit inf chauffe */
	float	    tbvismai;    				  /* Temp de bande visee maintien */
	float	    tbmaxmai;    				  /* Temp bd visee limit sup maintien */
	float	    tbminmai;    				  /* Temp bd visee limit inf maintien */
	float	    tbvisref;    				  /* Temp de bande visee refroidissement rapide */
	float	    tbmaxref;    				  /* Temp bd visee limit sup refroidissement rapide */
	float	    tbminref;    				  /* Temp bd visee limit inf refroidissement rapide */
	float	    rampz1;    					  /* Rampe de refroisissement zone 1 JET */
	float	    rampz2;    					  /* Rampe de refroisissement zone 2 JET */
	float	    rampz3;    					  /* Rampe de refroisissement zone 3 JET */
	float     vMaxLTOP;	    		    /* vitesse maximun théorique supportable par le four */
	float	    vMaxAlia_Pt_Triple;	  /* Vitesse max d'aliation en point triple, non utilisé */
	float	    vMaxAlia_Bas_Alu;		  /* Vitesse max d'aliation en bas alu, non utilisé */
	char	    Produit[3+1];	    	  /* Produit: GIX, GIZ, GAX, GAZ, BIC */
	char      res2[16];    				  /* Reserve */                                 
}; 

struct mesures
{
	/* LINE --> L-TOP */
	char 	    nobob[8+1]; 				/* Numero de bobine */  
	char 	    res8[3];    				/* réserve pour alignement */          
	float	    trac_dans; 					/* Traction danseur en kg */ 
	float     trac_chauf_1;			  /* Traction chauffage 1 en kg */
	float 	  trac_chauf_2;			  /* Traction chauffage 2 en kg */
	float	    trac_main; 					/* Traction maintien en kg */
	float	    vitcen;   					/* Vitesse ligne mesuree en m/mn */
	int       ctrlvit;   					/* controle vitesse par LTOP (0=operateur,1=L-TOP) */
	float	    vitmaxope;					/* Vitesse maxi operateur en m/mn */
	float			vitmaxalia;				  /* Vitesse maxi alliation en m/mn */
	char			res7[48];						/* Reserve */

	/* FUR --> L-TOP */
	float			tmpp3;   					  /* Temperature bande sortie chauffe P3 */
	float			Tmpp3co;   					/* Temperature bande sortie chauffe P3 en coin */
	float			tmpp3cm;   					/* Temperature bande sortie chauffe P3 en coin */
	float			tmpp3util;					/* Temperature bande sortie chauffe P3 utilisee */
	int				ctrlrtf;   					/* controle RTF par LTOP (0=operateur,1=L-TOP) */
	float			cha_chargz1;				/* Mesure de la charge z1 chauffe */
	float			cha_chargz2;				/* Mesure de la charge z2 chauffe */ 
	float			cha_chargz3;				/* Mesure de la charge z3 chauffe */
	float			cha_chargz4;				/* Mesure de la charge z4 chauffe */ 
	float			cha_chargz5;				/* Mesure de la charge z5 chauffe */ 
	float			cha_chargz6;				/* Mesure de la charge z6 chauffe */
	float			cha_chargz7;				/* Mesure de la charge z7 chauffe */
	float			cha_chargz8;				/* Mesure de la charge z8 chauffe */
	float			cha_tmp_t1z1;				/* Temperature tube 1 zone 1 */
	float			cha_tmp_t2z1;				/* Temperature tube 2 zone 1 */
	float			cha_tmp_t3z1;				/* Temperature tube 3 zone 1 */
	float			cha_tmp_t1z2;				/* Temperature tube 1 zone 2 */ 
	float			cha_tmp_t2z2;				/* Temperature tube 2 zone 2 */
	float			cha_tmp_t3z2;				/* Temperature tube 3 zone 2 */
	float			cha_tmp_t1z3;				/* Temperature tube 1 zone 3 */
	float			cha_tmp_t2z3;				/* Temperature tube 2 zone 3 */
	float			cha_tmp_t3z3;				/* Temperature tube 3 zone 3 */
	float			cha_tmp_t1z4;				/* Temperature tube 1 zone 4 */
	float			cha_tmp_t2z4;				/* Temperature tube 2 zone 4 */
	float			cha_tmp_t3z4;				/* Temperature tube 3 zone 4 */
	float			cha_tmp_t1z5;				/* Temperature tube 1 zone 5 */
	float			cha_tmp_t2z5;				/* Temperature tube 2 zone 5 */
	float			cha_tmp_t3z5;				/* Temperature tube 3 zone 5 */
	float			cha_tmp_t1z6;				/* Temperature tube 1 zone 6 */
	float			cha_tmp_t2z6;				/* Temperature tube 2 zone 6 */
	float			cha_tmp_t3z6;				/* Temperature tube 3 zone 6 */
	float			cha_tmp_t1z7;				/* Temperature tube 1 zone 7 */
	float			cha_tmp_t2z7;				/* Temperature tube 2 zone 7 */
	float			cha_tmp_t3z7;				/* Temperature tube 3 zone 7 */
	float			cha_tmp_t1z8;				/* Temperature tube 1 zone 8 */
	float			cha_tmp_t2z8;				/* Temperature tube 2 zone 8 */
	float			cha_tmp_t3z8;				/* Temperature tube 3 zone 8 */
	float			cha_tmp_z1;					/* Temperature zone 1 */
	float			cha_tmp_z2;					/* Temperature zone 2 */
	float			cha_tmp_z3;					/* Temperature zone 3 */
	float			cha_tmp_z4;					/* Temperature zone 4 */
	float			cha_tmp_z5;					/* Temperature zone 5 */
	float			cha_tmp_z6;					/* Temperature zone 6 */
	float			cha_tmp_z7;					/* Temperature zone 7 */
	float			cha_tmp_z8;					/* Temperature zone 8 */
	float			cha_tmp_fumz1;			/* Temperature fumee zone 1 */
	float			cha_tmp_fumz2;			/* Temperature fumee zone 2 */
	float			cha_tmp_fumz3;			/* Temperature fumee zone 3 */
	float			cha_tmp_fumz4;			/* Temperature fumee zone 4 */
	float			cha_tmp_fumz5;			/* Temperature fumee zone 5 */
	float			cha_tmp_fumz6;			/* Temperature fumee zone 6 */
	float			cha_tmp_fumz7;			/* Temperature fumee zone 7 */
	float			cha_tmp_fumz8;			/* Temperature fumee zone 8 */
	char			res4[28];	    			/* Reserve */   
	float			tmpp4;    					/* Temperature bande sortie maintien P4 */
	float			tmpp4b;   					/* Temperature bande sortie maintien P4 bis */
	float			tmpp4co;   					/* Temperature bande sortie maintien P4 en coin */  
	float			tmpp4cm;  					/* Temperature bande sortie maintien P4 en coin */
	float			tmpp4util;					/* Temperature bande sortie maintien P4 utilisee */
	int				ctrlmai;   					/* Controle MAINTIEN par LTOP (0=operateur,1=L-TOP) */
	int				stramai;   					/* Strategie du MAINTIEN (1=aucune,2=P3,3=P3 puis vitesse) */
	float			mai_tmp_z1;					/* Temperature zone 1 en degres */
	float			mai_tmp_z2;					/* Temperature zone 2 en degres */
	float			mai_tmp_z3;					/* Temperature zone 3 en degres */        
	float			mai_tmp_z4;					/* Temperature zone 4 en degres */        
	float			ter_tmp_z1;					/* Temperature zone 1 en degres */        
	float			ter_tmp_z2;					/* Temperature zone 2 en degres */        
	float			ter_tmp_z3;					/* Temperature zone 3 en degres */  
	char			res5[36];  					/* Reserve */                        
	float			offset_tmp;					/* Offset sur la temperature de bande chauffe et maintien (P3 et P4) en degres */
	int				ctrlref;   					/* controle Refroidissement rapide par LTOP (0=operateur,1=L-TOP) */
	float			tmpp10;    					/* Temperature bande sortie refroidissemt rapide P10 */
	float			tmpp11;   					/* Temperature bande sortie refroidissemt rapide P11 */
	float			tmpp12c1;					  /* temperature bande sortie refroidissemt rapide P12 en coin */ 
	float			tmpp12c2;  					/* Temperature bande sortie refroidissemt rapide P12 en coin */
	float			tmprefutil;					/* Temperature bande sortie refroidissemt rapide utilisee */
	float			ref_cha_z1av;				/* Charge zone 1 avant */
	float			ref_cha_z1ar;				/* Charge zone 1 arriere */                    
	float			ref_cha_z2av;				/* Charge zone 2 avant */                      
	float			ref_cha_z2ar;				/* Charge zone 1 arriere */
	float			ref_cha_z3av;				/* Charge zone 1 avant */              
	float			ref_cha_z3ar;				/* Charge zone 1 arriere */                    
	int				ctrlrefz1;					/* controle Z1 refroidissement [on/off] (0=operateur,1=L-TOP) */
	int				ctrlrefz2;					/* controle Z2 refroidissement [on/off] (0=operateur,1=L-TOP) */
	int				ctrlrefz3;					/* controle Z3 refroidissement [on/off] (0=operateur,1=L-TOP) */
	char			res6[12];  					/* Reserve */

	// Ajouts pour V-iModel début.
	int				iModelUsed;										      // Indique le modèle utilisé. (0 = L-TOP, 1 = V-iModel)
	int				iNbOfTubeInDefaultZone1;						// Nombre de tubes en défaut RTF zone 1.
	int				iNbOfTubeInDefaultZone2;						// Nombre de tubes en défaut RTF zone 2.
	int				iNbOfTubeInDefaultZone3;						// Nombre de tubes en défaut RTF zone 3.
	int				iNbOfTubeInDefaultZone4;						// Nombre de tubes en défaut RTF zone 4.
	int				iNbOfTubeInDefaultZone5;						// Nombre de tubes en défaut RTF zone 5.
	int				iNbOfTubeInDefaultZone6;						// Nombre de tubes en défaut RTF zone 6.
	int				iNbOfTubeInDefaultZone7;						// Nombre de tubes en défaut RTF zone 7.
	int				iNbOfTubeInDefaultZone8;						// Nombre de tubes en défaut RTF zone 8.
	float			fRTFGasFlowZone1;					// Nm3/h	// Débit de gaz RTF Zone 1.
	float			fRTFGasFlowZone2;					// Nm3/h	// Débit de gaz RTF Zone 2.
	float			fRTFGasFlowZone3;					// Nm3/h	// Débit de gaz RTF Zone 3.
	float			fRTFGasFlowZone4;					// Nm3/h	// Débit de gaz RTF Zone 4.
	float			fRTFGasFlowZone5;					// Nm3/h	// Débit de gaz RTF Zone 5.
	float			fRTFGasFlowZone6;					// Nm3/h	// Débit de gaz RTF Zone 6.
	float			fRTFGasFlowZone7;					// Nm3/h	// Débit de gaz RTF Zone 7.
	float			fRTFGasFlowZone8;					// Nm3/h	// Débit de gaz RTF Zone 8.
	// Ajouts pour V-iModel fin.

  
	// Ajouts pour V-iModel 20/05/2021 pour maintien début.
  float     fMaintienTempTubeZ1_1;    // °C     // Température de tube du maintien Zone 1 n°1.
  float     fMaintienTempTubeZ1_2;    // °C     // Température de tube du maintien Zone 1 n°2.
  float     fMaintienTempTubeZ1_3;    // °C     // Température de tube du maintien Zone 1 n°3.
  float     fMaintienTempTubeZ2_1;    // °C     // Température de tube du maintien Zone 2 n°1.
  float     fMaintienTempTubeZ2_2;    // °C     // Température de tube du maintien Zone 2 n°2.
  float     fMaintienTempTubeZ2_3;    // °C     // Température de tube du maintien Zone 2 n°3.
  float     fMaintienTempTubeZ3_1;    // °C     // Température de tube du maintien Zone 3 n°1.
  float     fMaintienTempTubeZ3_2;    // °C     // Température de tube du maintien Zone 3 n°2.
  float     fMaintienTempTubeZ3_3;    // °C     // Température de tube du maintien Zone 3 n°3.
  float     fMaintienTempTubeZ4_1;    // °C     // Température de tube du maintien Zone 4 n°1.
  float     fMaintienTempTubeZ4_2;    // °C     // Température de tube du maintien Zone 4 n°2.
  float     fMaintienTempTubeZ4_3;    // °C     // Température de tube du maintien Zone 4 n°3.
  
  float     fMaintienChargeZ1;        // %      // Puissance maintien zone 1.
  float     fMaintienChargeZ2;        // %      // Puissance maintien zone 2.
  float     fMaintienChargeZ3;        // %      // Puissance maintien zone 3.
  float     fMaintienChargeZ4;        // %      // Puissance maintien zone 4.
  float     fTunnelChargeZ1;          // %      // Puissance tunnel zone 1.
  float     fTunnelChargeZ2;          // %      // Puissance tunnel zone 2.
  float     fTunnelChargeZ3;          // %      // Puissance tunnel zone 3.
  
  
	// Ajouts pour V-iModel 20/05/2021 pour maintien fin.
}; 

struct consignes
{
	/* L-TOP --> FUR */
	int				mode_ctrl;					  /* Mode de controle (0=temperature , 1=charge) */     
	int				fou_cons_valid;				/* Validite des consignes four (0=invalide,1=valide) */
	float			cha_cons_tmp_z1;			/* Consigne temperature zone 1 */
	float			cha_cons_tmp_z2; 			/* Consigne temperature zone 2 */
	float     cha_cons_tmp_z3;			/* Consigne temperature zone 3 */
	float			cha_cons_tmp_z4; 			/* Consigne temperature zone 4 */  
	float     cha_cons_tmp_z5;			/* Consigne temperature zone 5 */ 
	float     cha_cons_tmp_z6;			/* Consigne temperature zone 6 */ 
	float			cha_cons_tmp_z7; 			/* Consigne temperature zone 7 */ 
	float			cha_cons_tmp_z8; 			/* Consigne temperature zone 8 */  
	float			cha_cons_chargz1;     /* Consigne charge z1 chauffe */
	float			cha_cons_chargz2;   	/* Consigne charge z2 chauffe */  
	float			cha_cons_chargz3;   	/* Consigne charge z3 chauffe */  
	float			cha_cons_chargz4;   	/* Consigne charge z4 chauffe */  
	float			cha_cons_chargz5;   	/* Consigne charge z5 chauffe */  
	float			cha_cons_chargz6;   	/* Consigne charge z6 chauffe */  
	float			cha_cons_chargz7;   	/* Consigne charge z7 chauffe */  
	float			cha_cons_chargz8;   	/* Consigne charge z8 chauffe */  
	char			res7[28];	     			  /* Reserve */
	float			mai_cons_tmp_z;				/* Consigne temperature zone en degres (appliquee sur 7 zones maintien) */
	char			res8[48];	   				  /* Reserve */
	int				ref_cons_valid;  			/* Validite des consignes ref rapide  (0=invalide,1=valide) */  
	float			ref_cons_cha_z1; 			/* Consigne charge zone 1 en % ref. rapide) */
	float			ref_cons_cha_z2; 			/* Consigne charge zone 2 en % ref. rapide) */
	float			ref_cons_cha_z3; 			/* Consigne charge zone 3 en % ref. rapide) */
	char			res9[32];	    			  /* Reserve */

	/* L-TOP --> LINE */
	int				eb_cons_valid;  			/* Validite des consignes entrainement de bande (0=invalide,1=valide) */
	float     cons_vitcen;   				/* Consigne vitesse ligne en m/mn */
	float			offset_tract;				  /* Offser traction (+ ou - 10%) */    
	int				demarrage_ok; 				/* Autorisation de demarrage vis a vis du risque de plis (0=non,1=oui) */
	int 			transient_encours;    /* transitoire en cours. 0=non 1=oui */
	float     Vmax_boballia_enc;    /* vitesse maximum sans limitation operateur */       
	char 			nobob_enc[8+1];       /* Numero de bobine */
	char			res11[3];             /* réserve pour alignement*/
	float     Vmax_boballia_sui;    /* vitesse maximum sans limitation operateur */       
	char 			nobob_sui[8+1];  			/* Numero de bobine */
	char			res12[3];	    			  /* réserve pour alignement*/      
	char			res10[48];    				/* Reserve */

	// Ajouts pour V-iModel début.
	float			fRTFGasFlowZone1;					// Nm3/h // Consigne de débit de gaz RTF Zone 1.
	float			fRTFGasFlowZone2;					// Nm3/h // Consigne de débit de gaz RTF Zone 2.
	float			fRTFGasFlowZone3;					// Nm3/h // Consigne de débit de gaz RTF Zone 3.
	float			fRTFGasFlowZone4;					// Nm3/h // Consigne de débit de gaz RTF Zone 4.
	float			fRTFGasFlowZone5;					// Nm3/h // Consigne de débit de gaz RTF Zone 5.
	float			fRTFGasFlowZone6;					// Nm3/h // Consigne de débit de gaz RTF Zone 6.
	float			fRTFGasFlowZone7;					// Nm3/h // Consigne de débit de gaz RTF Zone 7.
	float			fRTFGasFlowZone8;					// Nm3/h // Consigne de débit de gaz RTF Zone 8.
	// Ajouts pour V-iModel fin.

  // Ajouts pour V-iModel 20/05/2021 pour maintien début.
  float     fMaintienChargeZ1;        // %      // Puissance maintien zone 1.
  float     fMaintienChargeZ2;        // %      // Puissance maintien zone 2.
  float     fMaintienChargeZ3;        // %      // Puissance maintien zone 3.
  float     fMaintienChargeZ4;        // %      // Puissance maintien zone 4.
  float     fTunnelChargeZ1;          // %      // Puissance tunnel zone 1.
  float     fTunnelChargeZ2;          // %      // Puissance tunnel zone 2.
  float     fTunnelChargeZ3;          // %      // Puissance tunnel zone 3.
  
  int       iLifeCounter; 
	// Ajouts pour V-iModel 20/05/2021 pour maintien fin.

  
};


/* ATTENTION extern est très important sinon PB au link : undefsymb sur CMICOM */
//
//extern struct CMI
//{
//	long cmicom_deb;
//	struct fichbob m9100[8];
//	struct mesures m9101;    
//	struct consignes mcons;                                 
//	char res_cmicom[6587];
//	char cmicom_fin;
//};
//
//struct CMI CMICOM;


/* ATTENTION extern est très important sinon PB au link : undefsymb sur CMICOM */
/* note de JAB : l'extern était au dessus --> descendu pour compilation visual*/

struct p_BLD						/* note de JAB l'extern n'est plus */
{

	long cmicom_deb;
	struct fichbob m9100[8];
	struct mesures m9101;
	struct consignes mcons;
	//char res_cmicom[6587];
	char cmicom_fin;
};


struct com_globalSection
{
	char HVM_GLOBALSECTION_NAME[50 + 1];
	char HVM_GLOBALSECTION_FILE_NAME[50 + 1];
	struct p_BLD *                    pSec;                     // Pointer to GEN global section.
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
		if (NULL != HandleMutexAccess)  WaitForSingleObject(HandleMutexAccess, 2100L);
		else                              createOrOpenMutex(true);
	}

	// This function unlock the mutex.
	void unLockAccess()
	{
		if (NULL != HandleMutexAccess)  ReleaseMutex(HandleMutexAccess);
		else                              createOrOpenMutex(false);
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


	long map(long        actionType                       // Entry: Type of action: opening, creation.
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

		strncpy_s(this->HVM_GLOBALSECTION_NAME, 50, "HVM", strlen("HVM"));
		strncpy_s(this->HVM_GLOBALSECTION_FILE_NAME, 50, "HVM_GlobalSection_", strlen("HVM_GlobalSection_"));

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
				, GENERIC_READ | GENERIC_WRITE	           // Define acess mode.
				, FILE_SHARE_READ | FILE_SHARE_WRITE      // Define share mode.
				, &pointerSecurityAttributes							 // Pointer to security attributes.
				, localCreationType  		      			     // Define type of opening.
				// If the specified file exists, the function succeeds and the last-error code is set to ERROR_ALREADY_EXISTS (183).
				// If the specified file does not exist and is a valid path to a writable location, the function creates a file and the last - error code is set to zero.
				, FILE_ATTRIBUTE_NORMAL                   // The file or device attributes and flags, FILE_ATTRIBUTE_NORMAL being the most common default value for files.
				, NULL                                    // A valid handle to a template file with the GENERIC_READ access right. 
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

extern struct com_globalSection    gblSec;                       // Extern declaration of HVM global section.

struct SPE_specific 
{
  struct MOD_utility * pMOD = nullptr;
  double wallThermalLosses[DIM_SECTIONS] = {0};

  //------------------------------------------------------------------------------------
  // Application name.
  char                                    AppName       [DIM_NAMES + 1]     = "AM_MAR_GALMA2__"       ;           // Application name.
  char                                    heatCycleFile [DIM_NAMES + 1]     = "Heat Cycles AMNS HDGL4";           // Heat Cycles.
  char                                    heatCycleName [DIM_NAMES + 1]     = "Heat cycles HDGL4"     ;           // "Heat cycles"
  long                                    setPointCounter                   = 0;
  bool                                    hasCoilQueueIntegrated            = true;  
  bool                                    hasTrackingIntegrated             = true;
  double                                  onLiveDynModelDFFpyroTemp         = 0.;
  bool                                    useAdaptOnEmisElseThLosses        = true;
  bool                                    useSpeedJumpForTransitionning     = false;
  bool                                    hasSimulationMode                 = false;
  bool                                    doFullMPCCalculation              = false;
  DWORD                                   simulationSleepingTime            = 950;
  bool                                    isUsingDFFCascadeMode             = false;
  double                                  splitRangeDFFMax[DIM_ZONES]       = {0};
  double                                  splitRangeDFFMin[DIM_ZONES]       = {0};
  bool                                    simulationCombineTransfer         = false;
  double                                  coefTuningFixDFFEmissivity        = 0.;
  
  //------------------------------------------------------------------------------------
  // Standard Specific Data.
  long                                    computerNumber                    = 1;     // Indicate the computer number: server 1 or server 2.
  bool                                    useFixedDFFEmissivity             = false;
  bool                                    hasRecievedAllmessages [5]        = {0} ;
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
  double                                  lastRTFZoneHeatDemand[DIM_ZONES][200] = {0};
  long                                    hdIndex              [DIM_ZONES]      = {0};
  long                                    nbBurnerInDefaultRTF [DIM_ZONES]      = {0};
  long                                    nbHdSmothering                        = 5;
  bool                                    zoneOperationJET      [10]            = {0};
  double                                  HDDoubleZoneJET[4]                    = {0};
  struct UTL_pid_objects                  pidErrorRTF                           = {0};
  struct UTL_pid_objects                  pidErrorRTS                           = {0};
  struct jetMaxRotationSpeedPerQt         sJetMaxRotSpeedQt                     = {0};
  double                                  soakingTemperatureMeasurement         = 0.;
  double                                  soakingTemperatureSetpoint            = 0.;
  double                                  rtfZonesFumesTemperature [DIM_ZONES]  = {0};    
  double                                  alliationMaxSpeed                     = 0.; 
  struct UTL_pid_objects                  pidSoakingStripTemp                   = {0};  
  double                                  tractionOffset                        = 0.;
  

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
  void    readSpecificCommunication               ( FILE **traceFilePointer, struct MOD_HV_measures *    pMeasures, struct GEN_utility *  pGEN   );
  void    writeSpecificCommunication              ( FILE **traceFilePointer, struct MOD_HV_setpoints *   pSetpoints, struct GEN_utility *  pGEN  );
  double  calculateStripEntryTemperature          ( double * zoneHD, bool isHDElseTubeTemp );
  double  calculateStripEntryTemperature          ( double   zoneHDAvg );
};

//extern struct SPE_specific   specific;      // Declaration of specific structure.

#endif