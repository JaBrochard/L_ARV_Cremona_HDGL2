//------------------------------------------------------------------------------------
//		Copyrigth (r) BLD France   
//
//
//    General specific objects and methods.
//      
//      This file is concerning Yildiz CGL.
//      ---------------------------------------------
//
//------------------------------------------------------------------------------------
//
//
//
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
// Includes.
#include "stdafx.h"

//============================================================================
// SPE_specific: selectConfigurationFile
//
//    Select the name of the configuration file to be read.
//============================================================================
long SPE_specific::selectConfigurationFile
                                ( FILE **traceFilePointer		        // Entry   : Trace File pointer.
                                , char   configurationFileName[]    // Modified: Configuration file name.
                                , long   size                       // Entry   : File name size.
                                )
{
  long status = 1;
  char traceTempo [DIM_TRACES + 1] = {0};

  // Load AM NS HDCL 2.
  strcpy_s( traceTempo, DIM_PATH, "selectConfigurationFile: We are loading AM Mardyck Galma 2 Model." );
  pMOD->pUtl->writeTrace( traceFilePointer		  // Entry: File pointer.
                        , traceTempo            // Entry: Trace string.
                        );

  strcpy_s( traceTempo, DIM_PATH, "We are loading ARVEDI CREMONA HDGL2 Model." );

  strcpy_s( traceTempo, DIM_PATH, "ARV_Cremona_HDGL2.txt" );

  if ( sizeof("ARV_Cremona_HDGL2.txt") > size )
  {
    strcpy_s( traceTempo, DIM_PATH, "selectConfigurationFile: size of char too low. " );
    pMOD->pUtl->writeTrace( traceFilePointer		  // Entry: File pointer.
                          , traceTempo            // Entry: Trace string.
                          );
    status = -1;
  }
  else
  {
    strcpy_s( configurationFileName, size-1, traceTempo );
    status = 1;
  }

  return status;
}

//============================================================================
// SPE_specific: applicationInitialization
//
//   Specific application initilization.
//============================================================================
void SPE_specific::applicationInitialization( struct GEN_utility *  pGEN )
{
  char directory[DIM_PATH + 1] = {0};
  strcpy_s( directory, DIM_PATH, pGEN->pointerPath            );
  strcat_s( directory, DIM_PATH, GLOBALSECTION_PATH           );
  strcat_s( directory, DIM_PATH, "SPE_gblSec_ARV_CRE_HDGL2"   );
  strcat_s( directory, DIM_PATH, GLOBALSECTION_EXTENSION      );

  // MAP global section for communication.
  bool success = gblSec.map ( TYPE_CREATION                             // Entry: MAP global section.
                            , sizeof(struct p_BLD_specific)             // Entry: The low-order  DWORD of the maximum size of the file mapping object.
                            , "SPE_gblSec_ARV_CRE_HDGL2"                // Entry: Global section name.
                            , directory	                                // Entry: Global section filename.
                            , &gblSec.newGlobalSection                  // Exit: Creation indicator.
                            , (void**)&gblSec.pSpe                      // Exit: Adress of the global section.
                            , &gblSec.handleGlobalSection               // Exit: Handle of file.
                            );
 
  // Open or create mutex.
  gblSec.createOrOpenMutex( false );

  struct HVM_Parameter parameter = pGEN->pHvm->phvm->parameter;

  //// For each heat cycles.
  //for ( long i_cycle = 0; i_cycle < DIM_HEAT_CYCLES; i_cycle++ )
  //{
  //  parameter.lineCycle[0].shared.heatCycles[i_cycle].cycleNumber = i_cycle;
  //  
  //  // Each 100 cycles.
  //  if ( i_cycle % 100 == 0 )
  //  {
  //    if ( i_cycle / 100 < 55 || i_cycle / 100 > 87 ) continue;
  //  
  //    // For all sections.
  //    for ( long i_section = 0; i_section < pMOD->pConf->pconf->lines[0].nb_sections; i_section++ )
  //    {
  //      // RTF / SOA.
  //      if ( pMOD->pConf->pconf->lines[0].pSections[i_section]->type == TYPE_RTF ||
  //           pMOD->pConf->pconf->lines[0].pSections[i_section]->type == TYPE_SOA
  //         )
  //      {
  //        parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section].stripTarget = pMOD->pConv->kelvin( i_cycle / 10.      );
  //        parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section].stripLower  = pMOD->pConv->kelvin( i_cycle / 10. - 10. );
  //        parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section].stripUpper  = pMOD->pConv->kelvin( i_cycle / 10. + 30. );
  //        parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section].minimumTime = 20.;
  //      }
  //      // JET cooling.
  //      else if ( pMOD->pConf->pconf->lines[0].pSections[i_section]->type == TYPE_JET
  //              )
  //      {
  //        parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section].stripTarget = pMOD->pConv->kelvin( 460.      );
  //        parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section].stripLower  = pMOD->pConv->kelvin( 460. - 20 );
  //        parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section].stripUpper  = pMOD->pConv->kelvin( 460. + 20 );
  //        parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section].minimumTime = 0.;
  //        for ( long i_zone = 0; i_zone < pMOD->pConf->pconf->lines[0].pSections[i_section]->nb_zones; i_zone++ )
  //        {
  //          parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section].zones[i_zone].coolingRate = 10.;
  //        }
  //      }
  //      // NOF section type.
  //      else if ( pMOD->pConf->pconf->lines[0].pSections[i_section]->type == TYPE_NOF
  //              )
  //      {
  //        parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section].stripTarget = pMOD->pConv->kelvin( 620.        );
  //        parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section].stripLower  = pMOD->pConv->kelvin( 620. -   5. );
  //        parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section].stripUpper  = pMOD->pConv->kelvin( 620. + 120. );
  //        parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section].minimumTime = 0.;
  //      }
  //      // BASIC section type.
  //      else if ( pMOD->pConf->pconf->lines[0].pSections[i_section]->type == TYPE_BASE
  //              )
  //      {
  //        parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section].stripTarget = parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section-1].stripTarget;
  //        parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section].stripLower  = parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section-1].stripLower ;
  //        parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section].stripUpper  = parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section-1].stripUpper ;
  //        parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section].minimumTime = parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section-1].minimumTime;
  //      }
  //      // NOF section type.
  //      else if ( pMOD->pConf->pconf->lines[0].pSections[i_section]->type == TYPE_POT
  //              )
  //      {
  //        parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section].stripTarget = pMOD->pConv->kelvin( 460.      );
  //        parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section].stripLower  = pMOD->pConv->kelvin( 460. - 10 );
  //        parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section].stripUpper  = pMOD->pConv->kelvin( 460. + 10 );
  //        parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section].minimumTime = 0.;
  //      }
  //    } // END: For all sections.
  //  } // END: % 100
  //
  //}

  // For all zones.
  for ( long iZone = 0; iZone < pMOD->pConf->pconf->nb_zones; iZone++ )
  {
    long nuZoneGlobal = pMOD->pConf->pconf->pZones[iZone]->indexGlobal;
  
    if      ( pMOD->pConf->pconf->pZones[iZone]->type == TYPE_RTF ||
              pMOD->pConf->pconf->pZones[iZone]->type == TYPE_SOA
            )
    {
      parameter.zones[nuZoneGlobal].maximumTemperature  = pMOD->pConv->kelvin( 920. );
      parameter.zones[nuZoneGlobal].maximumPower        = 100.;
      parameter.zones[nuZoneGlobal].emissivityFactor    =   0.;
      for ( long i_point = 0; i_point < parameter.zones[nuZoneGlobal].efficiency.nb_points; i_point++ )
      {
        parameter.zones[nuZoneGlobal].efficiency.Y[i_point] = 1.;
      }
    }
    // NOF section type.
    else if ( pMOD->pConf->pconf->pZones[iZone]->type == TYPE_NOF
            )
    {
      long nuZoneType = pMOD->pConf->pconf->pZones[iZone]->indexType;
      parameter.zones[nuZoneGlobal].maximumTemperature  = pMOD->pConv->kelvin( 1350. );
      parameter.zones[nuZoneGlobal].maximumPower        = pMOD->pConf->pconf->zonesNOF[nuZoneType].Power;
      parameter.zones[nuZoneGlobal].emissivityFactor    =   1.;
      for ( long i_point = 0; i_point < parameter.zones[nuZoneGlobal].efficiency.nb_points; i_point++ )
      {
        parameter.zones[nuZoneGlobal].efficiency.Y[i_point] = 1.;
      }
    }
  }
  
  // Write parameters.
  pGEN->pHvm->writeParameters( &parameter, true );

  // Adaptation structure.
  for ( long i_emi = 0; i_emi < DIM_EMISSIVITY_CLASS; i_emi++ )
  {
    for ( long iSection = 0; iSection < DIM_SECTIONS; iSection++ )
    {
      if ( pGEN->pHvm->phvm->adapatation.emissivityClasses[i_emi].emissivity[iSection] < 0.05 ||
           pGEN->pHvm->phvm->adapatation.emissivityClasses[i_emi].emissivity[iSection] > 0.85 )
        pGEN->pHvm->phvm->adapatation.emissivityClasses[i_emi].emissivity[iSection] = 0.23;

      pGEN->pHvm->phvm->adapatation.emissivityClasses[i_emi].emissivity[iSection] = 
        min( max( pGEN->pHvm->phvm->adapatation.emissivityClasses[i_emi].emissivity[iSection], 0.05 ), 0.85 );
    }

  }

  ////------------------------------------------------------------------------------------
  //// Read new bisra file.
  //bool    status = true ;
  //char    fil_name[1000];
  //FILE    *bisra_file   ;
  //errno_t error         ;

  //char * pointerPathLocal = nullptr;
  //size_t length           =  0 ;

  //pointerPathLocal = NULL;
  //_dupenv_s (&pointerPathLocal, &length, "DLineProject");
  //strcpy_s( fil_name, pointerPathLocal                );
  ////strcat_s( fil_name, DIM_PATH, "\\"                  );
  //strcat_s( fil_name, DIM_PATH, DATA_PATH             );
  //strcat_s( fil_name, DIM_PATH, "BISRA - Galsa2.data" );

  //// Opening BISRA file.
  //if ( ( error = fopen_s( &bisra_file,fil_name, "r")) == 0 )
  //{
  //  // Set pointer to file begin.
  //  fseek(bisra_file, 0L, SEEK_SET);

  //  for ( long j = 0; j < 16; j++ )
  //  {
  //    memset( &pMOD->pConf->pconf->physicalProperties.material.materials[j], 0, sizeof(struct  UTL_material) );
  //    pMOD->pConf->pconf->physicalProperties.material.materials[j].nb_temperatures = 0;
  //  }

  //  pMOD->pConf->pconf->physicalProperties.material.nb_materials = 16;

  //  // For all 22 bisra codes.
  //  for ( long i = 0; i < 196; i++ )
  //  {
  //    float temperature = 0;
  //    fscanf_s( bisra_file, "%8f", &temperature );
  //    temperature = pMOD->pConv->kelvin( temperature );

  //    for ( long j = 0; j < 17; j++ )
  //    {
  //      if ( j == 16 )
  //      {
  //        float tempo = 0;
  //        fscanf_s( bisra_file, "%8f", &tempo );
  //        for ( long k = 0; k < 16; k++ )
  //          pMOD->pConf->pconf->physicalProperties.material.materials[k].VolumicMass[i] = (double)tempo;
  //      }
  //      else
  //      {
  //        pMOD->pConf->pconf->physicalProperties.material.materials[j].nb_temperatures ++;
  //        pMOD->pConf->pconf->physicalProperties.material.materials[j].Temperatures[i] = temperature;

  //        float tempo = 0;
  //        fscanf_s( bisra_file, "%8f", &tempo );
  //        pMOD->pConf->pconf->physicalProperties.material.materials[j].Enthalpies[i] = (double)tempo * 1000.;
  //      }
  //    }
  //  }
  //  // Close the file.
  //  fclose(bisra_file);

  //  // Post traitment for calorific value.
  //  for ( long j = 0; j < 16; j++ )
  //  {
  //    //double values[200] = {0};
  //    for ( long i = 1; i < 196; i++ )
  //    {
  //      pMOD->pConf->pconf->physicalProperties.material.materials[j].CalorificCapacities[i] = 
  //        ( pMOD->pConf->pconf->physicalProperties.material.materials[j].Enthalpies[i    ] - 
  //          pMOD->pConf->pconf->physicalProperties.material.materials[j].Enthalpies[i - 1] ) / 
  //        ( pMOD->pConf->pconf->physicalProperties.material.materials[j].Temperatures[i    ] - 
  //          pMOD->pConf->pconf->physicalProperties.material.materials[j].Temperatures[i - 1] );
  //    }
  //    pMOD->pConf->pconf->physicalProperties.material.materials[j].CalorificCapacities[0] = pMOD->pConf->pconf->physicalProperties.material.materials[j].CalorificCapacities[1];
  //  }

  //  // Post traitment for volumic mass.
  //  for ( long j = 0; j < 16; j++ )
  //  {
  //    double values[200] = {0};
  //    for ( long i = 0; i < 196; i++ )
  //    {
  //      if ( pMOD->pConf->pconf->physicalProperties.material.materials[j].VolumicMass[i] <= pMOD->pMath->epsilon )
  //      {
  //        long lowerIndex = -1;
  //        long upperIndex = -1;

  //        for ( long ii = i-1; ii >= 0; ii-- )
  //        {
  //          if ( pMOD->pConf->pconf->physicalProperties.material.materials[j].VolumicMass[ii] >= pMOD->pMath->epsilon )
  //          {
  //            lowerIndex = ii;
  //            break;
  //          }
  //        }

  //        for ( long ii = i+1; ii < 196; ii++ )
  //        {
  //          if ( pMOD->pConf->pconf->physicalProperties.material.materials[j].VolumicMass[ii] >= pMOD->pMath->epsilon )
  //          {
  //            upperIndex = ii;
  //            break;
  //          }
  //        }

  //        if ( lowerIndex < 0 )
  //        {
  //          values[i] = pMOD->pConf->pconf->physicalProperties.material.materials[j].VolumicMass [upperIndex];
  //        }
  //        else
  //          // Linear regression.
  //          values[i] = pMOD->pConf->pconf->physicalProperties.material.materials[j].VolumicMass[lowerIndex] + 
  //            ( pMOD->pConf->pconf->physicalProperties.material.materials[j].VolumicMass [upperIndex] - pMOD->pConf->pconf->physicalProperties.material.materials[j].VolumicMass [lowerIndex] ) / 
  //            ( pMOD->pConf->pconf->physicalProperties.material.materials[j].Temperatures[upperIndex] - pMOD->pConf->pconf->physicalProperties.material.materials[j].Temperatures[lowerIndex] ) * 
  //            ( pMOD->pConf->pconf->physicalProperties.material.materials[j].Temperatures[i]          - pMOD->pConf->pconf->physicalProperties.material.materials[j].Temperatures[lowerIndex] );

  //      }
  //      else
  //        values[i] = pMOD->pConf->pconf->physicalProperties.material.materials[j].VolumicMass[i];
  //    }

  //    for ( long i = 0; i < 196; i++ )
  //      pMOD->pConf->pconf->physicalProperties.material.materials[j].VolumicMass[i] = values[i];
  //  }

  //}
  //else
  //{
  //  status = false;
  //}

  //bool hsbfvsdjbsj = true;

  return;
}

//============================================================================
// SPE_specific: finalExecutionActions
//
//    Specific application final actions.
//============================================================================
void SPE_specific::finalExecutionActions( )
{
  if ( pScenarioDataInput != nullptr ) { delete ( pScenarioDataInput ); pScenarioDataInput = nullptr; }
  for ( long i = 0; i < 20; i++ )
  {
    if ( pHeatCycles[i] != nullptr ) { delete ( pHeatCycles[i] ); pHeatCycles[i] = nullptr; }
  }
  //gblSec.DestroyMutex( ); no to avoid final looking of one software...
  return;
}

//============================================================================
// SPE_specific: applicationConfigurationFinazation
//
//    Specific application configuration..
//============================================================================
void SPE_specific::applicationConfigurationFinalization
                              ( bool                  hasSimulationMode        // Entry: Has the model a simulation mode?
                              , struct GEN_utility *  pGEN
                              )
{
  // L-TOP is zero and V-iModel 1.
  this->computerNumber = 1;


  pScenarioDataInput = new struct MOD_HV_scenarioData;
  memset( pScenarioDataInput, 0, sizeof( struct MOD_HV_scenarioData ) );

  if ( pGEN->pConf->pconf->isCommunicationClass )
  {
    for ( long i = 0; i < 20; i++ )
    {
      pHeatCycles[i] = new ( struct MOD_HV_GLB_parameter_heat_cycle );
      memset( pHeatCycles[i], 0, sizeof ( struct MOD_HV_GLB_parameter_heat_cycle ) );
    }
    
  }

  //// To be removed later.
  //for ( long i_emi = 0; i_emi < DIM_EMISSIVITY_CLASS; i_emi++ )
  //{
  //  for ( long iSection = 0; iSection < pMOD->pConf->pconf->nb_sections; iSection++ )
  //  {
  //    long    sectionType = pMOD->pConf->pconf->pSections[iSection]->type;
  //
  //    if ( specific.useFixedDFFEmissivity && sectionType == TYPE_NOF )
  //    {
  //      if ( pGEN->pHvm->phvm->adapatation.emissivityClasses[i_emi].emissivity[iSection] < pMOD->pMath->epsilon )
  //      {
  //        //if      ( i_emi == 0 )   pGEN->pHvm->phvm->adapatation.emissivityClasses[i_emi].emissivity[iSection] = 0.28;
  //        //else if ( i_emi == 1 )   pGEN->pHvm->phvm->adapatation.emissivityClasses[i_emi].emissivity[iSection] = 0.29;
  //        //else if ( i_emi == 2 )   pGEN->pHvm->phvm->adapatation.emissivityClasses[i_emi].emissivity[iSection] = 0.30;
  //        //else if ( i_emi == 3 )   pGEN->pHvm->phvm->adapatation.emissivityClasses[i_emi].emissivity[iSection] = 0.32;
  //        //else if ( i_emi == 4 )   pGEN->pHvm->phvm->adapatation.emissivityClasses[i_emi].emissivity[iSection] = 0.34;
  //        //else if ( i_emi == 5 )   pGEN->pHvm->phvm->adapatation.emissivityClasses[i_emi].emissivity[iSection] = 0.36;
  //        //else                     pGEN->pHvm->phvm->adapatation.emissivityClasses[i_emi].emissivity[iSection] = 0.30;
  //      }
  //    }
  //    else if (  hasSimulationMode && sectionType == TYPE_RTF )
  //      pGEN->pHvm->phvm->adapatation.emissivityClasses[i_emi].emissivity[iSection] = 0.23;
  //    else if ( !hasSimulationMode && sectionType == TYPE_RTF && pGEN->pHvm->phvm->adapatation.emissivityClasses[i_emi].emissivity[iSection] < pMOD->pMath->epsilon )
  //      pGEN->pHvm->phvm->adapatation.emissivityClasses[i_emi].emissivity[iSection] = 0.23;
  //
  //  }
  //
  //}

  //// Find the furnace communication.
  //if ( !hasSimulationMode )
  //{
  //  for ( long i_com = 0; i_com < pMOD->pConf->pconf->coms.lNbTcpCommunication; i_com++ )
  //  {
  //    // If we have found the furnace TCP communication.
  //    if ( strncmp( pMOD->pConf->pconf->coms.comTcp[i_com].name , "TCP_Communication_Furnace", 29 ) == 0 )
  //    {
  //    
  //      //// Specify the port of the TCP communication with the furnace PLC, depending of the server.
  //      //if ( specific.computerNumber == 1 ) pMOD->pConf->pconf->coms.comTcp[i_com].lhostPortNumber = 15101;
  //      //else                                pMOD->pConf->pconf->coms.comTcp[i_com].lhostPortNumber = 15102;
  //    
  //    }
  //  }
  //}

  this->hasSimulationMode = hasSimulationMode;

  if ( hasSimulationMode || true )
  {
    struct HVM_Parameter parameter = pGEN->pHvm->phvm->parameter;

    for ( long i_zone = 0; i_zone < pMOD->pConf->pconf->lines[0].pSections[1]->nb_zones; i_zone++ )
    {
      long nuZoneType   = pMOD->pConf->pconf->lines[0].pSections[1]->pZones[i_zone]->indexType;
      long nuZoneGlobal = pMOD->pConf->pconf->lines[0].pSections[1]->pZones[i_zone]->indexGlobal;
      parameter.zones[i_zone].maximumTemperature        = pMOD->pConv->kelvin( 957. );
      for ( long i_point = 0; i_point < parameter.zones[i_zone].efficiency.nb_points; i_point++ )
      {
        parameter.zones[i_zone].efficiency.Y[i_point] = 1.;
      }
    }

    for ( long i_zone = 0; i_zone < pMOD->pConf->pconf->nb_zones; i_zone++ )
    {
      if ( pMOD->pConf->pconf->pZones[i_zone]->type == TYPE_RTF ||
           pMOD->pConf->pconf->pZones[i_zone]->type == TYPE_SOA
         )
      {
        EQP_config_zone_RTF * pZoneRTF = ( EQP_config_zone_RTF * ) pMOD->pConf->pconf->pZones[i_zone];
        parameter.zones[i_zone].maximumTemperature        = pMOD->pConv->kelvin( 957. );
        parameter.zones[i_zone].maximumPower              = pZoneRTF->Power;
        parameter.zones[i_zone].emissivityFactor          =   0.;
        parameter.zones[i_zone].boosterBumperLow          =   0.;
        parameter.zones[i_zone].boosterBumperUpp          =   0.;
        parameter.zones[i_zone].demandUppRamp             =   4.;
        parameter.zones[i_zone].demandDwRamp              =   4.;
        parameter.zones[i_zone].pots[0].replyTemperature  =   pMOD->pConv->kelvin( 460. );
        for ( long i_point = 0; i_point < parameter.zones[i_zone].efficiency.nb_points; i_point++ )
        {
          parameter.zones[i_zone].efficiency.Y[i_point] = 1.;
        }
      }
      // NOF section type.
      else if ( pMOD->pConf->pconf->pZones[i_zone]->type == TYPE_NOF
              )
      {
        long nuZoneType = pMOD->pConf->pconf->pZones[i_zone]->indexType;
        parameter.zones[i_zone].maximumTemperature  = pMOD->pConv->kelvin( 1350. );
        parameter.zones[i_zone].emissivityFactor    =   1.;
        parameter.zones[i_zone].maximumPower        = pMOD->pConf->pconf->zonesNOF[nuZoneType].Power;
          
        parameter.zones[i_zone].emissivityFactor  =   1.;
        //if ( !this->useAdaptOnEmisElseThLosses )
        //  parameter.zones[i_zone].emissivityFactor  =   1.;

        parameter.zones[i_zone].efficiency.nb_points = 40;
        for ( long i_point = 0; i_point < parameter.zones[i_zone].efficiency.nb_points; i_point++ )
        {
          parameter.zones[i_zone].efficiency.X[i_point] = pMOD->pConv->kelvin( 50. * i_point );

          parameter.zones[i_zone].efficiency.Y[i_point] = 1.;
          if ( this->useAdaptOnEmisElseThLosses )
            parameter.zones[i_zone].efficiency.Y[i_point] = 1.;
        }
      }
      // JET section type.
      else if ( pMOD->pConf->pconf->pZones[i_zone]->type == TYPE_JET
              )
      {
        long nuZoneType = pMOD->pConf->pconf->pZones[i_zone]->indexType;
          
        parameter.zones[i_zone].emissivityFactor          =   1.;
        parameter.zones[i_zone].demandUppRamp             =   4.;
        parameter.zones[i_zone].demandDwRamp              =   4.;
        parameter.zones[i_zone].jetMaximumFlowFactor      =   1.;
        parameter.zones[i_zone].pots[0].replyTemperature  =   pMOD->pConv->kelvin( 460. );

      }
    }
    

    pGEN->pHvm->phvm->parameter = parameter;

    // Write parameters.
    pGEN->pHvm->writeParameters( &parameter, true );
  }
  return;
}

//============================================================================
// SPE_specific: messagesRecieved
//
//    Specific actions on messages recieved.
//============================================================================
bool SPE_specific::messagesRecieved
                              ( MSG *pMessage      // Entry: Message peeked.
                              )
{
  bool actionDone = false;
  return actionDone;
}

//============================================================================
// SPE_specific: messagesToSend
//
//    Specific actions on messages to send.
//============================================================================
bool SPE_specific::messagesToSend
                              ( MSG *pMessage      // Entry: Message peeked.
                              )
{
  bool actionDone = false;
  return actionDone;
}

//============================================================================
// SPE_specific: storeMessageRecievedInformation
//
//    Store information on the message just recieved.
//============================================================================
void SPE_specific::storeMessageRecievedInformation
                              ( long                        messageId      // Entry: Recieved message Identity.
                              , struct MOD_HV_measures *    pMeasures      // Entry: Pointer to measurement structure.
                              )
{
  // Store message ID recieved.
  pMeasures->measurementMessage = messageId;

  if      ( messageId == 4001 || messageId == 4003 || 
            messageId == 4004 || messageId == 4012 || messageId == 4013 || messageId == 4011 )
  {
    pMeasures->measurementType        = TYPE_COILS;             // USED !!!
    pMeasures->nb_messageToSendBack   = 0 ;
    for ( long i_msg = 0; i_msg < DIM_MESSAGES_MAX; i_msg++ )
    {
      pMeasures->messageIdToSendBack[i_msg] = -1;
    }
  }
  else if ( messageId == 4002 ) // 4053
  {
    pMeasures->measurementType        = TYPE_TRACKING;          // USED !!!
    pMeasures->nb_messageToSendBack   = 0 ;
    for ( long i_msg = 0; i_msg < DIM_MESSAGES_MAX; i_msg++ )
    {
      pMeasures->messageIdToSendBack[i_msg] = -1;
    }
  }
  else if ( messageId == 4000 || messageId == 4050 || messageId == 4051 || 
            messageId == 4052 || messageId == 4053 || messageId == 4054 || 
            messageId == 4055 )      
  {
    pMeasures->measurementType        = TYPE_ALL_SECTION;
    if ( messageId == 4000 )   // <== Définir the one who rules the send.
    {
      pMeasures->nb_messageToSendBack   = 4;
      for ( long i_msg = 0; i_msg < DIM_MESSAGES_MAX; i_msg++ )
      {
        pMeasures->messageIdToSendBack[i_msg] = -1;
      }
      pMeasures->messageIdToSendBack[0] = 4102;   // Furnace and line setpoints message.
      pMeasures->messageIdToSendBack[1] = 4100;   // Alive message.
      pMeasures->messageIdToSendBack[2] = 8180;   // Observer message.
      pMeasures->messageIdToSendBack[3] = 5101;   // Observer message.
    }
  }
  // Heat cycle table.
  else if ( messageId == 5001 )
  {
    pMeasures->measurementType        = TYPE_MSG_HEAT_CYCLE_TABLE_RCV;          // USED !!!
    pMeasures->nb_messageToSendBack   = 0 ;
    for ( long i_msg = 0; i_msg < DIM_MESSAGES_MAX; i_msg++ )
    {
      pMeasures->messageIdToSendBack[i_msg] = -1;
    }
  }
  // Coils Heat cycle ID.
  else if ( messageId == 5002 )
  {
    pMeasures->measurementType        = TYPE_MSG_COIL_HEAT_CYCLE_TABLE_RCV;          // USED !!!
    pMeasures->nb_messageToSendBack   = 0 ;
    for ( long i_msg = 0; i_msg < DIM_MESSAGES_MAX; i_msg++ )
    {
      pMeasures->messageIdToSendBack[i_msg] = -1;
    }
  }
  // Static Simulation calculation.
  else if ( messageId == 5003 )
  {
    pMeasures->measurementType        = TYPE_MSG_STATIC_SIMUL_CALC;          // USED !!!
    pMeasures->nb_messageToSendBack   = 1 ;
    for ( long i_msg = 0; i_msg < DIM_MESSAGES_MAX; i_msg++ )
    {
      pMeasures->messageIdToSendBack[i_msg] = -1;
    }
    pMeasures->messageIdToSendBack[0] = 5103;
  }
  else if ( messageId == -9999 )
  {
    pMeasures->measurementType        = TYPE_SECTION;
    pMeasures->nb_messageToSendBack   = 1;
    for ( long i_msg = 0; i_msg < DIM_MESSAGES_MAX; i_msg++ )
    {
      pMeasures->messageIdToSendBack[i_msg] = -1;
    }
    pMeasures->messageIdToSendBack[0] = 129;
  }

  // Check if no message has been recieved from long time for heat cycle table and heat cycle table is not completed.
  time_t currentTimeLocal = 0;
  time( &currentTimeLocal );

  // If long time elapse and at least one table is missing.
  if ( difftime( currentTimeLocal, lastRecievedFirstHCMessage ) > 5 * 60 &&  // 5 * 60
       lNbHeatCycleStored < lNbHeatCycleToBeStored                       && 
       lNbHeatCycleStored > 0 
       )
  {
    // Reset number of heat cycle table stored.
    lNbHeatCycleStored = 0;
  }

  return;
}

