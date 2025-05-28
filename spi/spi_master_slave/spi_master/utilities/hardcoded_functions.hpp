#ifndef PICO_EXAMPLES_HARDCODED_FUNCTIONS_HPP
#define PICO_EXAMPLES_HARDCODED_FUNCTIONS_HPP
#include <vector>
#include <ctime>
#include <cstdint>
#include <iostream> 
#include <hardware/clocks.h>

#include "../utilities/base_types/io_ports.h"
#include "../devices/ad5664.hpp"
#include "../devices/DAC8563.hpp"
#include "../physical_devices/LinearDriver.hpp"  //240505
#include "../loop/common_data/device_variables.hpp"
#include "../loop/common_data/common_variables.hpp"


#define portx                    0 //порты сканнера! пьезодвижетеля ?
#define porty                    1

class HARDWARE
{
// WARNING HARDCODED FUNCTIONS
private:
 DAC8563    *dacspt;  // DAC BIAS,SetPoint
 DAC8563    *dacbv;   // DAC BIAS
 DAC8563    *dacxy;   // DAC X,Y
 DAC8563    *dacz;    // DAC Z
 InputPort  *busyport;     // FIXME TEMP!!!
 OutputPort *conv;
 OutputPort *dec;
 OutputPort *resetport; // FIXME TEMP
 OutputPort *ledPort;
 OutputPort *rdbLed;
 OutputPort *io1_0; 
 OutputPort *io1_1;
 OutputPort *gainPID0;
 OutputPort *gainPID1;
 OutputPort *gainPID2;
 OutputPort *freezeport;       // заморозить/разморозить
 OutputPort *protractport;     // втянуть сканнер/вытянуть сканнер
 // add
 OutputPort *modulateuport;    // вкл модуляцию U
 OutputPort *usesdport;        // порты  настройки СД читать modulalate signal I_STM
 OutputPort *sensorport;       // порты  настройки выбор сенсора
 OutputPort *signloopport;     // знак ПИД
 OutputPort *integratorinport; // выбор входного сигнала в ПИД из CD или ПТН(I)


 uint16_t *repeatTwoTimes(); 

 uint16_t gainPID;

 uint8_t PID_FBABS;
 uint8_t PID_ENA;
 uint8_t PID_STOP;
 uint8_t PID_SIGN;
 

  
 void get_result_from_adc();       // чтение АЦП

 void set_clock_enable();
 
 void _delay_us(double __us);
 
 void activateGreen();

 void activateRed();

 void activateBlue();

 void WriteDataToFPGA(FPGAWriteData writedata);

 void AscResult(FPGAAscData ascdata, uint8_t* dst, size_t len);

public:
   uint32_t LOOPGain;
   LinearDriverBase  *linearDriver;

   HARDWARE(ConfigHardWareBB confighardware);

   HARDWARE(ConfigHardWareBBFPGA confighardware);

   HARDWARE(ConfigHardWareWB confighardware);

  ~HARDWARE();

 void activateDark();

 void green();

 void blue();

 void red();
 
 void dark();

[[noreturn]] void activateError();
 //инициирование ЦАП1  SetPoint,BIAS
 void setDefaultSettings( ConfigHardWareBB  confighardware);      //BB

 void setDefaultSettings( ConfigHardWareBBFPGA  confighardware);  //FPGA

 void setDefaultSettings( ConfigHardWareWB  confighardware);      //WB
 
 void SetDev_GetSOFTHARDWAREVersion(uint8_t device);

 void set_Freq(uint32_t freq);    // установка заданной частоты генератора

 void setSignal_In_Loop(uint8_t value); // Ampl=1 ; I=0

 void ChooseLoopChannelInputFPGA(uint8_t dev, uint8_t nloop);
 
 void setUseSD(int8_t value);      // 1-> yes; 0->none use syncrodetector
 
 void setSensor(int8_t value);    // cantilever=1;  probe =0
 
 void setModulateU(int8_t value); // 0-> none; 1->use

 void init_Commutation(int8_t sensor ,uint8_t dev);

 void init_SPI(uint8_t port ,uint8_t v2 ,uint8_t v3, uint8_t v4); //инициирование SPI

 void init_LOOP();

 void init_DACSetPoint(uint8_t spiport);  

 void init_DACBiasV(uint8_t spiport);  

 void init_DACXY(uint8_t spiport);   //инициирование ЦАП2  XY

 void init_DACZ(uint8_t spiport);    //инициирование ЦАП2  Z

 void set_BiasV(int32_t BiasV);      //установка заданного значения напряжения
 //void set_BiasV(int32_t BiasV,int32_t flg,int32_t SignLoopValue,int32_t SetPointValue); // flg=1 изменять знак ПИД
 void setLoopSign_BiasV(int32_t BiasV,int32_t flg,int32_t SignLoopValue,int32_t SetPointValue);

 void setLoopSign(int8_t value);  // 0->+ ; 1-> -1

 void set_SetPoint(int32_t SetPoint);//установка заданной опроры для ПИД

 void set_GainApmlMod(uint8_t gain); //установить усиления модуляции амплитуды

 void set_GainPID(uint16_t gain);    //установить усиления ПИД

 void set_GainPID(uint32_t gain);    //установить усиления ПИД

 void set_GainPIDFPGA(uint32_t gain);    //установить усиления ПИД
 
 void set_DACXY(uint8_t channel, uint16_t value); 

 void set_DACZ(int16_t value); 

 void set_DACZero();

 void use_LowPassFilterADC(uint8_t turnon, uint8_t nchannel);
 
// virtual void SetPIDMode(uint8_t mode)=0;
 
 void reset_ADCPort();
 
 void move_scannerX(int x);

 void move_scannerY(int y);

 //uint16_t *getValuesFromAdc();  // чтение АЦП
 void getValuesFromAdc();  // чтение АЦП

 int32_t ReadDataFromFPGA(FPGAReadData readdata);
 
 uint8_t ReadDataFromFPGAArray(uint8_t count, uint16_t *arrayout);

 uint8_t ReadDataFromFPGAArrayALL(uint16_t *arrayout);

 void retract();       // втянуть сканер

 void retract(int16_t HeightJump); //втянуть на HeightJump

 void protract();      // вытянуть сканер
  
 void freezeLOOP(uint16_t delay);    // заморозить ПИД

 void unfreezeLOOP(uint16_t delay);  // разморозить ПИД 

 // void protract(uint16_t delay,int16_t DacZ0,int16_t HeightJump) ; //разморозить ПИД 
 
};
#endif
