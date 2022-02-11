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
// SPE_specific: 
//
//    Structure declaration.
//============================================================================
struct com_globalSection      gblSec;                       // Extern declaration of HVM global section.

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

  strcpy_s( traceTempo, DIM_PATH, "We are loading AM Mardyck Galma 2 Model." );

  strcpy_s( traceTempo, DIM_PATH, "AM_Mardyck_GALMA2.txt" );

  if ( sizeof("AM_Mardyck_GALMA2.txt") > size )
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
  // MAP global section for communication.
  gblSec.map                ( TYPE_CREATION                       // Entry: MAP global section.
                            , sizeof(struct p_BLD)                // Entry: The low-order  DWORD of the maximum size of the file mapping object.
                            , "CMI_gblSec"                        // Entry: Global section name.
                            , "D:\\LTOP\\CMI_gblSec.gbl"		      // Entry: Global section filename.
                            , &gblSec.newGlobalSection            // Exit: Creation indicator.
                            , (void**)&gblSec.pSec                // Exit: Adress of the global section.
                            , &gblSec.handleGlobalSection         // Exit: Handle of file.
                            );
 
  // Open or create mutex.
  gblSec.createOrOpenMutex( false );

  struct HVM_Parameter parameter = pGEN->pHvm->phvm->parameter;

  // For each heat cycles.
  for ( long i_cycle = 0; i_cycle < DIM_HEAT_CYCLES; i_cycle++ )
  {
    parameter.lineCycle[0].shared.heatCycles[i_cycle].cycleNumber = i_cycle;
    
    // Each 100 cycles.
    if ( i_cycle % 100 == 0 )
    {
      if ( i_cycle / 100 < 55 || i_cycle / 100 > 87 ) continue;
    
      // For all sections.
      for ( long i_section = 0; i_section < pMOD->pConf->pconf->lines[0].nb_sections; i_section++ )
      {
        // RTF / SOA.
        if ( pMOD->pConf->pconf->lines[0].pSections[i_section]->type == TYPE_RTF ||
             pMOD->pConf->pconf->lines[0].pSections[i_section]->type == TYPE_SOA
           )
        {
          parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section].stripTarget = pMOD->pConv->kelvin( i_cycle / 10.      );
          parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section].stripLower  = pMOD->pConv->kelvin( i_cycle / 10. - 10. );
          parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section].stripUpper  = pMOD->pConv->kelvin( i_cycle / 10. + 30. );
          parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section].minimumTime = 20.;
        }
        // JET cooling.
        else if ( pMOD->pConf->pconf->lines[0].pSections[i_section]->type == TYPE_JET
                )
        {
          parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section].stripTarget = pMOD->pConv->kelvin( 460.      );
          parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section].stripLower  = pMOD->pConv->kelvin( 460. - 20 );
          parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section].stripUpper  = pMOD->pConv->kelvin( 460. + 20 );
          parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section].minimumTime = 0.;
          for ( long i_zone = 0; i_zone < pMOD->pConf->pconf->lines[0].pSections[i_section]->nb_zones; i_zone++ )
          {
            parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section].zones[i_zone].coolingRate = 10.;
          }
        }
        // NOF section type.
        else if ( pMOD->pConf->pconf->lines[0].pSections[i_section]->type == TYPE_NOF
                )
        {
          parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section].stripTarget = pMOD->pConv->kelvin( 620.        );
          parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section].stripLower  = pMOD->pConv->kelvin( 620. -   5. );
          parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section].stripUpper  = pMOD->pConv->kelvin( 620. + 120. );
          parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section].minimumTime = 0.;
        }
        // BASIC section type.
        else if ( pMOD->pConf->pconf->lines[0].pSections[i_section]->type == TYPE_BASE
                )
        {
          parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section].stripTarget = parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section-1].stripTarget;
          parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section].stripLower  = parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section-1].stripLower ;
          parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section].stripUpper  = parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section-1].stripUpper ;
          parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section].minimumTime = parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section-1].minimumTime;
        }
        // NOF section type.
        else if ( pMOD->pConf->pconf->lines[0].pSections[i_section]->type == TYPE_POT
                )
        {
          parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section].stripTarget = pMOD->pConv->kelvin( 460.      );
          parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section].stripLower  = pMOD->pConv->kelvin( 460. - 10 );
          parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section].stripUpper  = pMOD->pConv->kelvin( 460. + 10 );
          parameter.lineCycle[0].shared.heatCycles[i_cycle].sections[i_section].minimumTime = 0.;
        }
      } // END: For all sections.
    } // END: % 100
  
  }

  //// For all zones.
  //for ( long iZone = 0; iZone < pMOD->pConf->pconf->nb_zones; iZone++ )
  //{
  //  long nuZoneGlobal = pMOD->pConf->pconf->pZones[iZone]->indexGlobal;
  //
  //  if      ( pMOD->pConf->pconf->pZones[iZone]->type == TYPE_RTF ||
  //            pMOD->pConf->pconf->pZones[iZone]->type == TYPE_SOA
  //          )
  //  {
  //    parameter.zones[nuZoneGlobal].maximumTemperature  = pMOD->pConv->kelvin( 920. );
  //    parameter.zones[nuZoneGlobal].maximumPower        = 100.;
  //    parameter.zones[nuZoneGlobal].emissivityFactor    =   0.;
  //    for ( long i_point = 0; i_point < parameter.zones[nuZoneGlobal].efficiency.nb_points; i_point++ )
  //    {
  //      parameter.zones[nuZoneGlobal].efficiency.Y[i_point] = 1.;
  //    }
  //  }
  //  // NOF section type.
  //  else if ( pMOD->pConf->pconf->pZones[iZone]->type == TYPE_NOF
  //          )
  //  {
  //    long nuZoneType = pMOD->pConf->pconf->pZones[iZone]->indexType;
  //    parameter.zones[nuZoneGlobal].maximumTemperature  = pMOD->pConv->kelvin( 1350. );
  //    parameter.zones[nuZoneGlobal].maximumPower        = pMOD->pConf->pconf->zonesNOF[nuZoneType].Power;
  //    parameter.zones[nuZoneGlobal].emissivityFactor    =   1.;
  //    for ( long i_point = 0; i_point < parameter.zones[nuZoneGlobal].efficiency.nb_points; i_point++ )
  //    {
  //      parameter.zones[nuZoneGlobal].efficiency.Y[i_point] = 1.;
  //    }
  //  }
  //}
  
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
  gblSec.DestroyMutex( );
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

  // Find the furnace communication.
  if ( !hasSimulationMode )
  {
    for ( long i_com = 0; i_com < pMOD->pConf->pconf->coms.lNbTcpCommunication; i_com++ )
    {
      // If we have found the furnace TCP communication.
      if ( strncmp( pMOD->pConf->pconf->coms.comTcp[i_com].name , "TCP_Communication_Furnace", 29 ) == 0 )
      {
      
        //// Specify the port of the TCP communication with the furnace PLC, depending of the server.
        //if ( specific.computerNumber == 1 ) pMOD->pConf->pconf->coms.comTcp[i_com].lhostPortNumber = 15101;
        //else                                pMOD->pConf->pconf->coms.comTcp[i_com].lhostPortNumber = 15102;
      
      }
    }
  }

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
        //parameter.zones[i_zone].maximumTemperature        = pMOD->pConv->kelvin( 957. );
        //parameter.zones[i_zone].maximumPower              = pZoneRTF->Power;
        //parameter.zones[i_zone].emissivityFactor          =   0.;
        //parameter.zones[i_zone].boosterBumperLow          =   0.;
        //parameter.zones[i_zone].boosterBumperUpp          =   0.;
        //parameter.zones[i_zone].demandUppRamp             =   4.;
        //parameter.zones[i_zone].demandDwRamp              =   4.;
        //parameter.zones[i_zone].pots[0].replyTemperature  =   pMOD->pConv->kelvin( 460. );
        //for ( long i_point = 0; i_point < parameter.zones[i_zone].efficiency.nb_points; i_point++ )
        //{
        //  parameter.zones[i_zone].efficiency.Y[i_point] = 1.;
        //}
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
          
        //parameter.zones[i_zone].emissivityFactor          =   1.;
        //parameter.zones[i_zone].demandUppRamp             =   4.;
        //parameter.zones[i_zone].demandDwRamp              =   4.;
        //parameter.zones[i_zone].jetMaximumFlowFactor      =   1.;
        //parameter.zones[i_zone].pots[0].replyTemperature  =   pMOD->pConv->kelvin( 460. );

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

  if      ( messageId == 2 )
  {
    pMeasures->measurementType        = TYPE_COILS;             // USED !!!
    pMeasures->nb_messageToSendBack   = 0 ;
    for ( long i_msg = 0; i_msg < DIM_MESSAGES_MAX; i_msg++ )
    {
      pMeasures->messageIdToSendBack[i_msg] = -1;
    }
  }
  else if ( messageId == 3 )
  {
    pMeasures->measurementType        = TYPE_TRACKING;          // USED !!!
    pMeasures->nb_messageToSendBack   = 0 ;
    for ( long i_msg = 0; i_msg < DIM_MESSAGES_MAX; i_msg++ )
    {
      pMeasures->messageIdToSendBack[i_msg] = -1;
    }
  }
  else if ( messageId == 1 )
  {
    pMeasures->measurementType        = TYPE_ALL_SECTION;       // USED !!!
    pMeasures->nb_messageToSendBack   = 2 ;
    for ( long i_msg = 0; i_msg < DIM_MESSAGES_MAX; i_msg++ )
    {
      pMeasures->messageIdToSendBack[i_msg] = -1;
    }
    pMeasures->messageIdToSendBack[0] = 101;      // Furnace setpoints
    pMeasures->messageIdToSendBack[1] = 102;   // Line setpoints
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

  //pSpt->specific.tractionOffset               = 0.;
  //pSpt->specific.stripTempVapeur1             = pMOD->pConv->kelvin( 751. );
  //pSpt->specific.stripTempVapeur2             = pMOD->pConv->kelvin( 752. );
  //pSpt->specific.EmissivityP2                 = 0.25;
  //pSpt->specific.EmissivityP3                 = 0.26;
  //pSpt->specific.derivePyro                   = false;
  //pSpt->specific.tempPyroVirtuelP2            = pMOD->pConv->kelvin( 801. );
  //pSpt->specific.tempPyroVirtuelP3            = pMOD->pConv->kelvin( 802. );
  //pSpt->specific.tempPyroVirtuelP4            = pMOD->pConv->kelvin( 803. );
  //pSpt->specific.tempPyroVirtuelP3Virtuel     = pMOD->pConv->kelvin( 804. );
  //pSpt->specific.tempPyroVirtuelP13           = 805.;
  //pSpt->specific.StatusPyrometerP1            = 0;
  //pSpt->specific.StatusPyrometerP2[0]         = 0;
  //pSpt->specific.StatusPyrometerP2[1]         = 1;
  //pSpt->specific.StatusPyrometerP3[0]         = 0;
  //pSpt->specific.StatusPyrometerP3[1]         = 1;
  //pSpt->specific.StatusPyrometerP4[0]         = 0;
  //pSpt->specific.StatusPyrometerP4[1]         = 1;
  //pSpt->specific.StatusPyrometerP10           = 1;
  //pSpt->specific.StatusPyrometerP11[0]        = 0;
  //pSpt->specific.StatusPyrometerP11[1]        = 1;
  //pSpt->specific.StatusPyrometerP16           = 0;
  //pSpt->tubes[0].state                        = 1;
  //for ( long i = 0; i < pMOD->pConf->pconf->nb_tubes; i++ )
  //{
  //  if ( i % 10 == 0 )
  //    pSpt->tubes[i].state                        = 1;
  //  else
  //    pSpt->tubes[i].state                        = 0;
  //}

  


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


  //else if ( strcmp( pInter->in.Mnemonic, "STRIP_TEMP_JET" ) == 0 )
  //{
  //  pValue->dvalue = stripTemperatureJET[pInter->in.firstIndex];
  //}


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

  //// Coils
  //else if ( strcmp( pInter->in.Mnemonic, "MaxSpeedPot" ) == 0 )
  //{
  //  //memset( pMea->coils[pInter->in.secondIndex].specific.combinaisonIdentifier, 0, DIM_NAMES );
  //  //strcpy( pMea->coils[pInter->in.secondIndex].specific.combinaisonIdentifier, pValue->stringValue );
  //  pMea->coils[pInter->in.secondIndex].specific.maxSpeedPot      = pValue->dvalue;
  //}
  //else if ( strcmp( pInter->in.Mnemonic, "MinSpeedCoil" ) == 0 )
  //{
  //  pMea->coils[pInter->in.secondIndex].specific.minSpeedCoil     = pValue->dvalue;
  //}
  //else if ( strcmp( pInter->in.Mnemonic, "MinSpeedDFF" ) == 0 )
  //{
  //  pMea->coils[pInter->in.secondIndex].specific.minSpeedDFF      = pValue->dvalue;
  //}
  //else if ( strcmp( pInter->in.Mnemonic, "OffsetTempEE" ) == 0 )
  //{
  //  pMea->coils[pInter->in.secondIndex].specific.offsetTempEE      = pValue->dvalue;
  //}



  //// Zones
  //else if ( strcmp( pInter->in.Mnemonic, "MAX_HEAT_DEMAND" ) == 0 )
  //{
  //  pMea->specific.zones[pInter->out.nu_direct].maxHeatDemand    = pValue->lvalue;
  //}
  //else if ( strcmp( pInter->in.Mnemonic, "MIN_HEAT_DEMAND" ) == 0 )
  //{
  //  pMea->specific.zones[pInter->out.nu_direct].minHeatDemand    = pValue->lvalue;
  //}
  //
  //// Other
  //else if ( strcmp( pInter->in.Mnemonic, "PYROMETER_DFF" ) == 0 )
  //{
  //  pMea->specific.PyrometerDFF[pInter->in.firstIndex]  = pValue->dvalue;
  //}
  //else if ( strcmp( pInter->in.Mnemonic, "PYRO_SELECTION_DFF" ) == 0 )
  //{
  //  pMea->specific.nuPyroDFF                    = pValue->lvalue;
  //}

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
                                  bool                       &authorizedToContinue
                                , struct GEN_utility *        pGEN )
{
  // We manually store the data.
  bool doRegularStorage   = true;
  authorizedToContinue    = true;
  hasRecievedAllmessages  = true;
  //long            nbAuthorization   = 1;
  //static  double  oldValue          = 0;
  //
  //this->hasRecievedAllmessages[0] = true;
  //authorizedToContinue            = true;
  //
  ////// Watch dog message.
  ////if      ( pMessageRecieve->measurementMessage == 1 )
  ////{
  ////  this->hasRecievedAllmessages[0] = true;
  ////  authorizedToContinue            = true;
  ////
  ////
  ////  // Effectuer ici l'enregistrement de toutes les données spécifique dans les structures.
  ////  // Effectuer aussi le traitement des données spécifique: sélection du pyromètre etc...
  ////
  ////
  ////  pGEN->pHvm->phvm->meaSptCal.measure.specific = pMessageRecieve->specific;
  ////  
  ////
  ////} // END: Message recieved.
  //
  //hasRecievedAllmessages = true;
  //for ( long i = 0; i < nbAuthorization; i++ )
  //{
  //  hasRecievedAllmessages &= this->hasRecievedAllmessages[i];
  //}

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

  //// Coils message.
  //if      ( pMessageRecieve->measurementMessage == 4001 )
  //{
  //  
  //}
  //
  //// Event welding.
  //else if ( pMessageRecieve->measurementMessage == 4002 )
  //{
  // 
  //}
  //// Event weld entry.
  //else if ( pMessageRecieve->measurementMessage == 4003 )
  //{
  //  
  //}
  //// Event weld exit.
  //else if ( pMessageRecieve->measurementMessage == 4004 )
  //{
  //  
  //}

  for ( long iCoil = 0; iCoil < DIM_COILS; iCoil++ )
  {
    pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[iCoil] = pMessageRecieve->coils[iCoil];
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
    // Check incoherent tracking.
    bool incoherentTracking = false;
    if ( ( pGEN->pHvm->phvm->meaSptCal.measure.line.weldPositions[0] < -10. ||
           pGEN->pHvm->phvm->meaSptCal.measure.line.weldPositions[0] > 500. ) && initialize )
    {
      incoherentTracking = strncmp( pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[0].entryIdentity, 
                                    pGEN->pHvm->phvm->meaSptCal.measure.coils      [0].entryIdentity, 
                                    strlen( pGEN->pHvm->phvm->meaSptCal.measure.coils[0].entryIdentity ) ) == 0;
      
    }
    sprintf_s( traceTempo, "incoherentTracking = %d , pGEN->pHvm->phvm->meaSptCal.measure.line.weldPositions[0] = %f m, coilsBuffer id = %s, coil id = %s "
                         , incoherentTracking
                         , pGEN->pHvm->phvm->meaSptCal.measure.line.weldPositions[0]
                         , pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[0].entryIdentity
                         , pGEN->pHvm->phvm->meaSptCal.measure.coils      [0].entryIdentity
                         );
    pMOD->pUtl->writeTrace( traceFilePointer		  // Entry: File pointer.
                          , traceTempo            // Entry: Trace string.
                          );
    if ( !initialize )
    {
      if ( pGEN->pHvm->phvm->meaSptCal.measure.line.weldPositions[0] < 400. )
      {
        for ( long i_coil = 0; i_coil < DIM_COILS; i_coil++ )
        {
          memset( &pGEN->pHvm->phvm->meaSptCal.measure.coils[i_coil], 0, sizeof(struct MOD_HV_coil_data_simulation) );
          pGEN->pHvm->phvm->meaSptCal.measure.coils[i_coil] = pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[i_coil];
        }
      }
      else
      {
        for ( long i_coil = 1; i_coil < DIM_COILS; i_coil++ )
        {
          memset( &pGEN->pHvm->phvm->meaSptCal.measure.coils[i_coil], 0, sizeof(struct MOD_HV_coil_data_simulation) );
          pGEN->pHvm->phvm->meaSptCal.measure.coils[i_coil] = pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[i_coil-1];
        }

        // Create a previous with a special identity.
        pGEN->pHvm->phvm->meaSptCal.measure.coils[0] = pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[0];
        strcpy_s( pGEN->pHvm->phvm->meaSptCal.measure.coils[0].entryIdentity, "Initialisation" );
      }
      initialize = true;
    }
    else
    {
      if ( pGEN->pHvm->phvm->meaSptCal.measure.line.weldPositions[0] < 400. )
      {
        for ( long i_coil = 0; i_coil < DIM_COILS; i_coil++ )
        {
          memset( &pGEN->pHvm->phvm->meaSptCal.measure.coils[i_coil], 0, sizeof(struct MOD_HV_coil_data_simulation) );
          pGEN->pHvm->phvm->meaSptCal.measure.coils[i_coil] = pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[i_coil];
        }
      }
      else
      {
        if ( !incoherentTracking || firstTime )
        {
          firstTime = false;
          for ( long i_coil = 1; i_coil < DIM_COILS; i_coil++ )
          {
            memset( &pGEN->pHvm->phvm->meaSptCal.measure.coils[i_coil], 0, sizeof(struct MOD_HV_coil_data_simulation) );
            pGEN->pHvm->phvm->meaSptCal.measure.coils[i_coil] = pGEN->pHvm->phvm->meaSptCal.measure.coilsBuffer[i_coil-1];
          }
        }
      }
    }

    static double weldPositionLast    = -1.;
    static double weldPositionLast2   = -1.;
    static double weldPositionElapse  = -1.;
    static double weldPositionIni     = -1.;
    static bool   overWrite           = false;

    // old way.
    //double lengthVariation            = weldPositionLast - pGEN->pHvm->phvm->meaSptCal.measure.line.weldPositions[1]; // positive.

    //if ( lengthVariation < 0 )
    //  lengthVariation                 = weldPositionLast - pGEN->pHvm->phvm->meaSptCal.measure.line.weldPositions[0];

    //if ( weldPositionLast2 - pGEN->pHvm->phvm->meaSptCal.measure.line.weldPositions[0] > 0 )
    //  lengthVariation = min( weldPositionLast2 - pGEN->pHvm->phvm->meaSptCal.measure.line.weldPositions[0], lengthVariation );

    // new way. only index 0.
    double lengthVariation            = weldPositionLast2 - pGEN->pHvm->phvm->meaSptCal.measure.line.weldPositions[0];
    if ( lengthVariation < pMOD->pMath->epsilon )  lengthVariation = 1.;

    weldPositionLast                  = pGEN->pHvm->phvm->meaSptCal.measure.line.weldPositions[1];
    weldPositionLast2                 = pGEN->pHvm->phvm->meaSptCal.measure.line.weldPositions[0];
    
    sprintf_s( traceTempo, "lengthVariation = %f m , weldPositions 1 = %f m, weldPositions 2 = %f m "
                         , lengthVariation
                         , pGEN->pHvm->phvm->meaSptCal.measure.line.weldPositions[1]
                         , pGEN->pHvm->phvm->meaSptCal.measure.line.weldPositions[2]
                         );
    pMOD->pUtl->writeTrace( traceFilePointer		  // Entry: File pointer.
                          , traceTempo            // Entry: Trace string.
                          );
    if ( overWrite || pGEN->pHvm->phvm->meaSptCal.measure.line.weldPositions[0] < 130. )
    {
      double weldPosition = 0.;
      if ( !overWrite )
      {
        weldPositionIni     = pGEN->pHvm->phvm->meaSptCal.measure.line.weldPositions[0];
        weldPositionElapse  = 0.;
        weldPosition        = pGEN->pHvm->phvm->meaSptCal.measure.line.weldPositions[0];
      }
      else
      {
        weldPositionElapse  += lengthVariation;
        weldPosition        = weldPositionIni - weldPositionElapse; // A vérifier pas sure.
      }
      weldsPosition[0]    = weldPosition;
      strncpy_s( referenceCoilIdentity, DIM_NAMES, pGEN->pHvm->phvm->meaSptCal.measure.coils[0].entryIdentity, strlen( pGEN->pHvm->phvm->meaSptCal.measure.coils[0].entryIdentity ) );
        
      sprintf_s( traceTempo, "overWrite = %d, weldPosition = %f m, weldPositionElapse = %f m, referenceCoilIdentity = %s "
                           , overWrite
                           , weldPosition
                           , weldPositionElapse
                           , referenceCoilIdentity
                           );
      pMOD->pUtl->writeTrace( traceFilePointer		  // Entry: File pointer.
                            , traceTempo            // Entry: Trace string.
                            );
      
      overWrite = true;
      if ( weldPositionElapse > 200. || weldPositionElapse < -400. )
      {
        weldPositionElapse  = 0.;
        overWrite           = false;
        weldsPosition[0]    = pGEN->pHvm->phvm->meaSptCal.measure.line.weldPositions[0];
        strncpy_s( referenceCoilIdentity, DIM_NAMES, pGEN->pHvm->phvm->meaSptCal.measure.line.entryIdentities[0], strlen( pGEN->pHvm->phvm->meaSptCal.measure.line.entryIdentities[0] ) );

        sprintf_s( traceTempo, "Return to regular, weldPosition = %f m, referenceCoilIdentity = %s "
                             , weldsPosition[0]
                             , referenceCoilIdentity
                             );
        pMOD->pUtl->writeTrace( traceFilePointer		  // Entry: File pointer.
                              , traceTempo            // Entry: Trace string.
                              );
      }
    }
    else
    {
      weldPositionElapse  = 0.;
      overWrite           = false;
      weldsPosition[0]    = pGEN->pHvm->phvm->meaSptCal.measure.line.weldPositions[0];
      strncpy_s( referenceCoilIdentity, DIM_NAMES, pGEN->pHvm->phvm->meaSptCal.measure.line.entryIdentities[0], strlen( pGEN->pHvm->phvm->meaSptCal.measure.line.entryIdentities[0] ) );

      sprintf_s( traceTempo, "regular, weldPosition = %f m, referenceCoilIdentity = %s "
                           , weldsPosition[0]
                           , referenceCoilIdentity
                           );
      pMOD->pUtl->writeTrace( traceFilePointer		  // Entry: File pointer.
                            , traceTempo            // Entry: Trace string.
                            );
    }
    //weldsPosition[1] = pGEN->pHvm->phvm->meaSptCal.measure.line.weldPositions[1];

    nbWelds = 1;

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

    sprintf_s( traceTempo, "Suivi soudure. Pos 0: %f m, Pos 1: %f m, Pos 2: %f m / Id 0 = %s, Id 1 = %s, Id 2 = %s, "
                         , pGEN->pHvm->phvm->meaSptCal.measure.line.weldPositions[0]
                         , pGEN->pHvm->phvm->meaSptCal.measure.line.weldPositions[1]
                         , pGEN->pHvm->phvm->meaSptCal.measure.line.weldPositions[2]
                         , pGEN->pHvm->phvm->meaSptCal.measure.line.entryIdentities[0]
                         , pGEN->pHvm->phvm->meaSptCal.measure.line.entryIdentities[1]
                         , pGEN->pHvm->phvm->meaSptCal.measure.line.entryIdentities[2]
                         );
    pMOD->pUtl->writeTrace( traceFilePointer		  // Entry: File pointer.
                          , traceTempo            // Entry: Trace string.
                          );

    sprintf_s( traceTempo, "Suivi soudure. referenceCoilIdentity = %s, entryIdentity 0 = %s, entryIdentity 1 = %s, entryIdentity 2 = %s"
                         , referenceCoilIdentity
                         , pGEN->pHvm->phvm->meaSptCal.measure.coils[0].entryIdentity
                         , pGEN->pHvm->phvm->meaSptCal.measure.coils[1].entryIdentity
                         , pGEN->pHvm->phvm->meaSptCal.measure.coils[2].entryIdentity
                         );
    pMOD->pUtl->writeTrace( traceFilePointer		  // Entry: File pointer.
                          , traceTempo            // Entry: Trace string.
                          );

    //bool ksjdbfjskc = true;
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

    //------------------------------------------------------------------------------------
    // Find emissivity class.
    if ( pGEN->pHvm->phvm->coilQueue.coils[i_coil].data.steelType < 0 ||
         pGEN->pHvm->phvm->coilQueue.coils[i_coil].data.steelType >= DIM_EMISSIVITY_CLASS / 5 )
      pGEN->pHvm->phvm->coilQueue.coils[i_coil].calcul.emissivityClass = 99;
    else
    {
      if      ( pGEN->pHvm->phvm->coilQueue.coils[i_coil].data.thickness <= 0.3 / 1000. ) pGEN->pHvm->phvm->coilQueue.coils[i_coil].calcul.emissivityClass = 0;
      else if ( pGEN->pHvm->phvm->coilQueue.coils[i_coil].data.thickness <= 0.5 / 1000. ) pGEN->pHvm->phvm->coilQueue.coils[i_coil].calcul.emissivityClass = 1;
      else if ( pGEN->pHvm->phvm->coilQueue.coils[i_coil].data.thickness <= 0.8 / 1000. ) pGEN->pHvm->phvm->coilQueue.coils[i_coil].calcul.emissivityClass = 2;
      else if ( pGEN->pHvm->phvm->coilQueue.coils[i_coil].data.thickness <= 1.1 / 1000. ) pGEN->pHvm->phvm->coilQueue.coils[i_coil].calcul.emissivityClass = 3;
      else if ( pGEN->pHvm->phvm->coilQueue.coils[i_coil].data.thickness <= 1.5 / 1000. ) pGEN->pHvm->phvm->coilQueue.coils[i_coil].calcul.emissivityClass = 4;
      else                                                                                pGEN->pHvm->phvm->coilQueue.coils[i_coil].calcul.emissivityClass = 5;
    }

    // Copy current RTF emissivity in next coil class if different.
    long nuCoilCurrentOrdered     = pGEN->pHvm->phvm->meaSptCal.lines[0].pSections[0]->pCalcul->tracking.nu_coilInOrdered;
    long nuCoilCurrentOrderedNext = nuCoilCurrentOrdered + 1;

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

    // Define line configuration.
    if ( pGEN->pHvm->phvm->coilQueue.coils[i_coil].data.soakingTime > 0. || true )
      pGEN->pHvm->phvm->coilQueue.coils[i_coil].calcul.nuLine = 0;
    else
      pGEN->pHvm->phvm->coilQueue.coils[i_coil].calcul.nuLine = 1;

    long nuLine = pGEN->pHvm->phvm->coilQueue.coils[i_coil].calcul.nuLine;

    // Store coil data in tempory structure. Done to reaorganize what is recieved in message versus requiered by the line defenition selected.
    struct MOD_HV_coil_data CoilData = pGEN->pHvm->phvm->coilQueue.coils[i_coil].data;
    
    for ( long iSection = 0; iSection < pMOD->pConf->pconf->lines[nuLine].nb_sections; iSection++ )
    {
      long nuSectionGlobal = pMOD->pConf->pconf->lines[nuLine].pSections[iSection]->indexGlobal;
    
      pGEN->pHvm->phvm->coilQueue.coils[i_coil].data.sections[iSection].stripTemp =
        CoilData.sections[nuSectionGlobal].stripTemp;
    }

    // Alliation: 
    // Till the weld has been gone through the tower, we cannot accelerate. Thus we limit next coil with a speed.

    
    //// Create the POT target tempe and tolerances.
    //for ( long iSection = 0; iSection < pMOD->pConf->pconf->lines[nuLine].nb_sections; iSection++ )
    //{
    //  long nuSectionGlobal = pMOD->pConf->pconf->lines[nuLine].pSections[iSection]->indexGlobal;
    //
    //  // POT section.
    //  if ( pMOD->pConf->pconf->lines[nuLine].pSections[iSection]->type == TYPE_POT )
    //  {
    //    // Copy previous section information.
    //    pGEN->pHvm->phvm->coilQueue.coils[i_coil].data.sections[iSection].stripTemp =
    //      pGEN->pHvm->phvm->coilQueue.coils[i_coil].data.sections[iSection - 1].stripTemp;
    //  }
    //
    //}

    
    // No heat cycle code.
    continue;
  }

  //// Minimum line speed.
  //long nuCoilCurrentOrdered     = pGEN->pHvm->phvm->meaSptCal.lines[0].pSections[0]->pCalcul->tracking.nu_coilInOrdered;
  //if ( pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoilCurrentOrdered] != nullptr )
  //{
  //  pGEN->pHvm->phvm->meaSptCal.measure.line.operatorSpeedLimit = max( pGEN->pHvm->phvm->meaSptCal.measure.line.operatorSpeedLimit, pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoilCurrentOrdered]->data.specific.minSpeedCoil );
  //  pGEN->pHvm->phvm->meaSptCal.measure.line.operatorSpeedLimit = max( pGEN->pHvm->phvm->meaSptCal.measure.line.operatorSpeedLimit, pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoilCurrentOrdered]->data.specific.minSpeedDFF  );
  //}

  //------------------------------------------------------------------------------------
  // For each coils in queue.
  for ( long i_coil = 0; i_coil < DIM_COILS; i_coil++ )
  {

    
    /*if ( abs( lastPIDExit - pidStripTargetTempEE.exit.pid ) >= 1. - pMOD->pMath->epsilon )
    {
      pGEN->pHvm->phvm->coilQueue.coils[i_coil].data.sections[nuSection].stripTemp.target += pidStripTargetTempEE.exit.pid;
      pGEN->pHvm->phvm->coilQueue.coils[i_coil].data.sections[nuSection].stripTemp.target = max( pGEN->pHvm->phvm->coilQueue.coils[i_coil].data.sections[nuSection].stripTemp.target,
                                                                                         pGEN->pHvm->phvm->coilQueue.coils[i_coil].data.sections[nuSection].stripTemp.lower );
      if ( i_coil == DIM_COILS - 1 ) lastPIDExit = pidStripTargetTempEE.exit.pid;
    }
    else
      pGEN->pHvm->phvm->coilQueue.coils[i_coil].data.sections[nuSection].stripTemp.target += lastPIDExit;*/

    /*if ( pGEN->pHvm->phvm->coilQueue.coils[i_coil].data.specific.maxSpeedPot > pMOD->pMath->epsilon )
      pGEN->pHvm->phvm->coilQueue.coils[i_coil].data.maxSpeedQuality = min( pGEN->pHvm->phvm->coilQueue.coils[i_coil].data.maxSpeedQuality, 
                                                                    pGEN->pHvm->phvm->coilQueue.coils[i_coil].data.specific.maxSpeedPot );*/
    

    ////------------------------------------------------------------------------------------
    //// Find product cycle.
    //long nu_cycle = -1;
    //if ( pGEN->pHvm->phvm->coilQueue.coils[i_coil].data.heatCycle == 0 )
    //  pGEN->pHvm->phvm->coilQueue.coils[i_coil].data.heatCycle = 1;
    //bool found = pGEN->pHvm->findProductCycle( pGEN->pHvm->phvm->coilQueue.coils[i_coil].data.heatCycle     // Entry: Product heat cycles number.
    //                                 , nu_cycle                                             // Out  : Cycle number in table.
    //                                 );
    //
    ////------------------------------------------------------------------------------------
    //// Store heat cycle.
    //// If heat cycle has been found.
    //long nuLine = pGEN->pHvm->phvm->coilQueue.coils[i_coil].calcul.nuLine;
    //if ( found )
    //{
    //  if ( nuLine == 0 )
    //  {
    //    // For all sections defined in line configuration.
    //    for ( long i_section = 0; i_section < pMOD->pConf->pconf->lines[nuLine].nb_sections; i_section++ )
    //    {
    //      pGEN->pHvm->phvm->coilQueue.coils[i_coil].data.sections[i_section].stripTemp.upper  = 
    //        pGEN->pHvm->phvm->parameter.lineCycle[0].shared.heatCycles[nu_cycle].sections[i_section].stripUpper;
    //      pGEN->pHvm->phvm->coilQueue.coils[i_coil].data.sections[i_section].stripTemp.target = 
    //        pGEN->pHvm->phvm->parameter.lineCycle[0].shared.heatCycles[nu_cycle].sections[i_section].stripTarget;
    //      pGEN->pHvm->phvm->coilQueue.coils[i_coil].data.sections[i_section].stripTemp.lower  = 
    //        pGEN->pHvm->phvm->parameter.lineCycle[0].shared.heatCycles[nu_cycle].sections[i_section].stripLower;
    //
    //    }
    //  }
    //  else if ( nuLine == 1 )
    //  {
    //    // For all sections defined in line configuration.
    //    for ( long i_section = 0; i_section < pMOD->pConf->pconf->lines[nuLine].nb_sections; i_section++ )
    //    {
    //      if ( i_section == 0 )
    //      {
    //        pGEN->pHvm->phvm->coilQueue.coils[i_coil].data.sections[i_section].stripTemp.upper  = 
    //          pGEN->pHvm->phvm->parameter.lineCycle[0].shared.heatCycles[nu_cycle].sections[i_section].stripUpper;
    //        pGEN->pHvm->phvm->coilQueue.coils[i_coil].data.sections[i_section].stripTemp.target = 
    //          pGEN->pHvm->phvm->parameter.lineCycle[0].shared.heatCycles[nu_cycle].sections[i_section].stripTarget;
    //        pGEN->pHvm->phvm->coilQueue.coils[i_coil].data.sections[i_section].stripTemp.lower  = 
    //          pGEN->pHvm->phvm->parameter.lineCycle[0].shared.heatCycles[nu_cycle].sections[i_section].stripLower;
    //      }
    //      else
    //      {
    //        pGEN->pHvm->phvm->coilQueue.coils[i_coil].data.sections[i_section].stripTemp.upper  = 
    //          pGEN->pHvm->phvm->parameter.lineCycle[0].shared.heatCycles[nu_cycle].sections[i_section+1].stripUpper;
    //        pGEN->pHvm->phvm->coilQueue.coils[i_coil].data.sections[i_section].stripTemp.target = 
    //          pGEN->pHvm->phvm->parameter.lineCycle[0].shared.heatCycles[nu_cycle].sections[i_section+1].stripTarget;
    //        pGEN->pHvm->phvm->coilQueue.coils[i_coil].data.sections[i_section].stripTemp.lower  = 
    //          pGEN->pHvm->phvm->parameter.lineCycle[0].shared.heatCycles[nu_cycle].sections[i_section+1].stripLower;
    //      }
    //
    //    }
    //
    //  }
    //}
    //// Product heat cycle has not been found.
    //else
    //{
    //  pGEN->pHvm->phvm->coilQueue.coils[i_coil].calcul.dataValid = false;
    //  sprintf_s( traceTempo, DIM_PATH, "Product heat cycle not found. ID = %s, Heat cycle number = %d"
    //                                 , pGEN->pHvm->phvm->coilQueue.coils[i_coil].data.entryIdentity
    //                                 , pGEN->pHvm->phvm->coilQueue.coils[i_coil].data.heatCycle
    //                                 );
    //  pMOD->pUtl->writeTrace( traceFilePointer		  // Entry: File pointer.
    //                , traceTempo            // Entry: Trace string.
    //                );
    //}
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

    
    // Get current coil ID.
    long nu_coilOrderedInSection  = pGEN->pHvm->phvm->meaSptCal.lines[0].pSections[1]->pCalcul->tracking.nu_coilInOrdered;


    //------------------------------------------------------------------------------------
    // Re work GJC.
    long nuSection              = 2;
    long nuSectionGlobal        = pMOD->pConf->pconf->lines[0].pSections[nuSection]->indexGlobal;
    long nuSectionInType        = pMOD->pConf->pconf->lines[0].pSections[nuSection]->indexType;

    double averageMaxHeatDemand  = 0.;
    double averageHeatDemand     = 0.;
    for ( long iZone = 0; iZone < pMOD->pConf->pconf->sectionsJET[nuSectionInType].nb_zones; iZone++ )
    {
      long    nuZoneGlobal  = pMOD->pConf->pconf->sectionsJET[nuSectionInType].pZones[iZone]->indexGlobal;
      long    nuZoneInType  = pMOD->pConf->pconf->sectionsJET[nuSectionInType].pZones[iZone]->indexType;
      averageMaxHeatDemand += pGEN->pHvm->phvm->parameter.zones[nuZoneGlobal].maximumHeatDemand;
      averageHeatDemand    += pGEN->pHvm->phvm->meaSptCal.zonesJET[nuZoneInType].pSetpoint->demandPercentage;
    }
    averageMaxHeatDemand /= ( pMOD->pConf->pconf->sectionsJET[nuSectionInType].nb_zones );
    averageHeatDemand    /= ( pMOD->pConf->pconf->sectionsJET[nuSectionInType].nb_zones );

    for ( long iZone = 0; iZone < pMOD->pConf->pconf->sectionsJET[nuSectionInType].nb_zones; iZone++ )
    {
      long    nuZoneGlobal  = pMOD->pConf->pconf->sectionsJET[nuSectionInType].pZones[iZone]->indexGlobal;
      long    nuZoneInType  = pMOD->pConf->pconf->sectionsJET[nuSectionInType].pZones[iZone]->indexType;
      pGEN->pHvm->phvm->meaSptCal.zonesJET[nuZoneInType].pSetpoint->demandPercentage = averageHeatDemand *  pGEN->pHvm->phvm->parameter.zones[nuZoneGlobal].maximumHeatDemand / 
                                                                                                    averageMaxHeatDemand;
      bool test = false;
    }

    // Traction offset.
    long nuCoilInOrder = pGEN->pHvm->phvm->meaSptCal.lines[0].pSections[0]->pCalcul->tracking.nu_coilInOrdered;
    if ( nuCoilInOrder >= 0 && nuCoilInOrder < DIM_COILS )
    {
      if ( pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoilInOrder] != nullptr )
      {
        if      ( pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoilInOrder]->data.thickness >= 1. / 1000. && 
                  pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoilInOrder]->data.width     <= 0.95 )
	          tractionOffset = 10.;                                                                                         
	      else if ( pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoilInOrder]->data.thickness <= 0.6 /1000. && 
                  pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoilInOrder]->data.width     >= 1.4 )
	          tractionOffset = -15.; 
	      else
	          tractionOffset = 0.f;
      }
    }

    
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

  //------------------------------------------------------------------------------------
  // For all RTF zones.                                                               RTF measurement treatment.
  static long counterTestAllBurnersOff = 0;
  if ( ++counterTestAllBurnersOff > 1000 ) counterTestAllBurnersOff = 500;
  
  bool hasNoPower = true;
  for ( long iZone = 0; iZone < pMOD->pConf->pconf->nb_zonesRTF; iZone++ )
  {
    struct EQP_pointers_zone_RTF *pZone = &pGEN->pHvm->phvm->meaSptCal.zonesRTF[iZone];
    long    nuZoneGlobal  = pMOD->pConf->pconf->zonesRTF[iZone].indexGlobal;

    // LHV.
    double lhv = pMOD->pPhysic->fuelLHV( 0 );

    if ( hasSimulationMode )
    {
      nbBurnerInDefaultRTF[iZone] = 0;
    }

    // If power exists.
    if ( pMOD->pConf->pconf->zonesRTF[iZone].Power > 0. )
    {
      double maxGasFlow = pMOD->pConf->pconf->zonesRTF[iZone].Power / lhv; 
      if ( ( maxGasFlow <= pMOD->pMath->epsilon ) || hasSimulationMode )
        maxGasFlow = pMOD->pConf->pconf->zonesRTF[iZone].Power / lhv;
      else
        maxGasFlow = ( 1. - (double) nbBurnerInDefaultRTF[iZone] / (double) pMOD->pConf->pconf->zonesRTF[iZone].nb_tubes ) * pMOD->pConf->pconf->zonesRTF[iZone].Power / lhv;
      maxGasFlow = max( maxGasFlow, 0. );
      parameter.zones[nuZoneGlobal].maximumPower = maxGasFlow * lhv;

      hasNoPower = hasNoPower && parameter.zones[nuZoneGlobal].maximumPower < pMOD->pMath->epsilon;

      // Store number of tube operational.
      pZone->pCalcul->nb_operationalBurners = pMOD->pConf->pconf->zonesRTF[iZone].nb_tubes - nbBurnerInDefaultRTF[iZone];
    }


    // Maximum heat demand.
    parameter.zones[nuZoneGlobal].maximumHeatDemand = (double) ( pMOD->pConf->pconf->zonesRTF[iZone].nb_tubes - nbBurnerInDefaultRTF[iZone] ) / (double) pMOD->pConf->pconf->zonesRTF[iZone].nb_tubes * 100.;
    
    // parameter.zones[nuZoneGlobal].minimumHeatDemand = 0.; to be added in IHM.
  }

  // If no power on all zones => set power design.
  if ( hasNoPower )
  {
    for ( long iZone = 0; iZone < pMOD->pConf->pconf->nb_zonesRTF; iZone++ )
    {
      long    nuZoneGlobal  = pMOD->pConf->pconf->zonesRTF[iZone].indexGlobal;
      parameter.zones[nuZoneGlobal].maximumPower = pMOD->pConf->pconf->zonesRTF[iZone].Power;
    }
  }


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
  
  //------------------------------------------------------------------------------------
  // JET.

  // We read the xls file containing the JET maximum fan rotation speed depending of QT.

  FILE *    pReadfile                       = nullptr;
  long      status                          = 0;
  char      fileNewPath   [DIM_PATH   + 1]  = {0};
  char      fileDirectory [DIM_PATH   + 1]  = {0};
  //char      traceTempo    [DIM_TRACES + 1]  = {0};
  WCHAR *   readWord      [90            ]  = { 0 };    // Table of char mapped on a file line.
  char      tempoName     [DIM_NAMES  + 1]  = { 0 };    // Tempory char name to recieved converted WCHAR.
  
  _locale_t localParameters                    ;    // Local computer parameter.
  localParameters = _get_current_locale()      ;    // Get local parameters.

  for ( long i = 0; i < 90; i++ )
  {
    readWord[i] = new WCHAR[500 + 1];
  }

  strcpy_s( fileDirectory, DIM_PATH , pGEN->pointerPath   );
  strcat_s( fileDirectory, DIM_PATH , DATA_PATH           );
  strcpy_s( fileNewPath  , DIM_PATH , fileDirectory       );
  strcat_s( fileNewPath  , DIM_PATH , this->AppName       );
  strcat_s( fileNewPath  , DIM_PATH , "MaxZoneJET.csv"    );

  // Open the trace file.
  status = pMOD->pUtl->fileManagement ( &pReadfile                     // Entry: File definition.
                              , fileNewPath                    // Entry: File Name. 
                              , TYPE_OPEN_FILE_READ_WRITE      // Entry: Define action to realize on the file. 
                              );
  
  // Reset memory of storing structure.
  memset( &sJetMaxRotSpeedQt, 0, sizeof( sJetMaxRotSpeedQt ) );

  if ( pReadfile != nullptr && status == 1 )
  {
    //------------------------------------------------------------------------------------
    // We loop until the end of the file.
    long loop = 0;
    status = 1;
    while ( status == 1 )
    {
      //------------------------------------------------------------------------------------
      // Read a line.
      WCHAR rline[512]      = { 0 };
      char  result[5][64]   = { 0 };
      unsigned length = 500;
      long index = 0;
      if ( fgetws( rline, 512, pReadfile ) != NULL ) 
      {
        swscanf_s( rline, L"%ls", readWord[0 ], length );

        char texte[500] = {0};
        pMOD->pUtl->convertWideCharToChar( readWord[0], 500,  texte );

        char value[50] = {0};
        for ( long i = 0; i < strlen( texte ); i++ )
        {
          if ( strncmp( &texte[i], ";", 1 ) == 0 )
          //if ( texte[i] == ";" )
          {
            strncpy_s( result[index], value, strlen( value ) );
            strcpy_s( value, "" );
            index ++;
          }
          else
          {
            strncat_s( value, &texte[i], 1);
          }
        }
        status = 1;
      }
      else 
        status = 0;

      //------------------------------------------------------------------------------------
      // Comment treatment.
      if ( wcscmp( readWord[0], L"") != 0 )
      {
        if      ( loop == 0 || loop == 1 ) { loop++; continue; }
        else if ( loop == 2 )
        {
          sJetMaxRotSpeedQt.DefaultValues.zoneMaxDemand[0] = _atof_l(result[1], localParameters );
          sJetMaxRotSpeedQt.DefaultValues.zoneMaxDemand[1] = _atof_l(result[2], localParameters );
          sJetMaxRotSpeedQt.DefaultValues.zoneMaxDemand[2] = _atof_l(result[3], localParameters );
        }
        else
        {
          //pMOD->pUtl->convertWideCharToChar( readWord[0], 16,  sJetMaxRotSpeedQt.QTs[sJetMaxRotSpeedQt.nbQt].QT );
          strncpy_s( sJetMaxRotSpeedQt.QTs[sJetMaxRotSpeedQt.nbQt].QT, result[0], strlen( result[0] ) );
          sJetMaxRotSpeedQt.QTs[sJetMaxRotSpeedQt.nbQt].zoneMaxDemand[0] = _atof_l(result[1], localParameters );
          sJetMaxRotSpeedQt.QTs[sJetMaxRotSpeedQt.nbQt].zoneMaxDemand[1] = _atof_l(result[2], localParameters );
          sJetMaxRotSpeedQt.QTs[sJetMaxRotSpeedQt.nbQt].zoneMaxDemand[2] = _atof_l(result[3], localParameters );
          sJetMaxRotSpeedQt.nbQt ++;
        }
      }

      if ( ++loop >= 1000 ) break;
    }

    // Close the trace file.
    status = pMOD->pUtl->fileManagement ( &pReadfile		                 // File definition.
                                        , fileNewPath                    // File Name. 
                                        , TYPE_CLOSE_FILE                // Define action to realize on the file. 
                                        );
  }
  else
  {
    bool totototo = false;
  }
  
  
  

  // Delete all memory used.
  for ( long i = 0; i < 90; i++ )
  {
    delete( readWord[i] );
  }

  //------------------------------------------------------------------------------------
  // Treatment of fans speed contraints.
  bool    hasContraint      = false;
  long    constraintIndex   = -1;
  double  valuesMaxHD[3]    = {0};
  if ( pGEN->pHvm->phvm->meaSptCal.lines[0].pSections[0] != nullptr )
  {
    long nuCoilOrdered      = pGEN->pHvm->phvm->meaSptCal.lines[0].pSections[0]->pCalcul->tracking.nu_coilInOrdered;
    long nuCoilOrderedNext  = nuCoilOrdered + 1;

    if ( nuCoilOrdered >= 0 && nuCoilOrdered < DIM_COILS && nuCoilOrderedNext >= 0 && nuCoilOrderedNext < DIM_COILS )
    {

      if ( pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoilOrdered    ] != nullptr &&
           pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoilOrderedNext] != nullptr )
      {
        char QtCur[DIM_NAMES + 1] = {0};
        char QtNxt[DIM_NAMES + 1] = {0};

        strcpy_s( QtCur, pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoilOrdered    ]->data.steelGrade );
        strcpy_s( QtNxt, pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoilOrderedNext]->data.steelGrade );

        bool curHasQtConstraint = false;
        bool nxtHasQtConstraint = false;
        long curNuQt            = -1;
        long nxtNuQt            = -1;

        for ( long i = 0; i < sJetMaxRotSpeedQt.nbQt; i++ )
        {
          if ( strcmp( QtCur, sJetMaxRotSpeedQt.QTs[i].QT ) == 0 )
          {
            curNuQt             = i;
            curHasQtConstraint  = true;
          }

          if ( strcmp( QtNxt, sJetMaxRotSpeedQt.QTs[i].QT ) == 0 )
          {
            nxtNuQt             = i;
            nxtHasQtConstraint  = true;
          }

          if ( nxtHasQtConstraint && curHasQtConstraint ) break;
        }

        if ( curHasQtConstraint )
        {
          hasContraint = true;
          valuesMaxHD[0] = sJetMaxRotSpeedQt.QTs[curNuQt].zoneMaxDemand[0];
          valuesMaxHD[1] = sJetMaxRotSpeedQt.QTs[curNuQt].zoneMaxDemand[1];
          valuesMaxHD[2] = sJetMaxRotSpeedQt.QTs[curNuQt].zoneMaxDemand[2];
        }
        else
        {
          valuesMaxHD[0] = sJetMaxRotSpeedQt.DefaultValues.zoneMaxDemand[0];
          valuesMaxHD[1] = sJetMaxRotSpeedQt.DefaultValues.zoneMaxDemand[1];
          valuesMaxHD[2] = sJetMaxRotSpeedQt.DefaultValues.zoneMaxDemand[2];
        }

        if ( nxtHasQtConstraint )
        {
          double remainingTime = pGEN->pHvm->phvm->meaSptCal.lines[0].pSections[0]->pCalcul->tracking.weldTime[0];

          if (  remainingTime < 10. * 60. )
          {
            hasContraint = true;
            valuesMaxHD[0] = min( valuesMaxHD[0], sJetMaxRotSpeedQt.QTs[nxtNuQt].zoneMaxDemand[0] );
            valuesMaxHD[1] = min( valuesMaxHD[1], sJetMaxRotSpeedQt.QTs[nxtNuQt].zoneMaxDemand[1] );
            valuesMaxHD[2] = min( valuesMaxHD[2], sJetMaxRotSpeedQt.QTs[nxtNuQt].zoneMaxDemand[2] );
          }
        }
      }
    }
  }

  if ( !hasContraint )
  {
    valuesMaxHD[0] = sJetMaxRotSpeedQt.DefaultValues.zoneMaxDemand[0];
    valuesMaxHD[1] = sJetMaxRotSpeedQt.DefaultValues.zoneMaxDemand[1];
    valuesMaxHD[2] = sJetMaxRotSpeedQt.DefaultValues.zoneMaxDemand[2];
  }

  
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
      parameter.zones[nuZoneGlobal].maximumHeatDemand = valuesMaxHD[iZone];
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

  // Section control.
  if ( !hasSimulationMode )
  {
    for ( long i_section = 0; i_section < pMOD->pConf->pconf->lines[0].nb_sections; i_section++ )
    {
      pGEN->pHvm->phvm->meaSptCal.lines[0].pSections[i_section]->pMeasure->control = ( pGEN->pHvm->phvm->meaSptCal.lines[0].pSections[i_section]->pMeasure->control && 
                                                                                      this->computerNumber == pGEN->pHvm->phvm->meaSptCal.measure.serverConnected );
    }
    
    // Speed control.
    pGEN->pHvm->phvm->meaSptCal.lines[0].pMeasure->speedControl = ( pGEN->pHvm->phvm->meaSptCal.lines[0].pMeasure->speedControl && 
                                                                  this->computerNumber == pGEN->pHvm->phvm->meaSptCal.measure.serverConnected );
  }

  
  //----------------------------------------------------------------------------
  // Speed ramp adaptation depending of GI or GA.
  bool isGA = false;
  if ( pGEN->pHvm->phvm->meaSptCal.lines[0].pSections[0] != nullptr )
  {
    long nuCoilOrdered      = pGEN->pHvm->phvm->meaSptCal.lines[0].pSections[0]->pCalcul->tracking.nu_coilInOrdered;

    if ( nuCoilOrdered >= 0 && nuCoilOrdered < DIM_COILS )
    {
      if ( pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoilOrdered] != nullptr )
      {
        if ( pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoilOrdered]->data.specific.isGAElseGi )
        {
          isGA = true;
        }
      }
    }
  }

  if ( isGA ) parameter.maximumSpeedRamp = 4. / 60. / 60.;
  else        parameter.maximumSpeedRamp = 16. / 60. / 60.;
  
  //----------------------------------------------------------------------------
  // We are not allowed to increase speed till the queue of a GA has not gone through the Tower.
  double safetyDistance = 200.; // JAB should be the distance between RTH pyro and tower exit. Mais je ne l'ai pas calculée.

  static struct MOD_HV_coil_data lastCoil;
  static struct MOD_HV_coil_data currentCoil;
  if ( pGEN->pHvm->phvm->meaSptCal.lines[0].pSections[0] != nullptr )
  {
    long nuCoilOrdered      = pGEN->pHvm->phvm->meaSptCal.lines[0].pSections[0]->pCalcul->tracking.nu_coilInOrdered;
    if ( nuCoilOrdered >= 0 && nuCoilOrdered < DIM_COILS )
    {
      if ( pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoilOrdered] != nullptr )
      {
        if ( strcmp( pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoilOrdered]->data.entryIdentity, 
                     currentCoil.entryIdentity ) != 0 )
        {
          lastCoil    = currentCoil;
          currentCoil = pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoilOrdered]->data;
          sprintf_s( traceTempo, "GA constraint Tower. coil change. ID new last coil = %s, is GA constraint = %d, max speed alliation = %f m/min."
                               , lastCoil.entryIdentity
                               , lastCoil.specific.isGAElseGi
                               , lastCoil.specific.alliationMaxSpeed / 60.
                               );
          pMOD->pUtl->writeTrace( ppTraceFilePointer    // Entry: File pointer.
                                , traceTempo            // Entry: Trace string.
                                );
        }
      }
    }
  }

  // If last coil is GA, we cannot accelerate till xx meters.
  static double lastWeldPosRTH                    = 0.;
  static double elapseDistanceSinceWeldUnderpyro  = 1000.;
  double        currentWeldPosRTH                 = 0.;
  if ( pGEN->pHvm->phvm->meaSptCal.lines[0].pSections[0] != nullptr )
  {
    currentWeldPosRTH = pGEN->pHvm->phvm->meaSptCal.lines[0].pSections[0]->pCalcul->tracking.weldPosition[0];

    // Check weld changement.
    if ( currentWeldPosRTH > lastWeldPosRTH + 100. )
    {
      elapseDistanceSinceWeldUnderpyro = 0.;
      sprintf_s( traceTempo, "GA constraint Tower. WELD CHANGE at RTH. currentWeldPosRTH = %f m, lastWeldPosRTH = %f m."
                            , currentWeldPosRTH
                            , lastWeldPosRTH
                            );
      pMOD->pUtl->writeTrace( ppTraceFilePointer    // Entry: File pointer.
                            , traceTempo            // Entry: Trace string.
                            );
    }
    else
      elapseDistanceSinceWeldUnderpyro += lastWeldPosRTH - currentWeldPosRTH;

    sprintf_s( traceTempo, "GA constraint Tower. elapseDistanceSinceWeldUnderpyro = %f m."
                          , elapseDistanceSinceWeldUnderpyro
                          );
    pMOD->pUtl->writeTrace( ppTraceFilePointer    // Entry: File pointer.
                  , traceTempo            // Entry: Trace string.
                  );

    lastWeldPosRTH = currentWeldPosRTH;

    struct MOD_HV_coil_data previousCoil;
    long nuCoilOrdered      = pGEN->pHvm->phvm->meaSptCal.lines[0].pSections[0]->pCalcul->tracking.nu_coilInOrdered;

    if ( nuCoilOrdered > 0 ) 
    {
      long nuCoilPrevious = nuCoilOrdered - 1;
      if ( pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoilPrevious] != nullptr ) previousCoil = pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoilPrevious]->data;
      else                                                               previousCoil = lastCoil;

      sprintf_s( traceTempo, "GA constraint Tower. Previous coil index = %d"
                           , nuCoilOrdered
                           );
      pMOD->pUtl->writeTrace( ppTraceFilePointer    // Entry: File pointer.
                            , traceTempo            // Entry: Trace string.
                            );
    }
    else 
      previousCoil = lastCoil;

    // If last coil is GA.
    if ( previousCoil.specific.isGAElseGi )
    {
      if ( elapseDistanceSinceWeldUnderpyro < safetyDistance )
      {
        if ( nuCoilOrdered > 0 && nuCoilOrdered < DIM_COILS )
        {
          // We apply last coil max speed on current coil.
          if ( pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoilOrdered] != nullptr )
          {
            sprintf_s( traceTempo, "GA constraint Tower. APPLICATION. previousCoil.maxSpeedQuality = %f m/min, current.maxSpeedQuality = %f m/min, current ID = %s."
                                 , previousCoil.maxSpeedQuality * 60.
                                 , pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoilOrdered]->data.maxSpeedQuality * 60.
                                 , pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoilOrdered]->data.entryIdentity
                                 );
            pMOD->pUtl->writeTrace( ppTraceFilePointer    // Entry: File pointer.
                                  , traceTempo            // Entry: Trace string.
                                  );

            if ( previousCoil.maxSpeedQuality > pMOD->pMath->epsilon )
              pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoilOrdered]->data.maxSpeedQuality = 
                min( pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoilOrdered]->data.maxSpeedQuality, previousCoil.maxSpeedQuality );
          }
        }
      }
    }
  }


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

  //pidGJC2FirstVent

  // GI Product. => résistances coupées.
  //  long nuSectionInLine = 4;
  //    if ( pGEN->pHvm->phvm->meaSptCal.lines[0].pSections[nuSectionInLine]->pPyrometers[0]->pSetpoint ->temperature < pMOD->pConv->kelvin( 550. ) )

  // GJC.
  if      ( nuSectionInType == 0 )
  {
    

    //// For all zones.
    //for ( long i_zone = 0; i_zone < pSectionMSC->pConfig->nb_zones; i_zone++ )
    //{
    //  struct EQP_pointers_zone_JET *pZoneMSC = ( struct EQP_pointers_zone_JET * ) pSectionMSC->pZones[i_zone];
    //  pZoneMSC->pSetpoint->demandPercentage = pStageSection->powerDemand[i_zone] + pPidJET->exit.pid;
    //  pZoneMSC->pSetpoint->demandPercentage = min ( max ( pZoneMSC->pSetpoint->demandPercentage, 5. ), 100. );
    //}
  }

  // APC Up pass.
  else if ( nuSectionInType == 1 )
  {
    //hasSpecificControl = true;
    //bool    isGIElseALUSI         = pGEN->pHvm->phvm->meaSptCal.lines[0].pSections[4]->pPyrometers[0]->pSetpoint ->temperature < pMOD->pConv->kelvin( 550. );
    //double  targetTemp2FirstVent  = -1.;
    //if ( isGIElseALUSI )  targetTemp2FirstVent = pMOD->pConv->kelvin( 420. ) - 30.;
    //else                  targetTemp2FirstVent = pMOD->pConv->kelvin( 580. ) - 35.;

    //// Save info.
    //this->isGIElseALUSI = isGIElseALUSI;

    //pidGJC2FirstVent.entry.processValue   = pProfil->stripTemperature[80][5];   // Index 80 is the element corresponding to the 2 ventilator of the GJC.
    //pidGJC2FirstVent.entry.setPointValue  = targetTemp2FirstVent;
    //pidGJC2FirstVent.data.Kp              =  - 0.15; 
    //pidGJC2FirstVent.data.Ki              =  - 30. ; 
    //pidGJC2FirstVent.data.maximumExit     =    100. - ( pStageSection->powerDemand[0] + pPidJET->exit.pid );
    //pidGJC2FirstVent.data.minimumExit     =         - ( pStageSection->powerDemand[0] + pPidJET->exit.pid );
    //pidGJC2FirstVent.data.deadBand        =     4. ;
    //pidGJC2FirstVent.functions.automatic  =   true ;
    //
    //// Calculate PID.
    //if ( abs( coolDemandMeasure[0] - ( pStageSection->powerDemand[0] + pidGJC2FirstVent.exit.pid ) ) < 3. )
    //  pMOD->pPid->calculate( &pidGJC2FirstVent      // Modified: pid objects structure.
    //                );
    //
    //for ( long i_zone = 0; i_zone < pSectionMSC->pConfig->nb_zones; i_zone++ )
    //{
    //  struct EQP_pointers_zone_JET *pZoneMSC = ( struct EQP_pointers_zone_JET * ) pSectionMSC->pZones[i_zone];
    //  long nuZoneGlobal = pSectionMSC->pZones[i_zone]->pConfig->indexGlobal;
    //  
    //  // First 2 zones.
    //  if ( i_zone == 0 || i_zone == 1 )
    //  {
    //    //pZoneMSC->pSetpoint->demandPercentage = pStageSection->powerDemand[0] + pPidJET->exit.pid + pidGJC2FirstVent.exit.pid;
    //    pZoneMSC->pSetpoint->demandPercentage = pStageSection->powerDemand[0] + pidGJC2FirstVent.exit.pid;
    //    pZoneMSC->pSetpoint->demandPercentage = min ( max ( pZoneMSC->pSetpoint->demandPercentage,  pGEN->pHvm->phvm->parameter.zones[nuZoneGlobal].minimumHeatDemand ), 
    //                                                                                                pGEN->pHvm->phvm->parameter.zones[nuZoneGlobal].maximumHeatDemand );
    //  }
    //  // Other zones.
    //  else
    //  {
    //    //pZoneMSC->pSetpoint->demandPercentage = pStageSection->powerDemand[i_zone] + pPidJET->exit.pid;
    //    pZoneMSC->pSetpoint->demandPercentage = pHD[i_zone] + pPidJET->exit.pid;
    //    pZoneMSC->pSetpoint->demandPercentage = min ( max ( pZoneMSC->pSetpoint->demandPercentage,  pGEN->pHvm->phvm->parameter.zones[nuZoneGlobal].minimumHeatDemand ), 
    //                                                                                                pGEN->pHvm->phvm->parameter.zones[nuZoneGlobal].maximumHeatDemand );
    //  }
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
// SPE_specific: jetOnLiveControl
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
    //hasSpecificControl = true;
    //bool    isGIElseALUSI         = pGEN->pHvm->phvm->meaSptCal.lines[0].pSections[4]->pPyrometers[0]->pSetpoint ->temperature < pMOD->pConv->kelvin( 550. );
    //double  targetTemp2FirstVent  = -1.;
    //if ( isGIElseALUSI )  targetTemp2FirstVent = pMOD->pConv->kelvin( 420. ) - 30.;
    //else                  targetTemp2FirstVent = pMOD->pConv->kelvin( 580. ) - 35.;
    //
    //// Save info.
    //this->isGIElseALUSI = isGIElseALUSI;

    //pidGJC2FirstVent.entry.processValue   = pProfil->stripTemperature[80][5];   // Index 80 is the element corresponding to the 2 ventilator of the GJC.
    //pidGJC2FirstVent.entry.setPointValue  = targetTemp2FirstVent;
    //pidGJC2FirstVent.data.Kp              =  - 0.15; 
    //pidGJC2FirstVent.data.Ki              =  - 30. ; 
    //pidGJC2FirstVent.data.maximumExit     =    70. ;
    //pidGJC2FirstVent.data.minimumExit     =  - 70. ;
    //pidGJC2FirstVent.data.deadBand        =     4. ;
    //pidGJC2FirstVent.functions.automatic  =   false;
    //
    //// Force PID output.
    //pMOD->pPid->fillInManual( &pidGJC2FirstVent                 // Modified: pid objects structure.
    //                , cv      [0]                       // Entry   : Actual CV.
    //                , modelSp [0]                       // Entry   : Actual model set point.
    //                );

    //// Calculate PID.
    //pMOD->pPid->calculate( &pidGJC2FirstVent      // Modified: pid objects structure.
    //             );

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
void SPE_specific::readSpecificCommunication               ( FILE **traceFilePointer, struct MOD_HV_measures *    pMeasures, struct GEN_utility *  pGEN   )
{
  char traceTempo [DIM_TRACES + 1] = {0};

  //strcpy_s( traceTempo, "selectConfigurationFile: size of char too low. " );
  //pMOD->pUtl->writeTrace( traceFilePointer		  // Entry: File pointer.
  //              , traceTempo            // Entry: Trace string.
  //              );

  // Secure access to global section.
	gblSec.lockAccess();
  
	/* Speed control */
  if ( gblSec.pSec->m9101.ctrlvit == 1 )  pMeasures->line.speedControl = true;
  else                                    pMeasures->line.speedControl = false;
  
	/* RTF control */
	if ( gblSec.pSec->m9101.ctrlrtf == 1 )  pMeasures->sectionsRTF[0].control = true;
  else                                    pMeasures->sectionsRTF[0].control = false;
  
	/* Soaking control */
	if ( gblSec.pSec->m9101.ctrlmai == 1 )  pMeasures->sectionsBASE[0].control = true;
  else                                    pMeasures->sectionsBASE[0].control = false;

	/* JET cooling control */
	if ( gblSec.pSec->m9101.ctrlref == 1 )  pMeasures->sectionsJET[0].control = true;
  else                                    pMeasures->sectionsJET[0].control = false;

	/*JET Z1 control */
	if ( gblSec.pSec->m9101.ctrlrefz1 == 1 )  zoneOperationJET[0] = true;
	else                                      zoneOperationJET[0] = false;

	/* JET Z2 control */
	if ( gblSec.pSec->m9101.ctrlrefz2 == 1 )  zoneOperationJET[1] = true;
	else                                      zoneOperationJET[1] = false;

	/* JET Z2 control */
	if ( gblSec.pSec->m9101.ctrlrefz3 == 1 )  zoneOperationJET[2] = true;
	else                                      zoneOperationJET[2] = false;

	///* Speed control */
	//if (gblSec.pSec->m9101.ctrlvit == 1)
	//	sps_entry_rtf.s_mea.spd_auto = true;
	//else
	//	sps_entry_rtf.s_mea.spd_auto = false;
 //                    
	///* RTF control */
	//if (gblSec.pSec->m9101.ctrlrtf == 1)
	//	sps_entry_rtf.s_mea.rtf_auto = true;
	//else
	//	sps_entry_rtf.s_mea.rtf_auto = false;

	///* Soaking control */
	//if (gblSec.pSec->m9101.ctrlmai == 1)
	//	s_spe_mar.s_mea.Soa_ctl = true;
	//else
	//	s_spe_mar.s_mea.Soa_ctl = false;

	///* JET cooling control */
	//if (gblSec.pSec->m9101.ctrlref == 1)
	//	sps_entry_cool.s_mea.jet_auto = true;
	//else
	//	sps_entry_cool.s_mea.jet_auto = false;

	///*JET Z1 control */
	//if (gblSec.pSec->m9101.ctrlrefz1 == 1)
	//	sps_entry_cool.s_mea.on_zone_jet[0] = true;
	//else
	//	sps_entry_cool.s_mea.on_zone_jet[0] = false;

	///* JET Z2 control */
	//if (gblSec.pSec->m9101.ctrlrefz2 == 1)
	//	sps_entry_cool.s_mea.on_zone_jet[1] = true;
	//else
	//	sps_entry_cool.s_mea.on_zone_jet[1] = false;

	///* JET Z2 control */
	//if (gblSec.pSec->m9101.ctrlrefz3 == 1)
	//	sps_entry_cool.s_mea.on_zone_jet[2] = true;
	//else
	//	sps_entry_cool.s_mea.on_zone_jet[2] = false;

	/* ------------------- */
	/* Soaking mea.        */
	/* ------------------- */
  pMeasures->pyrometers[1].temperature = gblSec.pSec->m9101.tmpp4util + 273.15;      /* Soaking strip temperature measurement */
	soakingTemperatureMeasurement =
		gblSec.pSec->m9101.mai_tmp_z1 + gblSec.pSec->m9101.mai_tmp_z2 + 
		gblSec.pSec->m9101.mai_tmp_z3 + gblSec.pSec->m9101.mai_tmp_z4 + 
		gblSec.pSec->m9101.ter_tmp_z1 + gblSec.pSec->m9101.ter_tmp_z2 + 
		gblSec.pSec->m9101.ter_tmp_z3;
	soakingTemperatureMeasurement /= 7.;
	soakingTemperatureMeasurement += 273.15;      /* average zone temperature measurement */


	//s_spe_mar.s_mea.Strategy_soa = gblSec.pSec->m9101.stramai;               /* Soaking strategy */ NOT USED
	//s_spe_mar.s_mea.Tmp_stp_soa = gblSec.pSec->m9101.tmpp4util + 273.f;      /* Soaking strip temperature measurement */
	//avg_tmp_zon_soa =
	//	gblSec.pSec->m9101.mai_tmp_z1 + gblSec.pSec->m9101.mai_tmp_z2 + 
	//	gblSec.pSec->m9101.mai_tmp_z3 + gblSec.pSec->m9101.mai_tmp_z4 + 
	//	gblSec.pSec->m9101.ter_tmp_z1 + gblSec.pSec->m9101.ter_tmp_z2 + 
	//	gblSec.pSec->m9101.ter_tmp_z3;
	//avg_tmp_zon_soa /= 7.f;
	//s_spe_mar.s_mea.Tmp_zon_soa = avg_tmp_zon_soa + 273.f;      /* average zone temperature measurement */
	//s_spe_mar.s_mea.Soa_ctl = gblSec.pSec->m9101.ctrlmai;

	/* ------------------- */
	/* RTF mea.            */
	/* ------------------- */
  pMeasures->pyrometers[0].temperature = gblSec.pSec->m9101.tmpp3util + 273.15;    /* RTF strip tmp. mea. */

	//sps_entry_rtf.s_mea.tmp_strip = gblSec.pSec->m9101.tmpp3util + 273.f;    /* RTF strip tmp. mea. */

  pMeasures->zonesRTF[0].demandPercentage = gblSec.pSec->m9101.cha_chargz1;                     /* RTF pwr Z1 */
  pMeasures->zonesRTF[1].demandPercentage = gblSec.pSec->m9101.cha_chargz2;                     /* RTF pwr Z2 */
  pMeasures->zonesRTF[2].demandPercentage = gblSec.pSec->m9101.cha_chargz3;                     /* RTF pwr Z3 */
  pMeasures->zonesRTF[3].demandPercentage = gblSec.pSec->m9101.cha_chargz4;                     /* RTF pwr Z4 */
  pMeasures->zonesRTF[4].demandPercentage = gblSec.pSec->m9101.cha_chargz5;                     /* RTF pwr Z5 */
  pMeasures->zonesRTF[5].demandPercentage = gblSec.pSec->m9101.cha_chargz6;                     /* RTF pwr Z6 */
  pMeasures->zonesRTF[6].demandPercentage = gblSec.pSec->m9101.cha_chargz7;                     /* RTF pwr Z7 */
  pMeasures->zonesRTF[7].demandPercentage = gblSec.pSec->m9101.cha_chargz8;                     /* RTF pwr Z8 */
  
  // Number of tubes out of order.
  nbBurnerInDefaultRTF[0] = gblSec.pSec->m9101.iNbOfTubeInDefaultZone1;
  nbBurnerInDefaultRTF[1] = gblSec.pSec->m9101.iNbOfTubeInDefaultZone2;
  nbBurnerInDefaultRTF[2] = gblSec.pSec->m9101.iNbOfTubeInDefaultZone3;
  nbBurnerInDefaultRTF[3] = gblSec.pSec->m9101.iNbOfTubeInDefaultZone4;
  nbBurnerInDefaultRTF[4] = gblSec.pSec->m9101.iNbOfTubeInDefaultZone5;
  nbBurnerInDefaultRTF[5] = gblSec.pSec->m9101.iNbOfTubeInDefaultZone6;
  nbBurnerInDefaultRTF[6] = gblSec.pSec->m9101.iNbOfTubeInDefaultZone7;
  nbBurnerInDefaultRTF[7] = gblSec.pSec->m9101.iNbOfTubeInDefaultZone8;

  // Soaking.  
  nbBurnerInDefaultRTF[8]   = 0;
  nbBurnerInDefaultRTF[9]   = 0;
  nbBurnerInDefaultRTF[10]  = 0;
  nbBurnerInDefaultRTF[11]  = 0;

  //// The values before are not implemented yet.
  //nbBurnerInDefaultRTF[0] = 1;
  //nbBurnerInDefaultRTF[1] = 2;
  //nbBurnerInDefaultRTF[2] = 1;
  //nbBurnerInDefaultRTF[3] = 0;
  //nbBurnerInDefaultRTF[4] = 0;
  //nbBurnerInDefaultRTF[5] = 0;
  //nbBurnerInDefaultRTF[6] = 0;
  //nbBurnerInDefaultRTF[7] = 0;

  // Correct demand %.
  pMeasures->zonesRTF[0].demandPercentage *= ( 1. - nbBurnerInDefaultRTF[0] / 26. );
  pMeasures->zonesRTF[1].demandPercentage *= ( 1. - nbBurnerInDefaultRTF[1] / 22. );
  pMeasures->zonesRTF[2].demandPercentage *= ( 1. - nbBurnerInDefaultRTF[2] / 18. );
  pMeasures->zonesRTF[3].demandPercentage *= ( 1. - nbBurnerInDefaultRTF[3] / 14. );
  pMeasures->zonesRTF[4].demandPercentage *= ( 1. - nbBurnerInDefaultRTF[4] / 20. );
  pMeasures->zonesRTF[5].demandPercentage *= ( 1. - nbBurnerInDefaultRTF[5] / 16. );
  pMeasures->zonesRTF[6].demandPercentage *= ( 1. - nbBurnerInDefaultRTF[6] / 17. );
  pMeasures->zonesRTF[7].demandPercentage *= ( 1. - nbBurnerInDefaultRTF[7] / 14. );
  

	//sps_entry_rtf.s_mea.pow_pct[0] = gblSec.pSec->m9101.cha_chargz1;                     /* RTF pwr Z1 */
	//sps_entry_rtf.s_mea.pow_pct[1] = gblSec.pSec->m9101.cha_chargz2;                     /* RTF pwr Z2 */
	//sps_entry_rtf.s_mea.pow_pct[2] = gblSec.pSec->m9101.cha_chargz3;                     /* RTF pwr Z3 */
	//sps_entry_rtf.s_mea.pow_pct[3] = gblSec.pSec->m9101.cha_chargz4;                     /* RTF pwr Z4 */
	//sps_entry_rtf.s_mea.pow_pct[4] = gblSec.pSec->m9101.cha_chargz5;                     /* RTF pwr Z5 */
	//sps_entry_rtf.s_mea.pow_pct[5] = gblSec.pSec->m9101.cha_chargz6;                     /* RTF pwr Z6 */
	//sps_entry_rtf.s_mea.pow_pct[6] = gblSec.pSec->m9101.cha_chargz7;                     /* RTF pwr Z7 */
	//sps_entry_rtf.s_mea.pow_pct[7] = gblSec.pSec->m9101.cha_chargz8;                     /* RTF pwr Z8 */

	rtfZonesFumesTemperature[0] = gblSec.pSec->m9101.cha_tmp_fumz1;   	       /* RTF tmp fumee Z1 */
	rtfZonesFumesTemperature[1] = gblSec.pSec->m9101.cha_tmp_fumz2;   	       /* RTF tmp fumee Z2 */
	rtfZonesFumesTemperature[2] = gblSec.pSec->m9101.cha_tmp_fumz3;   	       /* RTF tmp fumee Z3 */
	rtfZonesFumesTemperature[3] = gblSec.pSec->m9101.cha_tmp_fumz4;   	       /* RTF tmp fumee Z4 */
	rtfZonesFumesTemperature[4] = gblSec.pSec->m9101.cha_tmp_fumz5;   	       /* RTF tmp fumee Z5 */
	rtfZonesFumesTemperature[5] = gblSec.pSec->m9101.cha_tmp_fumz6;   	       /* RTF tmp fumee Z6 */
	rtfZonesFumesTemperature[6] = gblSec.pSec->m9101.cha_tmp_fumz7;   	       /* RTF tmp fumee Z7 */
	rtfZonesFumesTemperature[7] = gblSec.pSec->m9101.cha_tmp_fumz8;   	       /* RTF tmp fumee Z8 */

	//sps_entry_rtf.s_mea.pow_lim_overT[0] = gblSec.pSec->m9101.cha_tmp_fumz1;   	       /* RTF tmp fumee Z1 */
	//sps_entry_rtf.s_mea.pow_lim_overT[1] = gblSec.pSec->m9101.cha_tmp_fumz2;   	       /* RTF tmp fumee Z2 */
	//sps_entry_rtf.s_mea.pow_lim_overT[2] = gblSec.pSec->m9101.cha_tmp_fumz3;   	       /* RTF tmp fumee Z3 */
	//sps_entry_rtf.s_mea.pow_lim_overT[3] = gblSec.pSec->m9101.cha_tmp_fumz4;   	       /* RTF tmp fumee Z4 */
	//sps_entry_rtf.s_mea.pow_lim_overT[4] = gblSec.pSec->m9101.cha_tmp_fumz5;   	       /* RTF tmp fumee Z5 */
	//sps_entry_rtf.s_mea.pow_lim_overT[5] = gblSec.pSec->m9101.cha_tmp_fumz6;   	       /* RTF tmp fumee Z6 */
	//sps_entry_rtf.s_mea.pow_lim_overT[6] = gblSec.pSec->m9101.cha_tmp_fumz7;   	       /* RTF tmp fumee Z7 */
	//sps_entry_rtf.s_mea.pow_lim_overT[7] = gblSec.pSec->m9101.cha_tmp_fumz8;   	       /* RTF tmp fumee Z8 */

  // Tube temp. RTH.
  long nuTh = pGEN->pHvm->phvm->meaSptCal.sectionsRTF[0].pZones[0]->pThermocouples[0]->pConfig->index;
  nuTh = pGEN->pHvm->phvm->meaSptCal.sectionsRTF[0].pZones[0]->pThermocouples[0]->pConfig->index; pMeasures->thermocouples[nuTh].temperature = gblSec.pSec->m9101.cha_tmp_t1z1 + 273.15;
	nuTh = pGEN->pHvm->phvm->meaSptCal.sectionsRTF[0].pZones[0]->pThermocouples[1]->pConfig->index; pMeasures->thermocouples[nuTh].temperature = gblSec.pSec->m9101.cha_tmp_t2z1 + 273.15;
	nuTh = pGEN->pHvm->phvm->meaSptCal.sectionsRTF[0].pZones[0]->pThermocouples[2]->pConfig->index; pMeasures->thermocouples[nuTh].temperature = gblSec.pSec->m9101.cha_tmp_t3z1 + 273.15;

	nuTh = pGEN->pHvm->phvm->meaSptCal.sectionsRTF[0].pZones[1]->pThermocouples[0]->pConfig->index; pMeasures->thermocouples[nuTh].temperature = gblSec.pSec->m9101.cha_tmp_t1z2 + 273.15;
	nuTh = pGEN->pHvm->phvm->meaSptCal.sectionsRTF[0].pZones[1]->pThermocouples[1]->pConfig->index; pMeasures->thermocouples[nuTh].temperature = gblSec.pSec->m9101.cha_tmp_t2z2 + 273.15;
	nuTh = pGEN->pHvm->phvm->meaSptCal.sectionsRTF[0].pZones[1]->pThermocouples[2]->pConfig->index; pMeasures->thermocouples[nuTh].temperature = gblSec.pSec->m9101.cha_tmp_t3z2 + 273.15;

	nuTh = pGEN->pHvm->phvm->meaSptCal.sectionsRTF[0].pZones[2]->pThermocouples[0]->pConfig->index; pMeasures->thermocouples[nuTh].temperature = gblSec.pSec->m9101.cha_tmp_t1z3 + 273.15;
	nuTh = pGEN->pHvm->phvm->meaSptCal.sectionsRTF[0].pZones[2]->pThermocouples[1]->pConfig->index; pMeasures->thermocouples[nuTh].temperature = gblSec.pSec->m9101.cha_tmp_t2z3 + 273.15;
	nuTh = pGEN->pHvm->phvm->meaSptCal.sectionsRTF[0].pZones[2]->pThermocouples[2]->pConfig->index; pMeasures->thermocouples[nuTh].temperature = gblSec.pSec->m9101.cha_tmp_t3z3 + 273.15;

	nuTh = pGEN->pHvm->phvm->meaSptCal.sectionsRTF[0].pZones[3]->pThermocouples[0]->pConfig->index; pMeasures->thermocouples[nuTh].temperature = gblSec.pSec->m9101.cha_tmp_t1z4 + 273.15;
	nuTh = pGEN->pHvm->phvm->meaSptCal.sectionsRTF[0].pZones[3]->pThermocouples[1]->pConfig->index; pMeasures->thermocouples[nuTh].temperature = gblSec.pSec->m9101.cha_tmp_t2z4 + 273.15;
	nuTh = pGEN->pHvm->phvm->meaSptCal.sectionsRTF[0].pZones[3]->pThermocouples[2]->pConfig->index; pMeasures->thermocouples[nuTh].temperature = gblSec.pSec->m9101.cha_tmp_t3z4 + 273.15;

	nuTh = pGEN->pHvm->phvm->meaSptCal.sectionsRTF[0].pZones[4]->pThermocouples[0]->pConfig->index; pMeasures->thermocouples[nuTh].temperature = gblSec.pSec->m9101.cha_tmp_t1z5 + 273.15;
	nuTh = pGEN->pHvm->phvm->meaSptCal.sectionsRTF[0].pZones[4]->pThermocouples[1]->pConfig->index; pMeasures->thermocouples[nuTh].temperature = gblSec.pSec->m9101.cha_tmp_t2z5 + 273.15;
	nuTh = pGEN->pHvm->phvm->meaSptCal.sectionsRTF[0].pZones[4]->pThermocouples[2]->pConfig->index; pMeasures->thermocouples[nuTh].temperature = gblSec.pSec->m9101.cha_tmp_t3z5 + 273.15;

	nuTh = pGEN->pHvm->phvm->meaSptCal.sectionsRTF[0].pZones[5]->pThermocouples[0]->pConfig->index; pMeasures->thermocouples[nuTh].temperature = gblSec.pSec->m9101.cha_tmp_t1z6 + 273.15;
	nuTh = pGEN->pHvm->phvm->meaSptCal.sectionsRTF[0].pZones[5]->pThermocouples[1]->pConfig->index; pMeasures->thermocouples[nuTh].temperature = gblSec.pSec->m9101.cha_tmp_t2z6 + 273.15;
	nuTh = pGEN->pHvm->phvm->meaSptCal.sectionsRTF[0].pZones[5]->pThermocouples[2]->pConfig->index; pMeasures->thermocouples[nuTh].temperature = gblSec.pSec->m9101.cha_tmp_t3z6 + 273.15;

	nuTh = pGEN->pHvm->phvm->meaSptCal.sectionsRTF[0].pZones[6]->pThermocouples[0]->pConfig->index; pMeasures->thermocouples[nuTh].temperature = gblSec.pSec->m9101.cha_tmp_t1z7 + 273.15;
	nuTh = pGEN->pHvm->phvm->meaSptCal.sectionsRTF[0].pZones[6]->pThermocouples[1]->pConfig->index; pMeasures->thermocouples[nuTh].temperature = gblSec.pSec->m9101.cha_tmp_t2z7 + 273.15;
	nuTh = pGEN->pHvm->phvm->meaSptCal.sectionsRTF[0].pZones[6]->pThermocouples[2]->pConfig->index; pMeasures->thermocouples[nuTh].temperature = gblSec.pSec->m9101.cha_tmp_t3z7 + 273.15;

	nuTh = pGEN->pHvm->phvm->meaSptCal.sectionsRTF[0].pZones[7]->pThermocouples[0]->pConfig->index; pMeasures->thermocouples[nuTh].temperature = gblSec.pSec->m9101.cha_tmp_t1z8 + 273.15;
	nuTh = pGEN->pHvm->phvm->meaSptCal.sectionsRTF[0].pZones[7]->pThermocouples[1]->pConfig->index; pMeasures->thermocouples[nuTh].temperature = gblSec.pSec->m9101.cha_tmp_t2z8 + 273.15;
	nuTh = pGEN->pHvm->phvm->meaSptCal.sectionsRTF[0].pZones[7]->pThermocouples[2]->pConfig->index; pMeasures->thermocouples[nuTh].temperature = gblSec.pSec->m9101.cha_tmp_t3z8 + 273.15;

  // Tube temp. SOA.
  nuTh = pGEN->pHvm->phvm->meaSptCal.sectionsRTF[1].pZones[0]->pThermocouples[0]->pConfig->index; pMeasures->thermocouples[nuTh].temperature = gblSec.pSec->m9101.fMaintienTempTubeZ1_1 + 273.15;
	nuTh = pGEN->pHvm->phvm->meaSptCal.sectionsRTF[1].pZones[0]->pThermocouples[1]->pConfig->index; pMeasures->thermocouples[nuTh].temperature = gblSec.pSec->m9101.fMaintienTempTubeZ1_2 + 273.15;
	nuTh = pGEN->pHvm->phvm->meaSptCal.sectionsRTF[1].pZones[0]->pThermocouples[2]->pConfig->index; pMeasures->thermocouples[nuTh].temperature = gblSec.pSec->m9101.fMaintienTempTubeZ1_3 + 273.15;
  
  nuTh = pGEN->pHvm->phvm->meaSptCal.sectionsRTF[1].pZones[1]->pThermocouples[0]->pConfig->index; pMeasures->thermocouples[nuTh].temperature = gblSec.pSec->m9101.fMaintienTempTubeZ2_1 + 273.15;
	nuTh = pGEN->pHvm->phvm->meaSptCal.sectionsRTF[1].pZones[1]->pThermocouples[1]->pConfig->index; pMeasures->thermocouples[nuTh].temperature = gblSec.pSec->m9101.fMaintienTempTubeZ2_2 + 273.15;
	nuTh = pGEN->pHvm->phvm->meaSptCal.sectionsRTF[1].pZones[1]->pThermocouples[2]->pConfig->index; pMeasures->thermocouples[nuTh].temperature = gblSec.pSec->m9101.fMaintienTempTubeZ2_3 + 273.15;

  nuTh = pGEN->pHvm->phvm->meaSptCal.sectionsRTF[1].pZones[2]->pThermocouples[0]->pConfig->index; pMeasures->thermocouples[nuTh].temperature = gblSec.pSec->m9101.fMaintienTempTubeZ3_1 + 273.15;
	nuTh = pGEN->pHvm->phvm->meaSptCal.sectionsRTF[1].pZones[2]->pThermocouples[1]->pConfig->index; pMeasures->thermocouples[nuTh].temperature = gblSec.pSec->m9101.fMaintienTempTubeZ3_2 + 273.15;
	nuTh = pGEN->pHvm->phvm->meaSptCal.sectionsRTF[1].pZones[2]->pThermocouples[2]->pConfig->index; pMeasures->thermocouples[nuTh].temperature = gblSec.pSec->m9101.fMaintienTempTubeZ3_3 + 273.15;

  nuTh = pGEN->pHvm->phvm->meaSptCal.sectionsRTF[1].pZones[3]->pThermocouples[0]->pConfig->index; pMeasures->thermocouples[nuTh].temperature = gblSec.pSec->m9101.fMaintienTempTubeZ4_1 + 273.15;
	nuTh = pGEN->pHvm->phvm->meaSptCal.sectionsRTF[1].pZones[3]->pThermocouples[1]->pConfig->index; pMeasures->thermocouples[nuTh].temperature = gblSec.pSec->m9101.fMaintienTempTubeZ4_2 + 273.15;
	nuTh = pGEN->pHvm->phvm->meaSptCal.sectionsRTF[1].pZones[3]->pThermocouples[2]->pConfig->index; pMeasures->thermocouples[nuTh].temperature = gblSec.pSec->m9101.fMaintienTempTubeZ4_3 + 273.15;

  // Power demand SOA.
  long nuZoneInType = pGEN->pHvm->phvm->meaSptCal.sectionsRTF[1].pZones[0]->pConfig->indexType;
  pMeasures->zonesRTF[nuZoneInType].demandPercentage = gblSec.pSec->m9101.fMaintienChargeZ1;

  nuZoneInType = pGEN->pHvm->phvm->meaSptCal.sectionsRTF[1].pZones[1]->pConfig->indexType;
  pMeasures->zonesRTF[nuZoneInType].demandPercentage = gblSec.pSec->m9101.fMaintienChargeZ2;

  nuZoneInType = pGEN->pHvm->phvm->meaSptCal.sectionsRTF[1].pZones[2]->pConfig->indexType;
  pMeasures->zonesRTF[nuZoneInType].demandPercentage = gblSec.pSec->m9101.fMaintienChargeZ3;

  nuZoneInType = pGEN->pHvm->phvm->meaSptCal.sectionsRTF[1].pZones[3]->pConfig->indexType;
  pMeasures->zonesRTF[nuZoneInType].demandPercentage = gblSec.pSec->m9101.fMaintienChargeZ4;

	///* If the line is running */
	//if (gblSec.pSec->m9101.vitcen != 0.f)
	//{
	//    temp1 = gblSec.pSec->m9101.cha_tmp_t1z1 <= (TUBE_MAX_TEMP + 130.f - 273.f);                                  
	//    temp2 = gblSec.pSec->m9101.cha_tmp_t2z1 <= (TUBE_MAX_TEMP + 130.f - 273.f);                                  
	//    temp3 = gblSec.pSec->m9101.cha_tmp_t3z1 <= (TUBE_MAX_TEMP + 130.f - 273.f);                                  
	//    sps_entry_rtf.s_mea.tmp_tub[0] = 0.f;
	//    if (temp1) sps_entry_rtf.s_mea.tmp_tub[0] = max(sps_entry_rtf.s_mea.tmp_tub[0],gblSec.pSec->m9101.cha_tmp_t1z1);
	//    if (temp2) sps_entry_rtf.s_mea.tmp_tub[0] = max(sps_entry_rtf.s_mea.tmp_tub[0],gblSec.pSec->m9101.cha_tmp_t2z1);
	//    if (temp3) sps_entry_rtf.s_mea.tmp_tub[0] = max(sps_entry_rtf.s_mea.tmp_tub[0],gblSec.pSec->m9101.cha_tmp_t3z1);

	//	temp1 = gblSec.pSec->m9101.cha_tmp_t1z2 <= (TUBE_MAX_TEMP + 130.f - 273.f);
	//	temp2 = gblSec.pSec->m9101.cha_tmp_t2z2 <= (TUBE_MAX_TEMP + 130.f - 273.f);
	//	temp3 = gblSec.pSec->m9101.cha_tmp_t3z2 <= (TUBE_MAX_TEMP + 130.f - 273.f);
	//    sps_entry_rtf.s_mea.tmp_tub[1] = 0.f;
	//    if (temp1) sps_entry_rtf.s_mea.tmp_tub[1] = max(sps_entry_rtf.s_mea.tmp_tub[1],gblSec.pSec->m9101.cha_tmp_t1z2);   
	//    if (temp2) sps_entry_rtf.s_mea.tmp_tub[1] = max(sps_entry_rtf.s_mea.tmp_tub[1],gblSec.pSec->m9101.cha_tmp_t2z2);  
	//    if (temp3) sps_entry_rtf.s_mea.tmp_tub[1] = max(sps_entry_rtf.s_mea.tmp_tub[1],gblSec.pSec->m9101.cha_tmp_t3z2);

	//	temp1 = gblSec.pSec->m9101.cha_tmp_t1z3 <= (TUBE_MAX_TEMP + 130.f - 273.f);
	//	temp2 = gblSec.pSec->m9101.cha_tmp_t2z3 <= (TUBE_MAX_TEMP + 130.f - 273.f);
	//	temp3 = gblSec.pSec->m9101.cha_tmp_t3z3 <= (TUBE_MAX_TEMP + 130.f - 273.f);
	//    sps_entry_rtf.s_mea.tmp_tub[2] = 0.f;
	//    if (temp1) sps_entry_rtf.s_mea.tmp_tub[2] = max(sps_entry_rtf.s_mea.tmp_tub[2],gblSec.pSec->m9101.cha_tmp_t1z3);
	//    if (temp2) sps_entry_rtf.s_mea.tmp_tub[2] = max(sps_entry_rtf.s_mea.tmp_tub[2],gblSec.pSec->m9101.cha_tmp_t2z3);
	//    if (temp3) sps_entry_rtf.s_mea.tmp_tub[2] = max(sps_entry_rtf.s_mea.tmp_tub[2],gblSec.pSec->m9101.cha_tmp_t3z3);

	//	temp1 = gblSec.pSec->m9101.cha_tmp_t1z4 <= (TUBE_MAX_TEMP + 130.f - 273.f);
	//	temp2 = gblSec.pSec->m9101.cha_tmp_t2z4 <= (TUBE_MAX_TEMP + 130.f - 273.f);
	//	temp3 = gblSec.pSec->m9101.cha_tmp_t3z4 <= (TUBE_MAX_TEMP + 130.f - 273.f);
	//    sps_entry_rtf.s_mea.tmp_tub[3] = 0.f;
	//    if (temp1) sps_entry_rtf.s_mea.tmp_tub[3] = max(sps_entry_rtf.s_mea.tmp_tub[3],gblSec.pSec->m9101.cha_tmp_t1z4);
	//    if (temp2) sps_entry_rtf.s_mea.tmp_tub[3] = max(sps_entry_rtf.s_mea.tmp_tub[3],gblSec.pSec->m9101.cha_tmp_t2z4);
	//    if (temp3) sps_entry_rtf.s_mea.tmp_tub[3] = max(sps_entry_rtf.s_mea.tmp_tub[3],gblSec.pSec->m9101.cha_tmp_t3z4);

	//	temp1 = gblSec.pSec->m9101.cha_tmp_t1z5 <= (TUBE_MAX_TEMP + 130.f - 273.f);
	//	temp2 = gblSec.pSec->m9101.cha_tmp_t2z5 <= (TUBE_MAX_TEMP + 130.f - 273.f);
	//	temp3 = gblSec.pSec->m9101.cha_tmp_t3z5 <= (TUBE_MAX_TEMP + 130.f - 273.f);
	//    sps_entry_rtf.s_mea.tmp_tub[4] = 0.f;
	//    if (temp1) sps_entry_rtf.s_mea.tmp_tub[4] = max(sps_entry_rtf.s_mea.tmp_tub[4],gblSec.pSec->m9101.cha_tmp_t1z5);
	//    if (temp2) sps_entry_rtf.s_mea.tmp_tub[4] = max(sps_entry_rtf.s_mea.tmp_tub[4],gblSec.pSec->m9101.cha_tmp_t2z5);
	//    if (temp3) sps_entry_rtf.s_mea.tmp_tub[4] = max(sps_entry_rtf.s_mea.tmp_tub[4],gblSec.pSec->m9101.cha_tmp_t3z5);

	//	temp1 = gblSec.pSec->m9101.cha_tmp_t1z6 <= (TUBE_MAX_TEMP + 130.f - 273.f);
	//	temp2 = gblSec.pSec->m9101.cha_tmp_t2z6 <= (TUBE_MAX_TEMP + 130.f - 273.f);
	//	temp3 = gblSec.pSec->m9101.cha_tmp_t3z6 <= (TUBE_MAX_TEMP + 130.f - 273.f);
	//    sps_entry_rtf.s_mea.tmp_tub[5] = 0.f;
	//    if (temp1) sps_entry_rtf.s_mea.tmp_tub[5] = max(sps_entry_rtf.s_mea.tmp_tub[5],gblSec.pSec->m9101.cha_tmp_t1z6);
	//    if (temp2) sps_entry_rtf.s_mea.tmp_tub[5] = max(sps_entry_rtf.s_mea.tmp_tub[5],gblSec.pSec->m9101.cha_tmp_t2z6);
	//    if (temp3) sps_entry_rtf.s_mea.tmp_tub[5] = max(sps_entry_rtf.s_mea.tmp_tub[5],gblSec.pSec->m9101.cha_tmp_t3z6);

	//	temp1 = gblSec.pSec->m9101.cha_tmp_t1z7 <= (TUBE_MAX_TEMP + 130.f - 273.f);
	//	temp2 = gblSec.pSec->m9101.cha_tmp_t2z7 <= (TUBE_MAX_TEMP + 130.f - 273.f);
	//	temp3 = gblSec.pSec->m9101.cha_tmp_t3z7 <= (TUBE_MAX_TEMP + 130.f - 273.f);
	//    sps_entry_rtf.s_mea.tmp_tub[6] = 0.f;   
	//    if (temp1) sps_entry_rtf.s_mea.tmp_tub[6] = max(sps_entry_rtf.s_mea.tmp_tub[6],gblSec.pSec->m9101.cha_tmp_t1z7);
	//    if (temp2) sps_entry_rtf.s_mea.tmp_tub[6] = max(sps_entry_rtf.s_mea.tmp_tub[6],gblSec.pSec->m9101.cha_tmp_t2z7);
	//    if (temp3) sps_entry_rtf.s_mea.tmp_tub[6] = max(sps_entry_rtf.s_mea.tmp_tub[6],gblSec.pSec->m9101.cha_tmp_t3z7);

	//	temp1 = gblSec.pSec->m9101.cha_tmp_t1z8 <= (TUBE_MAX_TEMP + 130.f - 273.f);
	//	temp2 = gblSec.pSec->m9101.cha_tmp_t2z8 <= (TUBE_MAX_TEMP + 130.f - 273.f);
	//	temp3 = gblSec.pSec->m9101.cha_tmp_t3z8 <= (TUBE_MAX_TEMP + 130.f - 273.f);
	//    sps_entry_rtf.s_mea.tmp_tub[7] = 0.f;
	//    if (temp1) sps_entry_rtf.s_mea.tmp_tub[7] = max(sps_entry_rtf.s_mea.tmp_tub[7],gblSec.pSec->m9101.cha_tmp_t1z8);
	//    if (temp2) sps_entry_rtf.s_mea.tmp_tub[7] = max(sps_entry_rtf.s_mea.tmp_tub[7],gblSec.pSec->m9101.cha_tmp_t2z8);
	//    if (temp3) sps_entry_rtf.s_mea.tmp_tub[7] = max(sps_entry_rtf.s_mea.tmp_tub[7],gblSec.pSec->m9101.cha_tmp_t3z8);

	//    for (i_zon = 0;i_zon < NB_ZON_RTF;i_zon++)
	//	sps_entry_rtf.s_mea.tmp_tub[i_zon] += 273.f;

	//    	sps_entry_rtf.s_mea.tmp_zon[0] = 273.f + gblSec.pSec->m9101.cha_tmp_z1;
	//	sps_entry_rtf.s_mea.tmp_zon[1] = 273.f + gblSec.pSec->m9101.cha_tmp_z2;
	//	sps_entry_rtf.s_mea.tmp_zon[2] = 273.f + gblSec.pSec->m9101.cha_tmp_z3;
	//	sps_entry_rtf.s_mea.tmp_zon[3] = 273.f + gblSec.pSec->m9101.cha_tmp_z4;
	//	sps_entry_rtf.s_mea.tmp_zon[4] = 273.f + gblSec.pSec->m9101.cha_tmp_z5;
	//	sps_entry_rtf.s_mea.tmp_zon[5] = 273.f + gblSec.pSec->m9101.cha_tmp_z6;
	//	sps_entry_rtf.s_mea.tmp_zon[6] = 273.f + gblSec.pSec->m9101.cha_tmp_z7;
	//	sps_entry_rtf.s_mea.tmp_zon[7] = 273.f + gblSec.pSec->m9101.cha_tmp_z8; 
	//	    	
	//}
	///* The line is stopped */
	//else
	//{
	//	sps_entry_rtf.s_mea.tmp_tub[0] = 273.f + gblSec.pSec->m9101.cha_tmp_z1;
	//	sps_entry_rtf.s_mea.tmp_tub[1] = 273.f + gblSec.pSec->m9101.cha_tmp_z2;
	//	sps_entry_rtf.s_mea.tmp_tub[2] = 273.f + gblSec.pSec->m9101.cha_tmp_z3;
	//	sps_entry_rtf.s_mea.tmp_tub[3] = 273.f + gblSec.pSec->m9101.cha_tmp_z4;
	//	sps_entry_rtf.s_mea.tmp_tub[4] = 273.f + gblSec.pSec->m9101.cha_tmp_z5;
	//	sps_entry_rtf.s_mea.tmp_tub[5] = 273.f + gblSec.pSec->m9101.cha_tmp_z6;
	//	sps_entry_rtf.s_mea.tmp_tub[6] = 273.f + gblSec.pSec->m9101.cha_tmp_z7;
	//	sps_entry_rtf.s_mea.tmp_tub[7] = 273.f + gblSec.pSec->m9101.cha_tmp_z8;
	//}

	///* For all RTF zones */
	//for (i_zon=0; i_zon<NB_ZON_RTF; i_zon++)
	//{
	//	sps_entry_rtf.s_mea.on_zone[i_zon] = true;
	//}

	/* ------------------- */
	/* JET mea.            */
	/* ------------------- */
  pMeasures->pyrometers[2].temperature = gblSec.pSec->m9101.tmprefutil + 273.15;      /* JET strip tmp. mea. */
	pMeasures->zonesJET[0].demandPercentage = ( gblSec.pSec->m9101.ref_cha_z1av + gblSec.pSec->m9101.ref_cha_z1ar ) / 2.;
	pMeasures->zonesJET[1].demandPercentage = ( gblSec.pSec->m9101.ref_cha_z2av + gblSec.pSec->m9101.ref_cha_z2ar ) / 2.;
	pMeasures->zonesJET[2].demandPercentage = ( gblSec.pSec->m9101.ref_cha_z3av + gblSec.pSec->m9101.ref_cha_z3ar ) / 2.;

	//sps_entry_cool.s_mea.tmp_stp_jet = gblSec.pSec->m9101.tmprefutil + 273.f;      /* JET strip tmp. mea. */
	//sps_entry_cool.s_mea.pow_pct_jet[0] = (gblSec.pSec->m9101.ref_cha_z1av + gblSec.pSec->m9101.ref_cha_z1ar) / 2.f;
	//sps_entry_cool.s_mea.pow_pct_jet[1] = (gblSec.pSec->m9101.ref_cha_z2av + gblSec.pSec->m9101.ref_cha_z2ar) / 2.f;
	//sps_entry_cool.s_mea.pow_pct_jet[2] = (gblSec.pSec->m9101.ref_cha_z3av + gblSec.pSec->m9101.ref_cha_z3ar) / 2.f;

	/* ------------------- */
	/* Speed limitation    */
	/* ------------------- */

	//s_spe_mar.s_mea.V_mac_Alia = gblSec.pSec->m9101.vitmaxalia / 60.f;       /* Maximum aliation speed */
	//s_spe_mar.s_mea.V_mac_ope = gblSec.pSec->m9101.vitmaxope / 60.f;         /* Maximum operator speed */
  
	alliationMaxSpeed                                   = gblSec.pSec->m9101.vitmaxalia / 60.;          /* Maximum aliation speed */
	pMeasures->line.operatorSpeedLimit = gblSec.pSec->m9101.vitmaxope  / 60.;          /* Maximum operator speed */

	/* ------------------- */
	/* Weld position       */
	/* ------------------- */

	/* coils weld position */
	//for (i_coil=0; i_coil<s_coil.nb_coil; i_coil++)
	//	Weld_Pos[i_coil] = gblSec.pSec->m9100[i_coil].posqb_sormain - LGT_P3_P4 + 100.f;    // #define LGT_P3_P4   106.238f 

  // Store first 2 welds and ID.
  pMeasures->line.weldPositions[0] = gblSec.pSec->m9100[0].posqb_sormain + 95. - 15.; //- 106.238 + 100.; // 75 meters retrieve modification Avec Domi V. 30/06/2021
  pMeasures->line.weldPositions[1] = gblSec.pSec->m9100[1].posqb_sormain + 95. - 15.; //- 106.238 + 100.; // 75 meters retrieve modification Avec Domi V. 30/06/2021
  pMeasures->line.weldPositions[2] = gblSec.pSec->m9100[2].posqb_sormain + 95. - 15.; //- 106.238 + 100.; // 75 meters retrieve modification Avec Domi V. 30/06/2021
  strncpy_s( pMeasures->line.entryIdentities[0], gblSec.pSec->m9100[0].nobob, 8 );
  strncpy_s( pMeasures->line.entryIdentities[1], gblSec.pSec->m9100[1].nobob, 8 );

	///* Find first coil in queue */
	//loop = 0;
	//idx = -1;
	//do
	//{
	//	idx ++;
	//	loop ++;
	//} while (Weld_Pos[idx] < 0 && loop<MAX_ITER);

	///* Check if first coil in queue has changed */
	//if (strncmp(s_coil.s_coil_data[0].id, gblSec.pSec->m9100[idx].nobob,8) != 0 &&
	//     strcmp(s_coil.s_coil_data[0].id, "") != 0)
	//{
	//    /* Initialize Anticipation time boolean */
	//    Anticip = false;
 //
	//    s_coil.s_coil_data_last = s_coil.s_coil_data[0];
	//    for ( i_coil=idx;i_coil<s_coil.nb_coil;i_coil++ )
	//    {
	//    	if (i_coil+idx > s_coil.nb_coil - 1)
	//    	{
	//    	/* raz infos dernier numero de bobine */
	//    	s_coil.s_coil_data[i_coil].thi = 0.f;
	//    	s_coil.s_coil_data[i_coil].wid = 0.f;
	//    	s_coil.s_coil_data_old[i_coil] = s_coil.s_coil_data[i_coil];
	//    	}
	//    	else	    	 
	//    	{
	//       	    s_coil.s_coil_data[i_coil-idx] = s_coil.s_coil_data[i_coil];
	//    	    s_coil.s_coil_data_old[i_coil-idx] = s_coil.s_coil_data[i_coil]; 

	//    	    if (!s_coil.s_coil_data[i_coil - idx].valid)
 //           	    {   
 //           	    	/*trans_result[i_coil-idx].spd = 0.f;*/ 
 //           	    	/*trans_result[i_coil-idx].tmp_stp_rtf = 0.f + 273.f;*/  
 //           	    	/*trans_result[i_coil-idx].tmp_tub_ref = 0.f + 273.f;*/
 //           	    }
 //           	    else   
	//    	    	trans_result[i_coil-idx] = trans_result[i_coil];
	//    	}   
	//    }
	//    s_coil.s_coil_data[2].Trs_cal = false;
	//    Print_Traces_File("Depillage: %s --> %s", s_coil.s_coil_data[0].id, gblSec.pSec->m9100[idx].nobob);                            
	//    time(&time_weld2);
	//}

	/* Store Line speed */
	//sps_entry_rtf.s_mea.spd = gblSec.pSec->m9101.vitcen;
	//sps_entry_rtf.s_mea.spd /= 60.f;
	//sps_entry_cool.s_mea.spd = gblSec.pSec->m9101.vitcen;
	//sps_entry_cool.s_mea.spd /= 60.f;
  pMeasures->line.speed = gblSec.pSec->m9101.vitcen / 60.;
  //pMeasures->line.speed = 0.;

	/* Store weld position */
	//s_coil.s_coil_data[0].head_weld_pos = gblSec.pSec->m9100[idx].posqb_sormain;
	//sps_entry_rtf.s_mea.lgt_rtf = Weld_Pos[idx];	    /* gblSec.pSec->m9100[idx].posqb_sormain; */
	//if (sps_entry_rtf.s_mea.spd != 0.f)
	//	sps_entry_rtf.s_mea.time_rtf = sps_entry_rtf.s_mea.lgt_rtf / sps_entry_rtf.s_mea.spd;
	//else
	//	sps_entry_rtf.s_mea.time_rtf = 10000.f;

	///* JET cooling weld position  */
	//if (idx == 0)
	//	sps_entry_cool.s_mea.lgt_jet = gblSec.pSec->m9100[idx].posqb_sormain;
	//else
	//	sps_entry_cool.s_mea.lgt_jet = gblSec.pSec->m9100[idx-1].posqb_sormain;

	//if (sps_entry_cool.s_mea.spd != 0.f)
	//	sps_entry_cool.s_mea.time_jet = sps_entry_cool.s_mea.lgt_jet / sps_entry_cool.s_mea.spd;
	//else
	//	sps_entry_cool.s_mea.time_jet = 10000.f;

	/* ------------------- */
	/* Coil queue          */
	/* ------------------- */

  for ( long iCoil = 0; iCoil < DIM_COILS; iCoil++ )
  {
    memset( &pMeasures->coils[iCoil], 0, sizeof( struct  MOD_HV_coil_data_simulation ) );
		strncpy ( pMeasures->coils[iCoil].entryIdentity, gblSec.pSec->m9100[iCoil].nobob, 8 );

    pMeasures->coils[iCoil].thickness = gblSec.pSec->m9100[iCoil].epais / 1000000.;
    pMeasures->coils[iCoil].width     = gblSec.pSec->m9100[iCoil].larg  / 1000.;
    pMeasures->coils[iCoil].length    = gblSec.pSec->m9100[iCoil].longueur;
    
    pMeasures->coils[iCoil].sections[0].stripTemp.target = pMOD->pConv->kelvin( gblSec.pSec->m9100[iCoil].tbvischa );
    pMeasures->coils[iCoil].sections[0].stripTemp.lower  = pMOD->pConv->kelvin( gblSec.pSec->m9100[iCoil].tbmincha );
    pMeasures->coils[iCoil].sections[0].stripTemp.upper  = pMOD->pConv->kelvin( gblSec.pSec->m9100[iCoil].tbmaxcha );
    pMeasures->coils[iCoil].sections[0].stripTemp.target += gblSec.pSec->m9101.offset_tmp;

    pMeasures->coils[iCoil].sections[1].stripTemp.target = pMOD->pConv->kelvin( gblSec.pSec->m9100[iCoil].tbvismai );
    pMeasures->coils[iCoil].sections[1].stripTemp.lower  = pMOD->pConv->kelvin( gblSec.pSec->m9100[iCoil].tbminmai );
    pMeasures->coils[iCoil].sections[1].stripTemp.upper  = pMOD->pConv->kelvin( gblSec.pSec->m9100[iCoil].tbmaxmai );
    pMeasures->coils[iCoil].sections[1].stripTemp.target += gblSec.pSec->m9101.offset_tmp;
    
    pMeasures->coils[iCoil].sections[2].stripTemp.target = pMOD->pConv->kelvin( gblSec.pSec->m9100[iCoil].tbvisref );
    pMeasures->coils[iCoil].sections[2].stripTemp.lower  = pMOD->pConv->kelvin( gblSec.pSec->m9100[iCoil].tbminref );
    pMeasures->coils[iCoil].sections[2].stripTemp.upper  = pMOD->pConv->kelvin( gblSec.pSec->m9100[iCoil].tbmaxref );
    
		strcpy_s( pMeasures->coils[iCoil].steelGrade      , gblSec.pSec->m9100[iCoil].typeacier );   /* Steel code */
		strcpy_s( pMeasures->coils[iCoil].specific.sGiOrGa, gblSec.pSec->m9100[iCoil].Produit   );   /* Produit */
    if      ( strncmp( pMeasures->coils[iCoil].specific.sGiOrGa, "GI", 2 ) == 0 )
      pMeasures->coils[iCoil].specific.isGAElseGi = false;
    else if ( strncmp( pMeasures->coils[iCoil].specific.sGiOrGa, "GA", 2 ) == 0 )
      pMeasures->coils[iCoil].specific.isGAElseGi = true;
    else
      pMeasures->coils[iCoil].specific.isGAElseGi = false;

    pMeasures->coils[iCoil].maxSpeedQuality            = gblSec.pSec->m9100[iCoil].vmax  / 60.;
    pMeasures->coils[iCoil].specific.alliationMaxSpeed = gblSec.pSec->m9101.vitmaxalia   / 60.;

    pMeasures->coils[iCoil].maxSpeedQuality = min( pMeasures->coils[iCoil].maxSpeedQuality, 
                                                   pMeasures->coils[iCoil].specific.alliationMaxSpeed );

  }
  
  //char traceTempo [DIM_TRACES + 1] = {0};
  sprintf_s( traceTempo, "vitmaxalia = %f m/min, iModelUsed = %d, vmax 0 = %f m/min, vmax 1 = %f m/min, vmax 2 = %f m/min "
                        , gblSec.pSec->m9101.vitmaxalia
                        , gblSec.pSec->m9101.iModelUsed
                        , gblSec.pSec->m9100[0].vmax
                        , gblSec.pSec->m9100[1].vmax
                        , gblSec.pSec->m9100[2].vmax
                        );
  pMOD->pUtl->writeTrace( traceFilePointer      // Entry: File pointer.
                        , traceTempo            // Entry: Trace string.
                        );

  // Server connected.
  //gblSec.pSec->m9101.iModelUsed = 1;
  pMeasures->serverConnected = gblSec.pSec->m9101.iModelUsed;
  //pMeasures->serverConnected = 0;

	///* For all coils */
	//for (i_coil=0; i_coil<s_coil.nb_coil - idx; i_coil++ )
	//{
		//s_coil.s_coil_data[i_coil].id[0] =  '\0';   	    	    	    	    /* Coil number */
		//strncpy ( s_coil.s_coil_data[i_coil].id, gblSec.pSec->m9100[idx + i_coil].nobob, 8 );
		//s_coil.s_coil_data[i_coil].thi = gblSec.pSec->m9100[idx + i_coil].epais;             /* strip thickness */
		//s_coil.s_coil_data[i_coil].thi /= 1000000.f;
		//s_coil.s_coil_data[i_coil].lgt = gblSec.pSec->m9100[idx + i_coil].longueur;          /* strip length */
		//s_coil.s_coil_data[i_coil].wid = gblSec.pSec->m9100[idx + i_coil].larg;              /* strip width */
		//s_coil.s_coil_data[i_coil].wid /= 1000.f;
		//s_coil.s_coil_data[i_coil].tmp_in = 20.f + 273.f;              /* entry temperature */
		//s_coil.s_coil_data[i_coil].offset_tmp = gblSec.pSec->m9101.offset_tmp;
		//s_coil.s_coil_data[i_coil].tmp_low_rtf = gblSec.pSec->m9100[idx + i_coil].tbmincha;  /* lower tgt RTF */
		//s_coil.s_coil_data[i_coil].tmp_low_rtf += 273.f;
		//s_coil.s_coil_data[i_coil].tmp_upp_rtf = gblSec.pSec->m9100[idx + i_coil].tbmaxcha;  /* upper tgt RTF */
		//s_coil.s_coil_data[i_coil].tmp_upp_rtf += 273.f;
		//s_coil.s_coil_data[i_coil].tmp_tgt_rtf = gblSec.pSec->m9100[idx + i_coil].tbvischa;  /* tgt RTF */
		//s_coil.s_coil_data[i_coil].tmp_tgt_rtf += 273.f;
	 //   	/* Sur bobine en cours ou suivante */
	 //   	if (i_coil == 0 || i_coil == 1 || i_coil == 2)                   
		//    s_coil.s_coil_data[i_coil].tmp_tgt_rtf += gblSec.pSec->m9101.offset_tmp;
		//s_coil.s_coil_data[i_coil].tmp_low_jet = gblSec.pSec->m9100[idx + i_coil].tbminref;  /* lower tgt JET */
		//s_coil.s_coil_data[i_coil].tmp_low_jet += 273.f;
		//s_coil.s_coil_data[i_coil].tmp_upp_jet = gblSec.pSec->m9100[idx + i_coil].tbmaxref;  /* upper tgt JET */
		//s_coil.s_coil_data[i_coil].tmp_upp_jet += 273.f;
		//s_coil.s_coil_data[i_coil].tmp_tgt_jet = gblSec.pSec->m9100[idx + i_coil].tbvisref;  /* tgt JET */
		//s_coil.s_coil_data[i_coil].tmp_tgt_jet += 273.f;
		//s_coil.s_coil_data[i_coil].s_spe.Tmp_stp_soa = gblSec.pSec->m9100[idx + i_coil].tbvismai;
		//s_coil.s_coil_data[i_coil].s_spe.Tmp_stp_soa += 273.f;
	 //   	/* Sur bobine en cours ou suivante */
	 //   	if (i_coil == 0 || i_coil == 1 || i_coil == 2)                   
		//    s_coil.s_coil_data[i_coil].s_spe.Tmp_stp_soa += gblSec.pSec->m9101.offset_tmp;
		//s_coil.s_coil_data[i_coil].s_spe.Tmp_upp_soa = gblSec.pSec->m9100[idx + i_coil].tbmaxmai;
		//s_coil.s_coil_data[i_coil].s_spe.Tmp_upp_soa += 273.f;
		//s_coil.s_coil_data[i_coil].s_spe.Tmp_low_soa = gblSec.pSec->m9100[idx + i_coil].tbminmai;
		//s_coil.s_coil_data[i_coil].s_spe.Tmp_low_soa += 273.f;
		//strcpy(s_coil.s_coil_data[i_coil].s_spe.Steel_code, gblSec.pSec->m9100[idx + i_coil].typeacier);   /* Steel code */
		//s_coil.s_coil_data[i_coil].emi_rtf = Spe_Ktl_emis_search (i_coil);      /* Emissivity */
		//s_coil.s_coil_data[i_coil].Bisra = 2;                          /* Bisra code */
		//s_coil.s_coil_data[i_coil].Cool_rate[0] = gblSec.pSec->m9100[idx + i_coil].rampz1;   /* Cooling ramp Z1 */
		//s_coil.s_coil_data[i_coil].Cool_rate[1] = gblSec.pSec->m9100[idx + i_coil].rampz2;   /* Cooling ramp Z2 */
		//s_coil.s_coil_data[i_coil].Cool_rate[2] = gblSec.pSec->m9100[idx + i_coil].rampz3;   /* Cooling ramp Z3 */
		//s_coil.s_coil_data[i_coil].dummy = 0;                          /* No dummy coil */
		//s_coil.s_coil_data[i_coil].head_weld_pos = gblSec.pSec->m9100[idx + i_coil].posqb_sormain - LGT_P3_P4 + 100.; /* Weld pos */

		//// If next coil emissivity class is not known. We copy current emissivity in it.
		//if (s_coil.s_coil_data[i_coil].N_cla == MAX_NB_CLA - 1 && i_coil == 1)
		//{
		//	s_adp.s_pdt_RTF[s_coil.s_coil_data[i_coil].N_cla][0].alpha = s_adp.s_pdt_RTF[s_coil.s_coil_data[i_coil - 1].N_cla][0].alpha;
		//	s_coil.s_coil_data[i_coil].emi_rtf = s_adp.s_pdt_RTF[s_coil.s_coil_data[i_coil].N_cla][0].alpha * EMI_COIL;
		//}
		//
		//// We securise the second coil to be at +/- 0.03 on the first coil emissivity.
		//if ( i_coil == 1 )
		//{
		//	s_adp.s_pdt_RTF[s_coil.s_coil_data[i_coil].N_cla][0].alpha = min(max(	s_adp.s_pdt_RTF[s_coil.s_coil_data[i_coil    ].N_cla][0].alpha, 
		//																			s_adp.s_pdt_RTF[s_coil.s_coil_data[i_coil - 1].N_cla][0].alpha - 0.03), 
		//																			s_adp.s_pdt_RTF[s_coil.s_coil_data[i_coil - 1].N_cla][0].alpha + 0.03);
		//	s_coil.s_coil_data[i_coil].emi_rtf = s_adp.s_pdt_RTF[s_coil.s_coil_data[i_coil].N_cla][0].alpha * EMI_COIL;
		//}
		
		/* ==================================================
	    		 SPEED MANAGEMENT
		   ================================================== */

	    	///* Consigne en vitesse rampée */
	    	//if (!sps_entry_rtf.s_mea.spd_auto)             
	    	//    Spd_spt = sps_entry_rtf.s_mea.spd;                                            
      //          
	    	///* Rampe sur la consigne en vitesse opérateur */
	    	//vit_max_ope = gblSec.pSec->m9101.vitmaxope / 60.f;                              
	    	//if (Spd_spt != vit_max_ope)    
	    	//{                  
	    	//    if (strncmp(gblSec.pSec->m9100[idx + i_coil].Produit, "GI", 2) == 0)
	    	//    	spd_ramp = 150.f / 60.f;                    
	    	//    else if (strncmp(gblSec.pSec->m9100[idx + i_coil].Produit, "GA", 2) == 0)
	    	//    	spd_ramp = 150.f / 60.f;  
	    	//    else    
	    	//    	spd_ramp = 150.f / 60.f;      

	    	//    if ( vit_max_ope < Spd_spt)
	    	//    {
	    	//    	if (sps_entry_rtf.s_mea.spd < Spd_spt + 2.f / 60.f)
	    	//    	    Spd_spt = max(min(Spd_spt - spd_ramp, sps_entry_rtf.s_mea.spd - spd_ramp), vit_max_ope);
	    	//    }
	    	//    else if (vit_max_ope > Spd_spt)
	    	//    {
	    	//    	if (sps_entry_rtf.s_mea.spd > Spd_spt - 2.f / 60.f)                                           
	    	//    	    Spd_spt = min(max(Spd_spt + spd_ramp, sps_entry_rtf.s_mea.spd + spd_ramp), vit_max_ope);
	    	//    }                                                                                               
	    	//}

		///* Coil maximum speed */
		//s_coil.s_coil_data[i_coil].spd_max = gblSec.pSec->m9100[idx + i_coil].vmax / 60.f;         /* Maximum speed */
	 //   	s_coil.s_coil_data[i_coil].Vmax_boballia = gblSec.pSec->m9100[idx + i_coil].vmax / 60.f;
	 //   	s_coil.s_coil_data[i_coil].Vmax_boballia = 
	 //   	    	min(s_coil.s_coil_data[i_coil].Vmax_boballia, gblSec.pSec->m9101.vitmaxalia / 60.f);                                 
	 //   	/* Sur bobine en cours ou suivante */                                          
	 //   	if (i_coil == 0 || i_coil == 1 || i_coil == 2)                   
		//    s_coil.s_coil_data[i_coil].spd_max =             
		//	    min (s_coil.s_coil_data[i_coil].spd_max, gblSec.pSec->m9101.vitmaxope / 60.f); /* Speed lim. operator */
		//s_coil.s_coil_data[i_coil].spd_max =                                                     
		//	min (s_coil.s_coil_data[i_coil].spd_max, gblSec.pSec->m9101.vitmaxalia / 60.f);/* Speed lim. aliation */

		//s_coil.s_coil_data[i_coil].spd_dec = 0.f;                      /* Speed limitation */
		//s_coil.s_coil_data[i_coil].spd_min = MIN_SPEED;                /* Minimum speed */

		///* if speed set in automatic */
		//if (sps_entry_rtf.s_mea.spd_auto)
		//{
		//	/*s_coil.s_coil_data[i_coil].spd  = s_coil.s_coil_data[i_coil].spd_max;*/
		//}
		//else /* speed in manuel */
		//{
		//	/* If first coil in queue */
		//	if (i_coil == 0 || i_coil == 1 || i_coil == 2)      
		//	{
	 //   	    	    if (sps_entry_rtf.s_mea.spd > MIN_SPEED)
	 //   	    	    {
		//		s_coil.s_coil_data[i_coil].spd_max = sps_entry_rtf.s_mea.spd;
		//		s_coil.s_coil_data[i_coil].spd	   = sps_entry_rtf.s_mea.spd;
		//		s_coil.s_coil_data[i_coil].spd_dec = sps_entry_rtf.s_mea.spd;
	 //   	    	    }
	 //   	    	    else
	 //   	    	    {
		//		s_coil.s_coil_data[i_coil].spd	   = s_coil.s_coil_data[i_coil].spd_max;
		//		s_coil.s_coil_data[i_coil].spd_dec = s_coil.s_coil_data[i_coil].spd_max;           
	 //   	    	    }
		//		s_coil.s_coil_data[i_coil].spd_min = min ( s_coil.s_coil_data[i_coil].spd_min,
	 //   	    	    	    	    	    	    	    	   s_coil.s_coil_data[i_coil].spd_max );
		//	} /*  If first coil in queue*/
		//}/* speed in manuel */

	    ///* Gestion des rampes de vitesses */
	    ///* ============================== */
	    //if (i_coil == 0)                          
	    //{
	    //	if (strncmp(gblSec.pSec->m9100[idx + i_coil].Produit, "GI", 2) == 0)
	    //	    Coef_ramp = 3.0f;  
	    //	else if (strncmp(gblSec.pSec->m9100[idx + i_coil].Produit, "GA", 2) == 0)
	    //	    Coef_ramp = 0.8f;
	    //	else
	    //	    Coef_ramp = 3.0f;                                       
	    //}                                                                   
//
//} /* For all coils */

	// Release access to global section.
	gblSec.unLockAccess();


  return;
}

  
//============================================================================
// SPE_specific: writeSpecificCommunication
//
//    We write specifically the setpoint. Replacement regular communication.
//============================================================================
void SPE_specific::writeSpecificCommunication              ( FILE **traceFilePointer, struct MOD_HV_setpoints *   pSetpoints, struct GEN_utility *  pGEN  )
{
  char traceTempo [DIM_TRACES + 1] = {0};

  // If V-iModel used.
  if (gblSec.pSec->m9101.iModelUsed == 1)
  {
    // Secure access to global section.
	  gblSec.lockAccess();

	  //for ( i_coil = 0; i_coil<s_coil.nb_coil; i_coil++ )
	  //{
	  //    for ( i_coil_gbl = 0; i_coil_gbl<8; i_coil_gbl++ )
	  //    {
	  //    	if ( strncmp ( gblSec.pSec->m9100[i_coil_gbl].nobob, 
	  //    	    	       s_coil.s_coil_data[i_coil].id, 8 ) == 0 )
	  //    	{
	  //    	    gblSec.pSec->m9100[i_coil_gbl].vMaxLTOP =
	  //    	    	s_coil.s_coil_data[i_coil].spd_max_th * 60.f;
 	  //    	    break;
	  //    	}
	  //    } 
	  //}

    for ( long iCoil = 0; iCoil < DIM_COILS; iCoil++ )
    {
      for ( long jCoil = 0; jCoil < DIM_COILS; jCoil++ )
      {
        if ( strncmp (  gblSec.pSec->m9100[jCoil].nobob, 
                        pGEN->pHvm->phvm->coilQueue.coils[iCoil].data.entryIdentity, 8 ) == 0 )
        {
          gblSec.pSec->m9100[jCoil].vMaxLTOP = pGEN->pHvm->phvm->coilQueue.coils[iCoil].calcul.maximumLineSpeed * 60.;
          break;
        }
      }
    }

	  /* RTF control mode*/
	  //gblSec.pSec->mcons.mode_ctrl = sps_act_sp_rtf.ctl_pow;
    gblSec.pSec->mcons.mode_ctrl = pGEN->pHvm->phvm->meaSptCal.sectionsRTF[0].pSetpoint->controlType;

	  /* RTF set points validity*/
	  //gblSec.pSec->mcons.fou_cons_valid = sps_act_sp_rtf.Valid;
    gblSec.pSec->mcons.fou_cons_valid = pGEN->pHvm->phvm->meaSptCal.sectionsRTF[0].pSetpoint->valid;

	  /* RTF powers*/
	  //gblSec.pSec->mcons.cha_cons_chargz1 = sps_act_sp_rtf.pow_pct[0];
	  //gblSec.pSec->mcons.cha_cons_chargz2 = sps_act_sp_rtf.pow_pct[1];
	  //gblSec.pSec->mcons.cha_cons_chargz3 = sps_act_sp_rtf.pow_pct[2];
	  //gblSec.pSec->mcons.cha_cons_chargz4 = sps_act_sp_rtf.pow_pct[3];
	  //gblSec.pSec->mcons.cha_cons_chargz5 = sps_act_sp_rtf.pow_pct[4];
	  //gblSec.pSec->mcons.cha_cons_chargz6 = sps_act_sp_rtf.pow_pct[5];
	  //gblSec.pSec->mcons.cha_cons_chargz7 = sps_act_sp_rtf.pow_pct[6];
	  //gblSec.pSec->mcons.cha_cons_chargz8 = sps_act_sp_rtf.pow_pct[7];
    gblSec.pSec->mcons.cha_cons_chargz1 = pGEN->pHvm->phvm->meaSptCal.zonesRTF[0].pSetpoint->demandPercentage;
    gblSec.pSec->mcons.cha_cons_chargz2 = pGEN->pHvm->phvm->meaSptCal.zonesRTF[1].pSetpoint->demandPercentage;
    gblSec.pSec->mcons.cha_cons_chargz3 = pGEN->pHvm->phvm->meaSptCal.zonesRTF[2].pSetpoint->demandPercentage;
    gblSec.pSec->mcons.cha_cons_chargz4 = pGEN->pHvm->phvm->meaSptCal.zonesRTF[3].pSetpoint->demandPercentage;
    gblSec.pSec->mcons.cha_cons_chargz5 = pGEN->pHvm->phvm->meaSptCal.zonesRTF[4].pSetpoint->demandPercentage;
    gblSec.pSec->mcons.cha_cons_chargz6 = pGEN->pHvm->phvm->meaSptCal.zonesRTF[5].pSetpoint->demandPercentage;
    gblSec.pSec->mcons.cha_cons_chargz7 = pGEN->pHvm->phvm->meaSptCal.zonesRTF[6].pSetpoint->demandPercentage;
    gblSec.pSec->mcons.cha_cons_chargz8 = pGEN->pHvm->phvm->meaSptCal.zonesRTF[7].pSetpoint->demandPercentage;
    
    // Correct demand %.
    if ( 26 - nbBurnerInDefaultRTF[0] > 0 ) gblSec.pSec->mcons.cha_cons_chargz1 *= ( 26.f / ( 26.f - (float) nbBurnerInDefaultRTF[0] ) );
    if ( 22 - nbBurnerInDefaultRTF[1] > 0 ) gblSec.pSec->mcons.cha_cons_chargz2 *= ( 22.f / ( 22.f - (float) nbBurnerInDefaultRTF[1] ) );
    if ( 18 - nbBurnerInDefaultRTF[2] > 0 ) gblSec.pSec->mcons.cha_cons_chargz3 *= ( 18.f / ( 18.f - (float) nbBurnerInDefaultRTF[2] ) );
    if ( 14 - nbBurnerInDefaultRTF[3] > 0 ) gblSec.pSec->mcons.cha_cons_chargz4 *= ( 14.f / ( 14.f - (float) nbBurnerInDefaultRTF[3] ) );
    if ( 20 - nbBurnerInDefaultRTF[4] > 0 ) gblSec.pSec->mcons.cha_cons_chargz5 *= ( 20.f / ( 20.f - (float) nbBurnerInDefaultRTF[4] ) );
    if ( 16 - nbBurnerInDefaultRTF[5] > 0 ) gblSec.pSec->mcons.cha_cons_chargz6 *= ( 16.f / ( 16.f - (float) nbBurnerInDefaultRTF[5] ) );
    if ( 17 - nbBurnerInDefaultRTF[6] > 0 ) gblSec.pSec->mcons.cha_cons_chargz7 *= ( 17.f / ( 17.f - (float) nbBurnerInDefaultRTF[6] ) );
    if ( 14 - nbBurnerInDefaultRTF[7] > 0 ) gblSec.pSec->mcons.cha_cons_chargz8 *= ( 14.f / ( 14.f - (float) nbBurnerInDefaultRTF[7] ) );

    gblSec.pSec->mcons.cha_cons_chargz1 = min( max( gblSec.pSec->mcons.cha_cons_chargz1, 0.f ), 100.f );
    gblSec.pSec->mcons.cha_cons_chargz2 = min( max( gblSec.pSec->mcons.cha_cons_chargz2, 0.f ), 100.f );
    gblSec.pSec->mcons.cha_cons_chargz3 = min( max( gblSec.pSec->mcons.cha_cons_chargz3, 0.f ), 100.f );
    gblSec.pSec->mcons.cha_cons_chargz4 = min( max( gblSec.pSec->mcons.cha_cons_chargz4, 0.f ), 100.f );
    gblSec.pSec->mcons.cha_cons_chargz5 = min( max( gblSec.pSec->mcons.cha_cons_chargz5, 0.f ), 100.f );
    gblSec.pSec->mcons.cha_cons_chargz6 = min( max( gblSec.pSec->mcons.cha_cons_chargz6, 0.f ), 100.f );
    gblSec.pSec->mcons.cha_cons_chargz7 = min( max( gblSec.pSec->mcons.cha_cons_chargz7, 0.f ), 100.f );
    gblSec.pSec->mcons.cha_cons_chargz8 = min( max( gblSec.pSec->mcons.cha_cons_chargz8, 0.f ), 100.f );

	  /* RTF tubes temperatures*/
	  //gblSec.pSec->mcons.cha_cons_tmp_z1 = sps_act_sp_rtf.tmp_tub[0] - 273.f;
	  //gblSec.pSec->mcons.cha_cons_tmp_z2 = sps_act_sp_rtf.tmp_tub[1] - 273.f;
	  //gblSec.pSec->mcons.cha_cons_tmp_z3 = sps_act_sp_rtf.tmp_tub[2] - 273.f;
	  //gblSec.pSec->mcons.cha_cons_tmp_z4 = sps_act_sp_rtf.tmp_tub[3] - 273.f;
	  //gblSec.pSec->mcons.cha_cons_tmp_z5 = sps_act_sp_rtf.tmp_tub[4] - 273.f;
	  //gblSec.pSec->mcons.cha_cons_tmp_z6 = sps_act_sp_rtf.tmp_tub[5] - 273.f;
	  //gblSec.pSec->mcons.cha_cons_tmp_z7 = sps_act_sp_rtf.tmp_tub[6] - 273.f;
	  //gblSec.pSec->mcons.cha_cons_tmp_z8 = sps_act_sp_rtf.tmp_tub[7] - 273.f;
    gblSec.pSec->mcons.cha_cons_tmp_z1 = pMOD->pConv->celcius( pGEN->pHvm->phvm->meaSptCal.zonesRTF[0].pSetpoint->zoneTemperature );
    gblSec.pSec->mcons.cha_cons_tmp_z2 = pMOD->pConv->celcius( pGEN->pHvm->phvm->meaSptCal.zonesRTF[1].pSetpoint->zoneTemperature );
    gblSec.pSec->mcons.cha_cons_tmp_z3 = pMOD->pConv->celcius( pGEN->pHvm->phvm->meaSptCal.zonesRTF[2].pSetpoint->zoneTemperature );
    gblSec.pSec->mcons.cha_cons_tmp_z4 = pMOD->pConv->celcius( pGEN->pHvm->phvm->meaSptCal.zonesRTF[3].pSetpoint->zoneTemperature );
    gblSec.pSec->mcons.cha_cons_tmp_z5 = pMOD->pConv->celcius( pGEN->pHvm->phvm->meaSptCal.zonesRTF[4].pSetpoint->zoneTemperature );
    gblSec.pSec->mcons.cha_cons_tmp_z6 = pMOD->pConv->celcius( pGEN->pHvm->phvm->meaSptCal.zonesRTF[5].pSetpoint->zoneTemperature );
    gblSec.pSec->mcons.cha_cons_tmp_z7 = pMOD->pConv->celcius( pGEN->pHvm->phvm->meaSptCal.zonesRTF[6].pSetpoint->zoneTemperature );
    gblSec.pSec->mcons.cha_cons_tmp_z8 = pMOD->pConv->celcius( pGEN->pHvm->phvm->meaSptCal.zonesRTF[7].pSetpoint->zoneTemperature );
    
    gblSec.pSec->mcons.cha_cons_tmp_z1 = min( max( gblSec.pSec->mcons.cha_cons_tmp_z1, 100.f ), 1000.f );
    gblSec.pSec->mcons.cha_cons_tmp_z2 = min( max( gblSec.pSec->mcons.cha_cons_tmp_z2, 100.f ), 1000.f );
    gblSec.pSec->mcons.cha_cons_tmp_z3 = min( max( gblSec.pSec->mcons.cha_cons_tmp_z3, 100.f ), 1000.f );
    gblSec.pSec->mcons.cha_cons_tmp_z4 = min( max( gblSec.pSec->mcons.cha_cons_tmp_z4, 100.f ), 1000.f );
    gblSec.pSec->mcons.cha_cons_tmp_z5 = min( max( gblSec.pSec->mcons.cha_cons_tmp_z5, 100.f ), 1000.f );
    gblSec.pSec->mcons.cha_cons_tmp_z6 = min( max( gblSec.pSec->mcons.cha_cons_tmp_z6, 100.f ), 1000.f );
    gblSec.pSec->mcons.cha_cons_tmp_z7 = min( max( gblSec.pSec->mcons.cha_cons_tmp_z7, 100.f ), 1000.f );
    gblSec.pSec->mcons.cha_cons_tmp_z8 = min( max( gblSec.pSec->mcons.cha_cons_tmp_z8, 100.f ), 1000.f );

	  /* Soaking zone temperature*/
	  //gblSec.pSec->mcons.mai_cons_tmp_z = s_spe_mar.s_spt.Tmp_zon_soa - 273.f;
    gblSec.pSec->mcons.mai_cons_tmp_z = pMOD->pConv->celcius( soakingTemperatureSetpoint );

    gblSec.pSec->mcons.fMaintienChargeZ1 = pGEN->pHvm->phvm->meaSptCal.zonesRTF[8 ].pSetpoint->demandPercentage;
    gblSec.pSec->mcons.fMaintienChargeZ2 = pGEN->pHvm->phvm->meaSptCal.zonesRTF[9 ].pSetpoint->demandPercentage;
    gblSec.pSec->mcons.fMaintienChargeZ3 = pGEN->pHvm->phvm->meaSptCal.zonesRTF[10].pSetpoint->demandPercentage;
    gblSec.pSec->mcons.fMaintienChargeZ4 = pGEN->pHvm->phvm->meaSptCal.zonesRTF[11].pSetpoint->demandPercentage;
    gblSec.pSec->mcons.fTunnelChargeZ1   = pGEN->pHvm->phvm->meaSptCal.zonesRTF[11].pSetpoint->demandPercentage;
    gblSec.pSec->mcons.fTunnelChargeZ2   = pGEN->pHvm->phvm->meaSptCal.zonesRTF[11].pSetpoint->demandPercentage;
    gblSec.pSec->mcons.fTunnelChargeZ3   = pGEN->pHvm->phvm->meaSptCal.zonesRTF[11].pSetpoint->demandPercentage;

    //gblSec.pSec->mcons.mai_cons_tmp_z = min( max( gblSec.pSec->mcons.mai_cons_tmp_z, 0.f ), 100.f );

	  /* Jet validity*/
	  //gblSec.pSec->mcons.ref_cons_valid = sps_act_sp_cool.jet_valid;
    gblSec.pSec->mcons.ref_cons_valid = pGEN->pHvm->phvm->meaSptCal.sectionsJET[0].pSetpoint->valid;

	  /* Jet powers*/
	  //gblSec.pSec->mcons.ref_cons_cha_z1 = sps_act_sp_cool.pow_pct_jet[0];
	  //gblSec.pSec->mcons.ref_cons_cha_z2 = sps_act_sp_cool.pow_pct_jet[1];
	  //gblSec.pSec->mcons.ref_cons_cha_z3 = sps_act_sp_cool.pow_pct_jet[2];
    gblSec.pSec->mcons.ref_cons_cha_z1 = pGEN->pHvm->phvm->meaSptCal.zonesJET[0].pSetpoint->demandPercentage;
    gblSec.pSec->mcons.ref_cons_cha_z2 = pGEN->pHvm->phvm->meaSptCal.zonesJET[1].pSetpoint->demandPercentage;
    gblSec.pSec->mcons.ref_cons_cha_z3 = pGEN->pHvm->phvm->meaSptCal.zonesJET[2].pSetpoint->demandPercentage;
    
    gblSec.pSec->mcons.ref_cons_cha_z1 = min( max( gblSec.pSec->mcons.ref_cons_cha_z1, 0.f ), 100.f );
    gblSec.pSec->mcons.ref_cons_cha_z2 = min( max( gblSec.pSec->mcons.ref_cons_cha_z2, 0.f ), 100.f );
    gblSec.pSec->mcons.ref_cons_cha_z3 = min( max( gblSec.pSec->mcons.ref_cons_cha_z3, 0.f ), 100.f );

	  /* Speed set points validity*/
	  //gblSec.pSec->mcons.eb_cons_valid = sps_act_sp_rtf.Spd_Valid;
    gblSec.pSec->mcons.eb_cons_valid = pGEN->pHvm->phvm->meaSptCal.setpoint.line.valid;

	  /* speed set point*/
	  //gblSec.pSec->mcons.cons_vitcen = sps_act_sp_rtf.spd * 60.f;
    gblSec.pSec->mcons.cons_vitcen = (float) pMOD->pMath->round( pGEN->pHvm->phvm->meaSptCal.setpoint.line.speed * 60. );

	  /* traction offset*/
	  //gblSec.pSec->mcons.offset_tract = sps_act_sp_rtf.traction;
    gblSec.pSec->mcons.offset_tract = tractionOffset;

	  /* start*/
	  //gblSec.pSec->mcons.demarrage_ok = s_spe_mar.s_spt.demarrage_ok;
    gblSec.pSec->mcons.demarrage_ok = 1;
        
	  /* le modèle est en transitoire */
	  //gblSec.pSec->mcons.transient_encours = s_coil.s_coil_data[0].trs;
    gblSec.pSec->mcons.transient_encours = pGEN->pHvm->phvm->meaSptCal.sectionsRTF[0].pCalcul->tracking.weldTime[0] < pGEN->pHvm->phvm->meaSptCal.sectionsRTF[0].pCalcul->anticipationTime;
                                                                 
	  /* vitesse maximum sans limitation opérateur de la bobine en cours */
	  //gblSec.pSec->mcons.Vmax_boballia_enc = min(s_coil.s_coil_data[0].Vmax_boballia, s_coil.s_coil_data[0].spd_max_th) * 60.f;
    long nuCoilOrdered = pGEN->pHvm->phvm->meaSptCal.sectionsRTF[0].pCalcul->tracking.nu_coilInOrdered;
    if ( pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoilOrdered] != nullptr )
    {
      gblSec.pSec->mcons.Vmax_boballia_enc = min( pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoilOrdered]->data.specific.alliationMaxSpeed, 
                                                  pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoilOrdered]->calcul.maximumLineSpeed ) * 60.;
    }

	  /* numéros de bobine en cours */                                                            
	  //gblSec.pSec->mcons.nobob_enc[0] = '\0';  
	  //strncpy ( gblSec.pSec->mcons.nobob_enc, s_coil.s_coil_data[0].id, 8 );
    strcpy_s ( gblSec.pSec->mcons.nobob_enc, "" );
    if ( pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoilOrdered] != nullptr )
      strncpy_s( gblSec.pSec->mcons.nobob_enc, pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoilOrdered]->data.entryIdentity, 8 );

	  /* vitesse maximum sans limitation opérateur de la bobine suivante */                      
	  //gblSec.pSec->mcons.Vmax_boballia_sui = min(s_coil.s_coil_data[1].Vmax_boballia, s_coil.s_coil_data[1].spd_max_th) * 60.f; 
    long nuCoilOrderedNext = nuCoilOrdered + 1;

	  /* numéros de bobine suivante */                                                            
	  //gblSec.pSec->mcons.nobob_sui[0] = '\0';
	  //strncpy(gblSec.pSec->mcons.nobob_sui, s_coil.s_coil_data[1].id, 8);

    if ( pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoilOrderedNext] != nullptr )
    {
      gblSec.pSec->mcons.Vmax_boballia_sui = min( pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoilOrderedNext]->data.specific.alliationMaxSpeed, 
                                                  pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoilOrderedNext]->calcul.maximumLineSpeed ) * 60.;
      strcpy_s ( gblSec.pSec->mcons.nobob_sui, "" );
      strncpy_s( gblSec.pSec->mcons.nobob_sui, pGEN->pHvm->phvm->coilQueue.pCoilOrdered[nuCoilOrderedNext]->data.entryIdentity, 8 );
    }

    // Life bit.
    if ( ++gblSec.pSec->mcons.iLifeCounter > 10000 ) gblSec.pSec->mcons.iLifeCounter = 0;

	  // Release access to global section.
	  gblSec.unLockAccess();
  }

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