//============================================================================
// SPE_specific: storeMessageRecievedInformationSIM
//
//    Store information on the message just recieved for simulation.
//============================================================================
void SPE_specific::storeMessageRecievedInformationSIM
                              ( long                                   messageId      // Entry: Recieved message Identity.
                              , struct MOD_HV_measures_simulation *    pMeasures      // Entry: Pointer to measurement structure.
                              )
{
  if ( messageId == 102 )    // All setpoints.
  {
    pMeasures->measurementType        = TYPE_LINE_SETPOINT;
    pMeasures->nb_messageToSendBack   = 0 ;
    for ( long i_msg = 0; i_msg < DIM_MESSAGES_MAX; i_msg++ )
    {
      pMeasures->messageIdToSendBack[i_msg] = -1;
    }
  }
  else if ( messageId == 101 )
  {
    pMeasures->measurementType        = TYPE_ALL_SECTION_SETPOINT;
    pMeasures->nb_messageToSendBack   = 0 ;
    for ( long i_msg = 0; i_msg < DIM_MESSAGES_MAX; i_msg++ )
    {
      pMeasures->messageIdToSendBack[i_msg] = -1;
    }
  }
  return;
}

//============================================================================
// SPE_specific: simulationMessagesToSend
//
//    Set ID and number of messages to be send by simulation by categories of messages. 
//    Categories: COILS / SECTIONS / LINE
//============================================================================
void SPE_specific::simulationMessagesToSend
                              ( long                                   typeOfMessageTosend     // Entry: Type of message to send.
                              , struct   MOD_HV_setpoints_simulation * pSetpoints              // Entry: Pointer to tempory simulation setpoint structure.
                              )
{
  if      ( typeOfMessageTosend == TYPE_COILS )                 // Coils setpoints.
  {
    pSetpoints->nb_messageToSendBack   = 1;
    pSetpoints->messageIdToSendBack[0] = 2;
  }
  else if ( typeOfMessageTosend == TYPE_LINE_SETPOINT )         // Line / tracking setpoints.
  {
    pSetpoints->nb_messageToSendBack   = 1;
    pSetpoints->messageIdToSendBack[0] = 3;
  }
  else if ( typeOfMessageTosend == TYPE_ALL_SECTION_SETPOINT )  // Furnace setpoints.
  {
    pSetpoints->nb_messageToSendBack   = 1;
    pSetpoints->messageIdToSendBack[0] = 1;
  }

  return;
}

//============================================================================
// SPE_specific: storeValueInMeasurementStructure
//
//   Extract the value in the right place from the setpoint structure
//   using the interface structure.
//============================================================================
void SPE_specific::extractValuesFromSetpointStructure
                                ( struct UTL_message_element_values     *    pValue   // Entry: Element Value.
                                , struct EQP_interface_message_elements *    pInter   // Entry: Element interface.
                                , struct MOD_HV_setpoints               *    pSpt     // Out  : pointer to setpoint structure.
                                )
{
  // Example:
  if ( strcmp( pInter->in.Mnemonic, "nouvelleConsigne" ) == 0 )
  {
    // Au choix selon le type de variable que vous avez choisit:
    pValue->lvalue = pSpt->specific.maNouvelleConsigne1;
    pValue->dvalue = pSpt->specific.maNouvelleConsigne2;
    strncpy( pValue->stringValue, pSpt->specific.maNouvelleConsigne3, sizeof(pSpt->specific.maNouvelleConsigne3) );
  }
  // d'autres structure spécifique peuvent mapper d'autres sous structure.
  else if ( strcmp( pInter->in.Mnemonic, "mistrackingPrevention" ) == 0 )
  {
    //pValue->lvalue = pSpt->specific.maNouvelleConsigne1;
  }

  
  if ( strcmp( pInter->in.Mnemonic, "TOP_COUNTER" ) == 0 )
  {
    if ( ++setPointCounter > 1000 ) setPointCounter = 0;
    pValue->lvalue = setPointCounter;
    pValue->dvalue = setPointCounter;
  }
  if ( strcmp( pInter->in.Mnemonic, "BOTTOM_COUNTER" ) == 0 )
  {
    pValue->lvalue = setPointCounter;
    pValue->dvalue = setPointCounter;
  }
  else if ( strcmp( pInter->in.Mnemonic, "SCENARIO_ID" ) == 0 )
  {
    pScenarioDataInput->coils[0] = pSpt->stageSimulationResults.coils[0].data;
    pScenarioDataInput->coils[1] = pSpt->stageSimulationResults.coils[1].data;

    if ( lScenarioIDCurrent > 0 )
    {
      if ( pSpt->stageSimulationResults.lScenarioID == lScenarioIDCurrent )
      {
        pValue->lvalue = lScenarioIDCurrent;
        pValue->dvalue = lScenarioIDCurrent;
        bIsScenarioCalculationAUthorized = true;
        nuStageScenario     = 0;
        nuSectionScenario   = 0;
        nuZoneScenario      = 0;
        nuCoilScenario      = 0;
      }
    }
  }
  else if ( strcmp( pInter->in.Mnemonic, "SCN_Thickness" ) == 0 )
  {
    pValue->dvalue = pScenarioDataInput->coils[nuCoilScenario].thickness;
  }
  else if ( strcmp( pInter->in.Mnemonic, "SCN_Width" ) == 0 )
  {
    pValue->dvalue = pScenarioDataInput->coils[nuCoilScenario].width;
  }
  else if ( strcmp( pInter->in.Mnemonic, "SCN_Length" ) == 0 )
  {
    pValue->dvalue = pScenarioDataInput->coils[nuCoilScenario].length;
  }
  else if ( strcmp( pInter->in.Mnemonic, "SCN_Emissivity" ) == 0 )
  {
    pValue->dvalue = pScenarioDataInput->coils[nuCoilScenario].emissivity_input;
  }
  else if ( strcmp( pInter->in.Mnemonic, "SCN_MaxSpeedCoil" ) == 0 )
  {
    pValue->dvalue = pScenarioDataInput->coils[nuCoilScenario].maxSpeedQuality;
  }
  else if ( strcmp( pInter->in.Mnemonic, "SCN_HeatCycle" ) == 0 )
  {
    pValue->dvalue = pScenarioDataInput->coils[nuCoilScenario].heatCycle;
    pValue->lvalue = pScenarioDataInput->coils[nuCoilScenario].heatCycle;
    nuCoilScenario ++;
  }
  else if ( strcmp( pInter->in.Mnemonic, "SCN_Control" ) == 0 )
  {
    pValue->lvalue = pScenarioDataInput->line.speedControl;
  }
  else if ( strcmp( pInter->in.Mnemonic, "SCN_Speed" ) == 0 )
  {
    pValue->dvalue = pScenarioDataInput->line.speed;
  }
  else if ( strcmp( pInter->in.Mnemonic, "SCN_SpeedLimit" ) == 0 )
  {
    pValue->dvalue = pScenarioDataInput->line.operatorSpeedLimit;
  }
  else if ( strcmp( pInter->in.Mnemonic, "IS_VALID_SECTION_SCN" ) == 0 )
  {
    if ( nuSectionScenario < pMOD->pConf->pconf->lines[0].nb_sections )
    {
      pValue->lvalue = 1;
      pValue->dvalue = 1;
    }
    else
    {
      pValue->lvalue = 0;
      pValue->dvalue = 0;
    }

  }
  else if ( strcmp( pInter->in.Mnemonic, "SPEED_SCN" ) == 0 )
  {
    if ( nuSectionScenario  < pMOD->pConf->pconf->lines[0].nb_sections )
    {
      if      ( nuStageScenario == 0 )
        pValue->dvalue = pSpt->stageSimulationResults.coils[0].calcul.stages.maxSpeed .lines[0].sections[nuSectionScenario].speed;
      else if ( nuStageScenario == 1 )
        pValue->dvalue = pSpt->stageSimulationResults.coils[0].calcul.stages.steady   .lines[0].sections[nuSectionScenario].speed;
      else
        pValue->dvalue = pSpt->stageSimulationResults.coils[0].calcul.stages.transient.lines[0].sections[nuSectionScenario].speed;
    }
    else
      pValue->dvalue = 0.;
  }
  else if ( strcmp( pInter->in.Mnemonic, "STRIP01_SPEED_AFTER_JUMP_SCN" ) == 0 )
  {
    if ( nuSectionScenario  < pMOD->pConf->pconf->lines[0].nb_sections )
    {
      if      ( nuStageScenario == 0 )
        pValue->dvalue = pSpt->stageSimulationResults.coils[1].calcul.stages.maxSpeed .lines[0].sections[nuSectionScenario].speed;
      else if ( nuStageScenario == 1 )
        pValue->dvalue = pSpt->stageSimulationResults.coils[1].calcul.stages.steady   .lines[0].sections[nuSectionScenario].speed;
      else
        pValue->dvalue = pSpt->stageSimulationResults.coils[0].calcul.stages.transient.lines[0].sections[nuSectionScenario].speedAfterJump;
    }
    else
      pValue->dvalue = 0.;
  }
  else if ( strcmp( pInter->in.Mnemonic, "STRIP01_TEMPERATURE_AFTER_JUMP" ) == 0 )
  {
    if ( nuSectionScenario  < pMOD->pConf->pconf->lines[0].nb_sections )
    {
      if      ( nuStageScenario == 0 )
        pValue->dvalue = 0.;
      else if ( nuStageScenario == 1 )
        pValue->dvalue = 0.;
      else
        pValue->dvalue = pSpt->stageSimulationResults.coils[0].calcul.stages.transient.lines[0].sections[nuSectionScenario].secondStripTemp;
    }
    else
      pValue->dvalue = 0.;
  }
  else if ( strcmp( pInter->in.Mnemonic, "STRIP01_TARGET_TEMPERATURE" ) == 0 )
  {
    if ( nuSectionScenario  < pMOD->pConf->pconf->lines[0].nb_sections )
    {
      if ( pMOD->pConf->pconf->lines[0].pSections[nuSectionScenario]->type == TYPE_NOF )
      {
        long nuPyro = pMOD->pConf->pconf->lines[0].pSections[nuSectionScenario]->nb_pyrometers - 1;
        if      ( nuStageScenario == 0 )
          pValue->dvalue = pSpt->stageSimulationResults.coils[0].calcul.stages.maxSpeed .lines[0].sections[nuSectionScenario].pyrometerTarget[nuPyro];
        else if ( nuStageScenario == 1 )
          pValue->dvalue = pSpt->stageSimulationResults.coils[0].calcul.stages.steady   .lines[0].sections[nuSectionScenario].pyrometerTarget[nuPyro];
        else
          pValue->dvalue = pSpt->stageSimulationResults.coils[0].calcul.stages.transient.lines[0].sections[nuSectionScenario].pyrometerTarget[nuPyro];
      }
      else
      {
        if      ( nuStageScenario == 0 )
          pValue->dvalue = pSpt->stageSimulationResults.coils[0].calcul.stages.maxSpeed .lines[0].sections[nuSectionScenario].stripTarget;
        else if ( nuStageScenario == 1 )
          pValue->dvalue = pSpt->stageSimulationResults.coils[0].calcul.stages.steady   .lines[0].sections[nuSectionScenario].stripTarget;
        else
          pValue->dvalue = pSpt->stageSimulationResults.coils[0].calcul.stages.transient.lines[0].sections[nuSectionScenario].stripTarget;

      }
    }
    else
      pValue->dvalue = 0.;
  }
  else if ( strcmp( pInter->in.Mnemonic, "STRIP02_TARGET_TEMPERATURE" ) == 0 )
  {
    if ( nuSectionScenario  < pMOD->pConf->pconf->lines[0].nb_sections )
    {
      if ( pMOD->pConf->pconf->lines[0].pSections[nuSectionScenario]->type == TYPE_NOF )
      {
        long nuPyro = pMOD->pConf->pconf->lines[0].pSections[nuSectionScenario]->nb_pyrometers - 1;
        if      ( nuStageScenario == 0 )
          pValue->dvalue = pSpt->stageSimulationResults.coils[1].calcul.stages.maxSpeed .lines[0].sections[nuSectionScenario].pyrometerTarget[nuPyro];
        else if ( nuStageScenario == 1 )
          pValue->dvalue = pSpt->stageSimulationResults.coils[1].calcul.stages.steady   .lines[0].sections[nuSectionScenario].pyrometerTarget[nuPyro];
        else
          pValue->dvalue = 0.;
      }
      else
      {
        if      ( nuStageScenario == 0 )
          pValue->dvalue = pSpt->stageSimulationResults.coils[1].calcul.stages.maxSpeed .lines[0].sections[nuSectionScenario].stripTarget;
        else if ( nuStageScenario == 1 )
          pValue->dvalue = pSpt->stageSimulationResults.coils[1].calcul.stages.steady   .lines[0].sections[nuSectionScenario].stripTarget;
        else
          pValue->dvalue = 0.;

      }
    }
    else
      pValue->dvalue = 0.;
  }
  else if ( strcmp( pInter->in.Mnemonic, "REFERENCE_MODEL_INPUT" ) == 0 )
  {
    if ( nuSectionScenario  < pMOD->pConf->pconf->lines[0].nb_sections )
    {
      if      ( nuStageScenario == 0 )
        pValue->dvalue = pSpt->stageSimulationResults.coils[0].calcul.stages.maxSpeed .lines[0].sections[nuSectionScenario].refparameterModelInput;
      else if ( nuStageScenario == 1 )
        pValue->dvalue = pSpt->stageSimulationResults.coils[0].calcul.stages.steady   .lines[0].sections[nuSectionScenario].refparameterModelInput;
      else
        pValue->dvalue = pSpt->stageSimulationResults.coils[0].calcul.stages.transient.lines[0].sections[nuSectionScenario].refparameterModelInput;
    }
    else
      pValue->dvalue = 0.;
  }
  else if ( strcmp( pInter->in.Mnemonic, "IS_VALID_ZONE_SCN" ) == 0 )
  {
    if ( nuSectionScenario  < pMOD->pConf->pconf->lines[0].nb_sections &&
         nuZoneScenario     < pMOD->pConf->pconf->lines[0].pSections[nuSectionScenario]->nb_zones )
    {
      pValue->lvalue = 1;
      pValue->dvalue = 1;
    }
    else
    {
      pValue->lvalue = 0;
      pValue->dvalue = 0;
    }
  }
  else if ( strcmp( pInter->in.Mnemonic, "STRIP01_MODEL_INPUT" ) == 0 )
  {
    if ( nuSectionScenario  < pMOD->pConf->pconf->lines[0].nb_sections &&
         nuZoneScenario     < pMOD->pConf->pconf->lines[0].pSections[nuSectionScenario]->nb_zones )
    {
      if      ( nuStageScenario == 0 )
        pValue->dvalue = pSpt->stageSimulationResults.coils[0].calcul.stages.maxSpeed .lines[0].sections[nuSectionScenario].zonesModelInput[nuZoneScenario];
      else if ( nuStageScenario == 1 )
        pValue->dvalue = pSpt->stageSimulationResults.coils[0].calcul.stages.steady   .lines[0].sections[nuSectionScenario].zonesModelInput[nuZoneScenario];
      else
        pValue->dvalue = pSpt->stageSimulationResults.coils[0].calcul.stages.transient.lines[0].sections[nuSectionScenario].zonesModelInput[nuZoneScenario];
    
      if ( pMOD->pConf->pconf->lines[0].pSections[nuSectionScenario]->type == TYPE_NOF || 
           pMOD->pConf->pconf->lines[0].pSections[nuSectionScenario]->type == TYPE_RTF )
        pValue->dvalue = pMOD->pConv->celcius( pValue->dvalue );
    }
    else
      pValue->dvalue = 0.;
  }
  else if ( strcmp( pInter->in.Mnemonic, "STRIP02_MODEL_INPUT" ) == 0 )
  {
    if ( nuSectionScenario  < pMOD->pConf->pconf->lines[0].nb_sections &&
         nuZoneScenario     < pMOD->pConf->pconf->lines[0].pSections[nuSectionScenario]->nb_zones )
    {
      if      ( nuStageScenario == 0 )
        pValue->dvalue = pSpt->stageSimulationResults.coils[1].calcul.stages.maxSpeed .lines[0].sections[nuSectionScenario].zonesModelInput[nuZoneScenario];
      else if ( nuStageScenario == 1 )
        pValue->dvalue = pSpt->stageSimulationResults.coils[1].calcul.stages.steady   .lines[0].sections[nuSectionScenario].zonesModelInput[nuZoneScenario];
      else
        pValue->dvalue = 0.;
      
      if ( pMOD->pConf->pconf->lines[0].pSections[nuSectionScenario]->type == TYPE_NOF || 
           pMOD->pConf->pconf->lines[0].pSections[nuSectionScenario]->type == TYPE_RTF )
        pValue->dvalue = pMOD->pConv->celcius( pValue->dvalue );
    }
    else
      pValue->dvalue = 0.;
  }
  else if ( strcmp( pInter->in.Mnemonic, "STRIP01_DEMAND" ) == 0 )
  {
    if ( nuSectionScenario  < pMOD->pConf->pconf->lines[0].nb_sections &&
         nuZoneScenario     < pMOD->pConf->pconf->lines[0].pSections[nuSectionScenario]->nb_zones )
    {
      if      ( nuStageScenario == 0 )
        pValue->dvalue = pSpt->stageSimulationResults.coils[0].calcul.stages.maxSpeed .lines[0].sections[nuSectionScenario].powerDemand[nuZoneScenario];
      else if ( nuStageScenario == 1 )
        pValue->dvalue = pSpt->stageSimulationResults.coils[0].calcul.stages.steady   .lines[0].sections[nuSectionScenario].powerDemand[nuZoneScenario];
      else
        pValue->dvalue = pSpt->stageSimulationResults.coils[0].calcul.stages.transient.lines[0].sections[nuSectionScenario].powerDemand[nuZoneScenario];
    }
    else
      pValue->dvalue = 0.;
  }
  else if ( strcmp( pInter->in.Mnemonic, "STRIP02_DEMAND" ) == 0 )
  {
    if ( nuSectionScenario  < pMOD->pConf->pconf->lines[0].nb_sections &&
         nuZoneScenario     < pMOD->pConf->pconf->lines[0].pSections[nuSectionScenario]->nb_zones )
    {
      if      ( nuStageScenario == 0 )
        pValue->dvalue = pSpt->stageSimulationResults.coils[1].calcul.stages.maxSpeed .lines[0].sections[nuSectionScenario].powerDemand[nuZoneScenario];
      else if ( nuStageScenario == 1 )
        pValue->dvalue = pSpt->stageSimulationResults.coils[1].calcul.stages.steady   .lines[0].sections[nuSectionScenario].powerDemand[nuZoneScenario];
      else
        pValue->dvalue = 0.;
    }
    else
      pValue->dvalue = 0.;

    // Make the evolution of index.
    if ( ++nuZoneScenario >= 30 )
    {
      nuZoneScenario = 0;
      if ( ++nuSectionScenario >= 7 )
      {
        nuSectionScenario = 0;
        nuStageScenario ++;
      }
    }
  }
  else if ( strcmp( pInter->in.Mnemonic, "SpecificConsumptionALL" ) == 0 )
  {
    pValue->dvalue = ( this->specificConsumption[0] + this->specificConsumption[1] ) / pMOD->pPhysic->fuelLHV(0) * 1000.;
  }
  else if ( strcmp( pInter->in.Mnemonic, "EfficiencyALL" ) == 0 )
  {
    if ( ( this->specificConsumption[0] + this->specificConsumption[1] ) > 0.1 )
      pValue->dvalue = (  this->efficiency[0] * this->specificConsumption[0] + 
                          this->efficiency[1] * this->specificConsumption[1] ) / 
                         ( this->specificConsumption[0] + this->specificConsumption[1] );
    else
      pValue->dvalue = 0.;
  }
  else if ( strcmp( pInter->in.Mnemonic, "PyrometerHB" ) == 0 )
  {
    gblSec.lockAccess();
    pValue->dvalue = gblSec.pSpe->roofTempHBSetpoint;
    gblSec.unLockAccess();
  }

  return;		
}

//============================================================================ 
// SPE_specific: storeValueInMeasurementStructure
//
//   Extract the value in the right place from the setpoint structure simulation
//   using the interface structure.
//============================================================================
void SPE_specific::storeValueInMeasurementStructure  
                                ( struct UTL_message_element_values     *    pValue   // Entry: Element Value.
                                , struct EQP_interface_message_elements *    pInter   // Entry: Element interface.
                                , struct MOD_HV_setpoints_simulation    *    pSpt     // Out  : pointer to setpoint structure.
                                )
{

  // Example:
  if ( strcmp( pInter->in.Mnemonic, "nouvelleConsigne" ) == 0 )
  {
    //// Au choix selon le type de variable que vous avez choisit:
    //pValue->lvalue = pSpt->specific.maNouvelleConsigne1;
    //pValue->dvalue = pSpt->specific.maNouvelleConsigne2;
    //strncpy( pValue->stringValue, pSpt->specific.maNouvelleConsigne3, sizeof(pSpt->specific.maNouvelleConsigne3) );
  }

  
  else if ( strcmp( pInter->in.Mnemonic, "PYROMETER_DFF" ) == 0 )
  {
    pValue->dvalue = pSpt->pyrometers[0].temperature;       // DFF 
  }
  else if ( strcmp( pInter->in.Mnemonic, "PYRO_SELECTION_DFF" ) == 0 )
  {
    pValue->dvalue = 0.;       
  }

  return;		
}

