#include "device_variables.hpp"

//ADCData adcdata;
uint8_t         FPGADELIM=0x0A; //0xAA?
uint8_t        FPGACRCPAR=0xBB;
uint8_t          FPGAREAD=0x00;
uint8_t        FPGAREADOK=0x80; // если выполнена команда чтения и ОК
uint8_t         FPGAWRITE=0x01;
uint8_t           FPGAASC=0x80;
uint8_t       FPGAWRITEOK=0x81; //0x80+cmd?
uint8_t      FPGAREADADCM=0x40;   //read array
uint8_t   FPGAASCREADMAll=FPGAASC+FPGAREADADCM;// 0xCC; //  ACK(0x80) + READM(0x40 + COUNT(0x0C)) ответ на чтение 12 регистров
uint8_t   FPGAREADADCMALL=0x40;//C; // 0x40 + 0x0C  read array 12 registers

uint32_t    ZAdress=0x08410004;
uint32_t AmplAdress=0x08410008;
uint32_t    IAdress=0x0841000C;

uint16_t     spiBuf[NmbADCSignals];

Spi                  spi;
Decoder              decoder(4, 5, 6); 
 
ConfigHardWareBB     confighardwareBB
                      (
                       {                        
                        2, //DACSetPointPort
                        1, //DACSetPointMode
                        2, //DACBiasVPort
                        1, //DACBiasVMode
                        3, //DACXYPort
                        2, //DACXYMode
                        4, //DACZport
                        1, //DACZMode
                        16,//BUSYPort
                        7, //CONV
                        10,//DEC
                        17,//ResetPort
                        PICO_DEFAULT_LED_PIN,//LEDPort
                        23,//RDBPort
                        11,//IO1_0
                        12,//IO1_1
                        13,//GainPID0
                        14,//GainPID1
                        15,//GainPID2
                        26,//FreezePort  заморозить сканнер=1; разморозить =0
                        27 //ProtractPor  втянуть    сканнер=1; вытянуть    =0
                       }
                      );
ConfigHardWareBBFPGA    confighardwareBBFPGA
                      (
                       {                        
                        2, //DACSetPointPort
                        1, //DACSetPointMode
                        2, //DACBiasVPort
                        1, //DACBiasVMode
                        3, //DACXYPort
                        2, //DACXYMode
                        4, //DACZport
                        1, //DACZMode
                        16,//BUSYPort
                        7, //CONV
                        10,//DEC
                        17,//ResetPort
                        PICO_DEFAULT_LED_PIN,//LEDPort
                        23,//RDBPort
                        11,//IO1_0
                        12,//IO1_1
                        13,//GainPID0
                        14,//GainPID1
                        15,//GainPID2
                        26,//FreezePort  заморозить сканнер=1; разморозить =0
                        27 //ProtractPor  втянуть    сканнер=1; вытянуть    =0
                       }  
                      );                    
ConfigHardWareWB    confighardwareWB
                      (
                       {
                        4,//SetPointPort 
                        1,//SetPointMode
                        2,//BiasVPort
                        1,//BiasVMode
                        3,//DACXYPort
                        2,//DACXYMode
                        4,//DACZPort
                        1,//DACZMode
                        16,//BUSYPort
                        7, //CONV
                        10,//DEC
                        17,// ResetPort
                        PICO_DEFAULT_LED_PIN,//LEDPort
                        23,//RDBPort
                        13,//GainPID0
                        14,//GainPID1
                        15,//GainPID2
                        26,//FreezePort   заморозить сканнер=1; разморозить =0
                        27,//ProtractPort втянуть    сканнер=1; вытянуть    =0
                        28,//ModulateUPort вкл=1; выкд=0 модуляцию U  
                        11,//SD_1Port Порты  настройки СД I_STM=1; 0 =др
                        12,//SD_2Port порты  настройки СД Cantilever=0; 1-Piezo
                        8, //SignLoopPort знак ПИД // 0=+ ; 1=-
                        13 //Interator_InPort выбор вход сигнала на ПИД из1-SD; 0=ПТН(I) 
                        }
                       );
ConfigLinearDrive    configlineardrivev0({18,19,20,21,22,28});
//ConfigLinearDriveNew configlineardrivev1({18,19,20,21,22}); //2241003
ConfigLinearDriveNew configlineardrivev1({18,19,22,21,20});

FPGALOOPCTRAdress    arrLoopModule_0(
                      {
                       0x08430000,//wbKx[0]
                       0x08430004,//wbKx[1] //интегратор
                       0x08430008,//wbKx[2]
                       0x0843000C,//wbInMulKoef
                       0x08430010,//wbInShift //?
                       0x08430014,//wbOutMulKoef
                       0x08430018,//wbOutShift //DACZ
                       0x0843001C,//wbSetpoint
                       0x08430020 //pidControl // protract /retract
                       });
 FPGALOOPCTRAdress    arrLoopModule_1(
                        {
                         0x08430100,//wbKx[0]
                         0x08430104,//wbKx[1] //интегратор
                         0x08430108,//wbKx[2]
                         0x0843010C,//wbInMulKoef
                         0x08430110,//wbInShift //?
                         0x08430114,//wbOutMulKoef
                         0x08430118,//wbOutShift //DACZ
                         0x0843011C,//wbSetpoint
                         0x08430120 //pidControl // protract /retract
                         });     
FPGALOOPCTRAdress    arrLoopModule;
                           
FPGA_ADCAdress    arrADCadress(
    {
     0x08410004,  //Z
     0x08430008,  //Ampl
     0x0841000C,  //I
     0x08430010,
     0x08410014,
     0x08430018,
     0x0841001C,
     0x08430020,    
     });

