#include <iostream>
#include <bitset>
#include <pico/multicore.h>
#include <pico/multicore.h>
#include "../transceiver/rx_core.hpp"
#include "../utilities/debug_logger.hpp"
#include "hardcoded_functions.hpp"

HARDWARE::HARDWARE(ConfigHardWareBB confighardware)   // BB  mother BB+FPGA
{
       dacspt=new DAC8563(confighardware.DACSetPointMode); //set mode DAC BIAS,SetPoint
        dacbv=new DAC8563(confighardware.DACBiasVMode);    //set mode DAC BIAS,SetPoint
        dacxy=new DAC8563(confighardware.DACXYMode);       //set mode DAC X,Y
         dacz=new DAC8563(confighardware.DACZMode);        //set mode DAC Z  
     busyport=new InputPort(confighardware.BUSYPort);
         conv=new OutputPort(confighardware.CONV);
          dec=new OutputPort(confighardware.DEC);
    resetport=new OutputPort(confighardware.ResetPort); 
      ledPort=new OutputPort(PICO_DEFAULT_LED_PIN);
       rdbLed=new OutputPort(confighardware.RDBPort); 
        io1_0=new OutputPort(confighardware.IO1_0); //??
        io1_1=new OutputPort(confighardware.IO1_1); //?
     gainPID0=new OutputPort(confighardware.GainPID0);
     gainPID1=new OutputPort(confighardware.GainPID1); 
     gainPID2=new OutputPort(confighardware.GainPID2); 
   freezeport=new OutputPort(confighardware.FreezePort);  //заморозить/разморозить ПИД 
 protractport=new OutputPort(confighardware.ProtractPort);//вытянуть сканнер /втянуть сканнер  
 linearDriver=new LinearDriverBB(true,configlineardriveBB);   
}
HARDWARE::HARDWARE(ConfigHardWareBBFPGA confighardware)   // BB  mother BB+FPGA
{
       dacspt=new DAC8563(confighardware.DACSetPointMode); //set mode DAC BIAS,SetPoint
        dacbv=new DAC8563(confighardware.DACBiasVMode);    //set mode DAC BIAS,SetPoint
        dacxy=new DAC8563(confighardware.DACXYMode);       //set mode DAC X,Y
         dacz=new DAC8563(confighardware.DACZMode);        //set mode DAC Z  
     busyport=new InputPort(confighardware.BUSYPort);
         conv=new OutputPort(confighardware.CONV);
          dec=new OutputPort(confighardware.DEC);
    resetport=new OutputPort(confighardware.ResetPort); 
      ledPort=new OutputPort(PICO_DEFAULT_LED_PIN);
       rdbLed=new OutputPort(confighardware.RDBPort); 
        io1_0=new OutputPort(confighardware.IO1_0); //??
        io1_1=new OutputPort(confighardware.IO1_1); //?
     gainPID0=new OutputPort(confighardware.GainPID0);
     gainPID1=new OutputPort(confighardware.GainPID1); 
     gainPID2=new OutputPort(confighardware.GainPID2); 
   freezeport=new OutputPort(confighardware.FreezePort);  //заморозить/разморозить ПИД 
 protractport=new OutputPort(confighardware.ProtractPort);//вытянуть сканнер /втянуть сканнер  
 linearDriver=new LinearDriverBB(true,configlineardriveBB);   
}
HARDWARE::HARDWARE(ConfigHardWareWB confighardware) // WB
{
       dacspt=new DAC8563(confighardware.DACSetPointMode); //set mode DAC BIAS,SetPoint
        dacbv=new DAC8563(confighardware.DACBiasVMode);    //set mode DAC BIAS,SetPoint
        dacxy=new DAC8563(confighardware.DACXYMode);       //set mode DAC X,Y
         dacz=new DAC8563(confighardware.DACZMode);        //set mode DAC Z  
     busyport=new InputPort(confighardware.BUSYPort);
         conv=new OutputPort(confighardware.CONV);
          dec=new OutputPort(confighardware.DEC);
    resetport=new OutputPort(confighardware.ResetPort); 
      ledPort=new OutputPort(PICO_DEFAULT_LED_PIN);
       rdbLed=new OutputPort(confighardware.RDBPort); 
      //  io1_0=new OutputPort(_confighardware.IO1_0);
     //   io1_1=new OutputPort(_confighardware.IO1_1);
     gainPID0=new OutputPort(confighardware.GainPID0);
     gainPID1=new OutputPort(confighardware.GainPID1); 
     gainPID2=new OutputPort(confighardware.GainPID2); 
   freezeport=new OutputPort(confighardware.FreezePort);  //заморозить/разморозить ПИД 
 protractport=new OutputPort(confighardware.ProtractPort);//вытянуть сканнер /втянуть сканнер 
//add new ports
     modulateuport=new OutputPort(confighardware.ModulateUPort);   // вкл=1; выкд=0 модуляцию U  
        usesdport=new OutputPort(confighardware.SD_1Port);         // порты  настройки СД вкл =1; выкл = 0 
        sensorport=new OutputPort(confighardware.SD_2Port);        // порты  настройки СД Cantilever=0; 1-Piezo
      signloopport=new OutputPort(confighardware.SignLoopPort);    // знак ПИД // 0=+ ; 1=-
 integratorinport=new OutputPort(confighardware.IntegratorInPort); // выбор входного сигнала в ПИД из 1-SD(AMPL); из 0=ПТН(I) 
      linearDriver=new LinearDriverWB(configlineardriveWB);   
}