//============================================================================
// SPE_specific: storeValueInMeasurementStructure
//
//   Store the value in the right place in the measurement structure
//   using the interface structure.
//============================================================================
void SPE_specific::storeValueInMeasurementStructure
                                ( struct UTL_message_element_values     *      pValue   // Entry: Element Value.
                                , struct EQP_interface_message_elements *      pInter   // Entry: Element interface.
                                , struct MOD_HV_measures                *      pMea     // Out  : pointer to measurement structure.
                                )
{
  // Example: 
  if      ( strcmp( pInter->in.Mnemonic, "nouvelleMesure" ) == 0 )
  {
    //// Au choix selon le type de variable que vous avez choisit:
    //pMea->specific.maNouvelleMesure1    = (long) ( pValue->lvalue );
    //pMea->specific.maNouvelleMesure2    = pValue->dvalue;
    //strcpy( pMea->specific.maNouvelleMesure3, pValue->stringValue);
  }

  // Coils
  else if ( strcmp( pInter->in.Mnemonic, "COMBINATIONIDENTIFIER" ) == 0 )
  {
    memset( pMea->coils[pInter->in.secondIndex].specific.combinaisonIdentifier, 0, DIM_NAMES );
    strcpy( pMea->coils[pInter->in.secondIndex].specific.combinaisonIdentifier, pValue->stringValue );
  }
  else if ( strcmp( pInter->in.Mnemonic, "HEAT_ID" ) == 0 )
  {
    memset( pMea->coils[pInter->in.secondIndex].specific.heatID, 0, DIM_NAMES );
    strcpy( pMea->coils[pInter->in.secondIndex].specific.heatID, pValue->stringValue );
  }
  else if ( strcmp( pInter->in.Mnemonic, "PART_NUMBER" ) == 0 )
  {
    pMea->coils[pInter->in.secondIndex].specific.partNumber = (long) ( pValue->lvalue );
  }
  else if ( strcmp( pInter->in.Mnemonic, "COIL_COUNTER" ) == 0 )
  {
    pMea->coils[pInter->in.secondIndex].specific.coilCounter = (long) ( pValue->lvalue );
  }
  else if ( strcmp( pInter->in.Mnemonic, "WEIGHT" ) == 0 )
  {
    pMea->coils[pInter->in.secondIndex].specific.weight = ( pValue->dvalue );
  }
  else if ( strcmp( pInter->in.Mnemonic, "YIELD" ) == 0 )
  {
    pMea->coils[pInter->in.secondIndex].specific.yield = ( pValue->dvalue );
  }
  else if ( strcmp( pInter->in.Mnemonic, "TENSION_COIL" ) == 0 )
  {
    pMea->coils[pInter->in.secondIndex].specific.tension = ( pValue->dvalue );
  }

  // Sections
  else if ( strcmp( pInter->in.Mnemonic, "CONSUMPTION_ELECTRICAL" ) == 0 )
  {
    // If RTF section index.
    if ( pInter->out.sectionType == TYPE_RTF ||
         pInter->out.sectionType == TYPE_SOA ||
         pInter->out.sectionType == TYPE_SOX 
        )
      pMea->sectionsRTF [pInter->out.nu_section].specific.CONSUMPTION_ELECTRICAL    = pValue->dvalue;
    else if ( pInter->out.sectionType == TYPE_NOF )
      pMea->sectionsNOF [pInter->out.nu_section].specific.CONSUMPTION_ELECTRICAL    = pValue->dvalue;
    else if ( pInter->out.sectionType == TYPE_JET )
      pMea->sectionsJET [pInter->out.nu_section].specific.CONSUMPTION_ELECTRICAL    = pValue->dvalue;
    else if ( pInter->out.sectionType == TYPE_LYRE )
      pMea->sectionsLYR [pInter->out.nu_section].specific.CONSUMPTION_ELECTRICAL    = pValue->dvalue;
    else 
      pMea->sectionsBASE[pInter->out.nu_section].specific.CONSUMPTION_ELECTRICAL    = pValue->dvalue;

  }
  else if ( strcmp( pInter->in.Mnemonic, "CONSUMPTION_FUEL" ) == 0 )
  {
    // If RTF section index.
    if ( pInter->out.sectionType == TYPE_RTF ||
         pInter->out.sectionType == TYPE_SOA ||
         pInter->out.sectionType == TYPE_SOX 
        )
      pMea->sectionsRTF [pInter->out.nu_section].specific.CONSUMPTION_FUEL    = pValue->dvalue;
    else if ( pInter->out.sectionType == TYPE_NOF )
      pMea->sectionsNOF [pInter->out.nu_section].specific.CONSUMPTION_FUEL    = pValue->dvalue;
    else if ( pInter->out.sectionType == TYPE_JET )
      pMea->sectionsJET [pInter->out.nu_section].specific.CONSUMPTION_FUEL    = pValue->dvalue;
    else if ( pInter->out.sectionType == TYPE_LYRE )
      pMea->sectionsLYR [pInter->out.nu_section].specific.CONSUMPTION_FUEL    = pValue->dvalue;
    else 
      pMea->sectionsBASE[pInter->out.nu_section].specific.CONSUMPTION_FUEL    = pValue->dvalue;
  }
  else if ( strcmp( pInter->in.Mnemonic, "CONSUMPTION_NITROGEN" ) == 0 )
  {
    // If RTF section index.
    if ( pInter->out.sectionType == TYPE_RTF ||
         pInter->out.sectionType == TYPE_SOA ||
         pInter->out.sectionType == TYPE_SOX 
        )
      pMea->sectionsRTF [pInter->out.nu_section].specific.CONSUMPTION_NITROGEN    = pValue->dvalue;
    else if ( pInter->out.sectionType == TYPE_NOF )
      pMea->sectionsNOF [pInter->out.nu_section].specific.CONSUMPTION_NITROGEN    = pValue->dvalue;
    else if ( pInter->out.sectionType == TYPE_JET )
      pMea->sectionsJET [pInter->out.nu_section].specific.CONSUMPTION_NITROGEN    = pValue->dvalue;
    else if ( pInter->out.sectionType == TYPE_LYRE )
      pMea->sectionsLYR [pInter->out.nu_section].specific.CONSUMPTION_NITROGEN    = pValue->dvalue;
    else 
      pMea->sectionsBASE[pInter->out.nu_section].specific.CONSUMPTION_NITROGEN    = pValue->dvalue;
  }
  else if ( strcmp( pInter->in.Mnemonic, "CONSUMPTION_HYDROGEN" ) == 0 )
  {
    // If RTF section index.
    if ( pInter->out.sectionType == TYPE_RTF ||
         pInter->out.sectionType == TYPE_SOA ||
         pInter->out.sectionType == TYPE_SOX 
        )
      pMea->sectionsRTF [pInter->out.nu_section].specific.CONSUMPTION_HYDROGEN    = pValue->dvalue;
    else if ( pInter->out.sectionType == TYPE_NOF )
      pMea->sectionsNOF [pInter->out.nu_section].specific.CONSUMPTION_HYDROGEN    = pValue->dvalue;
    else if ( pInter->out.sectionType == TYPE_JET )
      pMea->sectionsJET [pInter->out.nu_section].specific.CONSUMPTION_HYDROGEN    = pValue->dvalue;
    else if ( pInter->out.sectionType == TYPE_LYRE )
      pMea->sectionsLYR [pInter->out.nu_section].specific.CONSUMPTION_HYDROGEN    = pValue->dvalue;
    else 
      pMea->sectionsBASE[pInter->out.nu_section].specific.CONSUMPTION_HYDROGEN    = pValue->dvalue;
  }
  else if ( strcmp( pInter->in.Mnemonic, "AIR_TEMPERATURE_ALL" ) == 0 )
  {
    // If RTF section index.
    if ( pInter->out.sectionType == TYPE_RTF ||
         pInter->out.sectionType == TYPE_SOA ||
         pInter->out.sectionType == TYPE_SOX 
        )
      pMea->sectionsRTF [pInter->out.nu_section].specific.AIR_TEMPERATURE_ALL     = pValue->dvalue;
    else if ( pInter->out.sectionType == TYPE_NOF )
      pMea->sectionsNOF [pInter->out.nu_section].specific.AIR_TEMPERATURE_ALL     = pValue->dvalue;
    else if ( pInter->out.sectionType == TYPE_JET )
      pMea->sectionsJET [pInter->out.nu_section].specific.AIR_TEMPERATURE_ALL     = pValue->dvalue;
    else if ( pInter->out.sectionType == TYPE_LYRE )
      pMea->sectionsLYR [pInter->out.nu_section].specific.AIR_TEMPERATURE_ALL     = pValue->dvalue;
    else 
      pMea->sectionsBASE[pInter->out.nu_section].specific.AIR_TEMPERATURE_ALL     = pValue->dvalue;
  }

  // Zones
  else if ( strcmp( pInter->in.Mnemonic, "COIL_L2_IDENTIFIER" ) == 0 )
  {
    pMea->specific.zones[pInter->out.nu_direct].COIL_L2_IDENTIFIER    = pValue->lvalue;
  }
  else if ( strcmp( pInter->in.Mnemonic, "COIL_PART_NUMBER" ) == 0 )
  {
    pMea->specific.zones[pInter->out.nu_direct].COIL_PART_NUMBER    = pValue->lvalue;
  }
  else if ( strcmp( pInter->in.Mnemonic, "STRIP_SAMPLE_POSITION" ) == 0 )
  {
    pMea->specific.zones[pInter->out.nu_direct].STRIP_SAMPLE_POSITION    = pValue->dvalue;
  }
  else if ( strcmp( pInter->in.Mnemonic, "ANALYSIS_CO" ) == 0 )
  {
    pMea->specific.zones[pInter->out.nu_direct].ANALYSIS_CO    = pValue->dvalue;
  }
  else if ( strcmp( pInter->in.Mnemonic, "ANALYSIS_CO2" ) == 0 )
  {
    pMea->specific.zones[pInter->out.nu_direct].ANALYSIS_CO2    = pValue->dvalue;
  }
  else if ( strcmp( pInter->in.Mnemonic, "ANALYSIS_H2" ) == 0 )
  {
    pMea->specific.zones[pInter->out.nu_direct].ANALYSIS_H2    = pValue->dvalue;
  }
  else if ( strcmp( pInter->in.Mnemonic, "ANALYSIS_O2" ) == 0 )
  {
    pMea->specific.zones[pInter->out.nu_direct].ANALYSIS_O2    = pValue->dvalue;
  }
  else if ( strcmp( pInter->in.Mnemonic, "ANALYSIS_DP" ) == 0 )
  {
    pMea->specific.zones[pInter->out.nu_direct].ANALYSIS_DP    = pValue->dvalue;
  }
  else if ( strcmp( pInter->in.Mnemonic, "MAX_FLOW" ) == 0 )
  {
    pMea->specific.zones[pInter->out.nu_direct].MAX_FLOW    = pValue->dvalue;
  }
  else if ( strcmp( pInter->in.Mnemonic, "MIN_FLOW" ) == 0 )
  {
    pMea->specific.zones[pInter->out.nu_direct].MIN_FLOW    = pValue->dvalue;
  }
  else if ( strcmp( pInter->in.Mnemonic, "MAX_HD" ) == 0 )
  {
    pMea->specific.zones[pInter->out.nu_direct].MAX_HD    = pValue->dvalue;
  }
  else if ( strcmp( pInter->in.Mnemonic, "MIN_HD" ) == 0 )
  {
    pMea->specific.zones[pInter->out.nu_direct].MIN_HD    = pValue->dvalue;
  }
  else if ( strcmp( pInter->in.Mnemonic, "READY" ) == 0 )
  {
    pMea->specific.zones[pInter->out.nu_direct].READY    = pValue->lvalue;
  }
  else if ( strcmp( pInter->in.Mnemonic, "READYBIS" ) == 0 )
  {
     //this->bits[pInter->in.firstIndex] = pValue->lvalue;
  }
  else if ( strcmp( pInter->in.Mnemonic, "TimeSpan" ) == 0 )
  {
    //timeStamp = pValue->lvalue;
  }
  else if ( strcmp( pInter->in.Mnemonic, "PZH_TEMP" ) == 0 )
  {
    pMea->specific.zones[pInter->out.nu_direct].PZH_TEMP    = pValue->lvalue;
  }
  else if ( strcmp( pInter->in.Mnemonic, "SPLIT_RANGE" ) == 0 )
  {
    pMea->specific.zones[pInter->out.nu_direct].SPLIT_RANGE  = pValue->dvalue;
  }	
  else if ( strcmp( pInter->in.Mnemonic, "OFFSET_STRIP_TT" ) == 0 )
  {
    gblSec.lockAccess();
    gblSec.pSpe->TTStripTempOffset  = pValue->dvalue;
    gblSec.unLockAccess();
  }	
  else if ( strcmp( pInter->in.Mnemonic, "OFFSET_STRIP_HH" ) == 0 )
  {
    gblSec.lockAccess();
    gblSec.pSpe->HHStripTempOffset  = pValue->dvalue;
    gblSec.unLockAccess();
  }	
  else if ( strcmp( pInter->in.Mnemonic, "OFFSET_STRIP_FF" ) == 0 )
  {
    gblSec.lockAccess();
    gblSec.pSpe->FFStripTempOffset  = pValue->dvalue;
    gblSec.unLockAccess();
  }	
  else if ( strcmp( pInter->in.Mnemonic, "OFFSET_STRIP_HB" ) == 0 )
  {
    gblSec.lockAccess();
    gblSec.pSpe->HBStripTempOffset  = pValue->dvalue;
    gblSec.unLockAccess();
  }	
  else if ( strcmp( pInter->in.Mnemonic, "OFFSET_STRIP_CC" ) == 0 )
  {
    gblSec.lockAccess();
    gblSec.pSpe->CCStripTempOffset  = pValue->dvalue;
    gblSec.unLockAccess();
  }	
  else if ( strcmp( pInter->in.Mnemonic, "OFFSET_STRIP_DD" ) == 0 )
  {
    gblSec.lockAccess();
    gblSec.pSpe->DDStripTempOffset  = pValue->dvalue;
    gblSec.unLockAccess();
  }	
  else if ( strcmp( pInter->in.Mnemonic, "TOP_COUNTER_WELD" ) == 0 )
  {
    gblSec.lockAccess();
    gblSec.pSpe->topCounterWeldEvent  = pValue->lvalue;
    gblSec.unLockAccess();
  }
  else if ( strcmp( pInter->in.Mnemonic, "SCENARIO_ID" ) == 0 )
  {
    if ( lScenarioIDCurrent < 0 )
    {
      lScenarioIDCurrent = pValue->lvalue;
      time( &recieveTimeScenarioID );
    }
    else 
    {
      time_t currenTime = 0;
      time( &currenTime );
      if ( difftime( currenTime, recieveTimeScenarioID ) > 120 )
      {
        bIsScenarioCalculationAUthorized = false;
        time( &recieveTimeScenarioID );
        lScenarioIDCurrent = pValue->lvalue;
      }
    }
  }

  // Heat cycle table.
  else if ( strcmp( pInter->in.Mnemonic, "TOTAL_NUMBER_HC" ) == 0 )
  {
    lNbHeatCycleToBeStored = pValue->lvalue;
    if ( lNbHeatCycleStored <= 0 ) lNbHeatCycleStored = 0;
    nuSectionHeatCycle = 0;

    if ( lNbHeatCycleStored <= 0 )
      time( &lastRecievedFirstHCMessage );
  }
  else if ( strcmp( pInter->in.Mnemonic, "INDEX_HC" ) == 0 )
  {
    lIndexHeatCycle = pValue->lvalue;
  }
  else if ( strcmp( pInter->in.Mnemonic, "DATABASE_KEY" ) == 0 )
  {
    pHeatCycles[lNbHeatCycleStored]->cycleNumber = pValue->lvalue;
  }
  else if ( strcmp( pInter->in.Mnemonic, "USER_CODE" ) == 0 )
  {
    long test  = pValue->lvalue;
  }
  else if ( strcmp( pInter->in.Mnemonic, "USER_NAME" ) == 0 )
  {
    strncpy_s( pHeatCycles[lNbHeatCycleStored]->name, pValue->stringValue, strlen( pValue->stringValue ) );
  }
  else if ( strcmp( pInter->in.Mnemonic, "MAX_SPEED" ) == 0 )
  {
    pHeatCycles[lNbHeatCycleStored]->speed = pValue->dvalue;
  }
  else if ( strcmp( pInter->in.Mnemonic, "IS_DEFAULT" ) == 0 )
  {
    pHeatCycles[lNbHeatCycleStored]->bIsDefault = (bool) pValue->lvalue;
  }
  else if ( strcmp( pInter->in.Mnemonic, "STRIP_UPPER" ) == 0 )
  {
    pHeatCycles[lNbHeatCycleStored]->sections[nuSectionHeatCycle].stripUpper  = pValue->dvalue;
  }
  else if ( strcmp( pInter->in.Mnemonic, "STRIP_TARGET" ) == 0 )
  {
    pHeatCycles[lNbHeatCycleStored]->sections[nuSectionHeatCycle].stripTarget = pValue->dvalue;
  }
  else if ( strcmp( pInter->in.Mnemonic, "STRIP_LOWER" ) == 0 )
  {
    pHeatCycles[lNbHeatCycleStored]->sections[nuSectionHeatCycle].stripLower  = pValue->dvalue;
  }
  else if ( strcmp( pInter->in.Mnemonic, "MINIMUM_TIME" ) == 0 )
  {
    pHeatCycles[lNbHeatCycleStored]->sections[nuSectionHeatCycle].minimumTime  = pValue->dvalue;
  }
  else if ( strcmp( pInter->in.Mnemonic, "COOLING_RATE_01" ) == 0 )
  {
    pHeatCycles[lNbHeatCycleStored]->sections[nuSectionHeatCycle].zones[0].coolingRate = pValue->dvalue;
  }
  else if ( strcmp( pInter->in.Mnemonic, "COOLING_RATE_02" ) == 0 )
  {
    pHeatCycles[lNbHeatCycleStored]->sections[nuSectionHeatCycle].zones[1].coolingRate = pValue->dvalue;
  }
  else if ( strcmp( pInter->in.Mnemonic, "COOLING_RATE_03" ) == 0 )
  {
    pHeatCycles[lNbHeatCycleStored]->sections[nuSectionHeatCycle].zones[2].coolingRate = pValue->dvalue;
  }
  else if ( strcmp( pInter->in.Mnemonic, "COOLING_RATE_04" ) == 0 )
  {
    pHeatCycles[lNbHeatCycleStored]->sections[nuSectionHeatCycle].zones[3].coolingRate = pValue->dvalue;
    nuSectionHeatCycle ++;
    if ( nuSectionHeatCycle == 7 )
      lNbHeatCycleStored ++;
  }

  // Coil thermal cycle message.
  else if ( strcmp( pInter->in.Mnemonic, "COIL_L2_IDENTIFIER_PR" ) == 0 )
  {
    // NOT USED
    long test  = pValue->lvalue;
  }
  else if ( strcmp( pInter->in.Mnemonic, "COIL_PART_NUMBER_PR" ) == 0 )
  {

    long test  = pValue->lvalue;
  }
  else if ( strcmp( pInter->in.Mnemonic, "THERMAL_CYCLE_ID" ) == 0 )
  {
    // NOT USED
    long test  = pValue->lvalue;
  }

  // CC section : damper position.
  else if ( strcmp( pInter->in.Mnemonic, "DIFFUSER_TRIMMING_INNER" ) == 0 )
  {
    long test  = pValue->dvalue;
    // to be done!!!
  }
  else if ( strcmp( pInter->in.Mnemonic, "DIFFUSER_TRIMMING_CENTER" ) == 0 )
  {
    long test  = pValue->lvalue;
    // to be done!!!
  }
  else if ( strcmp( pInter->in.Mnemonic, "DIFFUSER_TRIMMING_OUTER" ) == 0 )
  {
    long test  = pValue->lvalue;
    // to be done!!!
  }
  else if ( strcmp( pInter->in.Mnemonic, "SPEED_SET_OFFSET" ) == 0 )
  {
    pMea->specific.SPEED_SET_OFFSET = pValue->dvalue;
  }	
  else if ( strcmp( pInter->in.Mnemonic, "TOP_COUNTER" ) == 0 )
  {
    long test = pValue->lvalue;
    test = 0;
  }
  else if ( strcmp( pInter->in.Mnemonic, "BOTTOM_COUNTER" ) == 0 )
  {
    long test = pValue->lvalue;
    test = 0;
  }

  return;
}

//============================================================================
// SPE_specific: storeValueInMeasurementStructure
//
//   Store the value in the right place in the measurement structure
//   using the interface structure.
//============================================================================
void SPE_specific::storeValueInMeasurementStructure
                                ( struct UTL_message_element_values *          pValue   // Entry: Element Value.
                                , struct EQP_interface_message_elements *      pInter   // Entry: Element interface.
                                , struct MOD_HV_measures_simulation *          pMea     // Out  : pointer to measurement structure.
                                )
{
  // Example: 
  if      ( strcmp( pInter->in.Mnemonic, "nouvelleMesure" ) == 0 )
  {
    //// Au choix selon le type de variable que vous avez choisit:
    //pMea->specific.maNouvelleMesure1    = (long) ( pValue->lvalue );
    //pMea->specific.maNouvelleMesure2    = pValue->dvalue;
    //strcpy( pMea->specific.maNouvelleMesure3, pValue->stringValue);
  }

  //else if ( strcmp( pInter->in.Mnemonic, "SpeedMax" ) == 0 )
  //{
  //   pMea->specific.SpeedMax = pValue->dvalue;
  //}
  

  else if ( strcmp( pInter->in.Mnemonic, "STRIP_TEMP_JET" ) == 0 )
  {
  }
  else if ( strcmp( pInter->in.Mnemonic, "POWER_JET_COOL" ) == 0 )
  {
    long nuSectionInJet = 0;
    for ( long iZone = 0; iZone < pMOD->pConf->pconf->sectionsJET[nuSectionInJet].nb_zones; iZone++ )
    {
      long nuZoneInType = pMOD->pConf->pconf->sectionsJET[nuSectionInJet].pZones[iZone]->indexType;
      pMea->zonesJET[nuZoneInType].demandPercentage  = pValue->dvalue;
    }
  }
  else if ( strcmp( pInter->in.Mnemonic, "STRIP_TEMP_DRC5" ) == 0 )
  {
  }
  else if ( strcmp( pInter->in.Mnemonic, "STRIP_TEMP_HOTBRIDLE" ) == 0 )
  {
  }
  else if ( strcmp( pInter->in.Mnemonic, "STRIP_TEMP_DOWNCHUTE" ) == 0 )
  {
  }
  else if ( strcmp( pInter->in.Mnemonic, "POWER_APC_UP" ) == 0 )
  {
    long nuSectionInJet = 1;
    for ( long iZone = 0; iZone < pMOD->pConf->pconf->sectionsJET[nuSectionInJet].nb_zones; iZone++ )
    {
      long nuZoneInType = pMOD->pConf->pconf->sectionsJET[nuSectionInJet].pZones[iZone]->indexType;
      pMea->zonesJET[nuZoneInType].demandPercentage  = pValue->dvalue;
    }
  }
  else if ( strcmp( pInter->in.Mnemonic, "POWER_APC_DW" ) == 0 )
  {
    long nuSectionInJet = 2;
    for ( long iZone = 0; iZone < pMOD->pConf->pconf->sectionsJET[nuSectionInJet].nb_zones; iZone++ )
    {
      long nuZoneInType = pMOD->pConf->pconf->sectionsJET[nuSectionInJet].pZones[iZone]->indexType;
      pMea->zonesJET[nuZoneInType].demandPercentage  = pValue->dvalue;
    }
  }

  return;
}

//============================================================================
// SPE_specific: messageMeasurementsTreatment
//
//      Treatment of all specific measurement.
//============================================================================
bool SPE_specific::messageMeasurementsTreatment
                                ( bool                       hasSimulationMode, 
                                  struct MOD_HV_measures *   pMessageRecieve,
                                  bool                       &hasRecievedAllmessages,
                                  bool                       &authorizedToContinue,
                                  struct GEN_utility *       pGEN )
{
  // We manually store the data.
  bool doRegularStorage   = false;
  authorizedToContinue    = false;
  //hasRecievedAllmessages  = true;

  if ( hasSimulationMode ) this->useFixedDFFEmissivity = false;

  bool forceStaticCalculation = false;
  
  //------------------------------------------------------------------------------------
  // Split range definition.
  // MAX.
  this->splitRangeDFFMax[0] = 0.;
  this->splitRangeDFFMax[1] = 0.;
  this->splitRangeDFFMax[2] = 0.;
  this->splitRangeDFFMax[3] = 0.;
  this->splitRangeDFFMax[4] = 100.;
  this->splitRangeDFFMax[5] = 85.;
  this->splitRangeDFFMax[6] = 75.;
  this->splitRangeDFFMax[7] = 40.;

  // MIN.
  this->splitRangeDFFMin[0] = 0.;
  this->splitRangeDFFMin[1] = 0.;
  this->splitRangeDFFMin[2] = 0.;
  this->splitRangeDFFMin[3] = 0.;
  this->splitRangeDFFMin[4] = 55.;
  this->splitRangeDFFMin[5] = 40.;
  this->splitRangeDFFMin[6] = 15.;
  this->splitRangeDFFMin[7] = 0.;
  
  //------------------------------------------------------------------------------------
  // Watch dog message.
  if      ( pMessageRecieve->measurementMessage == 4000 )
  {
    // Store controls.
    pGEN->pHvm->phvm->meaSptCal.measure.line.speedControl       = pMessageRecieve->line.speedControl;
    pGEN->pHvm->phvm->meaSptCal.measure.sectionsNOF [0].control = pMessageRecieve->sectionsNOF [0].control;
    pGEN->pHvm->phvm->meaSptCal.measure.sectionsRTF [0].control = pMessageRecieve->sectionsRTF [0].control;
    pGEN->pHvm->phvm->meaSptCal.measure.sectionsRTF [1].control = pMessageRecieve->sectionsRTF [1].control;
    pGEN->pHvm->phvm->meaSptCal.measure.sectionsJET [0].control = pMessageRecieve->sectionsJET [0].control;
    pGEN->pHvm->phvm->meaSptCal.measure.sectionsJET [1].control = pMessageRecieve->sectionsJET [1].control;
    pGEN->pHvm->phvm->meaSptCal.measure.sectionsBASE[0].control = pMessageRecieve->sectionsBASE[0].control;

    //pGEN->pHvm->phvm->meaSptCal.measure.sectionsNOF [0].control = true;
    //pGEN->pHvm->phvm->meaSptCal.measure.sectionsRTF [0].control = true;
    //pGEN->pHvm->phvm->meaSptCal.measure.sectionsJET [0].control = true;

    this->hasRecievedAllmessages[0] = true;
    authorizedToContinue = true;
  }
  
