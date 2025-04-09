#include "main_core.hpp"
#include <bitset>
#include <iostream>
#include "../utilities/hardcoded_functions.hpp"
#include "common_data/common_variables.hpp"
#include "common_data/device_variables.hpp" //250409
#include "../physical_devices/scanner.hpp"
#include "../utilities/debug_logger.hpp"
#include <cmath>

void  MainCore::setAlgCodeNone()
{
   ALGCODE=ALGNONE;
}

void MainCore::launchOnCore1()
{ 
  while (true)
  {
    parse(Vector,Vupdateparams); //wait for data ! парсинг входящих данных из ПК 
    if (Vector.size()!=0)
    {
     switch (Vector[0])
     {
     /* case  ADC_RESET: 
        ADC_RESET = true;
        break;
     */  

  case VirtualCmd : //флаг симуляции работы микроконтроллера      
          ALGCODE=ALGNONE;
          flgVirtual=(bool)Vector[1];
          afc.clear();
          afc = code+std::to_string(DEBUG)+"set virtual "+ std::to_string(Vector[1]);
          afc +=endln;//"\n";
          std::cout << afc;
          afc.clear();
          sleep_ms(100);              
          break;
  case DebugLevelCmd: // флаг вывода отладочной информации debug level =2;  =3 запрет вывода!
         flgDebugLevel=Vector[1];
         break; 
  case CHRONOMETRIA:
         ALGCODE=ALGNONE; 
         flgTiming=(bool)Vector[1];
         break;         
  case DebugCmd: // флаг вывода отладочной информации  =1, нет =0
        ALGCODE=ALGNONE;
        flgDebug=(bool)Vector[1];
       /* 
        afc.clear();
        afc = code+std::to_string(DEBUG)+"debug Set Debug "+ std::to_string(flgDebug);
        afc += endln;//"\n";
        std::cout << afc;
        afc.clear();
        sleep_ms(100); 
        */
        break;
  case SetUseCritialSectAlgCode: // флаг использовать Сritical_section
        flgСritical_section=(bool)Vector[1]; 
        break;
  //***************************************  
  case SET_PID_GAIN:
        ALGCODE=ALGNONE;
        if (HARDWAREVERSION!=BBFPGA) scanner->hardware->set_GainPID((uint32_t)Vector[1]); //250116
        else                         scanner->hardware->set_GainPID((uint32_t)Vector[1]); 
        break;  
  case ChangeSpeed:
        ALGCODE=ALGNONE;
        delayFW=(uint16_t)Vector[1];
        delayBW=(uint16_t)Vector[2];
        break;  
  case ChangeHopeParams:
        ALGCODE=ALGNONE;              
        delayHope=(uint16_t)Vector[1];
        ZJump    =(uint16_t)Vector[2];
        break;
  case ADC_GET_VALUECmd:
        ALGCODE=ALGNONE;            
        ADC_GET_VALUE = true;// прочитатать сигналы АЦП      
        break;
  case TheadDoneCmd:
        ALGCODE=ALGNONE;
        TheadDone = true;
        break;
  case DRAWDONECmd:
        ALGCODE=ALGNONE;  
        DrawDone = true;
        break;  
  case STOPCmd:
        ALGCODE=ALGNONE;
        STOP=true; // stop the active algorithm 
        break; 
  default: 
      {
        if (Vector[0]>=0 && Vector[0]<100)  {ALGCODE=(int16_t)Vector[0];}
                                       else {ALGCODE=ALGNONE;           }  
        break;
      }  
     }   
    continue;
   } 
   if (Vupdateparams.size()!=0)  
   {
    if (Vupdateparams[0]==CONFIG_UPDATECmd)
    {
       CONFIG_UPDATE = true;
    } 
   }
  }//while
}