HARDWARE::~HARDWARE()
{
    delete(dacspt);
    delete(dacbv);
    delete(dacxy);
    delete(dacz);
    delete(busyport);
    delete(conv);
    delete(dec);
    delete(resetport);
    delete(ledPort);
    delete(rdbLed);
    delete(io1_0); //??
    delete(io1_1); //??
    delete(gainPID0);
    delete(gainPID1);
    delete(gainPID2);
    delete(freezeport);
    delete(protractport);
   if (HARDWAREVERSION==WB) 
   {       
     delete(modulateuport);
     delete(usesdport);
     delete(sensorport);
     delete(signloopport);
     delete(integratorinport);
   }
   if (linearDriver!=0) delete(linearDriver);
// io_ports.clear();
}
/*
void HARDWARE::set_io_value(int port, int value)  
{
  SET_IO_VALUE = false;
  if (port == 1) //????
  {
    std::string binary = std::bitset<2>(value).to_string();
    binary[1] == '1' ? io1_0->enable() : io1_0->disable();
    binary[0] == '1' ? io1_1->enable() : io1_1->disable();
  } 
  else
  if (port == 2) //gain PID
  {
    std::string binary = std::bitset<3>(value).to_string();
    binary[2] == '1' ? gainPID0->enable() : gainPID0->disable();
    binary[1] == '1' ? gainPID1->enable() : gainPID1->disable();
    binary[0] == '1' ? gainPID2->enable() : gainPID2->disable();
  }
  else if (port == 3) //0 заморозить сканнер=1; разморозить =0
  {
    std::string binary = std::bitset<2>(value).to_string();
    binary[1] == '1' ?   freezeport->enable() :   freezeport->disable();
    binary[0] == '1' ? protractport->enable() : protractport->disable();
  }
}
*/
void HARDWARE::reset_ADCPort()
{
  resetport->enable();
  sleep_us(10);
  resetport->disable();
}
void HARDWARE::SetLOOPParams(int32_t kp,int32_t ki,int32_t kd,int32_t gainscale)
{
//  loopParams.scale=scale;
  loopParams.GainScale=gainscale;
  loopParams.GainScaleVal=1<<loopParams.GainScale;
  loopParams.Ki=ki;
  loopParams.Kp=kp;
  loopParams.Kd=kd;
  loopParams.K1=loopParams.Kp;//+(loopParams.Ki+loopParams.Kd)*loopParams.GainScaleVal;
  loopParams.K2=loopParams.Ki*loopParams.GainScaleVal;//(-loopParams.Kp-2*loopParams.Kd);
  loopParams.K3=loopParams.Kd*loopParams.GainScaleVal;
}
void HARDWARE::init_LOOP(uint8_t device)
{
  //set dafault device SFM
 // device=SFM;
  sensor=PROBE;
  switch (HARDWAREVERSION)
  {
case BBFPGA: 
    {
     PID_FBABS=0x00000100; // 1<<8;    // abs value into loop
     PID_SIGN =0x00000080; // 1<<7;    // up or  down ???
     PID_ENA  =0x00000001;
     PID_STOP =0x00000000;
     PID_CONTROL=PID_FBABS|PID_SIGN|PID_ENA|PID_STOP;
     switch (nloop)
     { 
     case 0: { arrLoopModule=arrLoopModule_0; break;}
     case 1: { arrLoopModule=arrLoopModule_1; break;};
     }  
     scanner->hardware->ChooseLoopChannelInputFPGA(device,nloop);    // channel, nloop
     
      SetLOOPParams(1,1,0,8);//int32_t kp,int32_t ki,int32_t kd,int32_t gainscale
      loopParams.scale=1.0;
    /*
      ПИД в дискретном виде
      u[k]=u[k−1]+Kout2N(K1e[k]+K2e[k−1]+K3e[k−2])+uc
      где:
      u[k]−выходнашагеk
      e[k]−отклонениевходаотsetpointнашагеk
      Регистр wbInSetup[7, 0].KPOW
      N−количестворазрядовдробнойчастикоэффициентов
      Регистр wbOutMulKoef
      Kout−коэффициентмасштабавыхода
      Регистр wbOutShift
      uc−смещениевыхода
      Регистр wbKx[0]
      K1=KP+KI+KD
      Регистр wbKx[1]  
      K2=−KP−2KD
      Регистр wbKx[2]
      K3=KD
      KP,KI,KD−непосредственно сами коэффициенты регулятора
    */
    
      FPGAWriteData writedata;
      writedata.addr=arrLoopModule.wbInSetup;// 250623
      writedata.data=loopParams.GainScale;           
      WriteDataToFPGA(writedata);
      sleep_ms(10);
      writedata.data=loopParams.K1;//uint32_t(1*(2<<GainScale));
      writedata.addr=arrLoopModule.wbKx[0]; //tp
      WriteDataToFPGA(writedata);
      sleep_ms(30);
      writedata.data=loopParams.K2;//uint32_t(0.01*GainScaleVal);
      writedata.addr=arrLoopModule.wbKx[1]; //ti
      WriteDataToFPGA(writedata);
      sleep_ms(30);
      writedata.data=loopParams.K3;//uint32_t(0.0);
      writedata.addr=arrLoopModule.wbKx[2]; 
      WriteDataToFPGA(writedata); 
      sleep_ms(30);
      writedata.addr=arrLoopModule.wbOutMulKoef;
      writedata.data=1;
      WriteDataToFPGA(writedata);
      sleep_ms(30);
      writedata.data=PID_CONTROL;
      writedata.addr=arrLoopModule.pidControl;
      WriteDataToFPGA(writedata);
      sleep_ms(30);
      if(flgDebug && (!flgVirtual))
      {
       int32_t readpidcontroltok=ReadDataFromFPGA(arrLoopModule.pidControl); //250701
       sleep_ms(30);
      } 
      break;        
    } 
case WB:
    {
      scanner->hardware->init_Commutation(sensor, device); //add 250423
      break;        
    }      
    break;
  }    
 // need set select channel input !!!!!! 250408
}