  //------------------------------------------------------------------------------------
  // RR+FF
  else if ( pMessageRecieve->measurementMessage == 4050 )
  {
    // For all pyrometers.
    for ( long i = 0; i < pGEN->pConf->pconf->sectionsNOF[0].nb_pyrometers; i++ )
    {
      long nuPyro = pGEN->pConf->pconf->sectionsNOF[0].pPyroElements[i]->pPyrometer->index;
      pGEN->pHvm->phvm->meaSptCal.measure.pyrometers[nuPyro] = pMessageRecieve->pyrometers[nuPyro];

      //if ( i == 1 )
      //{
      //  double temp = pGEN->pHvm->phvm->meaSptCal.measure.pyrometers[nuPyro].temperature;
      //  this->DFFTemperature = pGEN->pHvm->phvm->meaSptCal.measure.pyrometers[nuPyro].temperature;
      //  temp = pGEN->pConv->celcius( temp );
      //  if ( !hasSimulationMode ) 
      //  {
      //    if ( !this->useFixedDFFEmissivity )
      //    {
      //      long    nuCoilOrdered   = pGEN->pHvm->phvm->meaSptCal.calcul.sectionsNOF[0].tracking.nu_coilInOrdered;
      //      if ( pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoilOrdered] != nullptr )
      //      {
      //        double  thickness       = pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoilOrdered]->data.thickness;
      //        double  maxThickness    = 1.00;
      //        double  minThickness    = 0.25;
      //        double  maxFactor       = 0.89;
      //        double  minFactor       = 0.63;
      //
      //        double  temporyA        = ( maxFactor - minFactor ) / ( maxThickness - minThickness );
      //        double  factor          = temporyA * ( thickness * 1000. ) + ( maxFactor - maxThickness * temporyA );
      //        factor = min( max( factor, minFactor ), maxFactor );
      //        temp = temp * factor;
      //      }
      //      else
      //        temp = temp * 0.85 ;
      //    }
      //    else
      //    {
      //      if ( onLiveDynModelDFFpyroTemp > pGEN->pMath->epsilon )
      //      {
      //        this->factorDFFPyrometer = pGEN->pConv->celcius( onLiveDynModelDFFpyroTemp ) / temp;
      //        temp = pGEN->pConv->celcius( onLiveDynModelDFFpyroTemp );
      //      }
      //      else
      //        temp = temp * 0.85 ;
      //    }
      //  }
      //  pGEN->pHvm->phvm->meaSptCal.measure.pyrometers[nuPyro].temperature = pGEN->pConv->kelvin( temp );
      //}
    }

    // For all zones.
    for ( long i = 0; i < pGEN->pConf->pconf->nb_zonesNOF; i++ )
    {
      pGEN->pHvm->phvm->meaSptCal.measure.zonesNOF [i] =  pMessageRecieve->zonesNOF [i];

      //if ( i == 7  && false ) //|| i == 6 || i == 5 || i == 4
      //{
      //  pGEN->pHvm->phvm->meaSptCal.measure.zonesNOF [i].gasFlowRate *= 0.94;
      //  pGEN->pHvm->phvm->meaSptCal.measure.zonesNOF [i].airFlowRate *= 0.94;
      //}

      // Security for remaining low gas flow.
      if ( pGEN->pHvm->phvm->meaSptCal.measure.zonesNOF [i].gasFlowRate < 24. / 3600. && !this->hasSimulationMode )
        pGEN->pHvm->phvm->meaSptCal.measure.zonesNOF [i].gasFlowRate = 0.;

      double lhv = pGEN->pPhysic->fuelLHV( 0 );
      if ( pGEN->pConf->pconf->zonesNOF[i].Power > pGEN->pMath->epsilon )
        pGEN->pHvm->phvm->meaSptCal.measure.zonesNOF [i].demandPercentage =  pGEN->pHvm->phvm->meaSptCal.measure.zonesNOF [i].gasFlowRate * 100. 
                                                                           / pGEN->pConf->pconf->zonesNOF[i].Power * lhv;
      
      // For all thermocouple contains.
      for ( long iTh = 0; iTh < pGEN->pConf->pconf->zonesNOF[i].nb_thermocouples; iTh++ )
      {
        long nuThermo = pGEN->pConf->pconf->zonesNOF[i].pThermocouples[iTh]->index;
        pGEN->pHvm->phvm->meaSptCal.measure.thermocouples[nuThermo] = pMessageRecieve->thermocouples[nuThermo];
      }

      long nuZone = pGEN->pConf->pconf->zonesNOF[i].indexGlobal;

      double oldValue = pGEN->pHvm->phvm->meaSptCal.measure.specific.zones[nuZone].MAX_FLOW;
      double newValue = pMessageRecieve->specific.zones[nuZone].MAX_FLOW / 100. * pGEN->pConf->pconf->zonesNOF[i].Power / lhv;
      forceStaticCalculation = forceStaticCalculation || abs ( oldValue - newValue ) > 10. / 3600.;

      pGEN->pHvm->phvm->meaSptCal.measure.specific.zones[nuZone] = pMessageRecieve->specific.zones[nuZone];
      pGEN->pHvm->phvm->meaSptCal.measure.specific.zones[nuZone].READY = !pGEN->pHvm->phvm->meaSptCal.measure.specific.zones[nuZone].READY;

      // Get parameters.
      struct MOD_HV_Parameter   parameter;
      pGEN->pHvm->getParameters( &parameter, false );

      // Store one parameter coefficient.
      coefTuningFixDFFEmissivity = parameter.spareDouble2[0];

      // If power exists.
      if ( pGEN->pConf->pconf->zonesNOF[i].Power > 0. )
      {
        pGEN->pHvm->phvm->meaSptCal.measure.specific.zones[nuZone].MAX_FLOW = pGEN->pHvm->phvm->meaSptCal.measure.specific.zones[nuZone].MAX_FLOW / 100. 
                                                                            * pGEN->pConf->pconf->zonesNOF[i].Power / lhv; 

        if ( pGEN->pHvm->phvm->meaSptCal.measure.zonesNOF [5].gasFlowRate > 40. / 3600. && pGEN->pHvm->phvm->meaSptCal.measure.zonesNOF [6].gasFlowRate < 20. / 3600. && i == 6 )
          pGEN->pHvm->phvm->meaSptCal.measure.specific.zones[nuZone].MAX_FLOW = 0.;

        if ( pGEN->pHvm->phvm->meaSptCal.measure.zonesNOF [4].gasFlowRate > 40. / 3600. && pGEN->pHvm->phvm->meaSptCal.measure.zonesNOF [5].gasFlowRate < 20. / 3600. && i == 5 )
          pGEN->pHvm->phvm->meaSptCal.measure.specific.zones[nuZone].MAX_FLOW = 0.;

        if ( pGEN->pHvm->phvm->meaSptCal.measure.zonesNOF [4].gasFlowRate > 40. / 3600. && pGEN->pHvm->phvm->meaSptCal.measure.zonesNOF [6].gasFlowRate < 20. / 3600. && i == 6 )
          pGEN->pHvm->phvm->meaSptCal.measure.specific.zones[nuZone].MAX_FLOW = 0.;

        double maxGasFlow = pGEN->pHvm->phvm->meaSptCal.measure.specific.zones[nuZone].MAX_FLOW;
        if ( ( maxGasFlow <= pGEN->pMath->epsilon && i != 6 && i != 5 ) || hasSimulationMode )
          maxGasFlow = pGEN->pConf->pconf->zonesNOF[i].Power / lhv; 
        parameter.zones[nuZone].maximumPower = maxGasFlow * lhv;

        parameter.zones[nuZone].maximumHeatDemand  = maxGasFlow / ( pGEN->pConf->pconf->zonesNOF[i].Power / lhv ) * 100.;
        parameter.zones[nuZone].minimumHeatDemand  = 20.;

        // Lambda.
        if ( pGEN->pHvm->phvm->meaSptCal.measure.zonesNOF [i].gasFlowRate > 0.0001 && !hasSimulationMode )
          pGEN->pHvm->phvm->meaSptCal.measure.zonesNOF [i].lambda = pGEN->pHvm->phvm->meaSptCal.measure.zonesNOF [i].airFlowRate 
                                                                  / pGEN->pHvm->phvm->meaSptCal.measure.zonesNOF [i].gasFlowRate 
                                                                  / pGEN->pPhysic->fuelVa( (long) 0, 1. );   
        else
          pGEN->pHvm->phvm->meaSptCal.measure.zonesNOF [i].lambda = 1.; // Air gas ratio set to 1. as default. to avoid any issue on calculation.

        bool skjdfkfkdslkdnfsdfsl = true;
      }
      else
        pGEN->pHvm->phvm->meaSptCal.measure.zonesNOF [i].lambda = 1.;

      pGEN->pHvm->phvm->meaSptCal.measure.zonesNOF [i].lambda = min( max( pGEN->pHvm->phvm->meaSptCal.measure.zonesNOF [i].lambda, 0.8 ), 1.2 );

      if ( !hasSimulationMode )
        pGEN->pHvm->phvm->meaSptCal.measure.zonesNOF [i].airTemperature = pGEN->pHvm->phvm->meaSptCal.sectionsNOF [0].pMeasure->specific.AIR_TEMPERATURE_ALL;
      else 
        pGEN->pHvm->phvm->meaSptCal.measure.zonesNOF [i].airTemperature = pGEN->pConv->kelvin( 200. );
      if ( i == 7 )
        pGEN->pHvm->phvm->meaSptCal.measure.zonesNOF [i].airTemperature = pGEN->pConv->kelvin( 20. );

      

      // Store parameters.
      pGEN->pHvm->writeParameters( &parameter, false );
    }

    pGEN->pHvm->phvm->meaSptCal.measure.sectionsNOF[0].specific = pMessageRecieve->sectionsNOF[0].specific;
    
    this->hasRecievedAllmessages[1] = true;
  }
  //------------------------------------------------------------------------------------
  // TT && HH.. BE AWARE TT has 3 confugure zones but first HH zone is recieved with the messsage.
  else if ( pMessageRecieve->measurementMessage == 4051 ||
            pMessageRecieve->measurementMessage == 4052 )
  {
    long nuSectionIntype = 0;

    if      ( pMessageRecieve->measurementMessage == 4051 ) nuSectionIntype = 0; // TT
    else if ( pMessageRecieve->measurementMessage == 4052 ) nuSectionIntype = 1; // HH

    if      ( pMessageRecieve->measurementMessage == 4051 )
    {
      //// For all pyrometers.
      //for ( long i = 0; i < pGEN->pConf->pconf->sectionsRTF[nuSectionIntype].nb_pyrometers; i++ )
      //{
      //  long nuPyro = pGEN->pConf->pconf->sectionsRTF[nuSectionIntype].pPyroElements[i]->pPyrometer->index;
      //  pGEN->pHvm->phvm->meaSptCal.measure.pyrometers[nuPyro] = pMessageRecieve->pyrometers[nuPyro];
      //}
      long nuPyro = pGEN->pConf->pconf->sectionsRTF[nuSectionIntype].pPyroElements[0]->pPyrometer->index;
      pGEN->pHvm->phvm->meaSptCal.measure.pyrometers[nuPyro] = pMessageRecieve->pyrometers[nuPyro];

      // Save also next pyro.
      pGEN->pHvm->phvm->meaSptCal.measure.pyrometers[nuPyro + 1] = pMessageRecieve->pyrometers[nuPyro + 1];
    }
    else
    {
      // We only save last pyro.
      long nuPyro = pGEN->pConf->pconf->sectionsRTF[nuSectionIntype].pPyroElements[1]->pPyrometer->index;
      pGEN->pHvm->phvm->meaSptCal.measure.pyrometers[nuPyro] = pMessageRecieve->pyrometers[nuPyro];
    }

    long nuFirst = 0;
    long nuLast  = 0;
    if ( pMessageRecieve->measurementMessage == 4051 )  nuFirst = 0;
    else                                                nuFirst = 1;
    if ( pMessageRecieve->measurementMessage == 4051 )  nuLast  = 1;
    else                                                nuLast  = 0;
    
    long nuZoneGlobal = -1;
    long nuZoneInType = -1;
    // For all zones.
    for ( long i = nuFirst; i < pGEN->pConf->pconf->sectionsRTF[nuSectionIntype].nb_zones + nuLast; i++ )
    {
      if ( pMessageRecieve->measurementMessage == 4051 && i == pGEN->pConf->pconf->sectionsRTF[nuSectionIntype].nb_zones )
      {
        nuZoneGlobal += 1;
        nuZoneInType += 1;
      }
      else
      {
        nuZoneGlobal = pGEN->pConf->pconf->sectionsRTF[nuSectionIntype].pZones[i]->indexGlobal;
        nuZoneInType = pGEN->pConf->pconf->sectionsRTF[nuSectionIntype].pZones[i]->indexType;
      }

      pGEN->pHvm->phvm->meaSptCal.measure.zonesRTF [nuZoneInType] =  pMessageRecieve->zonesRTF [nuZoneInType];

      // Store parameters.
      struct MOD_HV_Parameter   parameter;
      pGEN->pHvm->getParameters( &parameter, false );

      double toto = 0.;
      double maxHD = pMessageRecieve->specific.zones[nuZoneGlobal].MAX_HD * 1.0; 
      
      if ( maxHD < pGEN->pMath->epsilon )
        maxHD = 100.;

      if ( hasSimulationMode )
        maxHD = 100.;

      parameter.zones[nuZoneGlobal].maximumPower       = maxHD / 100. * pGEN->pConf->pconf->zonesRTF[nuZoneInType].Power;
      parameter.zones[nuZoneGlobal].maximumHeatDemand  = maxHD;
      parameter.zones[nuZoneGlobal].minimumHeatDemand  = 0.;
      if ( pMessageRecieve->specific.zones[nuZoneGlobal].PZH_TEMP > pGEN->pMath->epsilon && !hasSimulationMode )
      {
        double oldValue = parameter.zones[nuZoneGlobal].maximumTemperature;
        double newValue = pGEN->pConv->kelvin( pMessageRecieve->specific.zones[nuZoneGlobal].PZH_TEMP ) - 10.;
        forceStaticCalculation = forceStaticCalculation || abs ( oldValue - newValue ) > 5.;
        parameter.zones[nuZoneGlobal].maximumTemperature = pGEN->pConv->kelvin( pMessageRecieve->specific.zones[nuZoneGlobal].PZH_TEMP ) 
                                                           - 10.; // We retrieve 10 degrees to get some margin.
      }
      else 
        parameter.zones[nuZoneGlobal].maximumTemperature = pGEN->pConv->kelvin( 930. );

      // Store parameters.
      pGEN->pHvm->writeParameters( &parameter, false );

      if ( pMessageRecieve->measurementMessage == 4051 )
      {
        double lhv = pGEN->pPhysic->fuelLHV( 0 );
        if ( pGEN->pConf->pconf->zonesRTF[nuZoneInType].Power > pGEN->pMath->epsilon )
          pGEN->pHvm->phvm->meaSptCal.measure.zonesRTF [nuZoneInType].demandPercentage = 
                    pGEN->pHvm->phvm->meaSptCal.measure.zonesRTF [nuZoneInType].gasFlowRate * 100. 
                  / pGEN->pConf->pconf->zonesRTF[nuZoneInType].Power * lhv;
      }
      
      // For all thermocouple contains.
      for ( long iTh = 0; iTh < pGEN->pConf->pconf->zonesRTF[nuZoneInType].nb_thermocouples; iTh++ )
      {
        long nuThermo = pGEN->pConf->pconf->zonesRTF[nuZoneInType].pThermocouples[iTh]->index;
        pGEN->pHvm->phvm->meaSptCal.measure.thermocouples[nuThermo] = pMessageRecieve->thermocouples[nuThermo];
      }

      pGEN->pHvm->phvm->meaSptCal.measure.specific.zones[nuZoneGlobal] = pMessageRecieve->specific.zones[nuZoneGlobal];
      pGEN->pHvm->phvm->meaSptCal.measure.specific.zones[nuZoneGlobal].READY = 
        !pGEN->pHvm->phvm->meaSptCal.measure.specific.zones[nuZoneGlobal].READY;
    }
    
    pGEN->pHvm->phvm->meaSptCal.measure.sectionsRTF[nuSectionIntype].specific = 
      pMessageRecieve->sectionsRTF[nuSectionIntype].specific;
    
    if      ( pMessageRecieve->measurementMessage == 4051 ) this->hasRecievedAllmessages[2] = true; // TT
    else if ( pMessageRecieve->measurementMessage == 4052 ) this->hasRecievedAllmessages[3] = true; // HH

  }
  //------------------------------------------------------------------------------------
  // CC + DD
  else if ( pMessageRecieve->measurementMessage == 4053 ||
            pMessageRecieve->measurementMessage == 4055 )
  {
    long nuSectionIntype = 0;

    if      ( pMessageRecieve->measurementMessage == 4053 ) nuSectionIntype = 0; // CC
    else if ( pMessageRecieve->measurementMessage == 4055 ) nuSectionIntype = 1; // DD

    // For all pyrometers.
    for ( long i = 0; i < pGEN->pConf->pconf->sectionsJET[nuSectionIntype].nb_pyrometers; i++ )
    {
      long nuPyro = pGEN->pConf->pconf->sectionsJET[nuSectionIntype].pPyroElements[i]->pPyrometer->index;
      pGEN->pHvm->phvm->meaSptCal.measure.pyrometers[nuPyro] = pMessageRecieve->pyrometers[nuPyro];
    }
    
    // Store parameters.
    struct MOD_HV_Parameter   parameter;
    pGEN->pHvm->getParameters( &parameter, false );

    // For all zones.
    for ( long i = 0; i < pGEN->pConf->pconf->sectionsJET[nuSectionIntype].nb_zones; i++ )
    {
      long nuZoneGlobal = pGEN->pConf->pconf->sectionsJET[nuSectionIntype].pZones[i]->indexGlobal;
      long nuZoneInType = pGEN->pConf->pconf->sectionsJET[nuSectionIntype].pZones[i]->indexType;

      pGEN->pHvm->phvm->meaSptCal.measure.zonesJET [nuZoneInType] =  pMessageRecieve->zonesJET [nuZoneInType];

      pGEN->pHvm->phvm->meaSptCal.measure.specific.zones[nuZoneGlobal] = pMessageRecieve->specific.zones[nuZoneGlobal];
      pGEN->pHvm->phvm->meaSptCal.measure.specific.zones[nuZoneGlobal].READY = 
        !pGEN->pHvm->phvm->meaSptCal.measure.specific.zones[nuZoneGlobal].READY;

      parameter.zones[nuZoneGlobal].maximumHeatDemand  = 100.;
      parameter.zones[nuZoneGlobal].minimumHeatDemand  = 0.;

      if      ( pMessageRecieve->measurementMessage == 4053 ) parameter.zones[nuZoneGlobal].jetBlowingTemperature = pMOD->pConv->kelvin( 85. );
      else                                                    parameter.zones[nuZoneGlobal].jetBlowingTemperature = pMOD->pConv->kelvin( 30. );
    }
    
    pGEN->pHvm->phvm->meaSptCal.measure.sectionsJET[nuSectionIntype].specific = 
      pMessageRecieve->sectionsJET[nuSectionIntype].specific;

    if      ( pMessageRecieve->measurementMessage == 4053 ) pGEN->pHvm->phvm->meaSptCal.sectionsJET[nuSectionIntype].pMeasure->h2Rate = 5.;
    else                                                    pGEN->pHvm->phvm->meaSptCal.sectionsJET[nuSectionIntype].pMeasure->h2Rate = 0.;

    // Store parameters.
    pGEN->pHvm->writeParameters( &parameter, false );

    
    if      ( pMessageRecieve->measurementMessage == 4053 ) this->hasRecievedAllmessages[4] = true; // CC
    else if ( pMessageRecieve->measurementMessage == 4055 ) this->hasRecievedAllmessages[5] = true; // DD
  }
  //------------------------------------------------------------------------------------
  // EE
  else if ( pMessageRecieve->measurementMessage == 4054 )
  {
    // For all pyrometers.
    for ( long i = 0; i < pGEN->pConf->pconf->sectionsBASE[0].nb_pyrometers; i++ )
    {
      long nuPyro = pGEN->pConf->pconf->sectionsBASE[0].pPyroElements[i]->pPyrometer->index;
      pGEN->pHvm->phvm->meaSptCal.measure.pyrometers[nuPyro] = pMessageRecieve->pyrometers[nuPyro];
    }

    
    // For all zones.
    for ( long i = 0; i < pGEN->pConf->pconf->nb_zonesBASE; i++ )
    {
      pGEN->pHvm->phvm->meaSptCal.measure.zonesBASE [i] =  pMessageRecieve->zonesBASE [i];

      long nuZone = pGEN->pConf->pconf->zonesBASE[i].indexGlobal;
      pGEN->pHvm->phvm->meaSptCal.measure.specific.zones[nuZone] = pMessageRecieve->specific.zones[nuZone];
    }
    
    pGEN->pHvm->phvm->meaSptCal.measure.sectionsBASE[0].specific = pMessageRecieve->sectionsBASE[0].specific;
    
    //this->hasRecievedAllmessages[4] = true;
  }
  //------------------------------------------------------------------------------------
  // Line speed
  else if ( pMessageRecieve->measurementMessage == 4002 )
  {
    pGEN->pHvm->phvm->meaSptCal.measure.line.speed              = pMessageRecieve->line.speed;
    pGEN->pHvm->phvm->meaSptCal.measure.line.operatorSpeedLimit = pMessageRecieve->line.operatorSpeedLimit;
  }

  hasRecievedAllmessages = true;
  for ( long i = 0; i < 6; i++ )
  {
    hasRecievedAllmessages &= this->hasRecievedAllmessages[i];
  }

  // If static calculation requiered.
  if ( forceStaticCalculation ) pGEN->pHvm->setParameterForceStaticCalculation( );

  return doRegularStorage;
}

//============================================================================
// SPE_specific: treatmentOfSpxMeaMessage
//
//   Specifc actions to be done on reception of message in Masn thread.
//============================================================================
void SPE_specific::treatmentOfSpxMeaMessage( struct MOD_HV_measures &messageRecieve )
{
  
  return;
}

//============================================================================
// SPE_specific: coilMessageRecieved
//
//    Specific Treatment of a coil message recieved.
//============================================================================
void SPE_specific::coilMessageRecieved
                                ( FILE **                   traceFilePointer  // Entry   : Trace File pointer.
                                , struct MOD_HV_measures *  pMessageRecieve   // Entry: pointer to message structure recieved.
                                , struct GEN_utility *      pGEN
                                )
{
  char traceTempo [DIM_TRACES + 1] = {0};

  // Coils message.
  if      ( pMessageRecieve->measurementMessage == 4001 )
  {
    //------------------------------------------------------------------------------------
    // Get parameters maximum speed ramp.
    double speedPercentageQualityOffset = pGEN->pHvm->getParameterSpeedPercentQuaOffset( );
    speedPercentageQualityOffset = min( max( speedPercentageQualityOffset, 0. ), 30. );
    speedPercentageQualityOffset = speedPercentageQualityOffset / 100.;

    // Regular storage.
    for ( long i_coil = 0; i_coil < DIM_COILS; i_coil++ )
    {
      memset( &pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[i_coil], 0, sizeof(struct MOD_HV_coil_data) );
      if ( pMessageRecieve->coils[i_coil].length < 1. ) pMessageRecieve->coils[i_coil].length = 3000.;
      pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[i_coil] = pMessageRecieve->coils[i_coil];
      
      pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[i_coil].maxSpeedQuality = pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[i_coil].speedLimit;
      //if ( !hasSimulationMode )
      //{
      //  if      ( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[i_coil].thickness < 0.4 / 1000.)    pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[i_coil].maxSpeedQuality *= 1.00;
      //  else if ( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[i_coil].thickness > 0.8 / 1000.&&
      //            pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[i_coil].thickness < 1.0 / 1000.)    pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[i_coil].maxSpeedQuality *= ( 1.0 + speedPercentageQualityOffset * 
      //                                                                                      ( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[i_coil].thickness * 1000. - 0.8  ) / ( 1.0 - 0.8 ) );
      //  else if ( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[i_coil].thickness >= 1.0 / 1000. )  pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[i_coil].maxSpeedQuality *= ( 1. + speedPercentageQualityOffset );
      //  
      //}
    }
    
    sprintf( traceTempo, "coilMessageRecieved: coil MSG    ID 0 = %s. ID 1 = %s. ID 2 = %s. ID 3 = %s.", 
             pMessageRecieve->coils[0].entryIdentity, 
             pMessageRecieve->coils[1].entryIdentity, 
             pMessageRecieve->coils[2].entryIdentity, 
             pMessageRecieve->coils[3].entryIdentity );
    //utl.writeTrace( traceFilePointer		  // Entry: File pointer.
    //              , traceTempo            // Entry: Trace string.
    //              );

    if ( hasSimulationMode ) return;
    
    // If the two first coil are different and there were not before the weld has been created.
    if ( strncmp( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[0].entryIdentity, pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[1].entryIdentity, strlen( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[1].entryIdentity ) ) != 0 &&
         !twoFirstIDDifferent )
    {
      // Find second coil in stack.
      for ( long i_coil = 0; i_coil < DIM_COILS; i_coil++ )
      {
        if ( strncmp( pGEN->pHvm->phvm->coilQueue.coils[i_coil].data.entryIdentity, pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[1].entryIdentity, strlen( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[1].entryIdentity ) ) == 0 )
        {
          //time( &pGEN->pHvm->phvm->coilQueue.coils[i_coil].calcul.specific.weldingTime );
          break;
        }
      }
    }

    twoFirstIDDifferent = strncmp( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[0].entryIdentity, pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[1].entryIdentity, strlen( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[1].entryIdentity ) ) != 0;

    // If not intialize what to have 2 different identities.
    if ( !coilBufferStackInitialize && 
         strncmp( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[0].entryIdentity, pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[1].entryIdentity, strlen( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[1].entryIdentity ) ) != 0 )
    {
      coilBufferStackInitialize = true;
      strcpy_s( traceTempo, DIM_PATH, "coilMessageRecieved: coilBufferStackInitialize." );
      pGEN->pUtl->writeTrace( traceFilePointer		  // Entry: File pointer.
                            , traceTempo            // Entry: Trace string.
                            );

    }
    else if ( !coilBufferStackInitialize )
    {
      for ( long i_coil = 0; i_coil < DIM_COILS; i_coil++ )
        memset( &pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[i_coil], 0, sizeof(struct MOD_HV_coil_data_simulation) );
    }

    if ( coilBufferStackInitialize )
    {
      // First 2 coils have same ID.
      if ( strncmp( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[0].entryIdentity, pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[1].entryIdentity, 
                    max( strlen( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[0].entryIdentity ), strlen( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[1].entryIdentity ) ) ) == 0 )
      {
        memset( &pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[1], 0, sizeof(struct MOD_HV_coil_data_simulation) );

        if      ( strncmp( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[2].entryIdentity, pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[0].entryIdentity, strlen( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[0].entryIdentity ) ) != 0 &&
                  strncmp( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[2].entryIdentity, entryIdentityLast                                       , strlen( entryIdentityLast                                        ) ) != 0)
        {
          pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[1] = pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[2];
          memset( &pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[2], 0, sizeof(struct MOD_HV_coil_data_simulation) );
          memset( &pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[3], 0, sizeof(struct MOD_HV_coil_data_simulation) );
        }
        else if ( strncmp( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[3].entryIdentity, pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[0].entryIdentity, strlen( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[0].entryIdentity ) ) != 0 &&
                  strncmp( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[3].entryIdentity, entryIdentityLast                                       , strlen( entryIdentityLast                                        ) ) != 0)
        {
          pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[1] = pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[3];
          memset( &pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[2], 0, sizeof(struct MOD_HV_coil_data_simulation) );
          memset( &pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[3], 0, sizeof(struct MOD_HV_coil_data_simulation) );
        }
        else
        {
          memset( &pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[2], 0, sizeof(struct MOD_HV_coil_data_simulation) );
          memset( &pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[3], 0, sizeof(struct MOD_HV_coil_data_simulation) );
        }
      }

      // Different ID for the first 2 coils.
      else
      {
        strncpy_s( entryIdentityLast, DIM_NAMES, pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[0].entryIdentity, strlen( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[0].entryIdentity ) );
        
        //if      ( strncmp( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[2].entryIdentity, pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[0].entryIdentity, strlen( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[0].entryIdentity ) ) != 0 &&
        //          strncmp( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[2].entryIdentity, pGEN->pHvm->phvm->meaSptCal.measure.coils      [0].entryIdentity, strlen( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[0].entryIdentity ) ) != 0 &&
        //          strncmp( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[2].entryIdentity, pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[1].entryIdentity, strlen( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[1].entryIdentity ) ) != 0 
        //          )
        //{
        //  //pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[2] = pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[2];
        //  //memset( &pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[2], 0, sizeof(struct MOD_HV_coil_data_simulation) );
        //  memset( &pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[3], 0, sizeof(struct MOD_HV_coil_data_simulation) );
        //}
        //else if ( strncmp( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[3].entryIdentity, pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[0].entryIdentity, strlen( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[0].entryIdentity ) ) != 0 &&
        //          strncmp( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[3].entryIdentity, pGEN->pHvm->phvm->meaSptCal.measure.coils      [0].entryIdentity, strlen( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[0].entryIdentity ) ) != 0 &&
        //          strncmp( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[3].entryIdentity, pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[1].entryIdentity, strlen( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[1].entryIdentity ) ) != 0 
        //          )
        //{
        //  pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[2] = pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[3];
        //  memset( &pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[2], 0, sizeof(struct MOD_HV_coil_data_simulation) );
        //  memset( &pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[3], 0, sizeof(struct MOD_HV_coil_data_simulation) );
        //}
        //else
        //{
          memset( &pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[2], 0, sizeof(struct MOD_HV_coil_data_simulation) );
          memset( &pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[3], 0, sizeof(struct MOD_HV_coil_data_simulation) );
        //}

      }

      // If we have only one coil in Buffer. We need to add a tempory one. It is based on the first coil.
      if ( strcmp( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[1].entryIdentity, "" ) == 0 )
      {
        pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[1] = pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[0];
        strcat( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[1].entryIdentity, "G1" );
        lineWeldPositionAvailableForUse = false;
      }
      else
      {
        lineWeldPositionAvailableForUse = true;
        if ( strncmp( pGEN->pHvm->phvm->meaSptCal.measure.coils[0].entryIdentity, refCoilID, max( strlen( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[0].entryIdentity), strlen( refCoilID ) ) ) == 0 )
        {
          strcpy_s ( refCoilIDLine, "\0" );
          strncpy_s( refCoilIDLine, DIM_NAMES, pGEN->pHvm->phvm->meaSptCal.measure.coils[1].entryIdentity, strlen( pGEN->pHvm->phvm->meaSptCal.measure.coils[1].entryIdentity ) );
        }
        else
        {
          strcpy_s ( refCoilIDLine, "\0" );
          strncpy_s( refCoilIDLine, DIM_NAMES, pGEN->pHvm->phvm->meaSptCal.measure.coils[0].entryIdentity, strlen( pGEN->pHvm->phvm->meaSptCal.measure.coils[0].entryIdentity ) );
        }
        sprintf( traceTempo, "refCoilID = %s. refCoilIDLine = %s, measure.coils[1].entryIdentity = %s, measure.coils[0].entryIdentity = %s", 
                              refCoilID, 
                              refCoilIDLine, 
                              pGEN->pHvm->phvm->meaSptCal.measure.coils[0].entryIdentity, 
                              pGEN->pHvm->phvm->meaSptCal.measure.coils[1].entryIdentity );
        //utl.writeTrace( traceFilePointer		  // Entry: File pointer.
        //              , traceTempo            // Entry: Trace string.
        //              );
      }

      // Add another tempory coil.
      if ( strcmp( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[2].entryIdentity, "" ) == 0 )
      {
        pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[2] = pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[1];
        strcat( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[2].entryIdentity, "G2" );
      }

      
      sprintf( traceTempo, "coilMessageRecieved: coil BUFFER ID 0 = %s. ID 1 = %s. ID 2 = %s. ID 3 = %s.", 
               pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[0].entryIdentity, 
               pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[1].entryIdentity, 
               pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[2].entryIdentity, 
               pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[3].entryIdentity );
      //utl.writeTrace( traceFilePointer		  // Entry: File pointer.
      //              , traceTempo            // Entry: Trace string.
      //              );
    }

    //// Coil index 2 and 3 are coils on saddle in front of the entry section.
    //// If coil 2 is empty we store coil 3 instead.
    //if ( strncmp( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[2].entryIdentity, "", strlen( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[2].entryIdentity ) ) == 0 )
    //{
    //  pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[2] = pGEN->pHvm->phvm->meaSptCal.measure.coils[3];
    //  memset( &pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[3], 0, sizeof(struct MOD_HV_coil_data) );
    //}
    //
    //if ( strncmp( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[1].entryIdentity, pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[2].entryIdentity, strlen( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[2].entryIdentity ) ) == 0 )
    //{
    //  memset( &pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[2], 0, sizeof(struct MOD_HV_coil_data) );
    //  if ( strncmp( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[3].entryIdentity, "", strlen( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[3].entryIdentity ) ) != 0 )
    //  {
    //    pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[2] = pGEN->pHvm->phvm->meaSptCal.measure.coils[3];
    //    memset( &pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[3], 0, sizeof(struct MOD_HV_coil_data) );
    //  }
    //}
  }
  
  // Line measurement.
  else if ( pMessageRecieve->measurementMessage == 4002 )
  {
   
  }
  // Event weld.
  else if ( pMessageRecieve->measurementMessage == 4011 )
  {
    
  }
  // Event weld entry.
  else if ( pMessageRecieve->measurementMessage == 4012 )
  {
    
  }
  // Event weld exit.
  else if ( pMessageRecieve->measurementMessage == 4013 )
  {
    
  }
  // Process rules message.
  else if ( pMessageRecieve->measurementMessage == 5002 )
  {
    // Look if a code does not exists in the table. If so push back in front.
    for ( long iCoil = 0; iCoil < DIM_COILS; iCoil++ )
    {
      bool isInternalCodeExists = false;
      for ( long i = 0; i < 10; i++ )
      {
        if ( heatCycleCodeTable[i].internalCode == pMessageRecieve->coils[iCoil].internalCode )
        {
          // Update code.
          heatCycleCodeTable[i].heatCycleCode = pMessageRecieve->coils[iCoil].heatCycle;
          isInternalCodeExists = true;
          break;
        }
      }

      // If not found. Push back.
      if ( !isInternalCodeExists )
      {
        for ( long i = 10-1; i > 0; i-- )
        {
          heatCycleCodeTable[i] = heatCycleCodeTable[i-1];
        }

        // Store information.
        heatCycleCodeTable[0].internalCode  = pMessageRecieve->coils[iCoil].internalCode;
        heatCycleCodeTable[0].heatCycleCode = pMessageRecieve->coils[iCoil].heatCycle;
      }
    }

    bool totootoo = true;

  }

  return;
}

