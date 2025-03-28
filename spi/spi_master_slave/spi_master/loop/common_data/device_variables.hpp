#ifndef PICO_EXAMPLES_DEVICE_VARIABLES_HPP
#define PICO_EXAMPLES_DEVICE_VARIABLES_HPP
#pragma  pack(push, 1) //  240702 выравнивание структуры на границе byte
#include <pico.h>
#include "../../utilities/base_types/Spi.hpp"
#include "../../utilities/base_types/decoder.hpp"

// We are using pins 0 and 1, but see the GPIO function select table in the
// datasheet for information on which other pins can be used.
//#define UART_TX_PIN 16 //240703
//#define UART_RX_PIN 17 //240703
//#define USB_UART_ID     uart1//240703
//#define USBUART_TX_PIN  8
//#define USBUART_RX_PIN  9
#define FPGAUART_TX_PIN 8 //!
#define FPGAUART_RX_PIN 9 //!
#define FPGA_UART_ID    uart1
#define FPGA_BAUD_RATE  115200 //400000?
#define NmbADCSignals   8// 12 // ADC + control
/*
register map!!!!!!!!!!!!
0-timestamp_us
1-8 data ADC registers
control registers
9   0x08410024  not use
10  0x08410028 
    включить фильтрацию на канале АЦП x (от 0 до 7) в регистр 0x08410028 
    нужно записать 0x0000010x, где x - номер канала (от 0 до 7).
    В регистре controlReg2 включается фильтр низких частот,
    остальные регистры управления пока не используются.
    Чтобы включить фильтрацию на канале АЦП x (от 0 до 7) в регистр 0x08410028 
    нужно записать 0x0000010x, где x - номер канала (от 0 до 7).
11  0x0841002C  not use   
*/
extern uint16_t spiBuf[NmbADCSignals];
extern uint8_t  FPGADELIM;
extern uint8_t  FPGACRCPAR;
extern uint8_t  FPGAREAD;
extern uint8_t  FPGAREADOK;
extern uint8_t  FPGAREADADC;
extern uint8_t  FPGAREADADCM;
extern uint8_t  FPGAREADADCMALL;
extern uint8_t  FPGAWRITE;
extern uint8_t  FPGAWRITEOK;
extern uint8_t  FPGAASC;
extern uint8_t  FPGAASCREADMAll;
extern uint32_t ZAdress;
extern uint32_t AmplAdress;
extern uint32_t IAdress;

struct ADCData
{
 int16_t Z;
 int16_t Signal;
};
extern ADCData adcdata;

struct ConfigHardWareBB //BB 
{
  uint8_t DACSetPointPort;  //2 DAC8563_1  BIAS SetPoint
  uint8_t DACSetPointMode;  //1 DAC8563_1  BIAS SetPoint
  uint8_t DACBiasVPort;  //2 DAC8563_1  BIAS SetPoint
  uint8_t DACBiasVMode;  //1 DAC8563_1  BIAS SetPoint
  uint8_t DACXYPort;    //3 DAC8563_2  XY
  uint8_t DACXYMode;    //2 DAC8563_2  XY
  uint8_t DACZPort;     //4 dac8563_3  Z
  uint8_t DACZMode;     //1 
  uint8_t BUSYPort;     //16
  uint8_t CONV;         //7
  uint8_t DEC;          //10
  uint8_t ResetPort;    //17
  uint8_t LEDPort;      //PICO_DEFAULT_LED_PIN
  uint8_t RDBPort;      //23
  uint8_t IO1_0;        //11
  uint8_t IO1_1;        //12
  uint8_t GainPID0;     //13
  uint8_t GainPID1;     //14
  uint8_t GainPID2;     //15
  uint8_t FreezePort;   //26 заморозить сканнер=1; разморозить =0
  uint8_t ProtractPort; //27 втянуть    сканнер=1; вытянуть    =0
};

struct ConfigHardWareBBFPGA // BBFPGA
{
  uint8_t DACSetPointPort;  //2 DAC8563_1  BIAS SetPoint
  uint8_t DACSetPointMode;  //1 DAC8563_1  BIAS SetPoint
  uint8_t DACBiasVPort;  //2 DAC8563_1  BIAS SetPoint
  uint8_t DACBiasVMode;  //1 DAC8563_1  BIAS SetPoint
  uint8_t DACXYPort;    //3 DAC8563_2  XY
  uint8_t DACXYMode;    //2 DAC8563_2  XY
  uint8_t DACZPort;     //4 dac8563_3  Z
  uint8_t DACZMode;     //1 
  uint8_t BUSYPort;     //16
  uint8_t CONV;         //7
  uint8_t DEC;          //10
  uint8_t ResetPort;    //17
  uint8_t LEDPort;      //PICO_DEFAULT_LED_PIN
  uint8_t RDBPort;      //23
  uint8_t IO1_0;        //11
  uint8_t IO1_1;        //12
  uint8_t GainPID0;     //13
  uint8_t GainPID1;     //14
  uint8_t GainPID2;     //15
  uint8_t FreezePort;   //26 заморозить сканнер=1; разморозить =0
  uint8_t ProtractPort; //27 втянуть    сканнер=1; вытянуть    =0
};
struct ConfigHardWareWB  //WB
{
  uint8_t DACSetPointPort;    //2 DAC8563_1  BIAS SetPoint
  uint8_t DACSetPointMode; //1 DAC8563_1  BIAS SetPoint
  uint8_t DACBiasVPort;    //2
  uint8_t DACBiasVMode;    //1 DAC8563_1  BIAS SetPoint
  uint8_t DACXYPort;    //3 DAC8563_2  XY
  uint8_t DACXYMode;    //2 DAC8563_2  XY
  uint8_t DACZPort;     //4 dac8563_3  Z
  uint8_t DACZMode;     //1 
  uint8_t BUSYPort;     //16
  uint8_t CONV;         //7
  uint8_t DEC;          //10
  uint8_t ResetPort;    //17
  uint8_t LEDPort;      //PICO_DEFAULT_LED_PIN
  uint8_t RDBPort;      //23
 // uint8_t IO1_0;        //11
 // uint8_t IO1_1;        //12
  uint8_t GainPID0;     //13
  uint8_t GainPID1;     //14
  uint8_t GainPID2;     //15
  uint8_t FreezePort;   //26 заморозить сканнер=1; разморозить =0
  uint8_t ProtractPort; //27 втянуть    сканнер=1; вытянуть    =0