void MainCore::loop()
{
  uint64_t time = 0;
  while (time++ < UINT64_MAX - 1000)
  {
    switch (ALGCODE)
    {
case   ALGNONE:{break;}
case VersionCmd:
{
  ALGCODE=ALGNONE;  
  device=(uint8_t)Vector[1]; //add 250409
  afc.clear();
  afc = code+std::to_string(DEBUG)+" get version "+ " dev="+std::to_string(device);
  afc +=endln;//"\n";
  std::cout << afc;
  afc.clear();
  sleep_ms(100);
  switch (HARDWAREVERSION) 
 {
   case BBFPGA: 
    {
     scanner->hardware->ChooseLoopChannelInputFPGA(device,nloop);
     break;
    }
 case WB:
    {
     //uint8_t sensor ,uint8_t loopsign ,uint8_t signal_in_loop , uint8_t useSD,uint8_t useMod_U
      switch ((uint8_t)Vector[1])
     {
     case STM:
              device=STM;
              scanner->hardware->init_Commutation(0 , SignalIncrease , IPin , 0, 0); //add 250409
              break;        
     case SFM:
              device=SFM;
              scanner->hardware->init_Commutation(0 , SignalDecrease , AmplPin , 1, 0); //add 250409
              break;        
    case SICMDC:
              device=SICMDC;
              scanner->hardware->init_Commutation(0 , SignalDecrease , IPin , 0, 0); //add 250409
              break;  
     }      
     break;
    }  
  }              
  scanner->hardware->GetSOFTHARDWAREVersion(device);
  break;
}               
case ChangeHardWare:
              {
                ALGCODE=ALGNONE;
                afc.clear();
                afc =code+std::to_string(ChangeHardWare)+ " hardware "+ std::to_string(Vector[1]);
                afc += endln;//"\n";
                std::cout << afc;
                afc.clear();
                sleep_ms(300);       
                if (HARDWAREVERSION!= (int8_t)Vector[1])                                  
                { 
                  HARDWAREVERSION= (int8_t)Vector[1];   
                  switch (HARDWAREVERSION)
                  {       
                   case BB:{
                             scanner=new  Scanner(confighardwareBB); 
                             scanner->hardware->setDefaultSettings(confighardwareBB);                 
                             break; 
                           }
               case BBFPGA:{
                             scanner=new  Scanner(confighardwareBBFPGA); 
                             scanner->hardware->setDefaultSettings(confighardwareBBFPGA);                 
                             break; 
                            }                           
                    case WB:{
                             scanner=new  Scanner(confighardwareWB);
                             scanner->hardware->setDefaultSettings(confighardwareWB);
                             break;
                            }
                  } 
                  if (scanner==nullptr) 
                  {
                   afc.clear();
                   afc =code+std::to_string(ChangeHardWare)+ "error new scanner create "+std::to_string(HARDWAREVERSION);
                   std::cout << afc;
                   afc.clear();
                   sleep_ms(100); 
                  }
                }
                else
                {
                 /* if (scanner!=nullptr) delete(scanner);
                  HARDWAREVERSION_I= (uint8_t)Vector[1];   
                  switch (HARDWAREVERSION_I)
                  {       
                    case 0:   { scanner=new  Scanner(confighardwarev0); break; }
                    case 1:   { scanner=new  Scanner(confighardwarev1); break; }
                  } 
                  scanner->hardware->setDefaultSettings();
                  */
                }
               break;
              } 
case SETDEVICE:{
                ALGCODE=ALGNONE; 
                switch (HARDWAREVERSION)
               {
             case BBFPGA: 
                {
                  switch ((uint8_t)Vector[1])
                 {
                 case STM:
                          device=STM;
                          scanner->hardware->ChooseLoopChannelInputFPGA(channelcurrent,nloop); // channel, nloop
                          break;        
                 case SFM:
                          device=SFM;
                          scanner->hardware->ChooseLoopChannelInputFPGA(channelampl,nloop);    // channel, nloop
                          break;        
                case SICMDC:
                          device=SICMDC;
                          scanner->hardware->ChooseLoopChannelInputFPGA(channelcurrent,nloop); // channel, nloop
                          break;  
                 }      
                 break;
                } 
             case WB:
                {
                  //uint8_t sensor ,uint8_t loopsign ,uint8_t signal_in_loop , uint8_t useSD,uint8_t useMod_U
                   switch ((uint8_t)Vector[1])
                  {
                  case STM:
                           device=STM;
                           scanner->hardware->init_Commutation(0 , SignalIncrease , IPin , 0, 0); //add 250409
                           break;        
                  case SFM:
                           device=SFM;
                           scanner->hardware->init_Commutation(0 , SignalDecrease , AmplPin , 1, 0); //add 250409
                           break;        
                 case SICMDC:
                           device=SICMDC;
                           scanner->hardware->init_Commutation(0 , SignalDecrease , IPin , 0, 0); //add 250409
                           break;  
                  }      
                  break;
                 }  
                }
          
               afc.clear();
               afc =code+std::to_string(DEBUG)+" device="+std::to_string(device);
               std::cout << afc;
               afc.clear();
               sleep_ms(100); 
               break;
              }                
case INITCOMMMUTATION:
              {
                ALGCODE=ALGNONE;
                scanner->hardware->init_Commutation((uint8_t)Vector[1],(uint8_t)Vector[2],(uint8_t)Vector[3],(uint8_t)Vector[4],(uint8_t)Vector[5]);
                break;
              }              
case ADC_RESET:
              {          
              //  scanner->hardware->ADC_reset();
                break;
              }               
case RESONANCE:
              {
               ALGCODE=ALGNONE;
               scanner->start_frqscan();
               break;
              }
case APPROACH:{
                ALGCODE=ALGNONE;
                scanner->hardware->blue();
                scanner->approacphm(Vector);
                scanner->hardware->green();
              //  dark();
                break;
              }
case TESTMOVER:{
                 ALGCODE=ALGNONE;
                 scanner->testpiezomover(Vector);
                 break;
               }
case FREQ_SET:{
                ALGCODE=ALGNONE;
                scanner->hardware->set_Freq((uint32_t) Vector[1]);              
                break;
              }
case LID_MOVE_UNTIL_STOP:
              {
                ALGCODE=ALGNONE;
                scanner->positioningXYZ(Vector);               
                break;
              }
case MOVE_TOX0Y0:
              {
                ALGCODE=ALGNONE;
                scanner->move_toX0Y0(Vector[1], Vector[2], Vector[3],Vector[4]);
                break; 
              }     
case LID_MOVE_TOZ0:
              {
                ALGCODE=ALGNONE;
                scanner->LID_move_toZ0(Vector[1], Vector[2], Vector[3], Vector[4], Vector[5]);
               break; 
              }    
case GridMovetoNextScan:                      
              {
                ALGCODE=ALGNONE;
                scanner->LID_move_toNextGridScan(Vector[1], Vector[2], Vector[3], Vector[4], Vector[5]);
                //GridParams.StepsCurNmbX,GridParams.StepsCurNmbY,GridParams.StepsCurNmbZ,PidParams.TiApproach, PidParams.TiScan;
                break;
              }
case SCANNING:
              {  
                ALGCODE=ALGNONE;
                DrawDone=true;
                scanner->scanparams_update(Vector);
                if (!scanner->getHoppingFlg()){
                                              if (!scanner->getLinearFlg()) {scanner->start_scan();   }
                                              else                          {scanner->start_scanlin();}    
                                             }
                else                         {
                                              if (!scanner->getLinearFlg()) {scanner->start_hopingscan();   }
                                              else                          {scanner->start_hopingscanlin();}
                                             }
                DrawDone=true;
                break; 
              }
case FASTSCANNING:
              {
                ALGCODE=ALGNONE;
                scanner->scanparams_update(Vector);
                scanner->start_fastscan();  
                break; 
              }  
case SPECTROSOPY_IV:
              {
                ALGCODE=ALGNONE;
                scanner->spectroscopyIV(Vector);
                break;
              }  
case SPECTROSOPY_AIZ:
              {
                ALGCODE=ALGNONE;
                scanner->spectroscopyAIZ(Vector);
                break;
              }                
case SENDDATALIN:
              {
                ALGCODE=ALGNONE;
                scanner->readDATALin();
                break;
              }              
             
case SET_AMPLMOD_GAIN: // усиление раскачка зонда 
              {
                ALGCODE=ALGNONE;
                scanner->hardware->set_GainApmlMod((uint8_t)Vector[1]);
                break;
              }  
case SetDACZeroCmd:
              {
                ALGCODE=ALGNONE;
                scanner->hardware->set_DACZero();
                break;
              } 
case SET_BIAS:
              {
                ALGCODE=ALGNONE;
                scanner->hardware->set_BiasV(Vector[1]);
                break;
              }               
case SET_SETPOINT:
              {
                ALGCODE=ALGNONE;
                scanner->hardware->set_SetPoint(Vector[1]);
                break; 
              }   
case SET_SIGN_LOOP:
              {
                ALGCODE=ALGNONE;
                scanner->hardware->setLoopSign(Vector[1]);
                break;
              }
case Set_SIGN_LOOP_BIASV:              
              {
               ALGCODE=ALGNONE;
               scanner->hardware->setLoopSign_BiasV(Vector[1],Vector[2],Vector[3],Vector[4]);
               break;
              }
  case SET_Z: { 
                ALGCODE=ALGNONE;
                scanner->hardware->set_DACZ(Vector[1]);
                break;
              }     
 case SET_XY: {
                ALGCODE=ALGNONE;
                scanner->hardware->init_SPI(Vector[1],Vector[2],Vector[3],Vector[4]);//29, 3, 8, 0, 1, 1, value	
                if (Vector[5] == 0)
                {
                 scanner->hardware->move_scannerX(Vector[6]);
                } 
                else 
                if (Vector[5] == 1)
                {
                 scanner->hardware->move_scannerY(Vector[6]);
                }
                break;
              }   
case ADC_READCmd: //TIMER
              {
                ALGCODE=ALGNONE;
                switch (HARDWAREVERSION)
                {       
              case BBFPGA:
                {
                  scanner->readADC();
                  break;  
                }
                case BB:
                case WB:
                { 
                 if (ADC_IS_READY_TO_READ)
                 {
                  scanner->readADC();
                 }
                 break;
                }
              }  
              break;  
            }
case ReadFPGA:{ 
                 ALGCODE=ALGNONE;
                 FPGAReadData readdata;
                 readdata.addr=arrLoopModule_1.wbSetpoint;
                 scanner->hardware->ReadDataFromFPGA(readdata);
                 break;
              }            
case GET_CURRENTX0Y0:
              {
                ALGCODE=ALGNONE;
                scanner->getX0Y0();
                break;
              }   
case ProtractAlgCode:
              {
                ALGCODE=ALGNONE;
                scanner->hardware->protract(); 
                break;
              } 
case RetractAlgCode:
              {
                ALGCODE=ALGNONE;
                scanner->hardware->retract();
                break;
              }
case FreezePIDAlgCode:
              {
                ALGCODE=ALGNONE;
                scanner->hardware->freezeLOOP(Vector[1]);
                break;
              }
case UnFreezePIDAlgCode:
              {
                ALGCODE=ALGNONE;
                scanner->hardware->unfreezeLOOP(Vector[1]);
                break;
              }                           
default:      {/*activateError();*/  break;}                                                                                                                                            
   }
  }
}