//============================================================================
// SPE_specific: coilQueueAndTrackingManagement
//
//   Realize coil queue and tracking management.
//    Each business are different and mesurements are not always the same
//    for the tracking so that this part has to be done for each business.
//============================================================================
bool SPE_specific::coilQueueAndTrackingManagement
                                ( FILE ** traceFilePointer		                // Entry   : Trace File pointer.
                                , bool    hasSimulationMode        // Entry: Has the model a simulation mode?
                                , double  weldsPosition        []  // Out  : Weld position of reference coil regarding reference section.
                                , char    referenceCoilIdentity[]  // Out  : Reference coil identity: first coil necessary.
                                , long   &nbWelds                  // Out  : number of welds povided inside the communication.
                                , struct GEN_utility *  pGEN
                                )
{
  char    traceTempo [DIM_TRACES + 1] = {0};
  bool    status = true;

  double weld_pos_CVT = 0., weldPositions = 0.;
  double Lgt_RTF_ff   = 420. - 228. + 170.;
  long   idx, idx2, idx3;
  char   current_coil_ID  [DIM_NAMES + 1] = { 0 };
  static char lastID      [DIM_NAMES + 1] = {0};
  static bool initialize = false;
  static bool firstTime  = true;

  if ( hasSimulationMode ) 
  {
    // Regular storage.
    for ( long i_coil = 0; i_coil < DIM_COILS; i_coil++ )
    {
      memset( &pGEN->pHvm->phvm->meaSptCal.measure.coils[i_coil], 0, sizeof(struct MOD_HV_coil_data_simulation) );
      pGEN->pHvm->phvm->meaSptCal.measure.coils[i_coil] = pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[i_coil];

      //// Find color code.
      //if ( strcmp( entryIdentities[i_coil], "" ) == 0 )
      //{
      //  colorCodes[i_coil] = colorNumbers[nuNextColorIndex];
      //  if ( ++nuNextColorIndex >= this->nbColors ) nuNextColorIndex = 0;
      //  strncpy_s( entryIdentities[i_coil], DIM_NAMES, pGEN->pHvm->phvm->meaSptCal.measure.coils[i_coil].entryIdentity, 
      //             strlen( pGEN->pHvm->phvm->meaSptCal.measure.coils[i_coil].entryIdentity ) );
      //}
      //else if ( strncmp( entryIdentities[i_coil], pGEN->pHvm->phvm->meaSptCal.measure.coils[i_coil].entryIdentity, 
      //                   strlen( pGEN->pHvm->phvm->meaSptCal.measure.coils[i_coil].entryIdentity ) ) != 0 )
      //{
      //  if ( i_coil < this->nbColors - 1 )
      //    colorCodes[i_coil] = pGEN->pHvm->phvm->meaSptCal.measure.coils[i_coil + 1].color;
      //  else
      //  {
      //    colorCodes[i_coil] = colorNumbers[nuNextColorIndex];
      //    if ( ++nuNextColorIndex >= this->nbColors ) nuNextColorIndex = 0;
      //  }
      //
      //  strncpy_s( entryIdentities[i_coil], DIM_NAMES, pGEN->pHvm->phvm->meaSptCal.measure.coils[i_coil].entryIdentity, 
      //             strlen( pGEN->pHvm->phvm->meaSptCal.measure.coils[i_coil].entryIdentity ) );
      //}


      //// Store color code. 
      //pGEN->pHvm->phvm->meaSptCal.measure.coils[i_coil].color = colorCodes[i_coil];
    }

    if ( strcmp( entryIdentities[0], "" ) == 0 )
    {
      nuNextColorIndex = 0;
      strncpy_s( entryIdentities[0], DIM_NAMES, pGEN->pHvm->phvm->meaSptCal.measure.coils[0].entryIdentity, 
                  strlen( pGEN->pHvm->phvm->meaSptCal.measure.coils[0].entryIdentity ) );
    }
    if ( strncmp( entryIdentities[0], pGEN->pHvm->phvm->meaSptCal.measure.coils[0].entryIdentity, 
                  strlen( pGEN->pHvm->phvm->meaSptCal.measure.coils[0].entryIdentity ) ) != 0 )
    {
      if ( ++nuNextColorIndex >= this->nbColors ) nuNextColorIndex = 0;
      strncpy_s( entryIdentities[0], DIM_NAMES, pGEN->pHvm->phvm->meaSptCal.measure.coils[0].entryIdentity, 
                  strlen( pGEN->pHvm->phvm->meaSptCal.measure.coils[0].entryIdentity ) );
    }

    // Find color code.
    long nuColor = nuNextColorIndex;
    for ( long i_coil = 0; i_coil < DIM_COILS; i_coil++ )
    {
      // Store color code. 
      pGEN->pHvm->phvm->meaSptCal.measure.coils[i_coil].color = colorNumbers[nuColor];
      if ( ++nuColor >= this->nbColors ) nuColor = 0;
    }

    weldsPosition[0] = pGEN->pHvm->phvm->meaSptCal.measure.line.weldPositions[0];

    // Attention je ne reçois pas d'identité bobine.
    //strncpy_s( referenceCoilIdentity, DIM_NAMES, pGEN->pHvm->phvm->meaSptCal.measure.line.entryIdentities[0], 11 );
    strncpy_s( referenceCoilIdentity, DIM_NAMES, pGEN->pHvm->phvm->meaSptCal.measure.coils[0].entryIdentity, 11 );
    nbWelds = 1;
  }
  else
  {
    // Explication sur les positions de soudure:
    // La position index 2 est celle du PLC ligne. Elle s'active en positif décroissante quand une soudure est faite. 
    // Elle s'arrête vers -15 m entre le RR et FF. 
    // La position de soudure index 0 est celle du four. Il s'agit d'une intégration de la vitesse à partir du point 0 four (entrée four).
    // Avant on suppose une recopie de la position de soudure du PLC ligne.
    // La position de soudure index 1 est l'ancienne index 0.
    // La position index 0 prend la place de l'index 1 quand l'index 0 arrive à 0. 

    // On bascule la bobine suivante dans l'actuel quand la queue de l'actuelle atteint atteint la pooint 0: position de soudure à zero.

    //pGEN->pHvm->phvm->meaSptCal.measure.line.weldPositions[0] -= 35.;
    //pGEN->pHvm->phvm->meaSptCal.measure.line.weldPositions[3] += 15.;

    time_t currentTime = 0;
    time(&currentTime);

    long test = difftime( currentTime, lastUnPack );
    
    // Check for unpack.
    if ( lastWeldPosition < pGEN->pHvm->phvm->meaSptCal.measure.line.weldPositions[0] - 100.  &&
         difftime( currentTime, lastUnPack ) > 3 * 60                                 &&      // interdiction de dépiller si moins de 3 minutes. Problème d'entrelacement de message.
         strncmp( pGEN->pHvm->phvm->meaSptCal.measure.coils[2].entryIdentity, pGEN->pHvm->phvm->meaSptCal.measure.coils[3].entryIdentity, 
                  strlen( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[2].entryIdentity) ) != 0 // Attention comparaison unquiement à la la longueur de l'index 2.
         ) 
    {
      if ( pGEN->pHvm->phvm->coilQueue.pCoilOrdered[0] != nullptr )
        pGEN->pHvm->phvm->coilQueue.lastCoils[0] = *pGEN->pHvm->phvm->coilQueue.pCoilOrdered[0];

      memset( &pGEN->pHvm->phvm->meaSptCal.measure.coils[0], 0, sizeof(struct MOD_HV_coil_data_simulation) );
      for ( long iCoil = 0; iCoil < DIM_COILS - 1; iCoil++ )
      {
        memcpy( &pGEN->pHvm->phvm->meaSptCal.measure.coils[iCoil], &pGEN->pHvm->phvm->meaSptCal.measure.coils[iCoil + 1], sizeof(struct MOD_HV_coil_data_simulation) );
      }
      memset( &pGEN->pHvm->phvm->meaSptCal.measure.coils[DIM_COILS - 1], 0, sizeof(struct MOD_HV_coil_data_simulation) );

      if ( coilBufferStackInitialize && coilStackInitialize )
      {
        strncpy_s( refCoilID, DIM_NAMES, pGEN->pHvm->phvm->meaSptCal.measure.coils[0].entryIdentity, strlen( pGEN->pHvm->phvm->meaSptCal.measure.coils[0].entryIdentity ) );
      }
      sprintf( traceTempo, "coilQueueAndTrack  : weld UNPACK ID 0 = %s. ID 1 = %s. ID 2 = %s. ID 3 = %s.", 
               pGEN->pHvm->phvm->meaSptCal.measure.coils[0].entryIdentity, 
               pGEN->pHvm->phvm->meaSptCal.measure.coils[1].entryIdentity, 
               pGEN->pHvm->phvm->meaSptCal.measure.coils[2].entryIdentity, 
               pGEN->pHvm->phvm->meaSptCal.measure.coils[3].entryIdentity );
      //utl.writeTrace( traceFilePointer		  // Entry: File pointer.
      //              , traceTempo            // Entry: Trace string.
      //              );

      lastUnPack = currentTime;
    }

    // Store last weld position.
    lastWeldPosition = pGEN->pHvm->phvm->meaSptCal.measure.line.weldPositions[0];

    // Initialization.
    if ( coilBufferStackInitialize && !coilStackInitialize )
    {
      for ( long iCoil = 0; iCoil < DIM_COILS - 2; iCoil++ )
        pGEN->pHvm->phvm->meaSptCal.measure.coils[iCoil + 1] = pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[iCoil];
      coilStackInitialize = true;
      sprintf( traceTempo, "coilQueueAndTrackingManagement: coils stack initialize. " );
      //utl.writeTrace( traceFilePointer		  // Entry: File pointer.
      //              , traceTempo            // Entry: Trace string.
      //              );
    }
    // Find coil.
    long nuCoilInMeasureStruct = -1;
    bool found = false;
    for ( long iCoil = 0; iCoil < DIM_COILS; iCoil++ )
    {
      if ( strncmp( pGEN->pHvm->phvm->meaSptCal.measure.coils[iCoil].entryIdentity, pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[0].entryIdentity, 
                  max( strlen( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[iCoil].entryIdentity), strlen( pGEN->pHvm->phvm->meaSptCal.measure.coils[0].entryIdentity ) ) ) == 0 &&
           strcmp( pGEN->pHvm->phvm->meaSptCal.measure.coils[iCoil].entryIdentity, "" ) != 0 )
      {
        found = true;
        nuCoilInMeasureStruct = iCoil;
        break;
      }
    }

    if ( found )
    {
      sprintf( traceTempo, "coilQueueAndTrackingManagement: coil ID found idx = %d. ", nuCoilInMeasureStruct );
      //utl.writeTrace( traceFilePointer		  // Entry: File pointer.
      //              , traceTempo            // Entry: Trace string.
      //              );
      for ( long iCoil = 0; iCoil < DIM_COILS - nuCoilInMeasureStruct; iCoil++ )
      {
        // The first coil recieved has changed. We store last first coil in our first index.
        memset( &pGEN->pHvm->phvm->meaSptCal.measure.coils[iCoil + nuCoilInMeasureStruct], 0, sizeof(struct MOD_HV_coil_data_simulation) );
        pGEN->pHvm->phvm->meaSptCal.measure.coils[iCoil + nuCoilInMeasureStruct] = pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[iCoil];
      }
    }
    else if ( coilBufferStackInitialize && coilStackInitialize && !found )
    {
      sprintf( traceTempo, "coilQueueAndTrackingManagement: coil ID not found !!!!!! " );
      //utl.writeTrace( traceFilePointer		  // Entry: File pointer.
      //              , traceTempo            // Entry: Trace string.
      //              );
      nuCoilInMeasureStruct = 1;
      for ( long iCoil = 0; iCoil < DIM_COILS - nuCoilInMeasureStruct; iCoil++ )
      {
        // The first coil recieved has changed. We store last first coil in our first index.
        memset( &pGEN->pHvm->phvm->meaSptCal.measure.coils[iCoil + nuCoilInMeasureStruct], 0, sizeof(struct MOD_HV_coil_data_simulation) );
        pGEN->pHvm->phvm->meaSptCal.measure.coils[iCoil + nuCoilInMeasureStruct] = pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[iCoil];
      }
    }

    sprintf( traceTempo, "coilQueueAndTrack  : coil MEAS   ID 0 = %s. ID 1 = %s. ID 2 = %s. ID 3 = %s.", 
              pGEN->pHvm->phvm->meaSptCal.measure.coils[0].entryIdentity, 
              pGEN->pHvm->phvm->meaSptCal.measure.coils[1].entryIdentity, 
              pGEN->pHvm->phvm->meaSptCal.measure.coils[2].entryIdentity, 
              pGEN->pHvm->phvm->meaSptCal.measure.coils[3].entryIdentity );
    //utl.writeTrace( traceFilePointer		  // Entry: File pointer.
    //              , traceTempo            // Entry: Trace string.
    //              );

    status = coilBufferStackInitialize && coilStackInitialize;

    //// Check if modification of the first coil identity recieved in coils buffer.
    //if ( strncmp( pGEN->pHvm->phvm->meaSptCal.measure.coils[1].entryIdentity, pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[0].entryIdentity, 
    //              max( strlen( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[0].entryIdentity), strlen( pGEN->pHvm->phvm->meaSptCal.measure.coils[1].entryIdentity ) ) ) != 0 )
    //{
    //  weldingTime = pGEN->pHvm->phvm->meaSptCal.measure.coils[0].specific.weldingTime;
    //  entryTime   = pGEN->pHvm->phvm->meaSptCal.measure.coils[0].specific.entryTime  ;
    //  exitTime    = pGEN->pHvm->phvm->meaSptCal.measure.coils[0].specific.exitTime   ;
    //  // The first coil recieved has changed. We store last first coil in our first index.
    //  memset( &pGEN->pHvm->phvm->meaSptCal.measure.coils[0], 0, sizeof(struct MOD_HV_coil_data) );
    //  pGEN->pHvm->phvm->meaSptCal.measure.coils[0] = pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[1];
    //  pGEN->pHvm->phvm->meaSptCal.measure.coils[0].specific.weldingTime = weldingTime;
    //  pGEN->pHvm->phvm->meaSptCal.measure.coils[0].specific.entryTime   = entryTime  ;
    //  pGEN->pHvm->phvm->meaSptCal.measure.coils[0].specific.exitTime    = exitTime   ;
    //}
    //
    //// Regular storage.
    //for ( long i_coil = 1; i_coil < 4; i_coil++ )
    //{
    //  weldingTime = pGEN->pHvm->phvm->meaSptCal.measure.coils[i_coil].specific.weldingTime;
    //  entryTime   = pGEN->pHvm->phvm->meaSptCal.measure.coils[i_coil].specific.entryTime  ;
    //  exitTime    = pGEN->pHvm->phvm->meaSptCal.measure.coils[i_coil].specific.exitTime   ;
    //  memset( &pGEN->pHvm->phvm->meaSptCal.measure.coils[i_coil], 0, sizeof(struct MOD_HV_coil_data) );
    //  pGEN->pHvm->phvm->meaSptCal.measure.coils[i_coil] = pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[i_coil - 1];
    //  pGEN->pHvm->phvm->meaSptCal.measure.coils[i_coil].specific.weldingTime = weldingTime;
    //  pGEN->pHvm->phvm->meaSptCal.measure.coils[i_coil].specific.entryTime   = entryTime  ;
    //  pGEN->pHvm->phvm->meaSptCal.measure.coils[i_coil].specific.exitTime    = exitTime   ;
    //}

    // Store weld position.
    if ( !lineWeldPositionAvailableForUse || pGEN->pHvm->phvm->meaSptCal.measure.line.weldPositions[0] > - 300. ) // 500.
    {
      weldsPosition[0] = pGEN->pHvm->phvm->meaSptCal.measure.line.weldPositions[0] ;   // WELD- 15. * 2. - 10.
      
      // Store reference coil identities.
      strncpy_s( referenceCoilIdentity, DIM_NAMES, refCoilID, strlen( refCoilID ) );

    }
    else
    {
      weldsPosition[0] = -1. * pGEN->pHvm->phvm->meaSptCal.measure.line.weldPositions[3] ;   // WELD- 15. * 2. - 10.
      
      // Store reference coil identities.
      strncpy_s( referenceCoilIdentity, DIM_NAMES, refCoilIDLine, strlen( refCoilIDLine ) );
    }
    nbWelds = 1;

    sprintf( traceTempo, "coilQueueAndTrackingManagement: ref coil ID 0 = %s. lineWeldPositionAvailableForUse = %d, weld 0 = %f, weld 3 = %f, weld = %f", 
              referenceCoilIdentity, 
              lineWeldPositionAvailableForUse,
              pGEN->pHvm->phvm->meaSptCal.measure.line.weldPositions[0], 
              pGEN->pHvm->phvm->meaSptCal.measure.line.weldPositions[3], 
              weldsPosition[0] );
    //utl.writeTrace( traceFilePointer		  // Entry: File pointer.
    //              , traceTempo            // Entry: Trace string.
    //              );

    //weldsPosition[1] = pGEN->pHvm->phvm->meaSptCal.measure.line.weldPositions[0];
    //weldsPosition[2] = pGEN->pHvm->phvm->meaSptCal.measure.line.weldPositions[0];
    //weldsPosition[3] = pGEN->pHvm->phvm->meaSptCal.measure.line.weldPositions[0];


    //if ( initializeLastFistCoilID &&
    //     strcmp( referenceCoilIdentity, "" ) != 0)
    //{
    //  strncpy_s( this->lastFistCoilID, DIM_NAMES, referenceCoilIdentity, strlen( referenceCoilIdentity ) );     // A vérifier sur site.
    //  initializeLastFistCoilID = false;
    //}
    //
    //// If first coil ID changed.
    //if ( strncmp( this->lastFistCoilID, referenceCoilIdentity, strlen( referenceCoilIdentity ) ) != 0 && initializeLastFistCoilID )
    //{
    //  pGEN->pHvm->phvm->coilQueue.lastCoils[0] = pGEN->pHvm->phvm->coilQueue.coils[0];
    //  strncpy_s( this->lastFistCoilID, DIM_NAMES, referenceCoilIdentity, strlen( referenceCoilIdentity ) );     // A vérifier sur site.
    //}

    if ( coilBufferStackInitialize && coilStackInitialize )
    {
      
      if ( strcmp( entryIdentities[0], "" ) == 0 )
      {
        nuNextColorIndex = 0;
        strncpy_s( entryIdentities[0], DIM_NAMES, pGEN->pHvm->phvm->meaSptCal.measure.coils[0].entryIdentity, 
                   strlen( pGEN->pHvm->phvm->meaSptCal.measure.coils[0].entryIdentity ) );
      }
      if ( strncmp( entryIdentities[0], pGEN->pHvm->phvm->meaSptCal.measure.coils[0].entryIdentity, 
                    strlen( pGEN->pHvm->phvm->meaSptCal.measure.coils[0].entryIdentity ) ) != 0 )
      {
        if ( ++nuNextColorIndex >= 4 ) nuNextColorIndex = 0;
        strncpy_s( entryIdentities[0], DIM_NAMES, pGEN->pHvm->phvm->meaSptCal.measure.coils[0].entryIdentity, 
                   strlen( pGEN->pHvm->phvm->meaSptCal.measure.coils[0].entryIdentity ) );
      }

      //// Find color code.
      //if ( strcmp( entryIdentities[i_coil], "" ) == 0 && strcmp( pGEN->pHvm->phvm->meaSptCal.measure.coils[i_coil].entryIdentity, "" )  )
      //{
      //  colorCodes[i_coil] = colorNumbers[i_coil];
      //  strncpy_s( entryIdentities[i_coil], DIM_NAMES, pGEN->pHvm->phvm->meaSptCal.measure.coils[i_coil].entryIdentity, 
      //             strlen( pGEN->pHvm->phvm->meaSptCal.measure.coils[i_coil].entryIdentity ) );
      //}
      //else if ( strncmp( entryIdentities[i_coil], pGEN->pHvm->phvm->meaSptCal.measure.coils[i_coil].entryIdentity, 
      //                   strlen( pGEN->pHvm->phvm->meaSptCal.measure.coils[i_coil].entryIdentity ) ) != 0 )
      //{
      //  if ( strcmp( pGEN->pHvm->phvm->meaSptCal.measure.coils[i_coil].entryIdentity, "" ) != 0 )
      //  {
      //    if ( strcmp( entryIdentities[i_coil], "" ) == 0 || strcmp( entryIdentities[i_coil + 1], "" ) == 0 )
      //    {
      //      colorCodes[i_coil] = colorNumbers[nuNextColorIndex];
      //      if ( ++nuNextColorIndex >= 4 ) nuNextColorIndex = 0;
      //    }
      //    else
      //    {
      //      colorCodes[i_coil] = pGEN->pHvm->phvm->meaSptCal.measure.coils[i_coil + 1].color;
      //    }
      //
      //    strncpy_s( entryIdentities[i_coil], DIM_NAMES, pGEN->pHvm->phvm->meaSptCal.measure.coils[i_coil].entryIdentity, 
      //               strlen( pGEN->pHvm->phvm->meaSptCal.measure.coils[i_coil].entryIdentity ) );
      //  }
      //  else 
      //  {
      //    memset( entryIdentities[i_coil], 0, sizeof( char ) * DIM_NAMES );
      //  }
      //}
      // Find color code.
      long nuColor = nuNextColorIndex;
      for ( long i_coil = 0; i_coil < 4; i_coil++ )
      {
        // Store color code. 
        pGEN->pHvm->phvm->meaSptCal.measure.coils[i_coil].color = colorNumbers[nuColor];
        if ( ++nuColor >= 4 ) nuColor = 0;
      }
    } // END: initialized.
  }

  return true;
}

