#include "common_variables.hpp"

//////////////////////////////////////////////
// PICO(BB)=0 ; MotherBoard(WhiteBoard=WB)=1; BlueBoard+FPGA(BBFPGA)=2 ПЛИСC;
//int8_t       HARDWAREVERSION=WB;
//int8_t       HARDWAREVERSION=BB;  
int8_t       HARDWAREVERSION=BBFPGA;
std::string  SOFTVERSION="25.07.09.02 "+std::to_string(HARDWAREVERSION);
//std::string  SOFTVERSION="25.04.16.1 BBFPGA 12_LOOP";
//std::string  SOFTVERSION="25.04.04.1 WB";
std::string  SoftHARDWAREVERSION="0.1";

//int8_t       HARDWAREVERSION_I=-1;
int32_t  vectorSize;
std::string afc;  //dataout string
std::vector<int32_t>  Vector;
std::vector<int32_t>  Vupdateparams;
std::atomic<int16_t>  ALGCODE;
std::atomic<bool>     flgDebugGetOk=false;// need for synchronize debug with PC 
std::atomic<bool>     CONFIG_UPDATE;
std::atomic<bool>     STOP;
std::atomic<bool>     TheadDone;          // need for synchronization with PC 
std::atomic<bool>     DrawDone=true;      // need for synchronization with PC 
std::atomic<bool>     ADC_IS_READY_TO_READ = true;
std::atomic<bool>     flgVirtual = false; // start value!!! setPIDGain
std::atomic<bool>     flgDebug   = false; // start value!!!
std::atomic<uint16_t> delayFW;            // delay in the point FW scanning,..
std::atomic<uint16_t> delayBW;            // delay in the point BW scanning,..
std::atomic<uint16_t> delayHope;          // hopping
std::atomic<uint16_t> ZJump;              // hopping   

const std::string  code="code";  
const std::string  endln="\n";
//=std::endl 
//The usual convention is to just use std::endl instead of simply outputting '\n'; 
// std::endl outputs a '\n' and then flushes the stream ????
const char   separator=',';

bool   AD9833_SENDER = false;
bool   AD8400_SENDER = false;
bool AD8400_SET_GAIN = false;
bool          AD5664 = false;
bool    SET_IO_VALUE = false;
bool ADC_ENABLE_DISABLE = false;
bool ADC_READ_FOREVER = false;
bool    ADC_GET_VALUE = false;
//************************************************
bool    flgDebugSynchronize=false;//true;//false;

bool    flgUseTUD=false;
bool    flgParamsUpdated=false;
bool    flgСritical_section=true;
uint8_t flgDebugLevel = 3;      // уровень отладки
bool    flgUseUART = false;     // использовать UART для передачи данных
bool    flgLocalDebug=false;    // локальная отладка
bool    flgTiming=false;        // рассчет времени выполнения блока
//данные для симуляции 
/* 250409 перенос to dev var
uint8_t SignLoop;
int16_t ZValue         = maxint16_t;
int16_t SignalValue    = maxint16_t;
int16_t ZMaxValue      = maxint16_t;
int16_t SignalMaxValue = maxint16_t; //add 240710
int32_t ShiftDac       = 32768; // =32768 SHIFT 0 (-10V)  32767 - (0V); 65565 (+10V)
int8_t  SetPointScale  = 1;  ///??????

uint8_t ZPin    = 0; // Z
uint8_t AmplPin = 1; // амплитуда
uint8_t IPin    = 2; // ток  
*/

bool Z_STATE = false; //???

critical_section_t criticalSection;


void SendDataSynchro(bool flg, std::string str)
{
  if (flg)
  {
    while( !flgDebugGetOk) {sleep_ms(20);};
  }
  std::cout << str;
  flgDebugGetOk=false;
}