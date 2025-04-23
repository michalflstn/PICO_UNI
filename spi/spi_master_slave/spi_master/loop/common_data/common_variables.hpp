#ifndef PICO_EXAMPLES_COMMON_VARIABLES_HPP
#define PICO_EXAMPLES_COMMON_VARIABLES_HPP

#include <pico/critical_section.h>
#include <string>
#include <vector>
#include <atomic>

#define SFM                      0  // методики
#define STM                      1
#define SICMDC                   3  
#define PROBE                    0  // зонд
#define CANTILEVER               1

//#define maxint16_t               32767  //250409
//#define minint16_t              -32768

#define BB                        0  // код mother board
#define WB                        1
#define BBFPGA                    2
// algorithm codes
#define ALGNONE                   0
#define VersionCmd               10  // версия ПО контроллера
#define ChangeHardWare           11
#define ADC_READCmd              12  //AD7606 timer
#define DebugCmd                 13
#define VirtualCmd               14  // виртуальный режим работы контроллера
#define DebugLevelCmd            15
#define DEBUG                    16  // вывод отладки
#define END                      17  // выполнение алгоритма закончено
#define GET_CURRENTX0Y0          18
#define SET_BIAS                 19
#define SET_SETPOINT             20  //DAC8563_1
#define SET_PID_GAIN             21
#define SET_AMPLMOD_GAIN         22
#define SET_SIGN_LOOP            23
#define ADC_GET_VALUECmd         24  // don't use
#define SetDACZeroCmd            25
#define Set_SIGN_LOOP_BIASV      26  //DAC8563_3
#define ERROR                    27 
#define SET_Z                    28  //DAC8563_3
#define SET_XY                   29  //DAC8563_SET_VOLTAGE_2
#define FREQ_SET                 30  //AD9833
#define STOPPED                  31  //остановка выполнения алгоритма
#define PARAMUPDATEDCmd          32  //do not used
#define TheadDoneCmd             33
#define SetUseCritialSectAlgCode 34
#define RetractAlgCode           35
#define ADC_RESET                36
#define INITCOMMMUTATION         37
#define USEPLDCmd                38  //использовать ПЛИСС
#define ReadFPGA                 39 //test FPGA
//#define SET_PID_GAINFPGA         39  //не нужен 
#define RESONANCE                40  //AD9833 
#define GridMovetoNextScan       41
//#define LOCALDEBUG               42 // debug  в конкретном месте
#define CHRONOMETRIA             42 // debug времени выполнения в конкретном месте
#define SETDEVICE                43  // set device stm,sfm,sicm
#define USELOWPASSFILTER_ADC     44  // вкл фильтр низких частот АЦП
// чтобы включить фильтрацию на канале АЦП x (от 0 до 7) в регистр 0x08410028 нужно записать 0x0000010x, где x - номер канала (от 0 до 7).
// Восьмой бит регистра 0x08410028 отвечает за вкл/выкл фильтра. Младшие биты (3..0) за номер канала, к которому фильтр скоммутирован.
#define SCANNING                 50
#define MOVE_TOX0Y0              51  //переместиться в начальную точку  скана из начальной точке предыдущего скана
#define ChangeSpeed              52  //изменение скорости сканирования в процессе сканирования и тд
#define ChangeHopeParams         53  //изменение высота прыжков+ задержки при hopeсканирования в процессе сканирования и тд

#define CONFIG_UPDATECmd         55  //изменения параметров алгоритма в процессе выполнения
#define FASTSCANNING             56
#define SENDDATALIN              57  //данные линеализации

#define ProtractAlgCode          61
#define FreezePIDAlgCode         62
#define UnFreezePIDAlgCode       63

#define SPECTROSOPY_IV           65
#define SPECTROSOPY_AIZ          66

#define STOPCmd                  70 //остановка алгоритма
#define DRAWDONECmd              71 //окончание отрисовки данных алгоритма

#define APPROACH                 75
#define TESTMOVER                76

#define LID_MOVE_UNTIL_STOP      80

#define LID_MOVE_TOZ0            84 // отвестись в безопасную начальную точку по Z


extern const std::string  code;  
extern const std::string  endln;
extern const char   separator;
extern std::string  SOFTVERSION;
extern std::string  SoftHARDWAREVERSION;  //  {0,1,....} or {0.1, 1.1...} 
extern int8_t       HARDWAREVERSION;      //PICO=0; MotherBoard=1
extern uint32_t     DEBUG_LEVEL;
extern std::string  afc;

extern std::vector<int32_t> Vector;
extern std::vector<int32_t> Vupdateparams;
extern int32_t vectorSize;

extern bool AD9833_SENDER;
extern bool AD8400_SENDER;
extern bool AD8400_SET_GAIN;   // AD8400
extern bool AD5664;            

extern bool ADC_ENABLE_DISABLE;//AD7606 
extern bool ADC_READ_FOREVER;  //AD7606
extern bool ADC_GET_VALUE;     //AD7606

extern bool SET_IO_VALUE;
extern bool Z_STATE;
// add MF
extern std::atomic<int16_t> ALGCODE;
extern std::atomic<bool> CONFIG_UPDATE;
extern std::atomic<bool> STOP;
extern std::atomic<bool> TheadDone;   //need dor synchronization with PC 
extern std::atomic<bool> DrawDone;    //need dor synchronization with PC 
extern std::atomic<bool> ADC_IS_READY_TO_READ;
extern std::atomic<bool> flgVirtual; 
extern std::atomic<bool> flgDebug;
extern std::atomic<uint16_t> delayFW;      //delay in the point FW scanning,..
extern std::atomic<uint16_t> delayBW;      //delay in the point BW scanning,..
extern std::atomic<uint16_t> delayHope;  
extern std::atomic<uint16_t> ZJump;  

extern bool flgUseTUD;
extern bool flgParamsUpdated;
extern bool flgСritical_section;
extern bool flgUseUART;  
extern bool flgUseFPGA;       //Использовать ПЛИСС
extern bool flgLocalDebug;  
extern bool flgTiming; 
/* move to device var 
extern uint8_t SignLoop;
extern uint8_t ZPin;
extern uint8_t AmplPin;       //amplitude
extern uint8_t IPin;          //current
extern uint8_t flgDebugLevel; //leveldebug
extern int16_t ZValue;        //for simulation
extern int16_t SignalValue;   //for simulation   
extern int16_t ZMaxValue;
extern int16_t SignalMaxValue;
extern int32_t ShiftDac;      //=32768 SHIFT 0 (-10V)  32767 - (0V); 65565 (+10V)
extern int8_t  SetPointScale;
*/
extern critical_section_t criticalSection;
#endif //PICO_EXAMPLES_COMMON_VARIABLES_HPP