//============================================================================
// SPE_specific: productLineNumber
//
//   Set product line number based on any parameters desired.
//============================================================================
void SPE_specific::productLineNumber
                                ( FILE **traceFilePointer		        // Entry   : Trace File pointer.
                                , bool    hasSimulationMode         // Entry: Has the model a simulation mode?
                                , struct GEN_utility *  pGEN
                                )
{
  char traceTempo [DIM_TRACES + 1] = {0};

  // For each product in queue.
  for ( long i_coil = 0; i_coil < DIM_COILS; i_coil++ )
  {
    if ( strncmp( pGEN->pHvm->phvm->coilQueue.coils[i_coil].data.steelGrade, "GI", 4 ) == 0 )
    {
      pGEN->pHvm->phvm->coilQueue.coils[i_coil].calcul.nuLine = 0;
    }
    else
    {
      // Product line number not found.
      pGEN->pHvm->phvm->coilQueue.coils[i_coil].calcul.nuLine = 0;
      
    }
    
    // Line number for simulation and not simulation.
    if ( hasSimulationMode )
    {
      if ( pGEN->pHvm->phvm->coilQueue.coils[i_coil].data.origin == 1 )
        pGEN->pHvm->phvm->coilQueue.coils[i_coil].calcul.nuLine = 1;
      else
        pGEN->pHvm->phvm->coilQueue.coils[i_coil].calcul.nuLine = 0;
    }
    else
    {
      pGEN->pHvm->phvm->coilQueue.coils[i_coil].calcul.nuLine = 0;
    }
  }

  return;
}

//============================================================================
// SPE_specific: productcalculs
//
//   Realized all calculs on products.
//============================================================================
void SPE_specific::productCalculs
                                ( FILE **traceFilePointer		          // Entry: Trace File pointer.
                                , bool   hasSimulationMode            // Entry: Has the model a simulation mode?
                                , struct GEN_utility *  pGEN
                                )
{
  char traceTempo [DIM_TRACES + 1];

  //------------------------------------------------------------------------------------
  // Initialize data valid to true for all products.
  for ( long i_coil = 0; i_coil < DIM_COILS; i_coil++ )
  {
    pGEN->pHvm->phvm->coilQueue.coils[i_coil].calcul.dataValid = true;
  }
  
  //------------------------------------------------------------------------------------
  // Get product line number.
  productLineNumber( traceFilePointer, hasSimulationMode, pGEN );
  
  //------------------------------------------------------------------------------------
  // We store the heat cycle code inside the stack of coils.
  for ( long iCoil = 0; iCoil < DIM_COILS; iCoil++ )
  {
    bool found = false;
    if ( pGEN->pHvm->phvm->coilQueue.coils[iCoil].data.internalCode > 0 )
    {
      for ( long i = 0; i < 10; i++ )
      {
        if ( heatCycleCodeTable[i].internalCode > 0 )
        {
          // If internal code is corresponding.
          if ( pGEN->pHvm->phvm->coilQueue.coils[iCoil].data.internalCode == heatCycleCodeTable[i].internalCode )
          {
            // Store heat cycle code.
            pGEN->pHvm->phvm->coilQueue.coils[iCoil].data.heatCycle = heatCycleCodeTable[i].heatCycleCode;
            found = true;
          }
        }
      }
    }

    // If code has not been stored.
    if( !found )
    {
      // Look if what is recieved in coil queue is already exisiting in heat cycle table.
      if ( pGEN->pHvm->phvm->coilQueue.coils[iCoil].data.heatCycle > 0 )
      {
        long nu_cycle = -1;
        found = pGEN->pHvm->findProductCycle ( pGEN->pHvm->phvm->coilQueue.coils[iCoil].data.heatCycle      // Entry: Product heat cycles number.
                                             , nu_cycle                                                     // Out  : Cycle number in table.
                                             );
        if ( found )
        {
          pGEN->pHvm->phvm->coilQueue.coils[iCoil].data.heatCycle = nu_cycle;
        }
      }
    }
    
    // If code has not been stored.
    if( !found )
    {
      // Set default code.
      // For each heat cycles.
      long nu_cycle = -1;
      for ( long i_cycle = 0; i_cycle < DIM_HEAT_CYCLES; i_cycle++ )
      {
        if ( pGEN->pHvm->phvm->parameter.lineCycle[0].shared.heatCycles[i_cycle].bIsDefault )
        {
          nu_cycle = i_cycle;
          found    = true   ;
          break;
        }
      }

      if ( !found ) nu_cycle = 0;
      pGEN->pHvm->phvm->coilQueue.coils[iCoil].data.heatCycle = pGEN->pHvm->phvm->parameter.lineCycle[0].shared.heatCycles[nu_cycle].cycleNumber;

    }
  }

  //------------------------------------------------------------------------------------
  // Regular.
  for ( long i_coil = 0; i_coil < DIM_COILS; i_coil++ )
  {
    // Do regular coil management.
    this->productCalculsALL
      ( traceFilePointer                            // Entry: Trace File pointer.
      , hasSimulationMode                           // Entry: Has the model a simulation mode?
      , &pGEN->pHvm->phvm->coilQueue.coils[i_coil]  // Modified: Coil structure.
      , pGEN
      );
  }

  //------------------------------------------------------------------------------------
  // For each coils in queue.
  for ( long i_coil = 0; i_coil < DIM_COILS; i_coil++ )
  {
    if ( strcmp( pGEN->pHvm->phvm->coilQueue.coils[i_coil].data.entryIdentity, "" ) == 0 ) continue;

    pGEN->pHvm->phvm->coilQueue.coils[i_coil].calcul.bisraCode       = 0;   // Bisra code number 3.
    pGEN->pHvm->phvm->coilQueue.coils[i_coil].calcul.dataValid       = false;

    //------------------------------------------------------------------------------------
    // Current e * V calculation.
    double ev = pMOD->pConv->length( pGEN->pHvm->phvm->coilQueue.coils[i_coil].data.thickness, TYPE_METERS, TYPE_MM  ) * 
                pMOD->pConv->speed ( pGEN->pHvm->phvm->meaSptCal.measure.line.speed          , TYPE_MPS   , TYPE_MPM ); 

    // Copy current RTF emissivity in next coil class if different.
    long nuCoilCurrentOrdered     = pGEN->pHvm->phvm->meaSptCal.lines[0].pSections[0]->pCalcul->tracking.nu_coilInOrdered;
    long nuCoilCurrentOrderedNext = nuCoilCurrentOrdered + 1;
    
    if ( pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoilCurrentOrdered    ] != nullptr && 
         pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoilCurrentOrderedNext] != nullptr )
    {
      if ( pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoilCurrentOrdered    ]->calcul.emissivityClass !=
           pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoilCurrentOrderedNext]->calcul.emissivityClass  )
      {
        long nuClassNext    = pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoilCurrentOrderedNext]->calcul.emissivityClass;
        long nuClassCurrent = pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoilCurrentOrdered    ]->calcul.emissivityClass;
        for ( long iSection = 0; iSection < pMOD->pConf->pconf->nb_sections; iSection++ )
        {
          long sectionType = pMOD->pConf->pconf->pSections[iSection]->type;
  
          if ( sectionType == TYPE_RTF )
            pGEN->pHvm->phvm->adapatation.emissivityClasses[nuClassNext].emissivity[iSection] = pGEN->pHvm->phvm->adapatation.emissivityClasses[nuClassCurrent].emissivity[iSection];

          if ( sectionType == TYPE_NOF )
            pGEN->pHvm->phvm->adapatation.emissivityClasses[nuClassNext].emissivity[iSection] = pGEN->pHvm->phvm->adapatation.emissivityClasses[nuClassCurrent].emissivity[iSection];
        }
      }
    }

    //long nuLine = pGEN->pHvm->phvm->coilQueue.coils[i_coil].calcul.nuLine;
    //
    //// Store coil data in tempory structure. Done to reaorganize what is recieved in message versus requiered by the line defenition selected.
    //struct MOD_HV_coil_data CoilData = pGEN->pHvm->phvm->coilQueue.coils[i_coil].data;
    //
    //for ( long iSection = 0; iSection < pMOD->pConf->pconf->lines[nuLine].nb_sections; iSection++ )
    //{
    //  long nuSectionGlobal = pMOD->pConf->pconf->lines[nuLine].pSections[iSection]->indexGlobal;
    //
    //  pGEN->pHvm->phvm->coilQueue.coils[i_coil].data.sections[iSection].stripTemp =
    //    CoilData.sections[nuSectionGlobal].stripTemp;
    //}

    
  }

  return;
}

//============================================================================
// SPE_specific: productCalculsALL
//
//   Realized all calculs on products with a coil provided.
//============================================================================
void SPE_specific::productCalculsALL
                                ( FILE **traceFilePointer		          // Entry: Trace File pointer.
                                , bool   hasSimulationMode            // Entry: Has the model a simulation mode?
                                , struct MOD_HV_coil *  pCoil         // Modified: Coil structure.
                                , struct GEN_utility *  pGEN
                                )
{
  char traceTempo [DIM_TRACES + 1];

  //------------------------------------------------------------------------------------
  // Find emissivity class.
  if ( pCoil->data.steelType < 0 ||
        pCoil->data.steelType >= DIM_EMISSIVITY_CLASS / 5 )
    pCoil->calcul.emissivityClass = 99;
  else
  {
    if      ( pCoil->data.thickness <= 0.3 / 1000. )  pCoil->calcul.emissivityClass = 0;
    else if ( pCoil->data.thickness <= 0.5 / 1000. )  pCoil->calcul.emissivityClass = 1;
    else if ( pCoil->data.thickness <= 0.8 / 1000. )  pCoil->calcul.emissivityClass = 2;
    else if ( pCoil->data.thickness <= 1.1 / 1000. )  pCoil->calcul.emissivityClass = 3;
    else if ( pCoil->data.thickness <= 1.5 / 1000. )  pCoil->calcul.emissivityClass = 4;
    else                                              pCoil->calcul.emissivityClass = 5;
  }
  
  // Define line configuration.
  if ( pCoil->data.soakingTime > 0. || true )
    pCoil->calcul.nuLine = 0;
  else
    pCoil->calcul.nuLine = 1;

  long nuLine = pCoil->calcul.nuLine;

  // Store coil data in tempory structure. Done to reaorganize what is recieved in message versus requiered by the line defenition selected.
  struct MOD_HV_coil_data CoilData = pCoil->data;
    
  for ( long iSection = 0; iSection < pMOD->pConf->pconf->lines[nuLine].nb_sections; iSection++ )
  {
    long nuSectionGlobal = pMOD->pConf->pconf->lines[nuLine].pSections[iSection]->indexGlobal;
    
    pCoil->data.sections[iSection].stripTemp =
      CoilData.sections[nuSectionGlobal].stripTemp;
  }

  //------------------------------------------------------------------------------------
  // Find product cycle.
  long nu_cycle = -1;
  //if ( pCoil->data.heatCycle == 0 )
  //  pCoil->data.heatCycle = 1;
  bool found = pGEN->pHvm->findProductCycle ( pCoil->data.heatCycle     // Entry: Product heat cycles number.
                                            , nu_cycle                  // Out  : Cycle number in table.
                                            );
  
  //------------------------------------------------------------------------------------
  // Store heat cycle.
  // If heat cycle has been found.
  if ( found )
  {
    if ( nuLine == 0 )
    {
      // For all sections defined in line configuration.
      for ( long i_section = 0; i_section < pMOD->pConf->pconf->lines[nuLine].nb_sections; i_section++ )
      {
        if ( i_section <= 10 )  // to be checked that pot replace HB.
        {
          pCoil->data.sections[i_section].stripTemp.upper  = 
            pGEN->pHvm->phvm->parameter.lineCycle[0].shared.heatCycles[nu_cycle].sections[i_section].stripUpper;
          pCoil->data.sections[i_section].stripTemp.target = 
            pGEN->pHvm->phvm->parameter.lineCycle[0].shared.heatCycles[nu_cycle].sections[i_section].stripTarget;
          pCoil->data.sections[i_section].stripTemp.lower  = 
            pGEN->pHvm->phvm->parameter.lineCycle[0].shared.heatCycles[nu_cycle].sections[i_section].stripLower;

          // HB section.
          if ( i_section == 4 )
          {
            gblSec.lockAccess();
            gblSec.pSpe->roofTempHBSetpoint = pCoil->data.sections[i_section].stripTemp.target;
            gblSec.unLockAccess();
          }
        }
        else
        {
          pCoil->data.sections[i_section].stripTemp.upper  = 
            pGEN->pHvm->phvm->parameter.lineCycle[0].shared.heatCycles[nu_cycle].sections[i_section].stripUpper;
          pCoil->data.sections[i_section].stripTemp.target = 
            pGEN->pHvm->phvm->parameter.lineCycle[0].shared.heatCycles[nu_cycle].sections[i_section].stripTarget;
          pCoil->data.sections[i_section].stripTemp.lower  = 
            pGEN->pHvm->phvm->parameter.lineCycle[0].shared.heatCycles[nu_cycle].sections[i_section].stripLower;

        }

        gblSec.lockAccess();

        // Apply offset to FF section.
        if      ( i_section == 0 )
        {
          gblSec.pSpe->FFStripTempOffset = min( max( gblSec.pSpe->FFStripTempOffset, -40. ), 40. );
          pCoil->data.sections[i_section].stripTemp.target += gblSec.pSpe->FFStripTempOffset;
          pCoil->data.sections[i_section].stripTemp.lower  += gblSec.pSpe->FFStripTempOffset;
          pCoil->data.sections[i_section].stripTemp.lower   = min( pCoil->data.sections[i_section].stripTemp.lower, 
                                                                   pCoil->data.sections[i_section].stripTemp.target - 5.  );
          pCoil->data.sections[i_section].stripTemp.upper   = max( pCoil->data.sections[i_section].stripTemp.upper, 
                                                                   pCoil->data.sections[i_section].stripTemp.target + 10. );
        }
        // Apply offset to TT section.
        else if ( i_section == 1 )
        {
          gblSec.pSpe->TTStripTempOffset = min( max( gblSec.pSpe->TTStripTempOffset, -20. ), 20. );
          pCoil->data.sections[i_section].stripTemp.target += gblSec.pSpe->TTStripTempOffset;
          pCoil->data.sections[i_section].stripTemp.lower   = min( pCoil->data.sections[i_section].stripTemp.lower, 
                                                                   pCoil->data.sections[i_section].stripTemp.target - 10. );
          pCoil->data.sections[i_section].stripTemp.upper   = max( pCoil->data.sections[i_section].stripTemp.upper, 
                                                                   pCoil->data.sections[i_section].stripTemp.target + 10. );
        }
        // Apply offset to HH section.
        else if ( i_section == 2 )
        {
          gblSec.pSpe->HHStripTempOffset = min( max( gblSec.pSpe->HHStripTempOffset, -20. ), 20. );
          pCoil->data.sections[i_section].stripTemp.target += gblSec.pSpe->HHStripTempOffset;
          pCoil->data.sections[i_section].stripTemp.lower   = min( pCoil->data.sections[i_section].stripTemp.lower, 
                                                                   pCoil->data.sections[i_section].stripTemp.target - 10. );
          pCoil->data.sections[i_section].stripTemp.upper   = max( pCoil->data.sections[i_section].stripTemp.upper, 
                                                                   pCoil->data.sections[i_section].stripTemp.target + 10. );
        }
        // Apply offset to HH section.
        else if ( i_section == 3 )
        {
          gblSec.pSpe->CCStripTempOffset = min( max( gblSec.pSpe->CCStripTempOffset, -20. ), 20. );
          pCoil->data.sections[i_section].stripTemp.target += gblSec.pSpe->CCStripTempOffset;
          pCoil->data.sections[i_section].stripTemp.lower   = min( pCoil->data.sections[i_section].stripTemp.lower, 
                                                                   pCoil->data.sections[i_section].stripTemp.target - 10. );
          pCoil->data.sections[i_section].stripTemp.upper   = max( pCoil->data.sections[i_section].stripTemp.upper, 
                                                                   pCoil->data.sections[i_section].stripTemp.target + 10. );
        }
        // Apply offset on HB section.
        else if ( i_section == 4 )
        {
          gblSec.pSpe->HBStripTempOffset = min( max( gblSec.pSpe->HBStripTempOffset, -30. ), 20. );
          pCoil->data.sections[i_section].stripTemp.target += gblSec.pSpe->HBStripTempOffset;
          pCoil->data.sections[i_section].stripTemp.lower   = min( pCoil->data.sections[i_section].stripTemp.lower, 
                                                                   pCoil->data.sections[i_section].stripTemp.target - 10. );
          pCoil->data.sections[i_section].stripTemp.upper   = max( pCoil->data.sections[i_section].stripTemp.upper, 
                                                                   pCoil->data.sections[i_section].stripTemp.target + 10. );
          
          gblSec.lockAccess();
          gblSec.pSpe->roofTempHBSetpoint += gblSec.pSpe->HBStripTempOffset;
          gblSec.unLockAccess();
        }
        // Apply offset on DD section.
        else if ( i_section == 5 )
        {
          gblSec.pSpe->DDStripTempOffset = min( max( gblSec.pSpe->DDStripTempOffset, -30. ), 20. );
          pCoil->data.sections[i_section].stripTemp.target += gblSec.pSpe->DDStripTempOffset;
          pCoil->data.sections[i_section].stripTemp.lower   = min( pCoil->data.sections[i_section].stripTemp.lower, 
                                                                   pCoil->data.sections[i_section].stripTemp.target - 10. );
          pCoil->data.sections[i_section].stripTemp.upper   = max( pCoil->data.sections[i_section].stripTemp.upper, 
                                                                   pCoil->data.sections[i_section].stripTemp.target + 10. );
        }
        
        gblSec.unLockAccess();


      }
    }
    // Below not validated.
    //else if ( nuLine == 1 )
    //{
    //  // For all sections defined in line configuration.
    //  for ( long i_section = 0; i_section < pMOD->pConf->pconf->lines[nuLine].nb_sections; i_section++ )
    //  {
    //    if ( i_section == 0 )
    //    {
    //      pCoil->data.sections[i_section].stripTemp.upper  = 
    //        pGEN->pHvm->phvm->parameter.lineCycle[0].shared.heatCycles[nu_cycle].sections[i_section].stripUpper;
    //      pCoil->data.sections[i_section].stripTemp.target = 
    //        pGEN->pHvm->phvm->parameter.lineCycle[0].shared.heatCycles[nu_cycle].sections[i_section].stripTarget;
    //      pCoil->data.sections[i_section].stripTemp.lower  = 
    //        pGEN->pHvm->phvm->parameter.lineCycle[0].shared.heatCycles[nu_cycle].sections[i_section].stripLower;
    //    }
    //    else
    //    {
    //      pCoil->data.sections[i_section].stripTemp.upper  = 
    //        pGEN->pHvm->phvm->parameter.lineCycle[0].shared.heatCycles[nu_cycle].sections[i_section+1].stripUpper;
    //      pCoil->data.sections[i_section].stripTemp.target = 
    //        pGEN->pHvm->phvm->parameter.lineCycle[0].shared.heatCycles[nu_cycle].sections[i_section+1].stripTarget;
    //      pCoil->data.sections[i_section].stripTemp.lower  = 
    //        pGEN->pHvm->phvm->parameter.lineCycle[0].shared.heatCycles[nu_cycle].sections[i_section+1].stripLower;
    //    }
  
    //  }
  
    //}
  }
  // Product heat cycle has not been found.
  else
  {
    pCoil->calcul.dataValid = false;
    sprintf_s( traceTempo, DIM_PATH, "Product heat cycle not found. ID = %s, Heat cycle number = %d"
                                   , pCoil->data.entryIdentity
                                   , pCoil->data.heatCycle
                                   );
    pMOD->pUtl->writeTrace( traceFilePointer		  // Entry: File pointer.
                  , traceTempo            // Entry: Trace string.
                  );
  }

  return;
}

//============================================================================
// SPE_specific: finalSpecific
//
//   Realized final specifc actions.
//============================================================================
void SPE_specific::finalSpecificActions( struct GEN_utility *  pGEN )
{
  


  return;
}

//============================================================================
// SPE_specific: onLiveCalculationBegins
//
//   Specific actions when beginning the on live calculation.
//============================================================================
void SPE_specific::onLiveCalculationBegins( FILE **traceFilePointer, double timeSpan, struct GEN_utility *  pGEN )
{
  
	// Anticipation time.
  for ( long i_coil = 0; i_coil < DIM_COILS; i_coil++ )
  {
    // Line number.
    long nuLine = pGEN->pHvm->phvm->coilQueue.pCoilOrdered[i_coil]->calcul.nuLine;
    nuLine = 0;   // Everything is treated with basic line  definition.
    bool bIsManagingLineSpeed = true;

    for ( long i_section = 0; i_section < pMOD->pConf->pconf->lines[nuLine].nb_sections; i_section++ )
    {
      // Get anticipation factor.
      long    nuSectionGlobal           = pMOD->pConf->pconf->lines[nuLine].pSections[i_section]->indexGlobal;
      double  anticipationFactor        = pGEN->pHvm->getParameterAnticipationFactor( nuSectionGlobal );
      double  anticipationFactorSpeed   = pGEN->pHvm->getParameterAnticipationFactor( nuSectionGlobal );
      double  dStripLengthForSpeed      = 0.;
      double  dStripTimeForSpeed        = 0.;
      anticipationFactor      = min( max( anticipationFactor      , 0.01 ), 1000. );
      anticipationFactorSpeed = min( max( anticipationFactorSpeed , 0.01 ), 1000. );

      // If section is not managing line speed.
      if ( pGEN->pHvm->phvm->meaSptCal.lines[nuLine].pSections[i_section] != pGEN->pHvm->phvm->meaSptCal.lines[nuLine].pSectionsTransientSpeed )
      {
        bIsManagingLineSpeed = false;
        for ( long jSection = 0; jSection < pMOD->pConf->pconf->lines[nuLine].nb_sections; jSection++ )
        {
          if ( pGEN->pHvm->phvm->meaSptCal.lines[nuLine].pSections[jSection] == pGEN->pHvm->phvm->meaSptCal.lines[nuLine].pSectionsTransientSpeed )
          {
            long    nuSectionGlobalBis  = pMOD->pConf->pconf->lines[nuLine].pSections[jSection]->indexGlobal;
            anticipationFactorSpeed     = pGEN->pHvm->getParameterAnticipationFactor( nuSectionGlobalBis );
            anticipationFactorSpeed     = min( max( anticipationFactorSpeed, 0.01 ), 1000. );
            long nuLastPyroJsection     = pMOD->pConf->pconf->lines[nuLine].pSections[jSection ]->nb_pyrometers - 1;
            long nuLastPyroIsection     = pMOD->pConf->pconf->lines[nuLine].pSections[i_section]->nb_pyrometers - 1;
            dStripLengthForSpeed        = pMOD->pConf->pconf->lines[nuLine].pSections[jSection ]->tracking.stripLengthPyrometerToReference[nuLastPyroJsection] - 
                                          pMOD->pConf->pconf->lines[nuLine].pSections[i_section]->tracking.stripLengthPyrometerToReference[nuLastPyroIsection];
          }
        }
      }

      if ( pMOD->pConf->pconf->lines[nuLine].pSections[i_section]->type != TYPE_BASE ||
           pMOD->pConf->pconf->lines[nuLine].pSections[i_section]->type != TYPE_POT )
      {
        double deltaTemperature  = 0.;
        double deltaSpeed        = 0.;
        deltaTemperature  = pGEN->pHvm->phvm->coilQueue.pCoilOrdered[i_coil]->calcul.stages.transient.lines[nuLine].sections[i_section].stripTarget - 
                            pGEN->pHvm->phvm->coilQueue.pCoilOrdered[i_coil]->calcul.stages.steady   .lines[nuLine].sections[i_section].stripTarget;

        if ( pMOD->pConf->pconf->lines[nuLine].pSections[i_section]->type != TYPE_BASE ||
             pMOD->pConf->pconf->lines[nuLine].pSections[i_section]->type != TYPE_POT )
          deltaSpeed        = pGEN->pHvm->phvm->coilQueue.pCoilOrdered[i_coil]->calcul.stages.transient.lines[nuLine].sections[i_section].speed - 
                              pGEN->pHvm->phvm->coilQueue.pCoilOrdered[i_coil]->calcul.stages.steady   .lines[nuLine].sections[i_section].speed;

        if ( pMOD->pConf->pconf->lines[nuLine].pSections[i_section]->type == TYPE_NOF )
        {
          long nuPyro = pMOD->pConf->pconf->lines[nuLine].pSections[i_section]->nb_pyrometers - 1;
          deltaTemperature  = pGEN->pHvm->phvm->coilQueue.pCoilOrdered[i_coil]->calcul.stages.transient.lines[nuLine].sections[i_section].pyrometerTarget[nuPyro] - 
                              pGEN->pHvm->phvm->coilQueue.pCoilOrdered[i_coil]->calcul.stages.steady   .lines[nuLine].sections[i_section].pyrometerTarget[nuPyro];
        }
        
        deltaTemperature = min( max( deltaTemperature, -50.       ), 50.        );
        deltaSpeed       = min( max( deltaSpeed      , -30. / 60. ), 50.  / 60. );

        double lineSpeed = pGEN->pHvm->phvm->coilQueue.pCoilOrdered[i_coil]->calcul.stages.transient.lines[nuLine].sections[i_section].speed;

        // Simulation.
        if ( this->hasSimulationMode )
        {
          double distanceTemperature      = abs( deltaTemperature ) /   7.           * 120.;
          double distanceSpeed            = abs( deltaSpeed * 60. ) / ( 7. * 0.75 )  * 120.;
          double anticipationTemperature  = distanceTemperature / lineSpeed;
          double anticipationSpeed        = distanceSpeed       / lineSpeed;

          if      ( i_section == 1 )
          {
            pGEN->pHvm->phvm->coilQueue.pCoilOrdered[i_coil]->calcul.stages.anticipationTime[i_section] = max( anticipationTemperature, anticipationSpeed );
          }
          else if ( i_section == 0 )
            pGEN->pHvm->phvm->coilQueue.pCoilOrdered[i_coil]->calcul.stages.anticipationTime[i_section] = max( anticipationTemperature, anticipationSpeed );
          else 
            pGEN->pHvm->phvm->coilQueue.pCoilOrdered[i_coil]->calcul.stages.anticipationTime[i_section] = 0.;
        }
        // Reality.
        else
        {
          // We offset low transient variation temperature to get anought time to make it. 
          if ( pMOD->pConf->pconf->lines[nuLine].pSections[i_section]->type == TYPE_RTF && abs( deltaTemperature ) <= 10 + pMOD->pMath->epsilon )
          {
            deltaTemperature *= 1.5;
            deltaTemperature = min( deltaTemperature, 14. );
          }

          dStripTimeForSpeed = dStripLengthForSpeed / pGEN->pHvm->phvm->coilQueue.pCoilOrdered[i_coil]->calcul.stages.steady.lines[nuLine].sections[i_section].speed;

          double anticipationTemperature = abs( deltaTemperature ) /   anticipationFactor                 * 60.;
          double anticipationSpeed       = abs( deltaSpeed * 60. ) / ( anticipationFactorSpeed * 0.75 )   * 60.;
          if ( abs ( deltaSpeed ) > pMOD->pMath->epsilon ) 
          anticipationSpeed += dStripTimeForSpeed;
          anticipationSpeed = max( anticipationSpeed, 0. );

          if ( deltaSpeed <= 0.001 && deltaTemperature < -2. )
            deltaTemperature *= 1.15;

          if ( pGEN->pHvm->phvm->coilQueue.pCoilOrdered[i_coil]->calcul.stages.transient.lines[nuLine].sections[i_section].speed <= 60. / 60. )
            deltaTemperature += 60.;

          if      ( i_section == 1 )
            pGEN->pHvm->phvm->coilQueue.pCoilOrdered[i_coil]->calcul.stages.anticipationTime[i_section] = max( anticipationTemperature, anticipationSpeed );
          else if ( i_section == 0 )
            pGEN->pHvm->phvm->coilQueue.pCoilOrdered[i_coil]->calcul.stages.anticipationTime[i_section] = max( anticipationTemperature, anticipationSpeed );
          else 
            pGEN->pHvm->phvm->coilQueue.pCoilOrdered[i_coil]->calcul.stages.anticipationTime[i_section] = 0.;
        }
        
        if ( pGEN->pHvm->phvm->coilQueue.pCoilOrdered[i_coil]->calcul.stages.anticipationTime[i_section] > 3000. )
        {
          bool tototoottjdsnvlf = false;
        }
      }
    }

  }

  // Apply anticipation time on section calculation.
  long nuCoil    = -1;
  long nuSection = -1;

  // Get coil number inside the NOF: first section of line definition.
  nuSection = 0;
  nuCoil    = pGEN->pHvm->phvm->meaSptCal.lines[0].pSections[nuSection]->pCalcul->tracking.nu_coilInOrdered;
  pGEN->pHvm->phvm->meaSptCal.lines[0].pSections[nuSection]->pCalcul->anticipationTime = pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoil]->calcul.stages.anticipationTime[nuSection];

  // Get coil number inside the RTH: second section of line definition.
  nuSection = 1;
  nuCoil    = pGEN->pHvm->phvm->meaSptCal.lines[0].pSections[nuSection]->pCalcul->tracking.nu_coilInOrdered;
  pGEN->pHvm->phvm->meaSptCal.lines[0].pSections[nuSection]->pCalcul->anticipationTime = pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoil]->calcul.stages.anticipationTime[nuSection];

  // Get coil number inside the RTS: third section of line definition.
  nuSection = 2;
  nuCoil    = pGEN->pHvm->phvm->meaSptCal.lines[0].pSections[nuSection]->pCalcul->tracking.nu_coilInOrdered;
  pGEN->pHvm->phvm->meaSptCal.lines[0].pSections[nuSection]->pCalcul->anticipationTime = pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoil]->calcul.stages.anticipationTime[nuSection];

  //// Get coil number inside the JET: third section of line definition.
  //nuSection = 3;
  //nuCoil    = pGEN->pHvm->phvm->meaSptCal.lines[0].pSections[nuSection]->pCalcul->tracking.nu_coilInOrdered;
  //pGEN->pHvm->phvm->meaSptCal.lines[0].pSections[nuSection]->pCalcul->anticipationTime = pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoil]->calcul.stages.anticipationTime[nuSection];

  return;
}