void HARDWARE::setDefaultSettings(ConfigHardWareBBFPGA  confighardwarev)  //BBFPGA
{
 // BASIC SETTINGS
 // uart_init(uart1, 115200); //????
 // uart_init(USB_UART_ID, 115200); //????
 // gpio_set_function(USBUART_TX_PIN, GPIO_FUNC_UART);
 // gpio_set_function(USBUART_RX_PIN, GPIO_FUNC_UART); 
 // gpio_pull_down(resetport->getPort());
 //   ShiftDac=0; //250522
    SetPointScale=1;// 2;  //250522
    stdio_init_all(); //add 250325
    uart_init(FPGA_UART_ID, FPGA_BAUD_RATE); 
    gpio_set_function(FPGAUART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(FPGAUART_RX_PIN, GPIO_FUNC_UART);  
    // Enable UART
    // Set UART flow control CTS/RTS, we don't want these, so turn them off
    uart_set_hw_flow(FPGA_UART_ID, false, false);
   // uart_set_hw_flow(FPGA_UART_ID,true, true)  // Set our data format
    uart_set_format(FPGA_UART_ID, 8, 1, UART_PARITY_NONE);
    // Turn off FIFO's - we want to do this character by character
    uart_set_fifo_enabled(FPGA_UART_ID,true);  

// #warning should be undeleted
// RX_core rxCore;
// fixme mb should add & before isr
// gpio_set_irq_enabled_with_callback(busyport->getPort(), GPIO_IRQ_EDGE_FALL, true, RX_core::comReceiveISR);     
// multicore_launch_core1(RX_core::launchOnCore1); // 240508 ??
 
   gpio_pull_down(resetport->getPort()); 
   dec->enable();
   conv->enable();
   resetport->disable();
   gpio_pull_down(resetport->getPort());
   ledPort->enable();
   dark();
   //init_DACSetPoint(confighardwarev.DACSetPointPort);//инициирование ЦАП1  SetPoint
   init_DACBiasV(confighardwarev.DACBiasVPort);        //инициирование ЦАП1  BIAS
   init_DACXY(confighardwarev.DACXYPort);              //инициирование ЦАП2  DACXY   
   init_DACZ(confighardwarev.DACZPort);                //инициирование ЦАП3  DACZ
   device=SFM; 
   if (!flgVirtual)
   { 
    init_LOOP(device); //250522
   // channel is default ampl!!! need change  when changed dev
    uint32_t gain;
    gain=7; //?????
    LOOPGain=gain;
    set_GainPID(gain);  // 250522             // not virtual; not debug!
 //  retract();          // 250522             // втянуть    
    set_DACZ(0);        //250522
   } 
}

void HARDWARE::setDefaultSettings(ConfigHardWareBB  confighardwarev)  // BB
{
   gpio_pull_down(resetport->getPort());
// #warning should be undeleted
// RX_core rxCore;
// fixme mb should add & before isr
   gpio_set_irq_enabled_with_callback(busyport->getPort(), GPIO_IRQ_EDGE_FALL, true, RX_core::comReceiveISR);
 // multicore_launch_core1(RX_core::launchOnCore1); // 240508 ??
   dec->enable();
   conv->enable();
   resetport->disable();
   gpio_pull_down(resetport->getPort());
   ledPort->enable();
   dark();
   init_DACSetPoint(confighardwarev.DACSetPointPort);   //инициирование ЦАП1  SetPoint
   init_DACBiasV(confighardwarev.DACBiasVPort);         //инициирование ЦАП1  BIAS
   init_DACXY(confighardwarev.DACXYPort);               //инициирование ЦАП2  DACXY
   uint32_t gain;
   uint32_t gain0=7;
   gain=(gain0<<8)+100; 
   LOOPGain=gain;
   set_GainPID(gain);                    // not virtual; not debug!
   retract();                            // втянуть    
   init_DACZ(confighardwarev.DACZPort); // инициирование ЦАП3  DACZ
   set_DACZ(0); 
}
void HARDWARE::setDefaultSettings(ConfigHardWareWB  confighardwarev) //WB  
{
  gpio_pull_down(resetport->getPort());
// #warning should be undeleted
// RX_core rxCore;
// fixme mb should add & before isr
  gpio_set_irq_enabled_with_callback(busyport->getPort(), GPIO_IRQ_EDGE_FALL, true, RX_core::comReceiveISR);
 // multicore_launch_core1(RX_core::launchOnCore1); // 240508 ??

  dec->enable();
  conv->enable();
  resetport->disable();
  gpio_pull_down(resetport->getPort());
  ledPort->enable();
  dark();
  device=SFM;
  init_LOOP(device);//250409
 // init_Commutation(0 , 1 , 1 , 1, 0);   //default afm 
  /*
    default afm probe ?????
    sensor=0           // probe=0;  cantilever =1
    signLoop:=1;       // 1= -1 убывание ; 0 = +1 рост
    signal_to_loop:=1; // sd->to loop =1 Ampl  
    useSD:=1;          // use SD  =1 ;
    usemod_U:=0;       // use mod U; not=0
   */  
  //init_commutation(1 , 1 , 1 , 0, 0);  //afm  240624
  init_DACSetPoint(confighardwarev.DACSetPointPort);   //инициирование ЦАП1  SetPoint
  init_DACBiasV(confighardwarev.DACBiasVPort);   //инициирование ЦАП1  BIAS
  init_DACXY(confighardwarev.DACXYPort);    //инициирование ЦАП2  DACXY
  uint32_t gain=7; 
  LOOPGain=gain;
  set_GainPID(gain); // not virtual; not debug!
  retract();         //втянуть    
  init_DACZ(confighardwareWB.DACZPort);   //инициирование ЦАП3  DACZ
  set_DACZ(0); 
}

void HARDWARE::SetDev_GetSOFTHARDWAREVersion(uint8_t device)
{
  afc.clear();
  afc = code+std::to_string(VersionCmd)+",soft ver "+ SOFTVERSION+",softhardware ver "+SoftHARDWAREVERSION
        +",hardware "+std::to_string(HARDWAREVERSION) + ",device "+std::to_string(device)
        +",sensor "+std::to_string(sensor);
  afc +=endln;
  std::cout << afc;
  afc.clear();
  sleep_ms(100);
}

void HARDWARE::set_Freq(uint32_t freq)
{
  int64_t flag_freq = 1 << 14;
  int64_t scale = 1 << 28;
  int64_t n_reg = int64_t(freq * scale / 25.0e6);
  int64_t n_low = n_reg & 0x3fff;
  int64_t n_hi  = (n_reg >> 14) & 0x3fff;
  uint8_t buf[6];

  buf[0] = (flag_freq | n_low) / (0x100);
  buf[1] = (flag_freq | n_low) % (0x100);
  buf[2] = (flag_freq | n_hi) / (0x100);
  buf[3] = (flag_freq | n_hi) % (0x100);
  buf[4] = (0x2000) / (0x100);
  buf[5] = (0x2000) % (0x100);

  //logger(buf, 6);

  decoder.activePort(1);
  sleep_us(1);   // 240411 add
  Spi::setProperties(8, 1, 1);
  spi_write_blocking(spi_default, buf, 2);
  sleep_us(1); // 240411 add
  spi_write_blocking(spi_default, buf + 2, 2);
  sleep_us(1); // 240411 add
  spi_write_blocking(spi_default, buf + 4, 2);
  sleep_us(1);
  decoder.activePort(7); //240411  add
}

void HARDWARE::get_result_from_adc()
{
  ADC_IS_READY_TO_READ = false;
  conv->disable();
  sleep_us(10);
  conv->enable();
}
void HARDWARE::set_BiasV(int32_t BiasV)
{
//   code  22 , 2, 8, 0, 1, 1, value 
  if (!flgVirtual)
  { 
   switch (HARDWAREVERSION)
      {
      case WB:
              dacbv->writeB(-BiasV+ShiftDac);
              break;
      case BB:
              dacbv->writeB(-BiasV+ShiftDac); //+
              break;    
  case BBFPGA:
              dacbv->writeB(BiasV+ShiftDac); //+
              break;       
      }
  }    
/*
 if  (flgDebug)
 {
  afc.clear();
  afc =code+std::to_string(DEBUG)+ "debug Bias"+ std::to_string(BiasV);
  afc += endln;
  std::cout << afc;
  afc.clear();
  sleep_ms(100);
 }
*/
}   
void HARDWARE::setLoopSign_BiasV(int32_t BiasV,int32_t flg,int32_t SignLoopValue,int32_t SetPointValue)
{
 switch  (HARDWAREVERSION) 
  {
case BBFPGA:
         break;
case    BB:
         break;
case    WB:
         SignLoop=SignLoopValue;// debug
         //втянуть
         if (flg==1) 
         {
          retract(); 
          sleep_ms(50);
          switch (SignLoopValue)
          {
           case 0:{signloopport->disable(); break;} // +
           case 1:{signloopport->enable();  break;} // -
          }
          set_BiasV(BiasV);
          sleep_ms(100);
          set_SetPoint(SetPointValue);
          //вытянуть
           protract();
           sleep_ms(200);
         }
         else set_BiasV(BiasV);
        break;
  } 
 if  (flgDebug)
 {
  afc.clear();
  afc =code+std::to_string(DEBUG)+ "debug Bias="+ std::to_string(BiasV)+"retract="+std::to_string(flg)+"sign= "+std::to_string(SignLoopValue)
  +"setpoint="+std::to_string(SetPointValue);
  afc +=endln;
  std::cout << afc;
  afc.clear();
  sleep_ms(100);
 }
}
void HARDWARE::setLoopSign(int32_t value) //??????
{
   switch  (HARDWAREVERSION) 
  {
case BBFPGA:
     {
      FPGAWriteData writedata;
      writedata.addr=arrLoopModule.pidControl;
   //   if (value==1) PID_CONTROL=PID_CONTROL|(1<<7);  // *(-1)
   //                 PID_CONTROL=PID_CONTROL&(~(1<<7)) ;
      if (value==1) PID_CONTROL=PID_CONTROL|0x00000080;  // *(-1)
      else          PID_CONTROL=PID_CONTROL&0xFFFFFF7F;
      writedata.data=PID_CONTROL;
      WriteDataToFPGA(writedata);
      sleep_ms(10);
      if (flgDebug)  
      {
       afc.clear();
       afc = code+std::to_string(DEBUG)+"debug set PID sign PIDcontrol="+std::to_string(PID_CONTROL)
       + " adress=" +std::to_string(writedata.addr);
       afc += endln;
       std::cout << afc;
       afc.clear();
       sleep_ms(100); 
       }  
       break;
      } 
case  BB:
         break;
case  WB:
        // SignLoop=value;// debug
         //втянуть
       //  retract(); 
       //  sleep_ms(50);
         switch (value)
        {
         case 0:{signloopport->disable(); break;} // +
         case 1:{signloopport->enable();  break;} // -
        }
        //вытянуть
      //  protract();
      //  sleep_ms(200);
        break;
  } 
 /*
  afc.clear();
  afc = code+std::to_string(DEBUG)+ "debug sign loop " + std::to_string(value);
  afc += endln;
  std::cout << afc;
  afc.clear();
  sleep_ms(100);
 */
}
void  HARDWARE::

ChooseLoopChannelInputFPGA(uint8_t dev, uint8_t nloop)
{
  //FPGA only!
  uint8_t channel;
  FPGAWriteData writedata;
  writedata.addr=inSwitchadress;
  uint32_t chnl_select;
  switch (dev)
  {
   case     STM: channel=channelcurrent;
                 break;
   case     SFM: channel=channelampl;
                 break;
   case  SICMDC: channel=channelcurrent;
                 break;
  }
  chnl_select=1<<(2*channel+nloop); //??? for LOOP1  SFM =8 ; STM=32  
  writedata.data=(int32_t)chnl_select;       // set channel
  WriteDataToFPGA(writedata);
  sleep_ms(10);
 
  if (flgDebug)  
  {
   afc.clear();
   afc = code+std::to_string(DEBUG)+"debug PID Channel ="+ std::to_string(chnl_select)
             +"pidcontrol="+std::to_string(PID_CONTROL);
   afc +=endln;
   std::cout << afc;
   afc.clear();
   sleep_ms(100);
  } 
  
}
void HARDWARE::setSignal_In_Loop(uint8_t value)
{
  switch  (HARDWAREVERSION) 
 {
case BBFPGA:
        {
         break;
        }   
case    BB:
         break;
case    WB:
  {
    switch (value)
   {
    case 0:{integratorinport->disable(); break;}// I
    case 1:{integratorinport->enable();  break;}// Ampl
   }
   break;
  }
 }
} 
void HARDWARE::use_LowPassFilterADC(uint8_t turnon, uint8_t nchannel)
{
//Регистр управления controlReg2[31..0].
//controlReg2[3..0].FirSelect - 4 бита номера канала АЦП для подключения фильтра;
//controlReg2[8].FirEnable - бит включения/выключения фильтра
  FPGAWriteData writedata;
  writedata.addr=arrADCadress.FilterADC;
  if (turnon==1) { writedata.data=nchannel+(1<<8);}//7
            else { writedata.data=nchannel;       }
  WriteDataToFPGA(writedata);
  sleep_ms(10);
  if (flgDebug)  
  {
   afc.clear();
   afc = code+std::to_string(DEBUG)+"debug PID LowPass Filter Channel ="+ std::to_string(nchannel)
         + " turnon="+std::to_string(writedata.data);
   afc +=endln;
   std::cout << afc;
   afc.clear();
   sleep_ms(100);
  }  
}
void HARDWARE::setUseSD(int8_t value)
{ 
  switch (value)
 {
  case 0:{usesdport->disable(); break;} 
  case 1:{usesdport->enable();  break;} //Loop use модуляцию I->SD
 }
}
 
void HARDWARE::setSensor(int8_t value)
{
   switch (value)
 {
  case 0:{sensorport->disable(); break;}// cantilever
  case 1:{sensorport->enable();  break;}// piezo
 }
} 

void HARDWARE::setModulateU(int8_t value)
{
  switch (value)
 {
  case 0:{modulateuport->disable(); break;}
  case 1:{modulateuport->enable();  break;}// вкл модуляцию U
 }
}
void HARDWARE::init_Commutation(int8_t sensor ,uint8_t dev)
{ 
   setSensor(sensor); 
 switch (dev)
 {
   case STM: 
             setLoopSign(SignalIncrease);
             setSignal_In_Loop(0); //not use sd
             setUseSD(0);
             setModulateU(0);
             break; 
   case SFM: setLoopSign(SignalDecrease);
             setSignal_In_Loop(1); 
             setUseSD(1);
             setModulateU(0);
             break;
case SICMDC: setLoopSign(SignalDecrease);
             setSignal_In_Loop(0); 
             setUseSD(0);
             setModulateU(0);
             break;
 }
 afc.clear();
 afc = code+std::to_string(DEBUG)+ "Init commutation " +"dev="+std::to_string(dev)+" signalIncrease " +std::to_string(SignalIncrease)+ " in-loop"+std::to_string(IPin);
 afc+=" sensor "+std::to_string(sensor);
 afc +=endln;
 std::cout << afc;
 afc.clear();
 sleep_ms(100);
}

void HARDWARE::init_SPI( uint8_t port ,uint8_t v2 ,uint8_t v3, uint8_t v4 )
{
 decoder.activePort(port);
 Spi::setProperties(v2, v3, v4);
}

void HARDWARE::init_DACSetPoint(uint8_t spiport) //  4 для подставки
{
  dacspt->initialize(spiport); //code 23
}
void HARDWARE::init_DACBiasV(uint8_t spiport) //  4 для подставки
{
  dacbv->initialize(spiport); //code 23
}
void HARDWARE::init_DACXY(uint8_t spiport) //spi port
{
  dacxy->initialize(spiport); //code 27
  dacxy->setSpiProps();
  dacxy->writeA(0);
  dacxy->writeB(0);
}

void HARDWARE::init_DACZ(uint8_t spiport)
{
  dacz->initialize(spiport); //code 27
  dacz->setSpiProps();  // 241214
}
void HARDWARE::move_scannerX(int x)
{
 dacxy->writeA(x);
}
void HARDWARE::move_scannerY(int y)
{
 dacxy->writeB(y);
}

uint8_t HARDWARE::ReadDataFromFPGAArray(uint8_t count,uint32_t adr, int32_t *arrayout)
{
 if (!flgVirtual) 
 {
  uint8_t szread=8;
  uint8_t szasc=2+4+count*4+2; //get array adc 0A 80 adress dataarray BB 0A
  uint8_t outbuffer[szread];
  uint8_t inbuffer[szasc];
  FPGAReadDataArrayALL readdata;
  readdata.addr=adr;
  outbuffer[0]=readdata.delimbegin;
  outbuffer[1]=readdata.cmd+count; //// 0x40 + 0x0C  read array 12 registers
  outbuffer[2]=(uint8_t)((readdata.addr&0xFF000000)>>24);
  outbuffer[3]=(uint8_t)((readdata.addr&0x00FF0000)>>16);  
  outbuffer[4]=(uint8_t)((readdata.addr&0x0000FF00)>>8);
  outbuffer[5]=(uint8_t)(readdata.addr&0x000000FF);
  outbuffer[6]=readdata.crcpar;
  outbuffer[7]=readdata.delimend;
  while (uart_is_readable(FPGA_UART_ID)) //clean buffer
  { //250401 add oni
   tight_loop_contents();
   uart_get_hw(FPGA_UART_ID)->dr;
  }
  uart_write_blocking(FPGA_UART_ID, outbuffer,szread);
  uart_read_blocking(FPGA_UART_ID, inbuffer,szasc);   
  uint8_t k=6; 
 // ACK(0x80) + READM(0x40 + COUNT(0x0C)) = 0xCC,
   uint32_t val;  
   std::string afcc;
   if (flgDebug)  
   {
     afcc.clear();
     afcc=code+std::to_string(DEBUG)+" read LOOP Params"; 
   }
   if(inbuffer[1]==(FPGAASCREADMAll+count)) //???? get array adc 0A 80 adress dataarray BB 0A
    {
     for (size_t j = 0; j < count;j++)
     {
       val=(inbuffer[k]<<24)+(inbuffer[k+1]<<16)+(inbuffer[k+2]<<8)+inbuffer[k+3];
       arrayout[j]=(int32_t)val;
       k+=4;
       if (flgDebug)  
       {
       // afcc +=separator + std::to_string(float(arrayout[j]));
        afcc +=separator + std::to_string((arrayout[j]));
       }
     } 
     if (flgDebug)  
     { 
       afcc +=endln;
       std::cout << afcc;
       sleep_ms(200);
       afcc.clear();
     }
     return 0;   // ok
    }
    else return 1;
  }     
   else return 0;// error 

}
uint8_t HARDWARE::ReadDataFromFPGAArrayALL(uint16_t *arrayout) 
{
 uint8_t   res=0;
 if (!flgVirtual) 
 {
  uint8_t count=NmbADCSignals; //fix
  uint8_t szread=8;
  uint8_t szasc=2+4+count*4+2;  //=56 bytes//40;  //get array adc 0A 80 adress dataarray BB 0A
  uint8_t outbuffer[szread];
  uint8_t inbuffer[szasc];
  uint8_t res;
  FPGAReadDataArrayALL readdata;
  readdata.addr=arrADCadress.Z;
  outbuffer[0]=readdata.delimbegin;
  outbuffer[1]=readdata.cmd+count; // 0x40 + 0x0C  read array 12 registers
  outbuffer[2]=(uint8_t)((readdata.addr&0xFF000000)>>24);
  outbuffer[3]=(uint8_t)((readdata.addr&0x00FF0000)>>16);  
  outbuffer[4]=(uint8_t)((readdata.addr&0x0000FF00)>>8);
  outbuffer[5]=(uint8_t)((readdata.addr&0x000000FF));
  outbuffer[6]=readdata.crcpar;
  outbuffer[7]=readdata.delimend;
  uint8_t dst;
  while (uart_is_readable(FPGA_UART_ID)) //clean buffer
  { //250401 add oni
   tight_loop_contents();
   dst = (uint8_t) uart_get_hw(FPGA_UART_ID)->dr;
  }
   uart_write_blocking(FPGA_UART_ID, outbuffer,szread);
   uart_read_blocking(FPGA_UART_ID, inbuffer,szasc); 
   uint8_t k=6; //0A 80 adress=4 ??
   uint32_t val;
   res=1;
   if(inbuffer[1]==(FPGAASCREADMAll+count)) //???? get array adc 0A 80 adress dataarray BB 0A
    {
      res=0;
     for (size_t j = 0; j < count;j++)
      {
       val=(inbuffer[k]<<24)+(inbuffer[k+1]<<16)+(inbuffer[k+2]<<8)+inbuffer[k+3];
       arrayout[j]=(uint16_t)val;
       k+=4;
      }
    }   
  }
   return res; 
 }

int32_t HARDWARE::ReadDataFromFPGA(uint32_t adress)
{
 int32_t flgOk;
 FPGAReadData readdata;
 flgOk=1;
 if (!flgVirtual) 
 {
  uint8_t szread=8;
  uint8_t szasc=12; //get array adc 0A 80 adress data BB 0A
  uint8_t outbuffer[szread];
  uint8_t inbuffer[szasc];
  readdata.addr=adress;
  outbuffer[0]=readdata.delimbegin;
  outbuffer[1]=readdata.cmd;  // 0x00
  outbuffer[2]=(uint8_t)((readdata.addr&0xFF000000)>>24);
  outbuffer[3]=(uint8_t)((readdata.addr&0x00FF0000)>>16);  
  outbuffer[4]=(uint8_t)((readdata.addr&0x0000FF00)>>8);
  outbuffer[5]=(uint8_t)(readdata.addr&0x000000FF);
  outbuffer[6]=readdata.crcpar;
  outbuffer[7]=readdata.delimend;
 /*
  if (flgDebug)  
  {
    std::string afcc;
    afcc.clear();
    afcc=code+std::to_string(DEBUG)+" to read "; 
    for (size_t j = 0; j < szread; ++j)
    {
      afcc +=separator + std::to_string(outbuffer[j]);
    }
    afcc +=endln;
    std::cout << afcc;
    sleep_ms(200);
    afcc.clear();
  }
 */
 while (uart_is_readable(FPGA_UART_ID)) //clean buffer //250402
 { 
  tight_loop_contents();
  uart_get_hw(FPGA_UART_ID)->dr;
 }
  uart_write_blocking(FPGA_UART_ID, outbuffer,szread);
  uart_read_blocking(FPGA_UART_ID, inbuffer,szasc);   
 //get array adc 0A 80 adress data BB 0A
  if(inbuffer[1]==FPGAREADOK) { flgOk=0;}
                         else { flgOk=1;}                         
  if (flgDebug)  
    {
      uint32_t res;
      std::string afcc;
      afcc.clear();
      afcc=code+std::to_string(DEBUG)+" read="+ std::to_string(flgOk) ;  
      res=(inbuffer[6]<<24)+(inbuffer[7]<<16)+(inbuffer[8]<<8)+inbuffer[9];
      afcc +=" val=" + std::to_string(res); 
      afcc +=endln;
      std::cout << afcc;
      sleep_ms(200);
      afcc.clear();
    }
 } 
  return flgOk;
}
void HARDWARE::AscResult(FPGAAscData ascdata, uint8_t* dst, size_t len)
{
  uint8_t buffer[8];
  memcpy(buffer, &ascdata, sizeof(ascdata));
  if (flgDebug)  
  {
    std::string afcc;
    afcc.clear();
    afcc=code+std::to_string(DEBUG); 
    for (size_t j = 0; j < sizeof(ascdata); ++j)
    {
      afcc +=separator + std::to_string(buffer[j]);
    }
    afcc +=endln;
    std::cout << afcc;
    sleep_ms(200);
    afcc.clear();
  }
  if (uart_is_readable(FPGA_UART_ID)) 
  {
    uart_read_blocking(FPGA_UART_ID, dst,len);   
  }
}

void HARDWARE::WriteDataToFPGA(FPGAWriteData writedata)
{
 /*
    big_endian !!!! старшие - младшие байты
    truct.pack("BBBBBBBBBBBB",self.DELIM, self.CMD_WRITE,\
    (address&0xFF000000)>>24,(address&0x00FF0000)>>16, (address&0x0000FF00)>>8,(address&0x000000FF),\
    (data&0xFF000000)>>24,(data&0x00FF0000)>>16, (data&0x0000FF00)>>8,(data&0x000000FF),\
    self.CMD_CRC, self.DELIM
 */ 
if(!flgVirtual)
{
  uint8_t flgOK;          
  size_t szwrite;
  size_t szasc=8;//12;//8;//2; //for ASC FPGA
  szwrite=sizeof(writedata);
  uint8_t buffer[szwrite];
  uint8_t outbuffer[szasc];
  buffer[0]=writedata.delimbegin;
  buffer[1]=writedata.cmd;
  buffer[2]=(uint8_t)((writedata.addr&0xFF000000)>>24);
  buffer[3]=(uint8_t)((writedata.addr&0x00FF0000)>>16);  
  buffer[4]=(uint8_t)((writedata.addr&0x0000FF00)>>8);
  buffer[5]=(uint8_t)( writedata.addr&0x000000FF);
  buffer[6]=(uint8_t)((writedata.data&0xFF000000)>>24);
  buffer[7]=(uint8_t)((writedata.data&0x00FF0000)>>16);;
  buffer[8]=(uint8_t)((writedata.data&0x0000FF00)>>8);
  buffer[9]=(uint8_t)( writedata.data&0x000000FF);
  buffer[10]=writedata.crcpar;
  buffer[11]=writedata.delimend;
 /* if (flgDebug)  
  {
    std::string afcc;
    afcc.clear();
    afcc=code+std::to_string(DEBUG)+"FPGA write sz="+std::to_string(szwrite); 
    for (size_t j = 0; j < szwrite; ++j)
    {
      afcc +=separator + std::to_string(buffer[j]);
    }
    afcc +=endln;
    std::cout << afcc;
    sleep_ms(200);
    afcc.clear();
  }
*/
  uint8_t dst;
  while (uart_is_readable(FPGA_UART_ID)) //clear buffer
   { 
    tight_loop_contents();
    dst = (uint8_t) uart_get_hw(FPGA_UART_ID)->dr;
   }
  while (!uart_is_writable(FPGA_UART_ID)) {sleep_ms(30);} 
  uart_write_blocking(FPGA_UART_ID, buffer,szwrite);
 /*
  if (flgDebug)  
  { 
    uart_read_blocking(FPGA_UART_ID, outbuffer,szasc);
    std::string afcc;
    afcc.clear();
    afcc=code+std::to_string(DEBUG)+"FPGA write ask"+separator+std::to_string(readpidcontroltok); 
    {
      afcc +=separator +"ask="+std::to_string(outbuffer[1])+
       " adress "+std::to_string((outbuffer[2]<<24)+(outbuffer[3]<<16)+(outbuffer[4]<<8)+outbuffer[5]);
    }
    afcc +=endln;
    std::cout << afcc;
    sleep_ms(200);
    afcc.clear();
  }
  */
 }
}

void HARDWARE::set_SetPoint( int32_t SetPoint)
{//  code  22, 2, 8, 0, 1, 0, value
  int32_t readsetpointok;
  setpoint=(uint16_t)(SetPoint+ShiftDac);
  if (!flgVirtual)
  {
      switch (HARDWAREVERSION)
      {
      case WB:
              dacspt->writeA(SetPoint+ShiftDac); 
              break;
      case BB:
              dacspt->writeA(SetPoint+ShiftDac);
              break;       
  case BBFPGA:
              FPGAWriteData writedata;
              writedata.addr=arrLoopModule.wbSetpoint;
              writedata.data=(uint32_t)(SetPointScale*(SetPoint));   //+ShiftDac ?????ABS
              WriteDataToFPGA(writedata);
              sleep_ms(200);
              readsetpointok=ReadDataFromFPGA(arrLoopModule.wbSetpoint);
              break;
     }
  } 
  if  (flgDebug)
  {
   afc.clear();
   afc =code+std::to_string(DEBUG)+ "debug SetPoint write="+ std::to_string(SetPoint) ;
   if (HARDWAREVERSION==BBFPGA)  afc+=", SetPoint read OK="+ std::to_string(readsetpointok);
   afc += endln;
   std::cout << afc;
   afc.clear();
   sleep_ms(100); 
  }
}

void HARDWARE::set_GainApmlMod(uint8_t gain)
{
  uint8_t intBuf[1]; 
 // if (!flgVirtual)
//  { 
  /*  241113
    decoder.activePort(5);
    Spi::setProperties(8, 0, 0);
    intBuf[0] = 0;
    spi_write_blocking(spi_default, intBuf, 1); 
  //  sleep_us(2);//240405 
   if (HARDWAREVERSION==WB) intBuf[0] = 255-(uint8_t)gain;
   else                     intBuf[0] = (uint8_t)gain;
    spi_write_blocking(spi_default, intBuf, 1);
 //   sleep_us(2);//240405 
    decoder.activePort(7);
  } 
*/
    switch (HARDWAREVERSION)
      {
      case WB:
             decoder.activePort(5);
             Spi::setProperties(8, 0, 0);
             intBuf[0] = 0;
             spi_write_blocking(spi_default, intBuf, 1); 
             intBuf[0] = 255-(uint8_t)gain;               
             spi_write_blocking(spi_default, intBuf, 1);
             decoder.activePort(7); intBuf[0] = (uint8_t)gain;
             break;
      case BB:
             decoder.activePort(5);
             Spi::setProperties(8, 0, 0);
             intBuf[0] = 0;
             spi_write_blocking(spi_default, intBuf, 1); 
             intBuf[0] = (uint8_t)gain;
             spi_write_blocking(spi_default, intBuf, 1);
             decoder.activePort(7);
             break;       
  case BBFPGA:
             decoder.activePort(5);
             Spi::setProperties(8, 0, 0);
             intBuf[0] = 0;
             spi_write_blocking(spi_default, intBuf, 1); 
             intBuf[0] = (uint8_t)gain;
             spi_write_blocking(spi_default, intBuf, 1);
             decoder.activePort(7);
             break;
     }
  if (flgDebug)  
  {
   afc.clear();
   afc =code+std::to_string(DEBUG)+ "debug Ampl Gain "+ std::to_string(gain);
   afc += endln;
   std::cout << afc;
   afc.clear();
   sleep_ms(100); 
  } 
}
void HARDWARE::set_GainPID(uint32_t gain)
{ 
    uint8_t ti; 
   switch (HARDWAREVERSION)  
  {
  case BB:
      uint8_t tiadd;
      ti=(uint8_t)(gain>>8);
      tiadd=(uint8_t)(gain&0x00FF);
      if (!flgVirtual) 
      {  
       std::string binary = std::bitset<3>(ti).to_string();
       binary[2] == '1' ? gainPID0->enable() : gainPID0->disable();
       binary[1] == '1' ? gainPID1->enable() : gainPID1->disable();
       binary[0] == '1' ? gainPID2->enable() : gainPID2->disable();
       uint8_t intBuf[1]; 
       decoder.activePort(6);
       Spi::setProperties(8, 0, 0);
       intBuf[0] = 0;
       spi_write_blocking(spi_default, intBuf, 1); 
       intBuf[0] = tiadd;
       spi_write_blocking(spi_default, intBuf, 1); 
       decoder.activePort(7);
      }
      if (flgDebug)  
      {
       afc.clear();
       afc = code+std::to_string(DEBUG)+"debug PID Gain ti="+ std::to_string(ti)+"ti add="+ std::to_string(tiadd)+ "gainprev="+std::to_string(LOOPGain);
       afc += endln;
    //   SendDataSynchro(flgDebugSynchronize,afc);//250705
       std::cout << afc;
       afc.clear();
       sleep_ms(100); 
      }  
      break;
  case WB:  
     if (!flgVirtual) 
     { 
      if (LOOPGain<=gain)
      {
       for (size_t i = LOOPGain; i <= gain; i++)
       {
        ti=(uint8_t)i;
        uint8_t intBuf[1]; 
        decoder.activePort(6);
        Spi::setProperties(8, 0, 0);
        intBuf[0] = 0;
        spi_write_blocking(spi_default, intBuf, 1); 
        intBuf[0] = ti;
        spi_write_blocking(spi_default, intBuf, 1); 
        decoder.activePort(7);  
        sleep_ms(10);
       }
      }
      else
      {      
        for (size_t i = LOOPGain; i > gain; i--)
        {
         ti=(uint8_t)i;
         uint8_t intBuf[1]; 
         decoder.activePort(6);
         Spi::setProperties(8, 0, 0);
         intBuf[0] = 0;
         spi_write_blocking(spi_default, intBuf, 1); 
         intBuf[0] = ti;
         spi_write_blocking(spi_default, intBuf, 1); 
         decoder.activePort(7);
          sleep_ms(10);  
        }
      }
      }
     if (flgDebug)  
     {
       afc.clear();
       afc = code+std::to_string(DEBUG)+"debug PID Gain WB "+ std::to_string(255-gain)+ "gainprev="+std::to_string(LOOPGain); //?????
       afc += endln;
       SendDataSynchro(flgDebugSynchronize,afc); //250705
       afc.clear();
       sleep_ms(100); 
     } 
     break;
 case  BBFPGA:   
     {
      FPGAWriteData writedata;
      writedata.addr=arrLoopModule.wbKx[0];// 1 0x08430000;  //adress gain need sign
      loopParams.Ki=int32_t(gain/loopParams.scale*loopParams.GainScaleVal);
      loopParams.K1=loopParams.Ki;//loopParams.Kp++loopParams.Kd;
      writedata.data=loopParams.K1; //uint32_t(gain*0.00001*GainScaleVal);    //(uint32_t)gain; // gain need sign??    
    /*
      afc.clear();
      afc = code+std::to_string(DEBUG)+"debug PID Gain ti="+ std::to_string(gain)
            +" gainscaleval=" +std::to_string(loopParams.GainScaleVal);
      afc += endln;
      std::cout << afc;
      afc.clear();
      sleep_ms(100);
    */    
      WriteDataToFPGA(writedata);       
      sleep_ms(10);
      if (flgDebug&&(!flgVirtual))  
       {
        int32_t arrayout[9];
        ReadDataFromFPGAArray(9,arrLoopModule.wbKx[0],arrayout); 
       }  
     /*
      if (abs(LOOPGain)<=abs(gain))
      {
       for (size_t i = abs(LOOPGain); i >= abs(gain); i--)
       {
        writedata.data=gain;
        WriteDataToFPGA(writedata);
        sleep_ms(10);
       }
      } 
      else
      {
        for (size_t i = abs(LOOPGain); i > abs(gain); i--)
        {
          writedata.data=gain;
          WriteDataToFPGA(writedata); 
          sleep_ms(10);
        }
      } 
     */
      break;
     }    
  }
  LOOPGain=gain;
 }
 void HARDWARE::set_GainPIDFPGA(uint32_t gain)
 { 
    if (!flgVirtual) 
    { 
      FPGAWriteData writedata;
      writedata.addr=arrLoopModule.wbKx[0];//  0x08430000;  //adress gain need sign
    //  writedata.data=(uint32_t)gain; // gain need sign
    //  WriteDataToFPGA(writedata);  
     if (LOOPGain<=gain)
     {
     for (size_t i = LOOPGain; i > gain; i--)
     {
      writedata.data=(uint32_t)gain; // gain need sign
      WriteDataToFPGA(writedata);
      sleep_ms(10);
     }
     } 
     else
     {
      for (size_t i = LOOPGain; i > gain; i--)
      {
        writedata.data=(uint32_t)gain; // gain need sign
        WriteDataToFPGA(writedata); 
        sleep_ms(10);
      }
     } 
    }
    else //virtual
    {
      FPGAWriteData writedata;
      writedata.addr=arrLoopModule.wbKx[1];
      writedata.data=(uint32_t)gain; // gain need sign
      WriteDataToFPGA(writedata);
    } 
    if (flgDebug)  
    {
     afc.clear();
     afc = code+std::to_string(DEBUG)+"debug PID Gain FPGA "+ std::to_string(gain); 
    }  
  if (flgDebug)  
  {
   afc += endln;
   std::cout << afc;
   afc.clear();
   sleep_ms(100); 
  } 
  LOOPGain=gain; 
 }
/*
void HARDWARE::set_GainPID(uint16_t gain)
{
   uint8_t ti;
   if (HARDWAREVERSION==BB)  //BB
  {
   uint8_t tiadd;
   ti=(uint8_t)(gain>>8);
   tiadd=(uint8_t)(gain&0x00FF);
   if (!flgVirtual) 
   {  
    std::string binary = std::bitset<3>(ti).to_string();
    binary[2] == '1' ? gainPID0->enable() : gainPID0->disable();
    binary[1] == '1' ? gainPID1->enable() : gainPID1->disable();
    binary[0] == '1' ? gainPID2->enable() : gainPID2->disable();
     // отладка SPI
     uint8_t intBuf[1]; 
     decoder.activePort(6);
     Spi::setProperties(8, 0, 0);
     intBuf[0] = 0;
     spi_write_blocking(spi_default, intBuf, 1); 
     intBuf[0] = tiadd;
     spi_write_blocking(spi_default, intBuf, 1); 
     decoder.activePort(7);
    }
    if (flgDebug)  
    {
     afc.clear();
     afc = code+std::to_string(DEBUG)+"debug PID Gain "+ std::to_string(ti)+' '+ std::to_string(tiadd);
    }  
  }  
  else  //add 240603 WB+WBFPGA
  {
    ti=(uint8_t)gain; 
    if (!flgVirtual) 
    { 
     if (HARDWAREVERSION==WB)
     {
       if (LOOPGain<=gain)
      {
       for (size_t i = LOOPGain; i <= gain; i++)
       {
        ti=(uint8_t)i;
        uint8_t intBuf[1]; 
        decoder.activePort(6);
        Spi::setProperties(8, 0, 0);
        intBuf[0] = 0;
        spi_write_blocking(spi_default, intBuf, 1); 
        intBuf[0] = ti;
        spi_write_blocking(spi_default, intBuf, 1); 
        decoder.activePort(7);  
        sleep_ms(10);
       }
      }
      else
      {      
        for (size_t i = LOOPGain; i > gain; i--)
        {
         ti=(uint8_t)i;
         uint8_t intBuf[1]; 
         decoder.activePort(6);
         Spi::setProperties(8, 0, 0);
         intBuf[0] = 0;
         spi_write_blocking(spi_default, intBuf, 1); 
         intBuf[0] = ti;
         spi_write_blocking(spi_default, intBuf, 1); 
         decoder.activePort(7);
          sleep_ms(10);  
        }
      }
     }
     else //Use FPGA
     {
      FPGAWriteData writedata;
      writedata.addr=arrLoopModule.wbKx[0];//  0x08430000;  //adress gain need sign
      writedata.data=(uint32_t)ti; // gain need sign
      WriteDataToFPGA(writedata);
     }    
    }
    else //virtual
    {
     if (HARDWAREVERSION==BBFPGA)
     {
      FPGAWriteData writedata;
      writedata.addr=arrLoopModule.wbKx[0];
      writedata.data=(uint32_t)ti; // gain need sign
      WriteDataToFPGA(writedata);
     }
     if (flgDebug)  
     {
      afc.clear();
      afc = code+std::to_string(DEBUG)+"debug PID Gain "+ std::to_string(255-ti); 
     } 
    } 
  } 
  if (flgDebug)  
  {
   afc += endln;
   std::cout << afc;
   afc.clear();
   sleep_ms(100); 
  }  
}
*/
void HARDWARE::set_clock_enable()
{
  uint8_t intBuf[1];
  Spi::setProperties(8, 1, 1);
  decoder.activePort(7);
  spi_write_blocking(spi_default, intBuf, 1);
}
void HARDWARE::set_DACZero()
{ 
 set_DACXY(0,0); 
 set_DACXY(1,0); 
 set_DACZ(0); 
 sleep_us(10); 
}
void HARDWARE::set_DACXY(uint8_t channel, uint16_t value) 
{
  dacxy->setSpiProps(); 
  if (channel == 0)  dacxy->writeA(value);
  if (channel == 1)  dacxy->writeB(value);
  sleep_us(2);
}

void HARDWARE::set_DACZ(int16_t value) 
{
    switch (HARDWAREVERSION)
  {
   case WB:
       dacz->setSpiProps(); 
       dacz->writeB(int32_t(value)+ShiftDac);
       sleep_us(2);
       break;
   case BB: 
       dacz->setSpiProps(); 
       dacz->writeA(int32_t(value)+ShiftDac);
       sleep_us(2);
       break;
   case BBFPGA:
        FPGAWriteData writedata;
        writedata.addr=arrLoopModule.wbOutShift;
        writedata.data=(int32_t)value;//+ShiftDac); 
        WriteDataToFPGA(writedata);
        break;
  }
}
void HARDWARE::getValuesFromAdc()  // чтение АЦП
{
  if (HARDWAREVERSION!=BBFPGA)
  { 
   repeatTwoTimes();
   repeatTwoTimes(); //241215 delete!!
  }
  else
  {
    ReadDataFromFPGAArrayALL(spiBuf);         
  }
}

uint16_t *HARDWARE::repeatTwoTimes()
{
  get_result_from_adc();
  int j = 0;
  while (!ADC_IS_READY_TO_READ && j++ < 3)
  {
    sleep_us(100); //241215 decrease!!
  }
  return spiBuf;
}

void HARDWARE::retract() //втянуть
{
 if (HARDWAREVERSION!=BBFPGA)
 {
  protractport->enable();  //  port 6   элемент массива портов 
 }
 else
 {
     PID_ENA=1;
     PID_STOP=2;
     PID_CONTROL=PID_CONTROL|0x00000003;
     if (flgDebug)  
     {
      afc.clear();
      afc = code+std::to_string(DEBUG)+"debug retract "+std::to_string(PID_CONTROL);
      afc += endln;
      std::cout << afc;
      afc.clear();
      sleep_ms(100);
     }  
    FPGAWriteData writedata;
    writedata.addr=arrLoopModule.pidControl;
    writedata.data=PID_CONTROL;   
    WriteDataToFPGA(writedata);
 }
}
void HARDWARE::retract(int16_t HeightJump) //втянуть на HeightJump
{
 retract(); 
 set_DACZ(-abs(HeightJump));
}

void HARDWARE::protract() //вытянуть
{
 if (HARDWAREVERSION!=BBFPGA)
 {
  protractport->disable();  //port 6
 } 
 else
 {
   PID_ENA=1;  PID_STOP=0;
   PID_CONTROL=PID_CONTROL|0x00000001;
   PID_CONTROL=PID_CONTROL&0xFFFFFFF1;
   if (flgDebug)  
   {
    afc.clear();
    afc = code+std::to_string(DEBUG)+"debug protract "+std::to_string(PID_CONTROL);
    afc += endln;
    std::cout << afc;
    afc.clear();
    sleep_ms(100);
   }
   FPGAWriteData writedata;
   writedata.data=PID_CONTROL;
   writedata.addr=arrLoopModule.pidControl;
   WriteDataToFPGA(writedata);
 }
}

void HARDWARE::freezeLOOP(uint16_t delay) // заморозить ПИД
{
 if (HARDWAREVERSION!=BBFPGA)
 {
  freezeport->enable(); // 5 элемент массива портов ???
  sleep_ms(delay);
 }
 else 
 {
   PID_ENA=0;
   PID_STOP=0;
   //PID_CONTROL=PID_CONTROL|(0x00000000);
   PID_CONTROL=PID_CONTROL&(0xFFFFFFF0);
   if (flgDebug)  
   {
    afc.clear();
    afc = code+std::to_string(DEBUG)+"debug freeze "+std::to_string(PID_CONTROL);
    afc += endln;
    std::cout << afc;
    afc.clear();
    sleep_ms(100);
   }  
   FPGAWriteData writedata;
   writedata.addr=arrLoopModule.pidControl;  
   writedata.data=PID_CONTROL;
   WriteDataToFPGA(writedata);
 }
}

void HARDWARE::unfreezeLOOP(uint16_t delay)  // разморозить ПИД
{
if (HARDWAREVERSION!=BBFPGA)
 {
  freezeport->disable(); // 5 элемент массива портов ???
  sleep_ms(delay);
 }
 else 
 {
   PID_ENA=1;
   PID_STOP=0;
   PID_CONTROL=PID_CONTROL|0x00000001;
   PID_CONTROL=PID_CONTROL&0xFFFFFFF1;
   if (flgDebug)  
   {
    afc.clear();
    afc = code+std::to_string(DEBUG)+"debug unfreeze "+std::to_string(PID_CONTROL);
    afc += endln;
    std::cout << afc;
    afc.clear();
    sleep_ms(100);
   }  
   FPGAWriteData writedata;
   writedata.addr=arrLoopModule.pidControl;
   writedata.data=PID_CONTROL;
   WriteDataToFPGA(writedata);
 }
}
void HARDWARE::activateError()
{
  while (true)
  {
    ledPort->enable();
    activateRed();
    sleep_ms(1000);
    ledPort->disable();
    activateDark();
    sleep_ms(1000);
  }
}

void HARDWARE::_delay_us(double __us)
{
  uint32_t __count =
    (uint32_t) (__us / 0.008) - 3; // 8ns per cycle for 125MHz, from experimentation remove 3cycles for overhead
  busy_wait_at_least_cycles(__count);
}

void HARDWARE::green()
{
  int i = 0;
  while (i++ < 3)
  {
    activateDark();
    sleep_ms(100);
    activateDark();
    sleep_ms(100);
    activateDark();
    sleep_ms(100);
    activateGreen();
    sleep_ms(100);
  }
}

void HARDWARE::blue()
{
  int i = 0;
  while (i++ < 3)
  {
    activateDark();
    sleep_ms(100);
    activateDark();
    sleep_ms(100);
    activateDark();
    sleep_ms(100);
    activateBlue();
    sleep_ms(100);
  }
}

void HARDWARE::red()
{
  int i = 0;
  while (i++ < 3)
  {
    activateDark();
    sleep_ms(100);
    activateDark();
    sleep_ms(100);
    activateDark();
    sleep_ms(100);
    activateRed();
    sleep_ms(100);
  }
}

void HARDWARE::dark()
{
  int i = 0;
  while (i++ < 3)
  {
    activateDark();
    sleep_ms(100);
    activateDark();
    sleep_ms(100);
    activateDark();
    sleep_ms(100);
    activateDark();
    sleep_ms(100);
  }
}

void HARDWARE::activateGreen()
{
  rdbLed->disable();
  sleep_us(60);
  for (int i = 0; i < 8; ++i)
  {
    rdbLed->enable();
    busy_wait_at_least_cycles(85);
    rdbLed->disable();
    busy_wait_at_least_cycles(35);
  }
  for (int i = 0; i < 16; ++i)
  {
    rdbLed->enable();
    busy_wait_at_least_cycles(35);
    rdbLed->disable();
    busy_wait_at_least_cycles(85);
  }
}

void HARDWARE::activateRed()
{
  rdbLed->disable();
  sleep_us(60);
  for (int i = 0; i < 8; ++i)
  {
    rdbLed->enable();
    busy_wait_at_least_cycles(35);
    rdbLed->disable();
    busy_wait_at_least_cycles(85);
  }
  for (int i = 0; i < 8; ++i)
  {
    rdbLed->enable();
    busy_wait_at_least_cycles(85);
    rdbLed->disable();
    busy_wait_at_least_cycles(35);
  }
  for (int i = 0; i < 8; ++i)
  {
    rdbLed->enable();
    busy_wait_at_least_cycles(35);
    rdbLed->disable();
    busy_wait_at_least_cycles(85);
  }
}

void HARDWARE::activateBlue()
{
  rdbLed->disable();
  sleep_us(60);
  for (int i = 0; i < 16; ++i)
  {
    rdbLed->enable();
    busy_wait_at_least_cycles(35);
    rdbLed->disable();
    busy_wait_at_least_cycles(85);
  }
  for (int i = 0; i < 8; ++i)
  {
    rdbLed->enable();
    busy_wait_at_least_cycles(85);
    rdbLed->disable();
    busy_wait_at_least_cycles(35);
  }
}

void HARDWARE::activateDark()
{
  rdbLed->disable();
  sleep_us(60);
  for (int i = 0; i < 24; ++i)
  {
    rdbLed->enable();
    busy_wait_at_least_cycles(35);
    rdbLed->disable();
    busy_wait_at_least_cycles(85);
  }
}
 void HARDWARE::test()
 {
    loopParams.GainScale=Vector[1];
    SetLOOPParams(loopParams.Kp,loopParams.Ki,loopParams.Kd,loopParams.GainScale);
    FPGAWriteData writedata;
    writedata.addr=arrLoopModule.wbInSetup;// 250623
    writedata.data=loopParams.GainScale;           
    WriteDataToFPGA(writedata);
    sleep_ms(10);
    writedata.data=loopParams.K1;// 1;//uint32_t(1*(2<<GainScale));
    writedata.addr=arrLoopModule.wbKx[0]; //tp
    WriteDataToFPGA(writedata);
    sleep_ms(30);
    writedata.data=loopParams.K2;//uint32_t(0.01*GainScaleVal);
    writedata.addr=arrLoopModule.wbKx[1]; //ti
    WriteDataToFPGA(writedata);
    sleep_ms(30);
    writedata.data=loopParams.K3;//uint32_t(0.0);
    writedata.addr=arrLoopModule.wbKx[2]; 
    WriteDataToFPGA(writedata); 
    sleep_ms(30);
    //scanner->hardware->set_GainPID((uint32_t)Vector[2]);
 }