MainCore::
MainCore()
{
   multicore_launch_core1(launchOnCore1);
   switch (HARDWAREVERSION)
   {       
    case BB:
           {
            scanner=new  Scanner(confighardwareBB); //bb=v0
            scanner->hardware->setDefaultSettings(confighardwareBB);                 
            break; 
           }
    case WB:
           {
            scanner=new  Scanner(confighardwareWB);
            scanner->hardware->setDefaultSettings(confighardwareWB);
            break; 
           }
    case BBFPGA:
           { //BB+ WBFPGA
            scanner=new  Scanner(confighardwareBBFPGA); 
            scanner->hardware->setDefaultSettings(confighardwareBBFPGA);                 
            break; 
           }
   } 
   if (scanner==nullptr) 
   {
     afc.clear();
     afc =code+std::to_string(ChangeHardWare)+ "error new scanner create "+std::to_string(HARDWAREVERSION);
     std::cout << afc;
     afc.clear();
     sleep_ms(100); 
   }
}

void MainCore::parse(std::vector<int32_t> &vec)
{
  std::string s;
  getline(std::cin, s);
  Parser parser(s.data(),separator);//','); //240715
  vectorSize = parser.parseInts(vec);
}
void MainCore::parse(std::vector<int32_t> &vec,std::vector<int32_t> &vparams)
{
  std::string s;
  getline(std::cin, s);
  Parser parser(s.data(),separator); //','); 240715
  vectorSize = parser.parseInts(vec,vparams);
}