//============================================================================
// SPE_specific: pyrometerTransfertInOnLive
//
//    Specific ?? calvert
//============================================================================
void SPE_specific::pyrometerTransfertInOnLive
                              ( struct MOD_HV_pointers_section_line *        pSectionMSC   ,
                                struct MOD_HV_coil_calcul_stage_section *    pStageSection
                              )
{
  return;
}

//============================================================================
// SPE_specific: lastActionsBeforeSetpointSending
//
//    Specific last actions before sending all setpoints to communication.
//============================================================================
void SPE_specific::lastActionsBeforeSetpointSending( FILE ** ppTraceFilePointer, struct MOD_HV_TRS_profil * pProfil, struct MOD_HV_TRS_profil * pProfilDeviation, struct GEN_utility *  pGEN )
{
  static long counter = 0;

  if ( !hasSimulationMode || true )
  {
    
    //------------------------------------------------------------------------------------
    // JET cooling intemediate strip temperature.

    
    //// Get current coil ID.
    //long nu_coilOrderedInSection  = pGEN->pHvm->phvm->meaSptCal.lines[0].pSections[1]->pCalcul->tracking.nu_coilInOrdered;
    //
    //
    ////------------------------------------------------------------------------------------
    //// Re work GJC.
    //long nuSection              = 2;
    //long nuSectionGlobal        = pMOD->pConf->pconf->lines[0].pSections[nuSection]->indexGlobal;
    //long nuSectionInType        = pMOD->pConf->pconf->lines[0].pSections[nuSection]->indexType;
    //
    //double averageMaxHeatDemand  = 0.;
    //double averageHeatDemand     = 0.;
    //for ( long iZone = 0; iZone < pMOD->pConf->pconf->sectionsJET[nuSectionInType].nb_zones; iZone++ )
    //{
    //  long    nuZoneGlobal  = pMOD->pConf->pconf->sectionsJET[nuSectionInType].pZones[iZone]->indexGlobal;
    //  long    nuZoneInType  = pMOD->pConf->pconf->sectionsJET[nuSectionInType].pZones[iZone]->indexType;
    //  averageMaxHeatDemand += pGEN->pHvm->phvm->parameter.zones[nuZoneGlobal].maximumHeatDemand;
    //  averageHeatDemand    += pGEN->pHvm->phvm->meaSptCal.zonesJET[nuZoneInType].pSetpoint->demandPercentage;
    //}
    //averageMaxHeatDemand /= ( pMOD->pConf->pconf->sectionsJET[nuSectionInType].nb_zones );
    //averageHeatDemand    /= ( pMOD->pConf->pconf->sectionsJET[nuSectionInType].nb_zones );
    //
    //for ( long iZone = 0; iZone < pMOD->pConf->pconf->sectionsJET[nuSectionInType].nb_zones; iZone++ )
    //{
    //  long    nuZoneGlobal  = pMOD->pConf->pconf->sectionsJET[nuSectionInType].pZones[iZone]->indexGlobal;
    //  long    nuZoneInType  = pMOD->pConf->pconf->sectionsJET[nuSectionInType].pZones[iZone]->indexType;
    //  pGEN->pHvm->phvm->meaSptCal.zonesJET[nuZoneInType].pSetpoint->demandPercentage = averageHeatDemand *  pGEN->pHvm->phvm->parameter.zones[nuZoneGlobal].maximumHeatDemand / 
    //                                                                                                averageMaxHeatDemand;
    //  bool test = false;
    //}
    //
    //// Traction offset.
    //long nuCoilInOrder = pGEN->pHvm->phvm->meaSptCal.lines[0].pSections[0]->pCalcul->tracking.nu_coilInOrdered;
    //if ( nuCoilInOrder >= 0 && nuCoilInOrder < DIM_COILS )
    //{
    //  if ( pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoilInOrder] != nullptr )
    //  {
    //    if      ( pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoilInOrder]->data.thickness >= 1. / 1000. && 
    //              pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoilInOrder]->data.width     <= 0.95 )
	  //        tractionOffset = 10.;                                                                                         
	  //    else if ( pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoilInOrder]->data.thickness <= 0.6 /1000. && 
    //              pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoilInOrder]->data.width     >= 1.4 )
	  //        tractionOffset = -15.; 
	  //    else
	  //        tractionOffset = 0.f;
    //  }
    //}

    
  } // END: If not simulation.

  return;
}

//============================================================================
// SPE_specific: wallThermalLossesHDF
//
//    Specific calculation of wall thermal losses heat flow density.
//============================================================================
double SPE_specific::wallThermalLossesHFD( double wallTemperature, double correctionFactor, double stripWidth )
{
  double  wallThermalLosses = 0.;
  bool    useLinear         = true;

  // Convert wall temperature in celcius.
  wallTemperature = pMOD->pConv->celcius( wallTemperature );

  long nuSectionInGlobal = pMOD->pConf->pconf->lines[0].pSections[0]->indexGlobal;
  double wallThermalLossesPivot = this->wallThermalLosses[nuSectionInGlobal];

  if ( wallThermalLossesPivot < 1. ) wallThermalLossesPivot = 2000.;
  wallThermalLossesPivot = min( max( wallThermalLossesPivot, 100. ), 15000.);

  if ( useLinear )
  {
    //wallThermalLosses = 3500. * ( wallTemperature ) / ( 850. );
    wallThermalLosses = wallThermalLossesPivot * ( wallTemperature ) / ( 850. );
  }
  else if ( wallTemperature < 850 )
  {
    wallThermalLosses = 5000. * ( wallTemperature ) / ( 850. );
  }
  else
  {
    wallThermalLosses = 0.0380952 * pow( wallTemperature, 2. )  - 29.5238095* wallTemperature + 2571.4285714 ;
  }

  if ( correctionFactor > 0. )
    wallThermalLosses *= correctionFactor;

  wallThermalLosses = min( max( wallThermalLosses, 0. ), 40000. );

  return wallThermalLosses;
}

//============================================================================
// SPE_specific: onLiveAllowedToReduceSpeed
//
//    
//============================================================================
bool SPE_specific::onLiveAllowedToReduceSpeed
                    ( double stripThickness
                    )
{
  bool allowed = true;
  //if ( stripThickness < 0.3 / 1000. ) allowed = false;
  return allowed;
}

//============================================================================
// SPE_specific: measurementTreatement
//
//    Specific measurement treatment actions.
//============================================================================
void SPE_specific::measurementTreatement
                                ( FILE ** ppTraceFilePointer, 
                                  bool    hasSimulationMode        // Entry: Has the model a simulation mode? 
                                , struct GEN_utility *  pGEN
                                )
{ 
  char    traceTempo [DIM_TRACES + 1] = {0};
  static double maxZonesPower[DIM_ZONES] = {0};

  static time_t initialTime = 0;

  time_t current_time = 0;
  time(&current_time);
  if ( initialTime < 10 ) time(&initialTime);
  double elapseTime = difftime( current_time, initialTime );

  for ( long iSection = 0; iSection < pMOD->pConf->pconf->nb_sections; iSection++ )
  {
    wallThermalLosses[iSection] = pGEN->pHvm->phvm->parameter.sections[iSection].wallThermalLosses;
  }

  //if ( elapseTime > 120. )
  //{
  ////  pGEN->pHvm->phvm->meaSptCal.measure.sectionsNOF[0].control = true;
  //  pGEN->pHvm->phvm->meaSptCal.measure.sectionsJET[0].control = true;
  //  pGEN->pHvm->phvm->meaSptCal.measure.sectionsJET[1].control = true;
  //}

  //------------------------------------------------------------------------------------
  // Get parameters.
  struct MOD_HV_Parameter   parameter;
  pGEN->pHvm->getParameters( &parameter, false );

  ////------------------------------------------------------------------------------------
  //// For all RTF zones.                                                               RTF measurement treatment.
  //static long counterTestAllBurnersOff = 0;
  //if ( ++counterTestAllBurnersOff > 1000 ) counterTestAllBurnersOff = 500;
  //
  //bool hasNoPower = true;
  //for ( long iZone = 0; iZone < pMOD->pConf->pconf->nb_zonesRTF; iZone++ )
  //{
  //  struct EQP_pointers_zone_RTF *pZone = &pGEN->pHvm->phvm->meaSptCal.zonesRTF[iZone];
  //  long    nuZoneGlobal  = pMOD->pConf->pconf->zonesRTF[iZone].indexGlobal;
  //
  //  // LHV.
  //  double lhv = pMOD->pPhysic->fuelLHV( 0 );
  //
  //  if ( hasSimulationMode )
  //  {
  //    nbBurnerInDefaultRTF[iZone] = 0;
  //  }
  //
  //  // If power exists.
  //  if ( pMOD->pConf->pconf->zonesRTF[iZone].Power > 0. )
  //  {
  //    double maxGasFlow = pMOD->pConf->pconf->zonesRTF[iZone].Power / lhv; 
  //    if ( ( maxGasFlow <= pMOD->pMath->epsilon ) || hasSimulationMode )
  //      maxGasFlow = pMOD->pConf->pconf->zonesRTF[iZone].Power / lhv;
  //    else
  //      maxGasFlow = ( 1. - (double) nbBurnerInDefaultRTF[iZone] / (double) pMOD->pConf->pconf->zonesRTF[iZone].nb_tubes ) * pMOD->pConf->pconf->zonesRTF[iZone].Power / lhv;
  //    maxGasFlow = max( maxGasFlow, 0. );
  //    parameter.zones[nuZoneGlobal].maximumPower = maxGasFlow * lhv;
  //
  //    hasNoPower = hasNoPower && parameter.zones[nuZoneGlobal].maximumPower < pMOD->pMath->epsilon;
  //
  //    // Store number of tube operational.
  //    pZone->pCalcul->nb_operationalBurners = pMOD->pConf->pconf->zonesRTF[iZone].nb_tubes - nbBurnerInDefaultRTF[iZone];
  //  }
  //
  //
  //  // Maximum heat demand.
  //  parameter.zones[nuZoneGlobal].maximumHeatDemand = (double) ( pMOD->pConf->pconf->zonesRTF[iZone].nb_tubes - nbBurnerInDefaultRTF[iZone] ) / (double) pMOD->pConf->pconf->zonesRTF[iZone].nb_tubes * 100.;
  //  
  //  // parameter.zones[nuZoneGlobal].minimumHeatDemand = 0.; to be added in IHM.
  //}
  //
  //// If no power on all zones => set power design.
  //if ( hasNoPower )
  //{
  //  for ( long iZone = 0; iZone < pMOD->pConf->pconf->nb_zonesRTF; iZone++ )
  //  {
  //    long    nuZoneGlobal  = pMOD->pConf->pconf->zonesRTF[iZone].indexGlobal;
  //    parameter.zones[nuZoneGlobal].maximumPower = pMOD->pConf->pconf->zonesRTF[iZone].Power;
  //  }
  //}


  //------------------------------------------------------------------------------------
  // Pyrometers treatement.                                                         PYRO

  //static long counterStart = 0;
  //
  //if ( ++counterStart > 1000 ) counterStart = 500;
  //
  //if ( counterStart > 0 )
  //{
  //  pGEN->pHvm->phvm->meaSptCal.measure.line.speedControl         = true;
  //  pGEN->pHvm->phvm->meaSptCal.sectionsRTF[1].pMeasure->control  = true;
  //  pGEN->pHvm->phvm->meaSptCal.sectionsRTF[2].pMeasure->control  = true;
  //  pGEN->pHvm->phvm->meaSptCal.measure.line.speed                = 50. / 60.;
  //  //pGEN->pHvm->phvm->meaSptCal.measure.coils      [1].thickness  = 1.28 / 1000.;
  //  //pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[1].thickness  = 1.28 / 1000.;
  //}
  
  //------------------------------------------------------------------------------------
  // JET zone operating.                                                      JAT APC FANS Status
  //bool isJETElseAPC = true;
  //long firstSection = -1;
  //for ( long iSection = 0; iSection < pMOD->pConf->pconf->lines[0].nb_sections; iSection++ )
  //{
  //  if ( pMOD->pConf->pconf->lines[0].pSections[iSection]->type == TYPE_JET )
  //  {
  //    if ( firstSection < 0 ) firstSection = iSection;
  //    if ( firstSection == iSection && isJETElseAPC ) isJETElseAPC = true;
  //    else                                            isJETElseAPC = false;

  //    for ( long iZone = 0; iZone < pMOD->pConf->pconf->lines[0].pSections[iSection]->nb_zones; iZone++ )
  //    {
  //      long nuZoneGlobal = pMOD->pConf->pconf->lines[0].pSections[iSection]->pZones[iZone]->indexType;

  //      if ( !this->hasSimulationMode )
  //      {
  //        // Etat ( -1 = panne, 0 = pas actif, 1 = actif )
  //        if ( pGEN->pHvm->phvm->meaSptCal.measure.specific.zones[nuZoneGlobal].etatVentilateur == 1 ) 
  //        {
  //          if ( isJETElseAPC ) specific.zoneOperationJET[iZone] = true;
  //          else                specific.zoneOperationAPC[iZone] = true;
  //        }
  //        else 
  //        {
  //          if ( isJETElseAPC ) specific.zoneOperationJET[iZone] = false;
  //          else                specific.zoneOperationAPC[iZone] = false;
  //        }
  //      }
  //      else
  //      {
  //        if ( pGEN->pHvm->phvm->meaSptCal.measure.line.speed > 20. / 60. )
  //        {
  //          if ( isJETElseAPC ) specific.zoneOperationJET[iZone] = true;
  //          else                specific.zoneOperationAPC[iZone] = true;
  //        }
  //        else
  //        {
  //          if ( isJETElseAPC ) specific.zoneOperationJET[iZone] = false;
  //          else                specific.zoneOperationAPC[iZone] = false;
  //        }
  //      }
  //    }
  //  }
  //}
  
  

  
  if ( this->hasSimulationMode )
  {
    for ( long i = 0; i < 3; i++ )
    {
      this->zoneOperationJET[i] = true;
    }

    // For all JET zones.
    for ( long iZone = 0; iZone < pMOD->pConf->pconf->nb_zonesJET; iZone++ )
    {
      long    nuZoneGlobal  = pMOD->pConf->pconf->zonesJET[iZone].indexGlobal;

      // Maximum and minimum cool demand.
      parameter.zones[nuZoneGlobal].maximumHeatDemand = 100.;   // xls file.
      parameter.zones[nuZoneGlobal].minimumHeatDemand =   0.;
    }
  }

  else
  {
    // For all JET zones.
    for ( long iZone = 0; iZone < pMOD->pConf->pconf->nb_zonesJET; iZone++ )
    {
      long    nuZoneGlobal  = pMOD->pConf->pconf->zonesJET[iZone].indexGlobal;

      // Maximum and minimum cool demand.
      parameter.zones[nuZoneGlobal].maximumHeatDemand = 100.;
      parameter.zones[nuZoneGlobal].minimumHeatDemand = 0.;
    }
  }
 
  
  //----------------------------------------------------------------------------
  // Blowing temperature for JET sections.
  // For all JET zones.
  for ( long iSection = 0; iSection < pMOD->pConf->pconf->nb_sectionsJET; iSection++ )
  {
    for ( long iZone = 0; iZone < pMOD->pConf->pconf->sectionsJET[iSection].nb_zones; iZone++ )
    {
      long nuZoneInType   = pGEN->pHvm->phvm->meaSptCal.sectionsJET[iSection].pZones[iZone]->pConfig->indexType;
      long nuZoneInGlobal = pGEN->pHvm->phvm->meaSptCal.sectionsJET[iSection].pZones[iZone]->pConfig->indexGlobal;

      // Set full damper opening.
      parameter.zones[nuZoneInGlobal].maximumTemperature = 0.;

      // JET.
      if      ( iSection == 0 )
      {
        parameter.zones[nuZoneInGlobal].jetBlowingTemperature = pMOD->pConv->kelvin( 90. );
      }
    }
  }
  
  //----------------------------------------------------------------------------
  // Hydrogen rate for JET.
  pGEN->pHvm->phvm->meaSptCal.sectionsJET[0].pMeasure->h2Rate = 5.;

  //// Section control.
  //if ( !hasSimulationMode )
  //{
  //  for ( long i_section = 0; i_section < pMOD->pConf->pconf->lines[0].nb_sections; i_section++ )
  //  {
  //    pGEN->pHvm->phvm->meaSptCal.lines[0].pSections[i_section]->pMeasure->control = ( pGEN->pHvm->phvm->meaSptCal.lines[0].pSections[i_section]->pMeasure->control && 
  //                                                                                    this->computerNumber == pGEN->pHvm->phvm->meaSptCal.measure.serverConnected );
  //  }
  //  
  //  // Speed control.
  //  pGEN->pHvm->phvm->meaSptCal.lines[0].pMeasure->speedControl = ( pGEN->pHvm->phvm->meaSptCal.lines[0].pMeasure->speedControl && 
  //                                                                this->computerNumber == pGEN->pHvm->phvm->meaSptCal.measure.serverConnected );
  //}

  // RTS control is equal to RTH. We don't have separate boolean in communication. No SOAKING
  //pGEN->pHvm->phvm->meaSptCal.lines[0].pSections[2]->pMeasure->control = pGEN->pHvm->phvm->meaSptCal.lines[0].pSections[1]->pMeasure->control;

  // RTH control is equal to NOF. We don't have separate boolean in communication.
  //pGEN->pHvm->phvm->meaSptCal.lines[0].pSections[0]->pMeasure->control = pGEN->pHvm->phvm->meaSptCal.lines[0].pSections[1]->pMeasure->control;

  //------------------------------------------------------------------------------------
  // Store parameters.
  pGEN->pHvm->writeParameters( &parameter, false );
  

  return;
}

//============================================================================
// SPE_specific: simulationTreatment
//
//    Specific simulation treatment actions.
//============================================================================
void SPE_specific::simulationTreatment( struct GEN_utility *  pGEN )
{
  //// NOF control is equal to RTH
  //pGEN->pHvm->phvm->simulation.PLCMeasurement.sectionsRTF[1].control = pGEN->pHvm->phvm->simulation.PLCMeasurement.sectionsNOF[0].control;

  //// SOA control is equal to RTH
  //pGEN->pHvm->phvm->simulation.PLCMeasurement.sectionsRTF[2].control = pGEN->pHvm->phvm->simulation.PLCMeasurement.sectionsRTF[1].control;
  

  return;
}

//============================================================================
// SPE_specific: simulationTreatmentOnLinePLCRecieve
//
//    Specific simulation treatment actions on reception of a line PLC message.
//============================================================================
void SPE_specific::simulationTreatmentOnLinePLCRecieve( )
{ }

//============================================================================
// SPE_specific: simulationTreatmentOnFurnacePLCRecieve
//
//    Specific simulation treatment actions on reception of PLC message setpoint.
//============================================================================
void SPE_specific::simulationTreatmentOnFurnacePLCRecieve( struct GEN_utility *  pGEN )
{
  //// SOA validity is equal to RTF. SOA validity does not exist in tcp messages.
  //pGEN->pHvm->phvm->simulation.modelSetpoints.sectionsRTF[2].valid        = pGEN->pHvm->phvm->simulation.modelSetpoints.sectionsRTF[1].valid;
  //pGEN->pHvm->phvm->simulation.modelSetpoints.sectionsRTF[2].controlType  = pGEN->pHvm->phvm->simulation.modelSetpoints.sectionsRTF[1].controlType;

  return;
}

//============================================================================
// SPE_specific: simulationCoilsCharacteristics
//
//    Realize some specific coils characteristics selection.
//============================================================================
void SPE_specific::simulationCoilsCharacteristics( struct GEN_utility *  pGEN )
{
  pGEN->pHvm->phvm->simulation.PLCMeasurement.coils[DIM_COILS - 1].thickness = 
    min ( max( pGEN->pHvm->phvm->simulation.PLCMeasurement.coils[DIM_COILS - 1].thickness, 0.5/1000. ), 2.3/1000. );

  pGEN->pHvm->phvm->simulation.PLCMeasurement.coils[DIM_COILS - 1].width = 
    min ( max( pGEN->pHvm->phvm->simulation.PLCMeasurement.coils[DIM_COILS - 1].width, 0.85 ), 1.87 );

  
  pGEN->pHvm->phvm->simulation.PLCMeasurement.coils[DIM_COILS - 1].maxSpeedQuality  = 600. / 60.;

  return;
}

//============================================================================
// SPE_specific: simulationInitialization
//
//    Specific simulation initialization.
//============================================================================
void SPE_specific::simulationInitialization( struct GEN_utility *  pGEN )
{
  //------------------------------------------------------------------------------------
  // Coil queue.
  for ( long i_coil = 0; i_coil < DIM_COILS; i_coil++ )
  {
    pGEN->pHvm->phvm->simulation.PLCMeasurement.coils[i_coil].heatCycle        = 7600;
    pGEN->pHvm->phvm->simulation.PLCMeasurement.coils[i_coil].maxSpeedQuality  = 220. / 60.;
    pGEN->pHvm->phvm->simulation.PLCMeasurement.coils[i_coil].thickness        = ( 0.6 + ( double ) i_coil / 20. ) / 1000.;
    pGEN->pHvm->phvm->simulation.PLCMeasurement.coils[i_coil].thickness        = 1.0 / 1000.;
    pGEN->pHvm->phvm->simulation.PLCMeasurement.coils[i_coil].width            = 1.25;
    //pGEN->pHvm->phvm->simulation.PLCMeasurement.coils[i_coil].width            = 1.05;
    pGEN->pHvm->phvm->simulation.PLCMeasurement.coils[i_coil].bisraCode        = 9;

    // For all sections.
    for ( long i_section = 0; i_section < pMOD->pConf->pconf->nb_sections; i_section++ )
    {
      // RTF / SOA.
      if      ( pMOD->pConf->pconf->pSections[i_section]->type == TYPE_RTF ||
                pMOD->pConf->pconf->pSections[i_section]->type == TYPE_SOA
              )
      {
        pGEN->pHvm->phvm->simulation.PLCMeasurement.coils[i_coil].sections[i_section].stripTemp.upper  = pMOD->pConv->kelvin( 780. );
        pGEN->pHvm->phvm->simulation.PLCMeasurement.coils[i_coil].sections[i_section].stripTemp.target = pMOD->pConv->kelvin( 750. );
        pGEN->pHvm->phvm->simulation.PLCMeasurement.coils[i_coil].sections[i_section].stripTemp.lower  = pMOD->pConv->kelvin( 720. );
      }
      // NOF.
      else if ( pMOD->pConf->pconf->pSections[i_section]->type == TYPE_NOF
              )
      {
        pGEN->pHvm->phvm->simulation.PLCMeasurement.coils[i_coil].sections[i_section].stripTemp.upper  = pMOD->pConv->kelvin( 720. );
        pGEN->pHvm->phvm->simulation.PLCMeasurement.coils[i_coil].sections[i_section].stripTemp.target = pMOD->pConv->kelvin( 602. );
        pGEN->pHvm->phvm->simulation.PLCMeasurement.coils[i_coil].sections[i_section].stripTemp.lower  = pMOD->pConv->kelvin( 600. );
      }
      // JET cooling.
      else if ( pMOD->pConf->pconf->pSections[i_section]->type == TYPE_JET
              )
      {
        pGEN->pHvm->phvm->simulation.PLCMeasurement.coils[i_coil].sections[i_section].stripTemp.upper  = pMOD->pConv->kelvin( 460.     );
        pGEN->pHvm->phvm->simulation.PLCMeasurement.coils[i_coil].sections[i_section].stripTemp.target = pMOD->pConv->kelvin( 460. - 5 );
        pGEN->pHvm->phvm->simulation.PLCMeasurement.coils[i_coil].sections[i_section].stripTemp.lower  = pMOD->pConv->kelvin( 460. + 5 );
      }
      // LYR.
      else if ( pMOD->pConf->pconf->pSections[i_section]->type == TYPE_LYRE
              )
      {
        pGEN->pHvm->phvm->simulation.PLCMeasurement.coils[i_coil].sections[i_section].stripTemp.upper  = pMOD->pConv->kelvin( 570. );
        pGEN->pHvm->phvm->simulation.PLCMeasurement.coils[i_coil].sections[i_section].stripTemp.target = pMOD->pConv->kelvin( 550. );
        pGEN->pHvm->phvm->simulation.PLCMeasurement.coils[i_coil].sections[i_section].stripTemp.lower  = pMOD->pConv->kelvin( 520. );
      }
      // BASE.
      else if ( pMOD->pConf->pconf->pSections[i_section]->type == TYPE_BASE
              )
      {
        pGEN->pHvm->phvm->simulation.PLCMeasurement.coils[i_coil].sections[i_section].stripTemp.upper  = pMOD->pConv->kelvin( 780. );
        pGEN->pHvm->phvm->simulation.PLCMeasurement.coils[i_coil].sections[i_section].stripTemp.target = pMOD->pConv->kelvin( 750. );
        pGEN->pHvm->phvm->simulation.PLCMeasurement.coils[i_coil].sections[i_section].stripTemp.lower  = pMOD->pConv->kelvin( 720. );
      }
    }

    if      ( i_coil == 0 )
    {
      pGEN->pHvm->phvm->simulation.PLCMeasurement.coils[i_coil].heatCycle        = 7500;
    }
    else if ( i_coil == 1 )
    {
      pGEN->pHvm->phvm->simulation.PLCMeasurement.coils[i_coil].heatCycle        = 7600;
    }
  }

  //------------------------------------------------------------------------------------
  // Line speed and control.
  pGEN->pHvm->phvm->simulation.PLCMeasurement.line.speed        = 80. / 60.;
  //pGEN->pHvm->phvm->simulation.PLCMeasurement.line.speed        = 60. / 60.;
  pGEN->pHvm->phvm->simulation.PLCMeasurement.line.speedControl = false;

  // Operator speed limit.
  pGEN->pHvm->phvm->simulation.PLCMeasurement.line.operatorSpeedLimit = 220. / 60.;

  return;
}