  uint8_t ModulateUPort;   // вкл=1; выкд=0 модуляцию U  
  uint8_t SD_1Port;        // порты  настройки СД I_STM=1; 0 =др
  uint8_t SD_2Port;        // порты  настройки СД Cantilever=0; 1-Piezo
  uint8_t SignLoopPort;    // знак ПИД // 0=+ ; 1=-
  uint8_t Interator_InPort;// выбор вход сигнала на ПИД из1-SD; 0=ПТН(I) 
};

struct FPGALOOPCTRAdress
{
 uint32_t wbKx[3];
 uint32_t wbInMulKoef;
 uint32_t wbInShift;
 uint32_t wbOutMulKoef;
 uint32_t wbOutShift; //DACZ
 uint32_t wbSetpoint;
 uint32_t pidControl;
};
struct FPGA_ADCAdress 
{
  uint32_t Z;
  uint32_t Apml;
  uint32_t I;
  uint32_t Signal4;
  uint32_t Signal5;
  uint32_t Signal6;
  uint32_t Signal7;
  uint32_t Signal8;
};
/*
Rx Frame format big-endian Offs:
  Size:      1       1       4        4         1         1
Fields: [ DELIM ] [ CMD ] [ ADDR ] [<DATA>] [CRC/PAR] [ DELIM ]
*/
 struct FPGAWriteData
 {
  uint8_t  delimbegin=FPGADELIM;
  uint8_t  cmd=FPGAWRITE;
  uint32_t addr;
  uint32_t data;
  uint8_t  crcpar=FPGACRCPAR;
  uint8_t  delimend=FPGADELIM;
 };
 struct FPGAReadData
 {
  uint8_t  delimbegin=FPGADELIM;
  uint8_t  cmd=FPGAREAD;
  uint32_t addr;
  uint8_t  crcpar=FPGACRCPAR;
  uint8_t  delimend=FPGADELIM;
 };
 struct FPGAReadDataArray
 {
  uint8_t  delimbegin=FPGADELIM;
  uint8_t  cmd=FPGAREADADCM;
  uint32_t addr;//=  0x08410004; //dataBufferRd[0]
 //uint8_t  count;//???
  uint8_t  crcpar=FPGACRCPAR;
  uint8_t  delimend=FPGADELIM;
 };
 struct FPGAReadDataArrayALL
 {
  uint8_t  delimbegin=FPGADELIM;
  uint8_t  cmd=FPGAREADADCMALL; ///
  uint32_t addr;//=arrADCadress.Z;//0x08410004; //dataBufferRd[0] Z
  uint8_t  crcpar=FPGACRCPAR;
  uint8_t  delimend=FPGADELIM;
 };
 struct FPGAAscData
 {
  uint8_t  delimbegin=FPGADELIM;
  uint8_t  cmd=FPGAASC;
  uint32_t addr;
  uint8_t  crcpar=FPGACRCPAR;
  uint8_t  delimend=FPGADELIM;
};
struct ConfigLinearDrive
{
  uint8_t XA_Port;     
  uint8_t XB_Port;   
  uint8_t YA_Port;    
  uint8_t YB_Port;     
  uint8_t ZA_Port;     
  uint8_t ZB_Port;     
};
struct ConfigLinearDriveNew
{
  uint8_t A_Port;     
  uint8_t B_Port;   
  uint8_t XTurn_on_Port;    
  uint8_t YTurn_on_Port;     
  uint8_t ZTurn_on_Port;        
};

extern Spi                  spi;
extern Decoder              decoder;
extern ConfigHardWareBB     confighardwareBB; //v0
extern ConfigHardWareBBFPGA confighardwareBBFPGA; //v0
extern ConfigHardWareWB     confighardwareWB; //v1
extern ConfigLinearDrive    configlineardrivev0;
extern ConfigLinearDriveNew configlineardrivev1;
extern FPGALOOPCTRAdress    arrLoopModule_0;
extern FPGA_ADCAdress       arrADCadress;
#endif //PICO_EXAMPLES_DEVICE_VARIABLES_HPP