//============================================================================
// SPE_specific: onLiveManagementBasicSection
//
//    Realize some specific on live management for basic section.
//============================================================================
void SPE_specific::onLiveManagementBasicSection
                                ( long     nuSectionInLine     // Entry: Section index in Line definition.
                                , double   timeSpan            // Entry: Time span since last management.
                                , struct GEN_utility *  pGEN
                                )
{
  struct MOD_HV_pointers_line *           pLineMSC    = &pGEN->pHvm->phvm->meaSptCal.lines[0];
  struct MOD_HV_pointers_section_line  *  pSectionMSC = pLineMSC->pSections[nuSectionInLine];
  long                                    nu_coil     = pSectionMSC->pCalcul->tracking.nu_coilInOrdered;

  // Store strip temperature targeted.
  for ( long iPyrometer = 0; iPyrometer < pSectionMSC->pConfig->nb_pyrometers; iPyrometer++ )
  {
    pSectionMSC->pPyrometers[iPyrometer]->pSetpoint->temperature = pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nu_coil]->data.sections[nuSectionInLine].stripTemp.target;
  }

  // Get coil number inside the Soaking.
  long nuSection  = 1;
  long nuCoil     = pGEN->pHvm->phvm->meaSptCal.lines[0].pSections[nuSection]->pCalcul->tracking.nu_coilInOrdered;

  if ( pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoil] == nullptr ) return;

  double zoneTemperatureOffset = 20.;

  pidSoakingStripTemp.entry.processValue   = pGEN->pHvm->phvm->meaSptCal.lines[0].pSections[nuSection]->pPyrometers[0]->pMeasure ->temperature;
  pidSoakingStripTemp.entry.setPointValue  = pGEN->pHvm->phvm->meaSptCal.lines[0].pSections[nuSection]->pPyrometers[0]->pSetpoint->temperature;
  pidSoakingStripTemp.data.Kp              =    5.  ; 
  pidSoakingStripTemp.data.Ki              =    60. ; 
  pidSoakingStripTemp.data.maximumExit     =    150.;
  pidSoakingStripTemp.data.minimumExit     =   -150.;
  pidSoakingStripTemp.data.deadBand        =    0.5;
  pidSoakingStripTemp.functions.automatic  =   true;

  if ( pGEN->pHvm->phvm->meaSptCal.lines[0].pSections[nuSection]->pMeasure->control )
  {
    pMOD->pPid->calculate( &pidSoakingStripTemp      // Modified: pid objects structure.
                  );
    
    soakingTemperatureSetpoint = pidSoakingStripTemp.entry.setPointValue + zoneTemperatureOffset + pidSoakingStripTemp.exit.pid;
  }
  else
  {
    pidSoakingStripTemp.functions.automatic   = false;
    pidSoakingStripTemp.manual.modelValue     = pidSoakingStripTemp.entry.setPointValue + zoneTemperatureOffset;
    pidSoakingStripTemp.manual.currentValue   = soakingTemperatureMeasurement;
    pMOD->pPid->calculate( &pidSoakingStripTemp      // Modified: pid objects structure.
                  );

    soakingTemperatureSetpoint = soakingTemperatureMeasurement;
  }

  return;
}

//============================================================================
// SPE_specific: dynamicModelParameterInitialisation
//
//    
//============================================================================
void SPE_specific::dynamicModelParameterInitialisation
                                ( long                                                    nuLine,
                                  struct  MOD_HV_TRS_In_specific_parameters      *        pDynSpecific      // Modified  : Structure of specific dynamic data.
                                )
{
  //if      ( nuLine == 0 )
  //{
  //  pDynSpecific->rampUp[0] = 5. ;    // DFF.
  //  pDynSpecific->rampDw[0] = 5. ;    // DFF.
  //  pDynSpecific->rampUp[1] = 10.;    // RTF.
  //  pDynSpecific->rampDw[1] = 10.;    // RTF.
  //}
  //else if ( nuLine == 1 )
  //{
  //  pDynSpecific->rampUp[0] = 5. ;    // DFF.
  //  pDynSpecific->rampDw[0] = 5. ;    // DFF.
  //  pDynSpecific->rampUp[1] = 10.;    // RTF.
  //  pDynSpecific->rampDw[1] = 10.;    // RTF.
  //}
  //else if ( nuLine == 2 )
  //{
  //}
  return;
}

//============================================================================
// SPE_specific: onLiveControlParameterInitialisation
//
//    
//============================================================================
void SPE_specific::onLiveControlParameterInitialisation
                                ( struct  SPE_specific_on_live_parameters *       parameters
                                )
{
  parameters->PERCENTAGE_RED_SPEED_TRS     = 30.;       // 30. alabama
  parameters->PREDICTED_SECTION_TEMP_TIME  = 30.;       // 30.
  return;
}

//============================================================================
// SPE_specific: stripRollExchangeCoefficient
//
//    Provide a specific coefficient for strip roll thermal exchange for dynamic calculation.
//============================================================================
double SPE_specific::stripRollExchangeCoefficient( )
{
  // Alabama : 0.85

  return( 1.0 );
}

//============================================================================
// SPE_specific: jetOnLiveControl
//
//    Realized specific JET on live control.
//
//
//    On dispose du pyromètre en coin P16  en sortie du Top roll 1 
//    
//    Fonctionnement actuel en mode pyro .
//    Le P16 pilote les 6 ventilateurs en mode parallèle 
//    L’opérateur applique une contrainte mini sur les 2 ventilateurs du bas afin de figer le revêtement afin qu’il entre dans les 4 ventilateurs supérieurs
//    
//    EN ZINC    Contrainte MINI est a 40 % sur tous les formats
//    EN ALU   Contrainte MINI a  70 % sur épaisseur < 1 mm  et 80 % >= 1 mm 
//    
//    Je souhaite que le modèle pilote les 2 ventilateurs  pour avoir les températures suivantes au point 1 
//    580 degrés en ALUSI
//    420 degrés en GI
//    Je souhaite garder une contrainte MINI de 40 % sur tous les produite , sur tous les formats  
//============================================================================
bool SPE_specific::jetOnLiveControl
                                ( long                                           nuSectionInType, 
                                  struct UTL_pid_objects *                       pPidJET, 
                                  struct MOD_HV_pointers_section_line *          pSectionMSC, 
                                  struct MOD_HV_coil_calcul_stage_section *      pStageSection,
                                  double *                                       pHD,
                                  struct MOD_HV_TRS_profil *                     pProfil,
                                  double *                                       coolDemandMeasure )
{
  bool hasSpecificControl = false;
  long nuSectionGlobal    = pSectionMSC->pConfig->indexGlobal;

  // GJC.
  if      ( nuSectionInType == 0 )
  {
  }

  // APC Up pass.
  else if ( nuSectionInType == 1 )
  {
  }

  // APC Down pass.
  else if ( nuSectionInType == 2 )
  {
    hasSpecificControl = false;
  }

  return( !hasSpecificControl );
}
                                
//============================================================================
// SPE_specific: jetSaturationCalculation
//
//    Determine if each zone are saturated or not with specific zone control.
//
//
//============================================================================
bool SPE_specific::jetSaturationCalculation ( long                                           nuSectionInType, 
                                              struct UTL_pid_objects *                       pPidJET, 
                                              struct MOD_HV_pointers_section_line *          pSectionMSC, 
                                              struct MOD_HV_coil_calcul_stage_section *      pStageSection,
                                              double *                                       pHD,
                                              struct MOD_HV_TRS_profil *                     pProfil,
                                              double *                                       coolDemandMeasure,
                                              bool                                           &isSaturatedMax,
                                              bool                                           &isSaturatedMin
                                              )
{
  bool hasSpecificControl = false;
  long nuSectionGlobal    = pSectionMSC->pConfig->indexGlobal;

  // GJC.
  if      ( nuSectionInType == 0 )
  {
    
  }
  // APC Up pass.
  else if ( nuSectionInType == 1 )
  {
    //// Store parameters.
    //struct MOD_HV_Parameter   parameter;
    //pGEN->pHvm->getParameters( &parameter, true );

    //hasSpecificControl = true;

    //isSaturatedMax = true;
    //isSaturatedMin = true;
   
    //for ( long i_zone = 2; i_zone < pSectionMSC->pConfig->nb_zones; i_zone++ )
    //{
    //  struct EQP_pointers_zone_JET *pZoneMSC = ( struct EQP_pointers_zone_JET * ) pSectionMSC->pZones[i_zone];
    //  long nuZoneGlobal = pSectionMSC->pZones[i_zone]->pConfig->indexGlobal;

    //  // Check maximum saturation.
    //  isSaturatedMax &= coolDemandMeasure[i_zone] >= pGEN->pHvm->phvm->parameter.zones[nuZoneGlobal].maximumHeatDemand - 0.5;

    //  // Check minimum saturation.
    //  isSaturatedMin &= coolDemandMeasure[i_zone] <= pGEN->pHvm->phvm->parameter.zones[nuZoneGlobal].minimumHeatDemand + 0.5;
    //}
  }

  // APC Down pass.
  else if ( nuSectionInType == 2 )
  {
    hasSpecificControl = false;
  }

  return( !hasSpecificControl );
}

//============================================================================
// SPE_specific: jetOnLiveNotUnderControl
//
//    Realized specific JET on live control.
//============================================================================
bool SPE_specific::jetOnLiveNotUnderControl
                                ( long                           nuSectionInType, 
                                  double                         cv[], 
                                  double                         modelSp[], 
                                  struct UTL_pid_objects *       pPidJET,
                                  struct MOD_HV_TRS_profil *     pProfil )
{
  bool hasSpecificControl = false;

  // APC Up pass.
  if ( nuSectionInType == 1 )
  {

  }

  return( !hasSpecificControl );
}

//============================================================================
// SPE_specific: staticModelInputLimitation
//
//    Limit model input for static model.
//============================================================================
bool SPE_specific::staticModelInputLimitation
                                ( double &modelInputOnZones, double maximumTemperature, double thickness, double width )
{
  bool hasSpecificLimitation = false;

  //if      ( thickness > 0.7 / 1000. )
  //  modelInputOnZones = min( modelInputOnZones, maximumTemperature - 25. );
  //else if ( thickness > 0.35 / 1000. )
  //  modelInputOnZones = min( modelInputOnZones, maximumTemperature - 25. );
  //else
  //  modelInputOnZones = min( modelInputOnZones, maximumTemperature - 20. );
  
  return( hasSpecificLimitation );
}

//============================================================================
// SPE_specific: staticModelInputJET
//
//    Limit model input for static model.
//============================================================================
bool SPE_specific::staticModelInput   
                                ( struct struct_section  *pModel, double * modelInputOnZones, double refparameterModelInput
                                , double    firstWidth                  // Entry: First  coil width.
                                , double    firstThickness              // Entry: First  coil thickness.
                                , double    secondWidth                 // Entry: Second coil width     (transient situation).
                                , double    secondThickness             // Entry: Second coil thickness (transient situation). 
                                )
{
  bool hasSpecificLimitation = false;


  return( hasSpecificLimitation );
}

//============================================================================
// SPE_specific: getPotTemperature
//
//    Get the current pot temperature.
//============================================================================
double SPE_specific::getPotTemperature(  )
{
  double potTemperature = pMOD->pConv->kelvin( 460. );

  return( potTemperature );
}

//============================================================================
// SPE_specific: changeJETConfiguration
//
//    We change JET configuration dynamically. Possibility.
//============================================================================
void SPE_specific::changeJETConfiguration( struct zone_jet *  pZoneJet, struct JET_model * pEczJET, long nuZone, long nuECZ )
{
  if ( !this->hasSimulationMode )
  {
    
  }
  
  return;
}

//============================================================================
// SPE_specific: dffZoneMaximumPower
//
//    Provide the dff maximum power for the zone.
//============================================================================
bool SPE_specific::dffZoneMaximumPower( long nuSectionInLine, long nuZone, double width, double &maximumPower )
{
  bool providedCalculation = false;

   
  return providedCalculation;
}

//============================================================================
// SPE_specific: dffZoneFixedHD
//
//    Provide the dff fixed HD for the zone.
//============================================================================
bool SPE_specific::dffZoneFixedHD( long nuSectionInLine, long nuZone, double refparameterModelInput, double &FixedHD, bool &Imposed )
{
  bool providedCalculation = false;
   
  return providedCalculation;
}

//============================================================================
// SPE_specific: RTFwallThermalLossesHFD
//
//    Specific calculation of wall thermal losses heat flow density.
//============================================================================
double SPE_specific::RTFwallThermalLossesHFD( long nuSectionInGlobal, double wallTemperature, double correctionFactor, bool isSideWall )
{
  double  wallThermalLosses = 0.;
  bool    useLinear         = false;

  // Convert wall temperature in celcius.
  wallTemperature = pMOD->pConv->celcius( wallTemperature );

  double wallThermalLossesPivot = this->wallThermalLosses[nuSectionInGlobal];
  wallThermalLossesPivot      = min( max( wallThermalLossesPivot, 1000. ), 50000. );
  wallThermalLosses           = wallThermalLossesPivot * ( wallTemperature ) / ( 850. );
  wallThermalLosses           = min( max( wallThermalLosses, 0. ), 50000. );
  
  return wallThermalLosses;
}

//============================================================================
// SPE_specific: DFFSetpointCorrection
//
//    For Galsa 2. Recorrection of DFF zones heat demand setpoint.
//============================================================================
void SPE_specific::DFFSetpointCorrection( double &heatDemand, long nuZone )
{
  /*long    nuZoneGlobal  = pMOD->pConf->pconf->zonesNOF[nuZone].indexGlobal;
  if ( pGEN->pHvm->phvm->parameter.zones[nuZoneGlobal].maximumPower > 0. && pMOD->pConf->pconf->zonesNOF[nuZone].Power > 0. )
  {
    heatDemand = heatDemand * pGEN->pHvm->phvm->parameter.zones[nuZoneGlobal].maximumPower / pMOD->pConf->pconf->zonesNOF[nuZone].Power;
  }*/

  return;
}

//============================================================================
// SPE_specific: copyHeatCycle
//
//    For Galsa 2. Recorrection of DFF zones heat demand setpoint.
//============================================================================
void SPE_specific::copyHeatCycle( void * pPointer )
{
  //this->lockAccess  ( this->HandleMutexHeatCycle );
  //memcpy( pPointer, this->heatCyclesSpeedUsed, sizeof( heat_cycle_speed ) * DIM_HEAT_CYCLES_SPEED );
  //this->unLockAccess( this->HandleMutexHeatCycle );

  return;
}

//============================================================================
// SPE_specific: storeHeatCycleInSpecific
//
//    Specific store of heat cycle in specific. Used for HMI for business using heat cycle depending of line speed.
//============================================================================
void SPE_specific::storeHeatCycleInSpecific( )
{
  //------------------------------------------------------------------------------------
  // Build thermal cycle to be used with the offset.
  //memcpy( heatCyclesSpeedUsed, pGEN->pHvm->phvm->parameter.lineCycle[0].shared.heatCyclesSpeed, sizeof( heat_cycle_speed ) * DIM_HEAT_CYCLES_SPEED );

  return;
}

//============================================================================
// SPE_specific: errorOnRTFCorrection
//
//    
//============================================================================
void SPE_specific::errorOnRTFCorrection( long nuSectionInLine, double &errorOnPyrometerExit, bool control, struct GEN_utility *  pGEN )
{
  if ( nuSectionInLine == 0 )
  {
    pidErrorRTF.entry.processValue   = pGEN->pHvm->phvm->meaSptCal.lines[0].pSections[0]->pPyrometers[0]->pMeasure ->temperature;
    pidErrorRTF.entry.setPointValue  = pGEN->pHvm->phvm->meaSptCal.lines[0].pSections[0]->pPyrometers[0]->pSetpoint->temperature;
    pidErrorRTF.data.Kp              =   0.  ; 
    pidErrorRTF.data.Ki              =    80.; 
    pidErrorRTF.data.maximumExit     =    10.;
    pidErrorRTF.data.minimumExit     =   -10.;
    pidErrorRTF.data.deadBand        =    0.5;
    pidErrorRTF.functions.automatic  =   true;

    if ( control && abs( pidErrorRTF.entry.processValue - pidErrorRTF.entry.setPointValue ) < 10. )
      pMOD->pPid->calculate( &pidErrorRTF      // Modified: pid objects structure.
                   );

    //errorOnPyrometerExit -= min( max( pidErrorRTF.exit.pid, -10.), 10. );
  }
  //else if ( nuSectionInLine == 1 )
  //{
  //  static double setPointValueLast  = 0.;
  //  pidErrorRTS.entry.processValue   = pGEN->pHvm->phvm->meaSptCal.lines[0].pSections[2]->pPyrometers[0]->pMeasure ->temperature;
  //  pidErrorRTS.entry.setPointValue  = pGEN->pHvm->phvm->meaSptCal.lines[0].pSections[2]->pPyrometers[0]->pSetpoint->temperature;
  //  pidErrorRTS.data.Kp              =   0.  ; 
  //  pidErrorRTS.data.Ki              =    80.; 
  //  pidErrorRTS.data.maximumExit     =    10.;
  //  pidErrorRTS.data.minimumExit     =   -10.;
  //  pidErrorRTS.data.deadBand        =    0.5;
  //  pidErrorRTS.functions.automatic  =   true;

  //  if ( abs( setPointValueLast - pidErrorRTS.entry.setPointValue ) > 20. )
  //  pidErrorRTS.exit.integral = 0.;
  //  pidErrorRTS.exit.pid      = 0.;

  //  if ( control && abs( pidErrorRTS.entry.processValue - pidErrorRTS.entry.setPointValue ) < 10. )
  //    pMOD->pPid->calculate( &pidErrorRTS      // Modified: pid objects structure.
  //                 );


  //  errorOnPyrometerExit -= min( max( pidErrorRTS.exit.pid, -10.), 10. );
  //  setPointValueLast = pidErrorRTS.entry.setPointValue;
  //}

  return;
}

//============================================================================
// SPE_specific: rulesForSelectingHangingThBack
//
//    
//============================================================================
bool SPE_specific::rulesForSelectingHangingThBack
                              ( long nuZoneRTFGlobal, 
                                long nuTubeInZone, 
                                long nbTubeInZone, 
                                long nuHangingIndex, 
                                long &nuHangingTubeIndexInZone, 
                                long &nuHangingTubeIndexGlobal )
{

  return true;
}


//============================================================================
// SPE_specific: readSpecificCommunication
//
//    We read specifically the measurement. Replacement regular communication.
//============================================================================
//void SPE_specific::readSpecificCommunication               ( FILE **traceFilePointer, struct MOD_HV_measures *    pMeasures, struct GEN_utility *  pGEN, bool isComExe, long lType )
void SPE_specific::readSpecificCommunication               ( FILE **traceFilePointer, void *    pMeasures, struct GEN_utility *  pGEN, bool isComExe, long lType )
{
  char traceTempo [DIM_TRACES + 1] = {0};

 // // Secure access to global section.
	//gblSec.lockAccess();
  
  if      ( lType == TYPE_MSG_STATIC_SIMUL_CALC )
  {
    //struct MOD_HV_coil_steady_simulation *   pStage = ( struct MOD_HV_coil_steady_simulation * ) pMeasures;
    struct MOD_HV_measures *   pMeasure = ( struct MOD_HV_measures * ) pMeasures;

    if ( this->lScenarioIDCurrent > 0 )
    {
      pScenarioDataInput->coils[0]  = pMeasure->coils[0];
      pScenarioDataInput->coils[1]  = pMeasure->coils[1];
      pScenarioDataInput->line      = pMeasure->line;
      bIsScenarioCalculationAUthorized = false;
    }


  }
  else if ( lType == TYPE_MSG_MEASUREMENT )
  {
    struct MOD_HV_measures *   pMeasure = ( struct MOD_HV_measures * ) pMeasures;
  }
	//// Release access to global section.
	//gblSec.unLockAccess();


  return;
}

  
//============================================================================
// SPE_specific: writeSpecificCommunication
//
//    We write specifically the setpoint. Replacement regular communication.
//============================================================================
//void SPE_specific::writeSpecificCommunication              ( FILE **traceFilePointer, struct MOD_HV_setpoints *   pSetpoints, struct GEN_utility *  pGEN, bool isComExe, long lType )
void SPE_specific::writeSpecificCommunication              ( FILE **traceFilePointer, void *   pSetpoints, struct GEN_utility *  pGEN, bool isComExe, long lType )
{
  char traceTempo [DIM_TRACES + 1] = {0};

 // // Secure access to global section.
	//gblSec.lockAccess();
  
  if      ( lType == TYPE_MSG_STATIC_SIMUL_CALC )
  {
    struct MOD_HV_setpoints *   pSetpoint = ( struct MOD_HV_setpoints * ) pSetpoints;
    pSetpoint->nb_messageToSendBack                 = 1;
    pSetpoint->messageIdToSendBack[0]               = 5103;
    pSetpoint->stageSimulationResults.lScenarioID   = this->lScenarioIDCurrent;
  }
  else if ( lType == TYPE_MSG_SETPOINT )
  {
    struct MOD_HV_setpoints *   pSetpoint = ( struct MOD_HV_setpoints * ) pSetpoints;


    // Store specific consumption.
    this->specificConsumption[0] = pSetpoint->sectionsNOF[0].specificConsumption;
    this->specificConsumption[1] = pSetpoint->sectionsRTF[0].specificConsumption;
    this->efficiency         [0] = pSetpoint->sectionsNOF[0].efficiency;
    this->efficiency         [1] = pSetpoint->sectionsRTF[0].efficiency;
  }
	//// Release access to global section.
	//gblSec.unLockAccess();

  

  bool tototoott = true;

  return;
}

//============================================================================
// SPE_specific: calculateStripEntryTemperature
//
//    Calculate strip entry temperature in case of Gas JET preheating section.
//============================================================================
double SPE_specific::calculateStripEntryTemperature( double * zoneHD, bool isHDElseTubeTemp )
{
  double stripEntryTemperature = pMOD->pConv->kelvin( 20. );

  // Using HD.
  if ( isHDElseTubeTemp )
  {

    double avgHD = 0.;
    double coeff = 0.;
    for ( long iZone = 0; iZone < 8; iZone++ )
    {
      if      ( iZone == 0 ) { avgHD += zoneHD[iZone] * 26.; coeff += 26.; }
      else if ( iZone == 1 ) { avgHD += zoneHD[iZone] * 22.; coeff += 22.; }
      else if ( iZone == 2 ) { avgHD += zoneHD[iZone] * 18.; coeff += 18.; }
      else if ( iZone == 3 ) { avgHD += zoneHD[iZone] * 14.; coeff += 14.; }
      else if ( iZone == 4 ) { avgHD += zoneHD[iZone] * 20.; coeff += 20.; }
      else if ( iZone == 5 ) { avgHD += zoneHD[iZone] * 16.; coeff += 16.; }
      else if ( iZone == 6 ) { avgHD += zoneHD[iZone] * 17.; coeff += 17.; }
      else if ( iZone == 7 ) { avgHD += zoneHD[iZone] * 14.; coeff += 14.; }
    }

    avgHD /= coeff;
    avgHD = min( max( avgHD, 5. ), 60. );

    stripEntryTemperature =  ( avgHD - 5. ) / ( 60. - 5. ) * 40. + pMOD->pConv->kelvin( 30. );
  }

  // Using tube temp.
  else
  {

    /*double avgTemp = 0.;
    double coeff = 0.;
    for ( long iZone = 0; iZone < 8; iZone++ )
    {
      if      ( iZone == 0 ) { avgTemp += zoneHD[iZone] * 26. * 2. ; coeff += 26. * 2. ; }
      else if ( iZone == 1 ) { avgTemp += zoneHD[iZone] * 22. * 2. ; coeff += 22. * 2. ; }
      else if ( iZone == 2 ) { avgTemp += zoneHD[iZone] * 18. * 1.5; coeff += 18. * 1.5; }
      else if ( iZone == 3 ) { avgTemp += zoneHD[iZone] * 14. * 1.5; coeff += 14. * 1.5; }
      else if ( iZone == 4 ) { avgTemp += zoneHD[iZone] * 20. * 1. ; coeff += 20. * 1. ; }
      else if ( iZone == 5 ) { avgTemp += zoneHD[iZone] * 16. * 1. ; coeff += 16. * 1. ; }
      else if ( iZone == 6 ) { avgTemp += zoneHD[iZone] * 17. * 0.7; coeff += 17. * 0.7; }
      else if ( iZone == 7 ) { avgTemp += zoneHD[iZone] * 14. * 0.7; coeff += 14. * 0.7; }
    }

    avgTemp /= coeff;
    avgTemp = min( max( avgTemp, 600. ), 960. );

    stripEntryTemperature =  ( avgTemp - 600. ) / ( 960. - 600. ) * pMOD->pConv->kelvin( 120. ) + pMOD->pConv->kelvin( 20. );*/

    stripEntryTemperature = pMOD->pConv->kelvin( 70. );

  }


  return stripEntryTemperature;
}

//============================================================================
// SPE_specific: calculateStripEntryTemperature
//
//    Calculate strip entry temperature in case of Gas JET preheating section.
//============================================================================
double SPE_specific::calculateStripEntryTemperature( double zoneHDAvg )
{
  double zoneHD[8] = {0};

  for ( long iZone = 0; iZone < 8; iZone++ )
  {
    zoneHD[iZone] = zoneHDAvg;
  }

  return calculateStripEntryTemperature( zoneHD, false );
}


