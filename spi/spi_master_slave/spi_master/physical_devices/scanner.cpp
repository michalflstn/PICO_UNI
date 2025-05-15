#include <iostream>
#include <cmath>
#include <chrono>
//#include <time.h>
#include "../loop/common_data/common_variables.hpp"
#include "scanner.hpp"

Scanner *scanner;

Scanner::Scanner(ConfigHardWareBBFPGA confighardware) : pos_({0, 0}), conf_({})
{
  hardware=new  HARDWARE(confighardware);
}
Scanner::Scanner(ConfigHardWareWB confighardware) : pos_({0, 0}), conf_({})
{
  hardware=new  HARDWARE(confighardware);
}
Scanner::Scanner(ConfigHardWareBB confighardware) : pos_({0, 0}), conf_({})
{
  hardware=new  HARDWARE(confighardware);
}
Scanner::~Scanner()
{
  move_to({0, 0}, 10);
  delete(hardware);
}
void Scanner::sendStrData(std::string const& header)
{
 std::string afcc;
  afcc.clear();
  afcc=header; 
  afcc +=endln;//"\n";
  std::cout << afcc;
  afcc.clear();
 // sleep_ms(100);
}
 void Scanner::sendData(uint8_t algcode,std::vector<int16_t>  &data, const uint16_t delay,const bool flg)//TUD
 {
 }

void Scanner::sendStrData(std::string const& header, std::vector<int32_t> &data, const uint16_t delay,const bool flg)
{
  std::string afcc;
  afcc.clear();
  afcc=header; 
   //for (auto & element :data) 
  for (size_t j = 0; j < data.size(); ++j)
  {
   afcc +=separator + std::to_string(data[j]);
  }
  afcc +=endln;
  std::cout << afcc;
  afcc.clear();
  if (flg) data.clear();
  sleep_ms(delay);
}

void Scanner::sendStrData(std::string const& header,std::vector<int16_t> &data, const uint16_t delay, const bool flg)
{
  std::string afcc;
  afcc.clear();
  afcc=header;
  for (size_t j = 0; j < data.size(); ++j)
  {
   afcc +=separator + std::to_string(data[j]);
  }
  afcc +=endln;//"\n";
  std::cout << afcc;
  afcc.clear();
  sleep_ms(delay);
  if (flg) data.clear();
}
void Scanner::sendStrData(std::string const& header,std::string  data,const uint16_t delay,const bool flg)
{
  std::string afcc;
  afcc.clear();
  afcc=header;
  afcc +=data;
  afcc +=endln;
  std::cout << afcc;
  afcc.clear();
  sleep_ms(delay);
  if (flg) data.clear();
}

void Scanner::sendStrData(std::string const& header,std::vector<uint16_t> &data, const uint16_t delay, const bool flg)
{
  std::string afcc;
  afcc.clear();
  afcc=header;
  for (size_t j = 0; j < data.size(); ++j)
  {
   afcc +=separator + std::to_string(data[j]);
  }
  afcc +=endln;
  size_t sz=afcc.size();
  if (sz>64000) { afcc.clear(); afcc=code+std::to_string(ERROR)+endln; }
  std::cout << afcc;
  afcc.clear();
  sleep_ms(delay);
  if (flg) data.clear();
}
void Scanner::stopAll()
{
  STOP=false;
}

void Scanner::readDATALin()
{
  data_LinX.clear();
  data_LinY.clear();
  size_t szx=Vector[1]; //nx
  size_t szy=Vector[2]; //ny
  if (flgDebug)
  {   
   debugdata.emplace_back(szx);
   debugdata.emplace_back(szy);
   sendStrData(code+std::to_string(DEBUG)+" linxny ",debugdata,400,true);
  } 
   for (size_t j = 0; j <szx; ++j)
   {
    data_LinX.emplace_back((uint16_t)Vector[j+3]);
   }
//  sendStrData("debug linx ",data_LinX,400,false);
   for (size_t j = 0; j <szy; ++j)
   {
    data_LinY.emplace_back((uint16_t)Vector[j+3+szx]);
   } 
//  sendStrData("debug liny ",data_LinY,400,false);
}

void Scanner::readFPGA()
{
  if (!flgVirtual)
  {
     hardware->getValuesFromAdc();
     ZValue = (int16_t)spiBuf[ZPin];
      switch (Vector[1]) //прибор
   {
        case SFM: //SFM=0
                {
                 SignalValue = (int16_t) spiBuf[AmplPin];
                 break;  
                } 
        case STM://STM=1
     case SICMDC://SICMDC=3  
                {
                 SignalValue = (int16_t) spiBuf[IPin];
                 break;  
                } 
   }         
        debugdata.emplace_back(ZValue);
        debugdata.emplace_back(SignalValue);
        debugdata.emplace_back(Vector[1]);
        sendStrData(code+std::to_string(ADC_READCmd),debugdata,100,true);
  } 
  else
  {
        debugdata.emplace_back(ZValue);
        debugdata.emplace_back(SignalValue);
        debugdata.emplace_back(Vector[1]);
        sendStrData(code+std::to_string(ADC_READCmd),debugdata,100,true);     
  }
}
void Scanner::readADC()
{
  if (!flgVirtual)
  {
    switch (HARDWAREVERSION)  
    {
     case BB: case WB:
     { 
      hardware->getValuesFromAdc();
      break;
     }
     case BBFPGA:
     {
       hardware->ReadDataFromFPGAArrayALL(spiBuf);
       break;
     }
    }
   //hardware->ReadDataFromFPGAArray(3,spiBuf);
   ZValue = (int16_t)spiBuf[ZPin];
      switch (Vector[1]) //прибор
   {
        case SFM: //SFM=0
                {
                 SignalValue = (int16_t) spiBuf[AmplPin];
                 break;  
                } 
       case STM://STM=1
                {
                  SignalValue = (int16_t) spiBuf[IPin];
                  break;  
                } 
     case SICMDC://SICMDC=3  
                {
                 SignalValue = (int16_t) spiBuf[IPin];
                 break;  
                } 
   }         
        debugdata.emplace_back(ZValue);
        debugdata.emplace_back(SignalValue);
      //  debugdata.emplace_back(setpoint-ShiftDac);
     //   debugdata.emplace_back(Vector[1]);
     //   debugdata.emplace_back(SignLoop);
    //    debugdata.emplace_back(sensor);
        sendStrData(code+std::to_string(ADC_READCmd),debugdata,100,true);
  } 
  else
  {
        debugdata.emplace_back(ZValue);
        debugdata.emplace_back(SignalValue);
       // debugdata.emplace_back(setpoint-ShiftDac);
       // debugdata.emplace_back(Vector[1]);
       // debugdata.emplace_back(SignLoop);
       // debugdata.emplace_back(sensor);
         sendStrData(code+std::to_string(ADC_READCmd),debugdata,100,true);     
  }
}
bool Scanner::getHoppingFlg() //получить флаг- установлен ли флаг сканирования прыжками
{
  return (bool)conf_.flgHoping;
}
bool Scanner::getLinearFlg()
{
  return (bool)conf_.flgLin;
}
void Scanner::start_scan() //сканирование
{
  const int8_t oneline=11;
/*
struct Config
{
  uint16_t nPoints_x;        // точек по оси  X                              1
  uint16_t nPoints_y;        // точек по оси  Y                              2 
  uint8_t  path;             // сканирование  0 - по оси X, 1 - по оси Y     3
  uint8_t  method;           // что измерять Topo=0,Phase=1, Ampl=2...       4
  uint16_t delayF;           // задержка при сканировании вперёд             5
  uint16_t delayB;           // задержка при сканировании назад              6
  uint16_t betweenPoints_x;  // расстояние между точками по X в дискретах    7 
  uint16_t betweenPoints_y;  // расстояние между точками по Y в дискретах    8 
  uint8_t  size;             // size=1  -Z; size=2 - Z,Амплитуда             9
  uint16_t  Ti;              // усиление ПИД                                10
  uint16_t diskretinstep;    // размер шага в дискретах                     11
  uint16_t pause;            // время ожидания в точке измерения  мксек     12  
  uint8_t  flgLin;           // флаг линеализации                           13   
  int16_t  lineshift;        //сдвиг линии -учет неортогональности сканнера 14
  uint8_t  flgOneFrame;      // быстрое сканирование один кадр=1            15
  uint8_t  flgHoping;        // сканирование прыжками                       16
  uint16_t HopeDelay;        // задержка в точке измерения при прыжках      17

  not use!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  uint16_t HopeZ;            // прыжок по Z,если=0,то прыжок по максимуму                 18
  uint8_t  flgAutoUpdateSP;   // автообновление опоры на каждой линии                     19
  uint8_t  flgAutoUpdateSPDelta;// обновление опоры , если изменение тока превысило порог 20
  uint16_t ThresholdAutoUpdate; //изменения опоры, если изменение тока превысило порог    21
  uint16_t KoeffCorrectISat;    // опора  %  от тока насыщения                            22
  int16_t  SetPoint;            // опора  ток      
};
*/
  prev_point = pos_; //запоминание начальной точки скана
  vector_data.clear();
 // if (flgDebug)
  {
  /* for (int j = 0; j <= 17; ++j)
   {
    debugdata.emplace_back(Vector[j]);
   }
   */
   debugdata.emplace_back(pos_.x);
   debugdata.emplace_back(pos_.y);
   sendStrData(code+std::to_string(DEBUG)+" scan parameters",debugdata,100,true);
  } 

  uint16_t stepsx;
  uint16_t stepsy;
  uint16_t reststepfast;
  uint16_t reststepslow;
  uint16_t reststepx;
  uint16_t reststepy;
  uint16_t nfastline, nslowline;
  uint16_t stepsslowline, stepsfastline;
  uint8_t  portfast;
  uint8_t  portslow;
  uint16_t pos_fast;
  uint16_t pos_slow;
  
  std::string string_data;

   DrawDone=true;
  switch (conf_.path)
  {
    case 0://X+
    {
       portfast = portx;
       portslow = porty;
       pos_fast = pos_.x;
       pos_slow = pos_.y;
      nfastline = conf_.nPoints_x;
      nslowline = conf_.nPoints_y;
      break;
    }
    case 1: //Y+
    {
       portfast = porty;
       portslow = portx;
       pos_fast = pos_.y;
       pos_slow = pos_.x;
      nfastline = conf_.nPoints_y;
      nslowline = conf_.nPoints_x;
      break;
    }
  }
//main cycle
  auto beginscan = std::chrono::high_resolution_clock::now();  
  for (uint32_t i = 0; i < nslowline; ++i)
  {
    auto begin = std::chrono::high_resolution_clock::now(); 
    std::string string_dataout;
    stepsx = (uint16_t) conf_.betweenPoints_x / conf_.diskretinstep;
    stepsy = (uint16_t) conf_.betweenPoints_y / conf_.diskretinstep;
    reststepx =         conf_.betweenPoints_x % conf_.diskretinstep;
    reststepy =         conf_.betweenPoints_y % conf_.diskretinstep;
    switch (conf_.path)
    {
      case 0://X+
      {
        stepsslowline = stepsy;
        stepsfastline = stepsx;
        reststepfast  = reststepx;
        reststepslow  = reststepy;
        break;
      }
      case 1: //Y+
      {
        stepsslowline = stepsx;
        stepsfastline = stepsy;
        reststepfast  = reststepy;
        reststepslow  = reststepx;
        break;
      }
    }
    for (uint32_t j = 0; j < nfastline; ++j)
    {
      for (uint32_t k = 0; k < stepsfastline; ++k)  
      {
        if (!flgVirtual)
        {
          hardware->set_DACXY(portfast, pos_fast);
          pos_fast += conf_.diskretinstep;
        } 
        else
        { pos_fast += conf_.diskretinstep; }
       sleep_us(delayFW);
      }
      if (reststepfast != 0)// добирание остатка
      {
        if (!flgVirtual)
        {
          pos_fast += reststepfast;
          hardware->set_DACXY(portfast, pos_fast);
        } 
        else
        { pos_fast += reststepfast; }
        sleep_us(delayFW);
      }
      //******************************************************************************
      sleep_us(conf_.pause);    // 50 CONST 50ms wait for start get data
      //*******************************************************************************
      if (!flgVirtual)
      {
        hardware->getValuesFromAdc();
        int32_t ZValue=ZMaxValue-(int16_t) spiBuf[ZPin];
        int32_t SignalValue;
        string_dataout+=separator+std::to_string(ZValue);
        if (conf_.size == 2)
          switch (conf_.method)
          {
            case 3://phase 
            {
              SignalValue=(int16_t) spiBuf[ZPin];///?????
              string_dataout+=separator+std::to_string(SignalValue);
              break;
            }
            case 4://ampl
            {
              SignalValue=(int16_t) spiBuf[AmplPin];
              string_dataout+=separator+std::to_string(SignalValue); 
              break;
            }
            case 7://current
            {
              SignalValue=(int16_t) spiBuf[IPin];
              string_dataout+=separator+std::to_string(SignalValue);
              break;
            }
          }
      }
      else
      {
       double_t w;
       w= 10*M_PI/(nfastline);   
       if (conf_.method!=oneline) string_dataout+=separator+std::to_string(int16_t(10000.0 * (sin(w*j) + sin(w* i ))));  // get Z from adc
       else  
       {
         string_dataout+=separator+std::to_string(int16_t(10000.0 * (sin(w*j) + sin(w* i ))));
       }
       if (conf_.size == 2)  //дополнительный сигнал
       {
         string_dataout=string_dataout+separator+std::to_string(int16_t(10000.0 * (sin(w*j) + sin(w* i ))));
       }
      }
    }
    switch (conf_.path)
    {
      case 0://X+
      {
        stepsx    = (uint16_t) conf_.betweenPoints_x*conf_.nPoints_x / conf_.diskretinstep; //n-1
        stepsy    = (uint16_t) conf_.betweenPoints_y / conf_.diskretinstep;
        reststepx = (uint16_t) conf_.betweenPoints_x*conf_.nPoints_x % conf_.diskretinstep;
        reststepy = (uint16_t) conf_.betweenPoints_y % conf_.diskretinstep;
        stepsslowline = stepsy;
        stepsfastline = stepsx;
        reststepfast  = reststepx;
        reststepslow  = reststepy;
        break;
      }
      case 1: //Y+
      {
        stepsx    = (uint16_t) conf_.betweenPoints_x / conf_.diskretinstep;
        stepsy    = (uint16_t) conf_.betweenPoints_y*conf_.nPoints_y / conf_.diskretinstep; //n-1
        reststepx = (uint16_t) conf_.betweenPoints_x % conf_.diskretinstep;
        reststepy = (uint16_t) conf_.betweenPoints_y *conf_.nPoints_y% conf_.diskretinstep;
        stepsslowline = stepsx;
        stepsfastline = stepsy;
        reststepfast  = reststepy;
        reststepslow  = reststepx;
        break;
      }
    }
 /////////////////////////////////////////////   
    for (uint32_t l = 0; l < stepsfastline; ++l) // move  back
    {
      if (!flgVirtual)
      {
        pos_fast -= conf_.diskretinstep;
        hardware->set_DACXY(portfast, pos_fast);
      }
      else  { pos_fast -= conf_.diskretinstep; }
      sleep_us(delayBW);
    }
    if (reststepfast != 0)// добирание остатка
    {
      if (!flgVirtual)
      {
        pos_fast -= reststepfast;
        hardware->set_DACXY(portfast, pos_fast);
      }
      else  { pos_fast -= reststepfast; }
      sleep_us(delayBW);//241111
    }
     auto end = std::chrono::high_resolution_clock::now();  
     auto dur = end - begin;
      auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    if (flgTiming)
    {
     debugdata.emplace_back(ms);
     debugdata.emplace_back(pos_fast);
     debugdata.emplace_back(pos_slow);
     sendStrData(code+std::to_string(DEBUG)+"time per line  ms ",debugdata,100,true); 
    } 
    int16_t count0 = 0;
    while ((!DrawDone))// 241221 || (count0<20) )//ожидание ответа ПК для синхронизации
    {
     sleep_ms(10);
     count0++;
    } 
    DrawDone = false;
    sendStrData(code+std::to_string(SCANNING),string_dataout,60,true); //240314  60

    if (CONFIG_UPDATE)
    {
      CONFIG_UPDATE = false;
      conf_.delayF  = Vupdateparams[1];
      delayFW=conf_.delayF;
      conf_.delayB  = Vupdateparams[2];
      delayBW=conf_.delayB;
     // if (flgDebug) sleep_ms(100); //??????
      hardware->set_GainPID((uint32_t)Vupdateparams[3]); 
      conf_.diskretinstep = Vupdateparams[4]; 
      if (flgDebug)
      {
        sleep_ms(100);
       for (int j = 0; j <= 4; ++j)
       {
        debugdata.emplace_back(Vupdateparams[j]);
       }
       sendStrData(code+std::to_string(DEBUG)+" scan parameters update",debugdata,100,true); //240314 100
      }
    // if (flgСritical_section)  critical_section_enter_blocking(&criticalSection); //???
      Vupdateparams.clear();
    // if (flgСritical_section)  critical_section_exit(&criticalSection);  //???
     stepsx = (uint16_t) conf_.betweenPoints_x / conf_.diskretinstep;
     stepsy = (uint16_t) conf_.betweenPoints_y / conf_.diskretinstep;
     reststepx = conf_.betweenPoints_x % conf_.diskretinstep;
     reststepy = conf_.betweenPoints_y % conf_.diskretinstep;
     switch (conf_.path)
     {
      case 0://X+
      {
        stepsslowline = stepsy;
        stepsfastline = stepsx;
        reststepfast  = reststepx;
        reststepslow  = reststepy;
        break;
      }
      case 1: //Y+
      {
        stepsslowline = stepsx;
        stepsfastline = stepsy;
        reststepfast  = reststepy;
        reststepslow  = reststepx;
        break;
      }
     } 
     // sendStrData(code+std::to_string(PARAMUPDATEDCmd)); // 241112 //test !!!!!!!!!!!!!!!!!240314
    } // update parameters

    if (STOP)  
    {
      STOP = false;
      sleep_ms(200);
      sendStrData(code+std::to_string(STOPPED)+"stopped");
      break;
    }
    //next line
    if ((nslowline - 1 - i) > 0)  //если непоследняя линия
    {
      if (conf_.method != oneline) //не сканирование по одной линии
      {
        for (uint32_t n = 0; n < stepsslowline; ++n) // переход на следующую линию
        {
          if (!flgVirtual)
          {
            pos_slow += conf_.diskretinstep;
            hardware->set_DACXY(portslow, pos_slow);
          }
          else { pos_slow += conf_.diskretinstep; }
          sleep_us(delayFW);
        }
        if (reststepslow != 0)
        {
          if (!flgVirtual) 
          {
            pos_slow += reststepslow; // - 240404 !!!
            hardware->set_DACXY(portslow, pos_slow);
          } 
          else { pos_slow += reststepslow; } //-240404
          sleep_us(delayFW);
        }
      }
    }
  } 
// end scan;  
  switch (conf_.path) 
  {
    case 0:
    {
      pos_.x = pos_fast;
      pos_.y = pos_slow;
      break;
    }
    case 1:
    {
      pos_.x = pos_slow;
      pos_.y = pos_fast;
      break;
    }
  }
  stop_scan();  //возврат в начальную точку скана
   
    if (flgTiming)
    {  
     auto endscan = std::chrono::high_resolution_clock::now();  
     auto dur = endscan - beginscan;
     auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
     debugdata.emplace_back(ms);
     debugdata.emplace_back(pos_.x);
     debugdata.emplace_back(pos_.y);
     sendStrData(code+std::to_string(DEBUG)+"time per scan  ms ",debugdata,100,true); 
    } 
  sleep_ms(300); //200
  int16_t count = 0;
  while ((!TheadDone) || (count<20) )//ожидание ответа ПК для синхронизации
  {
    sleep_ms(100);
    count++;
  } 
  TheadDone = false;
//  hardware->green();
  sendStrData(code+std::to_string(END)+"end"); 
//  hardware->activateDark();
 // hardware->dark();
}
void Scanner::start_scanlin() //сканирование
{
  const int8_t oneline=11;
  prev_point = pos_; //запоминание начальной точки скана
  vector_data.clear();
  if (flgDebug)
  {
   for (int j = 0; j <= 17; ++j)
   {
    debugdata.emplace_back(Vector[j]);
   }
   debugdata.emplace_back(pos_.x);
   debugdata.emplace_back(pos_.y);
   sendStrData(code+std::to_string(DEBUG)+" scan lin parameters",debugdata,200,true);//200//
   sendStrData(code+std::to_string(DEBUG)+" linx ",data_LinX,200,false);
   sendStrData(code+std::to_string(DEBUG)+" liny ",data_LinY,100,false);
  }
  uint16_t stepsx;
  uint16_t stepsy;
  uint16_t reststepfast;
  uint16_t reststepslow;
  uint16_t reststepx;
  uint16_t reststepy;
  uint16_t nfastline, nslowline;
  uint16_t stepsslowline, stepsfastline;
  uint8_t  portfast;
  uint8_t  portslow;
  uint16_t pos_fast;
  uint16_t pos_slow;

  switch (conf_.path)
  {
    case 0://X+
    {
      portfast = portx;
      portslow = porty;
      pos_fast = pos_.x;
      pos_slow = pos_.y;
      nfastline = conf_.nPoints_x;
      nslowline = conf_.nPoints_y;
      break;
    }
    case 1: //Y+
    {
      portfast = porty;
      portslow = portx;
      pos_fast = pos_.y;
      pos_slow = pos_.x;
      nfastline = conf_.nPoints_y;
      nslowline = conf_.nPoints_x;
      break;
    }
  }
  for (size_t i = 0; i < nslowline; ++i)
  {
    std::string string_dataout;
    for (uint32_t j = 0; j < nfastline; ++j)
    {
    switch (conf_.path)
    {
      case 0://X+
      {
           stepsx = (uint16_t) data_LinX[j] / conf_.diskretinstep;
        reststepx = (uint16_t) data_LinX[j] % conf_.diskretinstep;
        stepsfastline = stepsx;
        reststepfast  = reststepx;
        break;
      }
      case 1: //Y+
      {
        stepsy    = (uint16_t) data_LinY[j] / conf_.diskretinstep;
        reststepy = (uint16_t) data_LinY[j] % conf_.diskretinstep;
        stepsfastline = stepsy;
        reststepfast = reststepy;
        break;
      }
    }
     for (uint32_t k = 0; k < stepsfastline; ++k)  
      {
        if (!flgVirtual)
        {
          hardware->set_DACXY(portfast, pos_fast);
          pos_fast += conf_.diskretinstep;
        }
        else
        { pos_fast += conf_.diskretinstep; }
         sleep_us(delayFW);
      }
      if (reststepfast != 0)// добирание остатка
      {
        if (!flgVirtual)
        {
          pos_fast += reststepfast;
          hardware->set_DACXY(portfast, pos_fast);
        } else
        { pos_fast += reststepfast; }
        //sleep_us(conf_.delayF); 
        sleep_us(delayFW);
      }
      //******************************************************************************
      sleep_us(conf_.pause);    // 50 CONST 50ms wait for start get data
      //*******************************************************************************
      if (!flgVirtual)
      {
        hardware->getValuesFromAdc();
        int32_t ZValue=ZMaxValue-(int16_t) spiBuf[ZPin];
        int32_t SignalValue;
        string_dataout+=separator+std::to_string(ZValue);
        if (conf_.size == 2)
         switch (conf_.method)
         {
          case 3://phase 
          {
            SignalValue=(int16_t) spiBuf[ZPin];/////phase  temp
            string_dataout+=separator+std::to_string(SignalValue);
            break;
          }
          case 4://ampl
          {
            SignalValue=(int16_t) spiBuf[AmplPin];
            string_dataout+=separator+std::to_string(SignalValue); 
            break;
          }
          case 7://current
          {
            SignalValue=(int16_t) spiBuf[IPin];
            string_dataout+=separator+std::to_string(SignalValue);
            break;
          }
         }
      }
      else
      {
         double_t w;
         w= 10*M_PI/(nfastline);   
         if (conf_.method!=oneline) string_dataout+=separator+std::to_string(int16_t(10000.0 * (sin(w*j) + sin(w* i ))));  
         if (conf_.size == 2)  //дополнительный сигнал
         {
          string_dataout+=separator+std::to_string(int16_t(10000.0 * (sin(M_PI * j * 0.1) + sin(M_PI * i * 0.1))));  
         }
      }
    }
// move  back  add 24/01/22 ////////////////////////////////////
    stepsx    = (uint16_t) data_LinX[conf_.nPoints_x] / conf_.diskretinstep;
    stepsy    = (uint16_t) data_LinY[conf_.nPoints_y] / conf_.diskretinstep;
    reststepx = (uint16_t) data_LinX[conf_.nPoints_x] % conf_.diskretinstep;
    reststepy = (uint16_t) data_LinY[conf_.nPoints_y] % conf_.diskretinstep;

    switch (conf_.path)
    {
      case 0://X+
      {
        stepsslowline = stepsy;
        stepsfastline = stepsx;
        reststepfast = reststepx;
        reststepslow = reststepy;
        break;
      }
      case 1: //Y+
      {
        stepsslowline = stepsx;
        stepsfastline = stepsy;
        reststepfast = reststepy;
        reststepslow = reststepx;
        break;
      }
    }
 /////////////////////////////////////////////   
    for (uint32_t l = 0; l < stepsfastline; ++l) // move  back
    {
      if (!flgVirtual)
      {
        pos_fast -= conf_.diskretinstep;
        hardware->set_DACXY(portfast, pos_fast);
      }
      else { pos_fast -= conf_.diskretinstep; }
//      sleep_us(conf_.delayB);
      sleep_us(delayBW);
    }
    if (reststepfast != 0)// добирание остатка
    {
      if (!flgVirtual)
      {
        pos_fast -= reststepfast;
        hardware->set_DACXY(portfast, pos_fast);
      }
      else { pos_fast -= reststepfast; }
//      sleep_us(conf_.delayB);
      sleep_us(delayBW);
     }
    int16_t count0 = 0;
    while ((!DrawDone) || (count0<20) )//ожидание ответа ПК для синхронизации
    {
     sleep_ms(10);
     count0++;
    } 
     DrawDone = false;
     sendStrData(code+std::to_string(SCANNING),string_dataout,60,true);
   // sendStrData(code+std::to_string(SCANNING),vector_data,40,true); //100
 
    if (CONFIG_UPDATE)
    {
      CONFIG_UPDATE = false;
      conf_.delayF        = Vupdateparams[1];
      conf_.delayB        = Vupdateparams[2];
      delayFW=conf_.delayF;//241111
      delayBW=conf_.delayB;
      if (flgDebug) sleep_ms(100);     
      hardware->set_GainPID((uint32_t)Vupdateparams[3]);
      if (flgDebug) sleep_ms(100);              
      conf_.diskretinstep = Vupdateparams[4]; 
      if (flgDebug)
      { 
       for (int j = 0; j <= 4; ++j)
       {
        debugdata.emplace_back(Vupdateparams[j]);
       }
       sendStrData(code+std::to_string(DEBUG)+ " parameters update",debugdata,100,true);
      } 
      Vupdateparams.clear();
    }
    if (STOP)   // stop
    {
      STOP = false;
      sleep_ms(100);
      sendStrData(code+std::to_string(STOPPED)+"stopped");
      break;
    }
    // next line
    if ((nslowline - 1 - i) > 0)  //если не последняя линия
    {
      if (conf_.method != oneline) //не сканирование по одной линии
      {
        switch (conf_.path)
       {
         case 0://X+
        {
            stepsy = (uint16_t) data_LinY[i] / conf_.diskretinstep;
         reststepy = (uint16_t) data_LinY[i] % conf_.diskretinstep;
         stepsslowline = stepsy;
         reststepslow  = reststepy;
         break;
        }
        case 1: //Y+
        {
         stepsx    = (uint16_t) data_LinX[i] / conf_.diskretinstep;
         reststepx = (uint16_t) data_LinX[i] % conf_.diskretinstep;
         stepsslowline = stepsx;
         reststepslow = reststepx;
         break;
        }
       }
        for (uint32_t n = 0; n < stepsslowline; ++n) // переход на следующую линию
        {
          if (!flgVirtual)
          {
            pos_slow += conf_.diskretinstep;
            hardware->set_DACXY(portslow, pos_slow);
          } 
          else { pos_slow += conf_.diskretinstep; }
        //  sleep_us(conf_.delayF);
          sleep_us(delayFW);
        }
        if (reststepslow != 0)
        {
          if (!flgVirtual) 
          {
            pos_slow += reststepslow;
            hardware->set_DACXY(portslow, pos_slow);
          }
          else { pos_slow += reststepslow; }
          //sleep_us(conf_.delayF);
          sleep_us(delayFW);
        }
      }
    }
  } 
  //end scan
 // hardware->blue();
  switch (conf_.path)
  {
    case 0:
    {
      pos_.x = pos_fast;
      pos_.y = pos_slow;
      break;
    }
    case 1:
    {
      pos_.x = pos_slow;
      pos_.y = pos_fast;
      break;
    }
  }
  stop_scan();  //возврат в начальную точку скана
  sleep_ms(300); //200
  //red();
  int16_t count = 0;
  while ((!TheadDone) || (count<20) )//ожидание ответа ПК для синхронизации
  {
    sleep_ms(100);
    count++;
  } 
  TheadDone = false;
//  hardware->green();
  sendStrData(code+std::to_string(END)+"end"); 
//   hardware->dark();
}

void Scanner::start_hopingscan()
{
  /*
struct Config
{
  uint16_t nPoints_x;        // точек по оси  X                              1
  uint16_t nPoints_y;        // точек по оси  Y                              2 
  uint8_t  path;             // сканирование  0 - по оси X, 1 - по оси Y     3
  uint8_t  method;           // что измерять Topo=0,Phase=1, Ampl=2...       4
  uint16_t delayF;           // задержка при сканировании вперёд             5
  uint16_t delayB;           // задержка при сканировании назад              6
  uint16_t betweenPoints_x;  // расстояние между точками по X в дискретах    7 
  uint16_t betweenPoints_y;  // расстояние между точками по Y в дискретах    8 
  uint8_t  size;             // size=1  -Z; size=2 - Z,Амплитуда             9
  uint16_t  Ti;               // усиление ПИД                               10
  uint16_t diskretinstep;    // размер шага в дискретах                     11
  uint16_t pause;            // время ожидания в точке измерения  мксек     12  
  uint8_t  flgLin;           // флаг линеализации                           13   
  int16_t  lineshift;        //сдвиг линии -учет неортогональности сканнера 14
  uint8_t  flgOneFrame;      // быстрое сканирование один кадр=1            15
  uint8_t  flgHoping;        // сканирование прыжками                       16
  uint16_t HopeDelay;        // задержка в точке измерения при прыжках      17
  uint16_t HopeZ;            // прыжок по Z,если=0,то прыжок по максимуму                 18
  uint8_t  flgAutoUpdateSP;   // автообновление опоры на каждой линии                     19
  uint8_t  flgAutoUpdateSPDelta;// обновление опоры , если изменение тока превысило порог 20
  uint16_t ThresholdAutoUpdate; //изменения опоры, если изменение тока превысило порог    21
  uint16_t KoeffCorrectISat;    // опора  %  от тока насыщения                            22
  int16_t  SetPoint;            // опора  ток                                             23
  uint16_t HopeDelayFP          // Задержка  в первой точке линии                         24  //add 24/05/02
};
*/
  const int8_t oneline=11;
  prev_point = pos_; //запоминание начальной точки скана
  vector_data.clear();
  if (flgDebug)
  {
   for (int j = 0; j <= 24; ++j)
   {
    debugdata.emplace_back(Vector[j]);
   }
   debugdata.emplace_back(pos_.x);
   debugdata.emplace_back(pos_.y);
   sendStrData(code+std::to_string(DEBUG)+" hoping scan parameters",debugdata,100,true);
  } 
  uint16_t stepsx;
  uint16_t stepsy;
  uint16_t reststepfast;
  uint16_t reststepslow;
  uint16_t reststepx;
  uint16_t reststepy;
  uint16_t nfastline, nslowline;
  uint16_t stepsslowline, stepsfastline;
  uint8_t  portfast;
  uint8_t  portslow;
  uint16_t pos_fast;
  uint16_t pos_slow;
  int16_t  ISatCur;
  int16_t  ZCur;
  int16_t  ISatCurPrev;
  int16_t  DACZ0;

  bool  flgMaxJump;
  ZJump=conf_.HopeZ;
  flgMaxJump=(ZJump==0);
  srand( time(0 ) ); //add 24/03/11
  switch (conf_.path)
  {
    case 0://X+
    {
      portfast  = portx;
      portslow  = porty;
      pos_fast  = pos_.x;
      pos_slow  = pos_.y;
      nfastline = conf_.nPoints_x;
      nslowline = conf_.nPoints_y;
      break;
    }
    case 1: //Y+
    {
      portfast  = porty;
      portslow  = portx;
      pos_fast  = pos_.y;
      pos_slow  = pos_.x;
      nfastline = conf_.nPoints_y;
      nslowline = conf_.nPoints_x;
      break;
    }
  }
  if (!flgVirtual)
  {
    hardware->getValuesFromAdc();
    ISatCurPrev=(int16_t) spiBuf[IPin];
    ZCur=(int16_t) spiBuf[ZPin];
  }
  else
  {
    ISatCurPrev=(int16_t)round(conf_.SetPoint);
    ISatCur=ISatCurPrev;
  }
//****************************************************************
//   start  
  auto beginscan = std::chrono::high_resolution_clock::now();  
  for (uint32_t i = 0; i < nslowline; ++i)
  { 
     std::string string_dataout;
    auto begin = std::chrono::high_resolution_clock::now();  
    stepsx = (uint16_t) conf_.betweenPoints_x / conf_.diskretinstep;
    stepsy = (uint16_t) conf_.betweenPoints_y / conf_.diskretinstep;
    reststepx = conf_.betweenPoints_x % conf_.diskretinstep;
    reststepy = conf_.betweenPoints_y % conf_.diskretinstep;
    switch (conf_.path)
    {
      case 0://X+
      {
        stepsslowline = stepsy;
        stepsfastline = stepsx;
        reststepfast  = reststepx;
        reststepslow  = reststepy;
        break;
      }
      case 1: //Y+
      {
        stepsslowline = stepsx;
        stepsfastline = stepsy;
        reststepfast  = reststepy;
        reststepslow  = reststepx;
        break;
      }
    }  
    for (uint32_t j = 0; j < nfastline; ++j)
    {
      if (!flgVirtual)
      {
        if (flgMaxJump)  hardware->retract(); //втянуться на max
        else       
        {
         DACZ0= ZCur-ZJump;
         if (ZCur<ZJump) DACZ0=0; 
         hardware->retract(DACZ0); //втянуться на ZJump
        }        
      }   
      sleep_us(50);
      for (uint32_t k = 0; k < stepsfastline; ++k) //move to the next point
      {
        if (!flgVirtual)
        {
          hardware->set_DACXY(portfast, pos_fast);
          pos_fast += conf_.diskretinstep;
        } 
        else { pos_fast += conf_.diskretinstep; }
        sleep_us(delayFW);
      }
      if (reststepfast != 0)
      {
        if (!flgVirtual)
        {
          pos_fast += reststepfast;
          hardware->set_DACXY(portfast, pos_fast);
        }
        else { pos_fast += reststepfast; }
        sleep_us(delayFW);
      }
  //******************************************************************************
      if (!flgVirtual)
      {
        if (flgMaxJump) hardware->protract();// вытянуться
        else            hardware->protract();// protract(0, ZJump);// вытянуться на ZJump
      }
      sleep_ms(delayHope);   
      sleep_us(conf_.pause);    // CONST 50ms wait for start get data
  //*******************************************************************************
      if (!flgVirtual)
      {
        hardware->getValuesFromAdc(); 
        ZCur=(int16_t) spiBuf[ZPin];
        int32_t ZValue=ZMaxValue-ZCur;
        int32_t SignalValue;
        string_dataout+=separator+std::to_string(ZValue);
     
        switch (conf_.method)
          //added signal  Const  BackPass=2;    //PM  Const  Phase=3;  Const  UAM=4;   //Force Image
        {
          case 3://phase !!!!
          {
           string_dataout+=separator+std::to_string((int16_t) spiBuf[1]);
           break;
          }
          case 4://ampl
          {
            string_dataout+=separator+std::to_string((int16_t)AmplPin);
            break;
          }
          case 7://current
          {
            string_dataout+=separator+std::to_string((int16_t)IPin);
            break;
          }
        }
      }
      else
      {
        double_t w;
        w= 10*M_PI/(nfastline);  
        string_dataout+=separator+std::to_string(int16_t(10000.0*(sin(w*j) + sin(w* i )))); 
        if (conf_.size == 2) // added signal
        {
         string_dataout+=separator+std::to_string(int16_t(10000.0*(sin(M_PI * j * 0.1) + sin(M_PI * i * 0.1))));
        }
      }
    } //fast line
     
 //move to the start line point   
      if (!flgVirtual)
      {
        hardware->retract(); //втянуться на макс
        DACZMove(DACZ0,DACZ0,-10, 0); // обнуление DACZ   //240405
        DACZ0=0;
      } 
      sleep_us(50);
// move backward 
      if (!flgVirtual)
      {
        pos_fast -= conf_.diskretinstep * stepsfastline * nfastline;
        hardware->set_DACXY(portfast, pos_fast);
      }
      else
      {
        pos_fast -= conf_.diskretinstep * stepsfastline * nfastline;
      }
      sleep_us(delayBW);

      if (reststepfast != 0)
      {
        if (!flgVirtual)
        {
         pos_fast -= reststepfast;
         hardware->set_DACXY(portfast, pos_fast);
        }
        else { pos_fast -= reststepfast; }
        sleep_us(delayBW);
      }// move backward 
      //next line
     if ((nslowline - 1 - i) > 0)  //если не последняя линия
     {
      if (conf_.method !=oneline) //не сканирование по одной линии
      {
        for (uint32_t n = 0; n < stepsslowline; ++n) // переход на следующую линию
        {
          if (!flgVirtual)
          {
            pos_slow += conf_.diskretinstep;
            hardware->set_DACXY(portslow, pos_slow);
          }
          else { pos_slow += conf_.diskretinstep; }
         sleep_us(delayFW);
        }
        if (reststepslow != 0)
        {
          if (!flgVirtual) 
          {
            pos_slow += reststepslow; // - 240404!!!!!
            hardware->set_DACXY(portslow, pos_slow);
          }
          else { pos_slow += reststepslow; }  // -240404
          sleep_us(delayFW);
        }
      }
     }   //next line 
      sleep_ms(conf_.HopeDelayFP);  //400
      sleep_us(conf_.pause);  
     
     if (!flgVirtual)  //read  Saturation Current
      {
        hardware->getValuesFromAdc();
        ISatCur=(int16_t) spiBuf[IPin];
        string_dataout+=separator+std::to_string(ISatCur); 
      }
      else
      {
       ISatCur=ISatCur-int16_t(100*rand() % 5);// random_num;  //add 24/03/11
       string_dataout+=separator+std::to_string(ISatCur);
      }
// auto correction setpoint for sicm
     if (conf_.flgAutoUpdateSP) 
     {
       if (conf_.flgAutoUpdateSPDelta) 
       {
         if (abs((ISatCurPrev-ISatCur)/ISatCurPrev)>0.01*conf_.ThresholdAutoUpdate) 
         {
          conf_.SetPoint=round(ISatCur*conf_.KoeffCorrectISat*0.01 );
          ISatCurPrev=ISatCur;
          hardware->set_SetPoint(conf_.SetPoint);
          sleep_ms(delayHope);   
         }
       }
       else
       { 
        conf_.SetPoint=round(ISatCur*conf_.KoeffCorrectISat*0.01 );
        hardware->set_SetPoint(conf_.SetPoint);
        ISatCurPrev=ISatCur;
        sleep_ms(delayHope);   
       }
     }
     string_dataout+=separator+std::to_string(round(conf_.SetPoint));
     auto end = std::chrono::high_resolution_clock::now();  
     auto dur = end - begin;
     auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    if (flgTiming)
    {
     debugdata.emplace_back(ms);
     debugdata.emplace_back(pos_fast);
     debugdata.emplace_back(pos_slow);
     sendStrData(code+std::to_string(DEBUG)+"time per line  ms ",debugdata,100,true); 
    } 
     int16_t count0 = 0;
     while ((!DrawDone))// || (count0<20))//ожидание ответа ПК для синхронизации
     {
      sleep_ms(10);
      count0++;
     } 
      DrawDone = false;
//*****************************************************************
    sendStrData(code+std::to_string(SCANNING),string_dataout,60,true);
//*****************************************************************
    if (STOP)  // stop
    {
      STOP = false;
      sleep_ms(300);
      sendStrData(code+std::to_string(STOPPED)+"stopped");
      break;
    }
    if (CONFIG_UPDATE)
    {
       CONFIG_UPDATE              = false;
      conf_.delayF               = Vupdateparams[1];
      conf_.delayB               = Vupdateparams[2];
      delayFW                    =  conf_.delayF;
      delayBW                    =  conf_.delayB;
      conf_.diskretinstep        = Vupdateparams[3];
      sleep_ms(100);           
      hardware->set_GainPID((uint32_t)Vupdateparams[4]); 
      conf_.HopeDelay            = Vupdateparams[5];
      delayHope                  =  conf_.HopeDelay;  
      conf_.HopeZ                = Vupdateparams[6];
      conf_.flgAutoUpdateSP      = Vupdateparams[7];; // автообновление опоры на каждой линии                     19
      conf_.flgAutoUpdateSPDelta = Vupdateparams[8];; // обновление опоры , если изменение тока превысило порог 20
      conf_.ThresholdAutoUpdate  = Vupdateparams[9];; // изменения опоры, если изменение тока превысило порог     21
      conf_.KoeffCorrectISat     = Vupdateparams[10]; // опора  %  от тока насыщения  
      conf_.HopeDelayFP          = Vupdateparams[11]; //задержка в первой точке линии   // add 240503  
      ZJump=conf_.HopeZ;
      flgMaxJump=(ZJump==0);  
      if (flgDebug)
      { 
       sleep_ms(100);   
       for (int j = 0; j <= 11; ++j)
       {
        debugdata.emplace_back(Vupdateparams[j]);
       }
       sendStrData(code+std::to_string(DEBUG)+" hoping parameters update",debugdata,100,true);
      } 
      if (flgСritical_section) critical_section_enter_blocking(&criticalSection); 
       Vupdateparams.clear();
      if (flgСritical_section) critical_section_exit(&criticalSection);  

      stepsx = (uint16_t) conf_.betweenPoints_x / conf_.diskretinstep;
      stepsy = (uint16_t) conf_.betweenPoints_y / conf_.diskretinstep;
      reststepx = conf_.betweenPoints_x % conf_.diskretinstep;
      reststepy = conf_.betweenPoints_y % conf_.diskretinstep;
       switch (conf_.path)
     {
      case 0://X+
      {
        stepsslowline = stepsy;
        stepsfastline = stepsx;
        reststepfast = reststepx;
        reststepslow = reststepy;
        break;
      }
      case 1: //Y+
      {
        stepsslowline = stepsx;
        stepsfastline = stepsy;
        reststepfast = reststepy;
        reststepslow = reststepx;
        break;
      }
     }
      if (flgDebug)
      {
       //   sendStrData(code+std::to_string(PARAMUPDATEDCmd)); //241112
      } //240314
    } //update
      if (!flgVirtual)
      {
        hardware->protract();    //вытянуться
        sleep_ms(700); //240411 250
      }
      if (!flgVirtual) //get current Z contact
      {
        hardware->getValuesFromAdc();
        ZCur=(int16_t) spiBuf[ZPin];
      }
      else
      {
        ZCur=(int16_t)round(conf_.SetPoint);
      } 
  } //for slowline
  //end scanning 
 // blue();
  switch (conf_.path)
  {
    case 0:
    {
      pos_.x = pos_fast;
      pos_.y = pos_slow;
      break;
    }
    case 1:
    {
      pos_.x = pos_slow;
      pos_.y = pos_fast;
      break;
    }
  }
  stop_scan();  //возврат в начальную точку скана - сканер втянут
  sleep_ms(200);
  if (!flgVirtual)
  {
   hardware->protract();// protract(30,DACZ0,DACZ0); 
   DACZMove(DACZ0,DACZ0,-20, 30);//вытянуть            !!!!!!!!!!!!!!!!!!!!!!!!!!
  }
  sleep_ms(1000);
  int16_t count = 0;
    if (flgTiming)
    {  
     auto endscan = std::chrono::high_resolution_clock::now();  
     auto dur = endscan - beginscan;
     auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
     debugdata.emplace_back(ms);
     debugdata.emplace_back(pos_.x);
     debugdata.emplace_back(pos_.y);
     sendStrData(code+std::to_string(DEBUG)+"time per scan  ms ",debugdata,100,true); 
    } 
  while ((!TheadDone) || (count<20) )//ожидание ответа ПК для синхронизации
  {
    sleep_ms(100);
    count++;
  } 
  TheadDone = false;
  conf_.flgHoping=0;
  sendStrData(code+std::to_string(END)+"end");
 // activateDark();
}

void Scanner::start_hopingscanlin()
{
  const int8_t oneline=11;
  prev_point = pos_; //запоминание начальной точки скана
  vector_data.clear();
  if (flgDebug)
  {  
   for (int j = 0; j <= 24; ++j)
   {
    debugdata.emplace_back(Vector[j]);
   }
   sendStrData(code+std::to_string(DEBUG)+" hoping scan parameters",debugdata,100,true);
  }
  uint16_t stepsx;
  uint16_t stepsy;
  uint16_t reststepfast;
  uint16_t reststepslow;
  uint16_t reststepx;
  uint16_t reststepy;
  uint16_t nfastline, nslowline;
  uint16_t stepsslowline, stepsfastline;
  uint8_t  portfast;
  uint8_t  portslow;
  uint16_t pos_fast;
  uint16_t pos_slow;
  //int16_t  ZJump;
  int16_t  ISatCur;
  int16_t  ZCur;
  int16_t  ISatCurPrev;
  int16_t  DACZ0;

  bool  flgMaxJump;
  ZJump=conf_.HopeZ;
  flgMaxJump=(ZJump==0);

  stepsx = (uint16_t) conf_.betweenPoints_x / conf_.diskretinstep;
  stepsy = (uint16_t) conf_.betweenPoints_y / conf_.diskretinstep;
  reststepx = conf_.betweenPoints_x % conf_.diskretinstep;
  reststepy = conf_.betweenPoints_y % conf_.diskretinstep;

  switch (conf_.path)
  {
    case 0://X+
    {
      portfast = portx;
      portslow = porty;
      pos_fast = pos_.x;
      pos_slow = pos_.y;
      nfastline = conf_.nPoints_x;
      nslowline = conf_.nPoints_y;
      stepsslowline = stepsy;
      stepsfastline = stepsx;
      reststepfast = reststepx;
      reststepslow = reststepy;
      break;
    }
    case 1: //Y+
    {
      portfast = porty;
      portslow = portx;
      pos_fast = pos_.y;
      pos_slow = pos_.x;
      nfastline = conf_.nPoints_y;
      nslowline = conf_.nPoints_x;
      stepsslowline = stepsx;
      stepsfastline = stepsy;
      reststepfast = reststepy;
      reststepslow = reststepx;
      break;
    }
  }

 if (!flgVirtual)
  {
    hardware->getValuesFromAdc();
    ISatCurPrev=(int16_t) spiBuf[IPin];
    ZCur=(int16_t) spiBuf[ZPin];
  }
  else
  {
    ISatCurPrev=(int16_t)round(conf_.SetPoint);
  }

  for (uint32_t i = 0; i < nslowline; ++i)
  { 
   for (uint32_t j = 0; j < nfastline; ++j)
    {
      switch (conf_.path)
     {
      case 0://X+
      {
           stepsx = (uint16_t) data_LinX[j] / conf_.diskretinstep;
        reststepx = (uint16_t) data_LinX[j] % conf_.diskretinstep;
        stepsfastline = stepsx;
         reststepfast = reststepx;
        break;
      }
      case 1: //Y+
      {
           stepsy = (uint16_t) data_LinY[j] / conf_.diskretinstep;
        reststepy = (uint16_t) data_LinY[j] % conf_.diskretinstep;
        stepsfastline = stepsy;
         reststepfast = reststepy;
        break;
      }
     }
      if (!flgVirtual)
      {
        if (flgMaxJump)  hardware->retract();           //втянуться на max
        else       
        {
         DACZ0= ZCur-ZJump;    
         if (ZCur<ZJump) DACZ0=0;  //240220
         hardware->retract(DACZ0); //втянуться на ZJump
        }        
      }   
      sleep_us(50);
//
      for (uint32_t k = 0; k < stepsfastline; ++k) 
      {
        if (!flgVirtual)
        {
          hardware->set_DACXY(portfast, pos_fast);
          pos_fast += conf_.diskretinstep;
        }
        else { pos_fast += conf_.diskretinstep; }
       // sleep_us(conf_.delayF);
        sleep_us(delayFW);
      }
      if (reststepfast != 0)
      {
        if (!flgVirtual)
        {
          pos_fast += reststepfast;
          hardware->set_DACXY(portfast, pos_fast);
        }
        else { pos_fast += reststepfast; }
//        sleep_us(conf_.delayF);
        sleep_us(delayFW);
      }
  //******************************************************************************
      if (!flgVirtual)
      {
        if (flgMaxJump) hardware->protract();  //вытянуться
        else            hardware->protract();//  protract(0, ZJump);// ;//вытянуться на ZJump
      }
     // sleep_ms(conf_.HopeDelay);
      sleep_ms(delayHope);   
      sleep_us(conf_.pause);    // CONST 50ms wait for start get data
  //*******************************************************************************
  
      if (!flgVirtual)
      {
         hardware->getValuesFromAdc();
         ZCur=(int16_t) spiBuf[ZPin];
         vector_data.emplace_back(ZMaxValue-ZCur);     // считать  Z 
        switch (conf_.method)
          //added signal  Const  BackPass=2;    //PM  Const  Phase=3;  Const  UAM=4;   //Force Image
        {
          case 3://phase !!!!
          {
            vector_data.emplace_back((int16_t) spiBuf[1]);
            break;
          }
          case 4://ampl
          {
            vector_data.emplace_back((int16_t) spiBuf[AmplPin]);
            break;
          }
          case 7://current
          {
            vector_data.emplace_back((int16_t) spiBuf[IPin]);
            break;
          }
        }
      } else
      {
       double_t w;
       w= 10*M_PI/(nfastline);   
       vector_data.emplace_back(int16_t(10000.0 * (sin(w*j) + sin(w* i ))));  // get Z from adc
     if (conf_.size == 2)                               // added signal
        {
          vector_data.emplace_back(int16_t(10000.0 * (sin(M_PI * j * 0.1) + sin(M_PI * i * 0.1))));
        }
      }
    } //fast line
      //move to the start line point   
      if (!flgVirtual)
      {
        hardware->retract(); //втянуться на макс
        DACZMove(DACZ0,DACZ0,-10, 0); // обнуление DACZ  //09 240405
        DACZ0=0;
      } 
      sleep_us(50);
// move backward 
    switch (conf_.path)
    {
      case 0://X+
      {
        stepsx    = (uint16_t) data_LinX[conf_.nPoints_x] / conf_.diskretinstep;
        reststepx = (uint16_t) data_LinX[conf_.nPoints_x] % conf_.diskretinstep;
        stepsfastline = stepsx;
        reststepfast = reststepx;
        break;
      }
      case 1: //Y+
      {
        stepsy    = (uint16_t) data_LinY[conf_.nPoints_y] / conf_.diskretinstep;
        reststepy = (uint16_t) data_LinY[conf_.nPoints_y] % conf_.diskretinstep;
        stepsfastline = stepsy;
        reststepfast = reststepy;
        break;
      }
    }
    if (!flgVirtual)
    {
      pos_fast -= conf_.diskretinstep * stepsfastline * nfastline;
      hardware->set_DACXY(portfast, pos_fast);
    }
    else
    {
      pos_fast -= conf_.diskretinstep * stepsfastline * nfastline; 
    }
//     sleep_us(conf_.delayB);
     sleep_us(delayBW);
     if (reststepfast != 0)
     {
      if (!flgVirtual)
      {
        pos_fast -= reststepfast;
        hardware->set_DACXY(portfast, pos_fast);
      } 
      else { pos_fast -= reststepfast; }
     // sleep_us(conf_.delayB);
       sleep_us(delayBW);
     }
      sleep_ms(conf_.HopeDelayFP);// 240503
      sleep_us(conf_.pause);  

     if (!flgVirtual)  //read  Saturation Current
      {
        hardware->getValuesFromAdc();
        ISatCur=(int16_t) spiBuf[IPin];
        vector_data.emplace_back(ISatCur);
      }
      else
      {
      // uint16_t random_num =i;   
       ISatCur=ISatCur-int16_t(100*rand() % 5);// random_num;  //add 24/03/11
       vector_data.emplace_back(ISatCur);
      }
// auto correction setpoint for sicm
     if (conf_.flgAutoUpdateSP) 
     {
       if (conf_.flgAutoUpdateSPDelta) 
       {
         if (abs((ISatCurPrev-ISatCur)/ISatCurPrev)>0.01*conf_.ThresholdAutoUpdate) 
         {
          conf_.SetPoint=round(ISatCur*conf_.KoeffCorrectISat*0.01 );
          ISatCurPrev=ISatCur;
          hardware->set_SetPoint(conf_.SetPoint);
         // sleep_ms(conf_.HopeDelay);
          sleep_ms(delayHope);    
         }
       }
       else
       { 
        conf_.SetPoint=round(ISatCur*conf_.KoeffCorrectISat*0.01 );
        hardware->set_SetPoint(conf_.SetPoint);
        ISatCurPrev=ISatCur;
      //  sleep_ms(conf_.HopeDelay);
        sleep_ms(delayHope);   
       }
     }   
     vector_data.emplace_back(round(conf_.SetPoint));
     int16_t count0 = 0;
     while ((!DrawDone) || (count0<20) )//ожидание ответа ПК для синхронизации
     {
      sleep_ms(10);
      count0++;
     } 
      DrawDone = false; 
//*******************************************************  
    sendStrData(code+std::to_string(SCANNING),vector_data,60,true);
//********************************************************
    if (STOP)  // stop
    {
      STOP = false;
      sleep_ms(300);
      sendStrData(code+std::to_string(STOPPED)+"stopped");
      break;
    }  
    if (CONFIG_UPDATE)
    {
      CONFIG_UPDATE              = false;
      conf_.delayF               = Vupdateparams[1];
      conf_.delayB               = Vupdateparams[2];
      delayFW                    =  conf_.delayF;
      delayBW                    =  conf_.delayB;
      conf_.diskretinstep        = Vupdateparams[3];
      sleep_ms(100);             //240314
      hardware->set_GainPID((uint32_t)Vupdateparams[4]); //240320
      conf_.HopeDelay            = Vupdateparams[5];
      delayHope                  = conf_.HopeDelay;   
      conf_.HopeZ                = Vupdateparams[6];
      conf_.flgAutoUpdateSP      = Vupdateparams[7];; // автообновление опоры на каждой линии                     19
      conf_.flgAutoUpdateSPDelta = Vupdateparams[8];; // обновление опоры , если изменение тока превысило порог 20
      conf_.ThresholdAutoUpdate  = Vupdateparams[9];; // изменения опоры, если изменение тока превысило порог     21
      conf_.KoeffCorrectISat     = Vupdateparams[10]; // опора  %  от тока насыщения  
      conf_.HopeDelayFP          = Vupdateparams[11]; //задержка в первой точке линии   // add 240503  
      ZJump=conf_.HopeZ;   
      flgMaxJump=(conf_.HopeZ==0);
      ZJump=-conf_.HopeZ;
      sleep_ms(100);   
      if (flgDebug)
      {  
       for (int j = 0; j <= 11; ++j)
       {
        debugdata.emplace_back(Vector[j]);
       }
       sendStrData(code+std::to_string(DEBUG)+" parameters update",debugdata,100,true);
      }
    }
///move next line
    switch (conf_.path)
    {
      case 0://X+
      {
        stepsy    = (uint16_t) data_LinY[i] / conf_.diskretinstep;
        reststepy = (uint16_t) data_LinY[i] % conf_.diskretinstep;
        stepsslowline = stepsy;
        reststepslow = reststepy;
        break;
      }
      case 1: //Y+
      {
        stepsx    = (uint16_t) data_LinX[i] / conf_.diskretinstep;
        reststepx = (uint16_t) data_LinX[i] % conf_.diskretinstep;
        stepsslowline = stepsx;
        reststepslow = reststepx;
        break;
      }
    }
    if ((nslowline - 1 - i) > 0)  //если не последняя линия
    {
     if (conf_.method !=oneline) //не сканирование по одной линии
     { 
        for (uint32_t n = 0; n < stepsslowline; ++n) // переход на следующую линию
        {
          if (!flgVirtual)
          {
            pos_slow += conf_.diskretinstep;
            hardware->set_DACXY(portslow, pos_slow);
          } 
          else { pos_slow += conf_.diskretinstep; }
         // sleep_us(conf_.delayF);
           sleep_us(delayFW);
        }
        if (reststepslow != 0)
        {
          if (!flgVirtual) 
          {
            pos_slow += reststepslow;
            hardware->set_DACXY(portslow, pos_slow);
          }
          else { pos_slow += reststepslow; }
        //  sleep_us(conf_.delayF);
          sleep_us(delayFW);
        }
      }
    } 
      if (!flgVirtual)
      {
        hardware->protract();    //вытянуться
        sleep_ms(700); //240411 250
      }
      if (!flgVirtual) //get current Z contact
      {
        hardware->getValuesFromAdc();
        ZCur=(int16_t) spiBuf[ZPin];
      }
      else
      {
        ZCur=(int16_t)round(conf_.SetPoint);
      } 
   } // slow line 
 // blue();
  switch (conf_.path)
  {
    case 0:
    {
      pos_.x = pos_fast;
      pos_.y = pos_slow;
      break;
    }
    case 1:
    {
      pos_.x = pos_slow;
      pos_.y = pos_fast;
      break;
    }
  }
  stop_scan();  //возврат в начальную точку скана - сканер втянут
  sleep_ms(200);
  if (!flgVirtual)
  {
   hardware->protract();
   DACZMove(DACZ0,DACZ0,-20,30); //вытянуть -20??
  }
  sleep_ms(1000);

  int16_t count = 0;
  while ((!TheadDone) || (count<20) )//ожидание ответа ПК для синхронизации
  {
    sleep_ms(100);
    count++;
  } 
  TheadDone = false;
  conf_.flgHoping=0;
  sendStrData(code+std::to_string(END)+"end");
  hardware->activateDark();
} //hoppinglin

void Scanner::

start_fastscan()
{
  prev_point = pos_; //запоминание начальной точки скана
  vector_data.clear();
  if(flgDebug)
  {
   for (int j = 0; j <= 17; ++j)
   {
    debugdata.emplace_back(Vector[j]);
   }
   debugdata.emplace_back(pos_.x);
   debugdata.emplace_back(pos_.y);
  // debugdata.emplace_back(conf_.flgOneFrame);
   sendStrData(code+std::to_string(DEBUG)+" fastscan parameters",debugdata,100,true);
  }
  uint16_t stepsx;
  uint16_t stepsy;
  uint16_t reststepfast;
  uint16_t reststepslow;
  uint16_t reststepx;
  uint16_t reststepy;
  uint16_t nfastline, nslowline;
  uint16_t stepsslowline, stepsfastline;
  uint8_t  portfast;
  uint8_t  portslow;
  uint16_t pos_fast;
  uint16_t pos_slow;
  uint16_t pos_fast_start;
  uint16_t pos_slow_start;
  DrawDone=true;
  switch (conf_.path)
  {
    case 0://X+
    {
       portfast = portx;
       portslow = porty;
       pos_fast = pos_.x;
       pos_slow = pos_.y;
       pos_slow_start=pos_slow;
       pos_fast_start=pos_fast;
      nfastline = conf_.nPoints_x;
      nslowline = conf_.nPoints_y;
      break;
    }
    case 1: //Y+
    {
       portfast = porty;
       portslow = portx;
       pos_fast = pos_.y;
       pos_slow = pos_.x;
       pos_slow_start=pos_slow;
       pos_fast_start=pos_fast;
      nfastline = conf_.nPoints_y;
      nslowline = conf_.nPoints_x;
      break;
    }
  }
    stepsx = (uint16_t) conf_.betweenPoints_x / conf_.diskretinstep;
    stepsy = (uint16_t) conf_.betweenPoints_y / conf_.diskretinstep;
    reststepx = conf_.betweenPoints_x % conf_.diskretinstep;
    reststepy = conf_.betweenPoints_y % conf_.diskretinstep;
    switch (conf_.path)
    {
      case 0://X+
      {
        stepsslowline = stepsy;
        stepsfastline = stepsx;
        reststepfast  = reststepx;
        reststepslow  = reststepy;
        break;
      }
      case 1: //Y+
      {
        stepsslowline = stepsx;
        stepsfastline = stepsy;
        reststepfast  = reststepy;
        reststepslow  = reststepx;
        break;
      }
    } 
     DrawDone=true;
     auto beginscan = std::chrono::high_resolution_clock::now();    
  while (!STOP)
  {
    std::string string_dataout;
    auto begin = std::chrono::high_resolution_clock::now();  
    for (uint32_t i = 0; i < nslowline; ++i) //slow
    {
      for (uint32_t j = 0; j < nfastline; ++j) //fast
      {
        for (uint32_t k = 0; k < stepsfastline; ++k) 
        {
          if (!flgVirtual)
          {
            hardware->set_DACXY(portfast, pos_fast);
            pos_fast += conf_.diskretinstep;
          }
          else { pos_fast += conf_.diskretinstep; }
          sleep_us(conf_.delayF);
        }
        if (reststepfast != 0)
        {
          if (!flgVirtual)
          {
            pos_fast += reststepfast;
            hardware->set_DACXY(portfast, pos_fast);
          } 
          else { pos_fast += reststepfast; }
          sleep_us(conf_.delayF);
        }
//******************************************************************************
         sleep_us(conf_.pause);    // 50 CONST 50ms wait for start get data
//******************************************************************************     
        if (!flgVirtual)
        {
          hardware->getValuesFromAdc();
          string_dataout+=separator+std::to_string(ZMaxValue-(int16_t) spiBuf[ZPin]); 
         }
        else
        {
          string_dataout+=separator+std::to_string(int16_t(10000.0 * (sin(M_PI * j * 0.1) + sin(M_PI * i * 0.1)))); 
        }
      } //j fast
 // возврат в начальную точку линии
      for (uint32_t j = 0; j < stepsfastline * nfastline; ++j)//??
      {
        if (!flgVirtual)
        {
          pos_fast -= conf_.diskretinstep;
          hardware->set_DACXY(portfast, pos_fast);
        } 
        else { pos_fast -= conf_.diskretinstep; }
        sleep_us(conf_.delayB);
      }
      if (reststepfast != 0)// добирание остатка
      {
        if (!flgVirtual)
        {
          pos_fast -= reststepfast;
          hardware->set_DACXY(portfast, pos_fast);
        }
        else { pos_fast -= reststepfast; }
        sleep_us(conf_.delayB);
      } // x0
      if ((nslowline - 1 - i) > 0)  //если непоследняя линия
      {
        for (uint32_t n = 0; n < stepsslowline; ++n) 
        {
          if (!flgVirtual)
          {
            pos_slow += conf_.diskretinstep;
            hardware->set_DACXY(portslow, pos_slow);
          } 
          else { pos_slow += conf_.diskretinstep; }
          sleep_us(conf_.delayF);
        }
        if (reststepslow != 0)
        {
          if (!flgVirtual) 
          {
            pos_slow += reststepslow;
            hardware->set_DACXY(portslow, pos_slow);
          }
          else { pos_slow += reststepslow; }
          sleep_us(conf_.delayF);
        }
      } //y next
    } // slow i
     if (flgTiming)
     { 
       auto end = std::chrono::high_resolution_clock::now();  
       auto dur = end - begin;
       auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
       debugdata.emplace_back(ms);
       sendStrData(code+std::to_string(DEBUG)+"time per scan ms ",debugdata,200,true); 
     } 
     int16_t count0 = 0;
     while ((!DrawDone))// || (count0<20))//ожидание ответа ПК для синхронизации
     {
      sleep_ms(50);
      //count0++;
     } 
      DrawDone = false;
//*************************************************************************
     sendStrData(code+std::to_string(FASTSCANNING),string_dataout,200,true);
//*************************************************************************
     
   switch (conf_.path) //add 241217
   {
    case 0:
    {
      pos_.x = pos_fast;
      pos_.y = pos_slow;
      break;
    }
    case 1:
    {
      pos_.x = pos_slow;
      pos_.y = pos_fast;
      break;
    }
   }
    stop_scan();  //возврат в начальную точку скана
    pos_slow=pos_slow_start; //add 241217
    pos_fast=pos_fast_start;
    if (conf_.flgOneFrame == 1) 
    { 
      STOP = true;
    };
    if (flgTiming)
     { 
       auto end = std::chrono::high_resolution_clock::now();  
       auto dur = end - beginscan;
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
       debugdata.emplace_back(ms);
       debugdata.emplace_back(pos_.x);
       debugdata.emplace_back(pos_.y);
      sendStrData(code+std::to_string(DEBUG)+"time per scan2 ms ",debugdata,200,true); 
     } 
  } //stop
//  blue();
   STOP=false;
   switch (conf_.path)
  {
    case 0:
    {
      pos_.x = pos_fast;
      pos_.y = pos_slow;
      break;
    }
    case 1:
    {
      pos_.x = pos_slow;
      pos_.y = pos_fast;
      break;
    }
  }
 // stop_scan();   //250115                   //возврат в начальную точку скана
 // sleep_ms(100); //250115
  int16_t count = 0;
  while ((!TheadDone) || (count<20) )//ожидание ответа ПК для синхронизации
  {
    sleep_ms(50);
    count++;
  } 
  TheadDone = false;
  sendStrData(code+std::to_string(END)+"end");
 // hardware->activateDark();
}

void Scanner::stop_scan()
{
  if (flgDebug) 
  {
   debugdata.emplace_back(prev_point.x);
   debugdata.emplace_back(prev_point.y);
   debugdata.emplace_back(pos_.x);
   debugdata.emplace_back(pos_.y);
   sendStrData(code+std::to_string(DEBUG)+" stopscan parameters",debugdata,100,true);
   sleep_ms(400);
  }
  move_to(prev_point, 10);
  debugdata.clear();
  if (flgDebug) 
  {
   debugdata.emplace_back(prev_point.x);
   debugdata.emplace_back(prev_point.y);
   debugdata.emplace_back(pos_.x);
   debugdata.emplace_back(pos_.y);
   sendStrData(code+std::to_string(DEBUG)+" stopscan parameters",debugdata,100,true);
  }
}

void Scanner::scanparams_update(const std::vector<int32_t> &Vector)
{
//  conf_ = config;
 if (flgСritical_section) critical_section_enter_blocking(&criticalSection);
  conf_.nPoints_x=(uint16_t)Vector[1]; //uint16_t nPoints_x;        // точек по оси  X                                            1
  conf_.nPoints_y=(uint16_t)Vector[2];        // точек по оси  Y                                            2 
  conf_.path=(uint16_t)Vector[3];             // сканирование  0 - по оси X, 1 - по оси Y                   3
  conf_.method=(uint16_t)Vector[4];           // что измерять Topo=0,Phase=1, Ampl=2...                     4
  conf_.delayF=(uint16_t)Vector[5];           // задержка при сканировании вперёд                           5
  conf_.delayB=(uint16_t)Vector[6];           // задержка при сканировании назад                            6
  conf_.betweenPoints_x=(uint16_t)Vector[7];  // расстояние между точками по X в дискретах                  7 
  conf_.betweenPoints_y=(uint16_t)Vector[8];  // расстояние между точками по Y в дискретах                  8 
  conf_.size=(uint16_t)Vector[9];             // size=1  -Z; size=2 - Z,Амплитуда                           9
  conf_.Ti=(uint16_t)Vector[10];               // усиление ПИД                                              10
  conf_.diskretinstep=(uint16_t)Vector[11];    // размер шага в дискретах                                   11
  conf_.pause=(uint16_t)Vector[12];            // время ожидания в точке измерения  мксек                   12  
  conf_.flgLin=(uint8_t)Vector[13];           // флаг линеализации                                          13   
  conf_.lineshift=(uint16_t)Vector[14];        //сдвиг линии -учет неортогональности сканнера               14
  conf_.flgOneFrame=(uint8_t)Vector[15];      // быстрое сканирование один кадр=1                           15
  conf_.flgHoping=(uint8_t)Vector[16];        // сканирование прыжками                                      16
 //hoping
  conf_.HopeDelay=(uint16_t)Vector[17];        // задержка в точке измерения при прыжках                    17
// add hoping params  
 if (sizeof(Vector)>18) 
 {
  conf_.HopeZ=(uint16_t)Vector[18];             // прыжок по Z,если=0,то прыжок по максимуму                18
  conf_.flgAutoUpdateSP=(uint8_t)Vector[19];     // автообновление опоры на каждой линии                    19
  conf_.flgAutoUpdateSPDelta=(uint8_t)Vector[20]; // обновление опоры , если изменение тока превысило порог 20
  conf_.ThresholdAutoUpdate=(uint8_t)Vector[21];  //изменения опоры, если изменение тока превысило порог    21
  conf_.KoeffCorrectISat=(uint16_t)Vector[22];    // опора  %  от тока насыщения                            22
  conf_.SetPoint=(uint16_t)Vector[23];            // опора  ток                                             23
  conf_.HopeDelayFP=(uint16_t)Vector[24];         // Задержка  в первой точке линии                         24  //add 24/05/02
 } 
 if (flgСritical_section) critical_section_exit(&criticalSection);
   /*
            static_cast<uint16_t>(Vector[1]), static_cast<uint16_t>(Vector[2]),
                             static_cast<uint8_t>(Vector[3]),  static_cast<uint8_t>(Vector[4]),
                             static_cast<uint16_t>(Vector[5]), static_cast<uint16_t>(Vector[6]),
                             static_cast<uint16_t>(Vector[7]), static_cast<uint16_t>(Vector[8]),
                             static_cast<uint8_t>(Vector[9]),  static_cast<uint16_t>(Vector[10]),
                             static_cast<uint16_t>(Vector[11]),static_cast<uint16_t>(Vector[12]),
                             static_cast<uint8_t>(Vector[13]), static_cast<int16_t>(Vector[14]),  
                             static_cast<uint8_t>(Vector[15]), static_cast<uint8_t>(Vector[16]),
                             static_cast<uint16_t>(Vector[17]),static_cast<uint16_t>(Vector[18]),
                             static_cast<uint8_t>(Vector[19]), static_cast<uint8_t>(Vector[20]),
                             static_cast<uint16_t>(Vector[21]),static_cast<uint16_t>(Vector[22]),
                             static_cast<int16_t>(Vector[23])
     */       
  delayFW=conf_.delayF; //241111
  delayBW=conf_.delayB; 
  delayHope=conf_.HopeDelay; 
  ZJump=conf_.HopeZ;        
}

Point Scanner::getX0Y0()
{
  sleep_ms(200);
  debugdata.emplace_back(pos_.x);
  debugdata.emplace_back(pos_.y);
  sendStrData(code+std::to_string(GET_CURRENTX0Y0),debugdata,100,true);
  return pos_;
}

void Scanner::move_toX0Y0(uint16_t x, uint16_t y, uint16_t delay, int8_t flg)
 //переместиться в начальную точку  скана из начальной точке предыдущего скана
 //flg=1 Hoping
{
  Point pointX0Y0;
  pointX0Y0.x = (uint16_t) (x);
  pointX0Y0.y = (uint16_t) (y);
  delay = (uint16_t) (delay);
 // sleep_ms(100);  //240306
  if (flgDebug) 
  {
   debugdata.emplace_back(pointX0Y0.x);
   debugdata.emplace_back(pointX0Y0.y);
   debugdata.emplace_back(delay);
   debugdata.emplace_back(pos_.x);
   debugdata.emplace_back(pos_.y);
   sendStrData(code+std::to_string(DEBUG)+" moveto parameters",debugdata,200,true);
  }
  if (flg==1)
  {
    hardware->retract();
    sleep_ms(100);
  }
  move_to(pointX0Y0, delay);
  if (flg==1)
  {
    hardware->protract();
    sleep_ms(delay);
  }
  sleep_ms(200);
  sendStrData(code+std::to_string(STOPPED)+"stopped");
   int16_t count = 0;
  while ((!TheadDone) || (count<20) )//ожидание ответа ПК для синхронизации
  {
    sleep_ms(100);
    count++;
  } 
  TheadDone = false;
  sendStrData(code+std::to_string(END)+"end");
}

void Scanner::move_to(const Point &point, uint16_t delay)
{
  if (!flgVirtual)
  {
    while (pos_.x < point.x)
    {
      hardware->set_DACXY(portx, ++pos_.x);// 1
      sleep_us(delay);
    }
    while (pos_.x > point.x)
    {
      hardware->set_DACXY(portx, --pos_.x);
      sleep_us(delay);
    }
    while (pos_.y < point.y)//2
    {
      hardware->set_DACXY(porty, ++pos_.y);
      sleep_us(delay);
    }
    while (pos_.y > point.y)
    {
      hardware->set_DACXY(porty, --pos_.y); //2
      sleep_us(delay);
    }
  }
  else
  {
    pos_.x = point.x;
    pos_.y = point.y;
    sleep_us(delay * (std::abs(pos_.x - point.x) + std::abs(pos_.y - point.y)));
  }
}

void Scanner::LID_move_toZ0(int lid_name, int freq, int duty, int n, int dir)  //отвестись в безопасную начальную точку по Z
{
 // sleep_ms(1000);
 if (!flgVirtual)
 {
  hardware->retract();  //втянуть сканер
  sleep_ms(50);
  if (!flgVirtual)  hardware->linearDriver->move(lid_name, freq,duty, std::abs(n), dir);
  hardware->protract();  //вытянуть сканер
 } 
  sleep_ms(1000);
 if (flgDebug)
 {  
  debugdata.emplace_back(n);
  debugdata.emplace_back(dir);
  sendStrData(code+std::to_string(DEBUG)+" autorising done ",debugdata,100,true);
 } 
}

void Scanner::LID_move_toNextGridScan(int StepsNmbX,int StepsNmbY,int  StepsNmbZ,int TiApproach, int TiScan)
{
 if (!flgVirtual)
 {
 // LID_move_toZ0(lid_name, freq,  duty,  n,  dir);


 }
 else
 {


 } 
  sendStrData(code+std::to_string(GridMovetoNextScan));
}

void Scanner::positioningXYZ(std::vector<int32_t> &Vector)
{
  uint8_t  lid_name;
  uint16_t flgSICMPrePos;
  uint16_t GATE_Z_MAX, GATE_Z_MIN;
  int8_t status;
  const int none = 30;
  const int ok = 3;
  const int touch = 2;
  int16_t lnsteps;  
  bool ldir;
  bool APPROACHDIR;
  int16_t freq, duty;
  uint16_t flgDev;
 // SET VALUE FROM RX_CORE
          lid_name=(uint8_t)Vector[1]; //  int lid_name
              freq=Vector[2]; 
              duty=Vector[3]; 
           lnsteps=abs((int16_t)Vector[4]); //  int nsteps
              ldir=(bool)Vector[5]; //  int dir
       APPROACHDIR=(bool)Vector[6]; // approach direction set in ini files main delphi program  
        GATE_Z_MAX=(uint16_t)Vector[7]; //  int Z gate max
        GATE_Z_MIN=(uint16_t)Vector[8]; //  int Z gate min
            flgDev=(uint16_t)Vector[9]; //  0= SFM, 1=STM ;SICMAC-2; SICMDC-3;  device type
     flgSICMPrePos=(uint16_t)Vector[10];
   //   pos_data[7] / //  0= SFM, 1=STM ;SICMAC-2; SICMDC-3;  device type
  //    pos_data[8]/ //  Voltage
   if (flgDebug)
   {
    for (int j = 0; j <= 9; ++j)
    {
     debugdata.emplace_back(Vector[j]);
    }
    sendStrData(code+std::to_string(DEBUG)+" parameters posisionXYZ ",debugdata,100,true);
   } 

   if (!flgVirtual) 
   {
        hardware->getValuesFromAdc();
        ZValue = (int16_t)spiBuf[ZPin];
        switch(flgDev)
     {
       case SFM:{
                 SignalValue = (int16_t)spiBuf[AmplPin];
                 break;
                }
       case STM:         
    case SICMDC:{
                 SignalValue = (int16_t)spiBuf[IPin];
                 break;
                }     
     }
   }
  if (lid_name == AxisX || lid_name == AxisY) //X,Y
  {
    while (!STOP) //LID_MOVE_UNTIL_STOP)
    {
      if (CONFIG_UPDATE)
      {
        CONFIG_UPDATE = false;
          lnsteps  = Vupdateparams[1];  //abs
              ldir = Vupdateparams[2]; 
        GATE_Z_MAX = (uint16_t)Vupdateparams[3];
        GATE_Z_MIN = (uint16_t)Vupdateparams[4];
        lnsteps = abs(lnsteps);
        sleep_ms(100);
        if (flgDebug)
        {  
         for (int j =0; j <= 4; ++j)
         {
          debugdata.emplace_back(Vupdateparams[j]);
         }
         sendStrData(code+std::to_string(DEBUG)+"posXYZ parameters update",debugdata,100,true);
        }   
        Vupdateparams.clear();
      }
      status = none;
      if (!flgVirtual) 
      {
        hardware->linearDriver->move(lid_name, freq, duty, std::abs(lnsteps), ldir);
      } 
      else  {    }
      debugdata.emplace_back(status);
      debugdata.emplace_back(ZValue);
      debugdata.emplace_back(SignalValue);
      sendStrData(code+ std::to_string(lid_name) ,debugdata,100,true);
    }
  }
  if (lid_name == AxisZ) //Z
  {
    status = none;
    while (!STOP) //(LID_MOVE_UNTIL_STOP)
    {
      if (CONFIG_UPDATE)
      { 
        CONFIG_UPDATE = false;
              lnsteps =  (int16_t)Vupdateparams[1];
                 ldir =  (int16_t)Vupdateparams[2]; 
           GATE_Z_MAX = (uint16_t)Vupdateparams[3];
           GATE_Z_MIN = (uint16_t)Vupdateparams[4];
        sleep_ms(100);
        if (flgDebug)
        {  
         for (int j = 0; j <= 3; ++j)
         {
          debugdata.emplace_back(Vector[j]);
         }
         sendStrData(code+std::to_string(DEBUG)+" parameters posistionXYZ update",debugdata,100,true);
        } 
     // if (flgСritical_section)  critical_section_enter_blocking(&criticalSection); ???? 241110
       Vupdateparams.clear();
     // if (flgСritical_section)  critical_section_exit(&criticalSection); 
      }
      status = none;
      if (!flgVirtual) 
      {
        hardware->getValuesFromAdc();
        ZValue = (int16_t)spiBuf[ZPin];
        switch(flgDev)
       {
       case SFM:{
                 SignalValue = (int16_t)spiBuf[AmplPin];
                 break;
                }
       case STM:         
    case SICMDC:{
                 SignalValue = (int16_t)spiBuf[IPin];
                 break;
                } 
       }
        // check if z > <
       if (flgSICMPrePos!=1)
       {
        if (ldir == APPROACHDIR)
        {
          if (ZValue < GATE_Z_MIN)
          {
            status = touch;
            break;
          }
          if (ZValue < GATE_Z_MAX)
          {
            status = ok;
            break;
          }
        }
       }
       else
       {
        if (abs(SignalValue)>400) 
        {
         status = ok;
         break;
        }
       }
        hardware->linearDriver->move(lid_name, freq, duty, std::abs(lnsteps), ldir);
      } 
      else //virtual
      {
        if (ldir == APPROACHDIR) { ZValue -= lnsteps; }
        else
        {
          if (ZValue < (ZMaxValue - lnsteps)) { ZValue += lnsteps; }
          else                                { ZValue = ZMaxValue;}
        }
        if (ZValue < GATE_Z_MIN)
        {
          status = touch;
          break;
        }
        if (ZValue < GATE_Z_MAX)
        {
          status = ok;
          break;
        }
        sleep_ms(100); //need to adjust       
      }
      debugdata.emplace_back(status);
      debugdata.emplace_back(ZValue);
      debugdata.emplace_back(SignalValue);
      sendStrData(code+ std::to_string(lid_name),debugdata,100,true);
    }
  }
   STOP=false;
   debugdata.emplace_back(status);
   debugdata.emplace_back(ZValue);
   debugdata.emplace_back(SignalValue);
   sendStrData(code+ std::to_string(lid_name),debugdata,200,true);
   sendStrData(code+std::to_string(STOPPED)+"stopped");
   int16_t count = 0;
  while ((!TheadDone) || (count<20) )//ожидание ответа ПК для синхронизации
  {
    sleep_ms(100);
    count++;
  } 
  TheadDone = false;
  sendStrData(code+std::to_string(END)+"end");
  hardware->dark();
}

 int16_t  Scanner::DACZMove( int16_t Z0, int16_t dZ, int16_t stepsize, uint16_t delay )   // stepsize=+-1  sign  -> dir 
	{
    //  dacZ    0  втянут
    //     -32768  вытянут
    // tested 250423 ok
	  int16_t Zt;
    int16_t dir;
    if (stepsize>0) dir= 1; 
    else            dir=-1; 
      
	  Zt =Z0;
    uint16_t nsteps;
    uint16_t nreststeps;
    nsteps=(uint16_t)abs(dZ/stepsize);
    nreststeps=(uint16_t)abs(dZ) % abs(stepsize);
	  for (int16_t j=0; j< nsteps; j++)
	  {
      if (dir==1)  //втягивание 
      {
        if (Zt>=(maxint16_t-stepsize)) { Zt=maxint16_t;}
        else  Zt=Zt+stepsize; 
      }
      else
      {
        if (Zt<=(minint16_t-stepsize)) { Zt=minint16_t;}
        else  Zt=Zt+stepsize; 
      }         
      if (!flgVirtual) hardware->set_DACZ(Zt);    
      //sleep_us(10); 
      sleep_us(delay); //250514
      //for(int16_t k=0; k < delay; k++) { }// задержка в каждом шаге  ?????
	  }
/*
    if  (flgDebug)
  {
   afc.clear();
   afc =code+std::to_string(DEBUG)+ "zt="+ std::to_string(Zt); 
   afc += endln;
   std::cout << afc;
   afc.clear();
   sleep_ms(100); 
  }
   */
    if (nreststeps!=0)
    {
      if (dir==1)  //втягивание 
      {
        if (Zt>=(maxint16_t-nreststeps)) { Zt=maxint16_t;} 
        else Zt=Zt+nreststeps;
      }
      else
      {
        if (Zt<=(minint16_t+nreststeps)) { Zt=minint16_t;}
        else Zt=Zt-nreststeps;
      } 
      if (!flgVirtual) hardware->set_DACZ(Zt);   
     //sleep_us(300); // 250514    
     sleep_us(30); // 250514    
    }   
    return(Zt);
	}
void Scanner::spectroscopyAIZ(std::vector<int32_t> &Vector) // спектроскопия Ampl-Z
{
/*
   params[0]:=(SpectrParams.Npoints);
   params[1]:=(round(SpectrParams.StartP*TransformUnit.Znm_d) );   //SFM
   params[2]:=(round(SpectrParams.Step*TransformUnit.Znm_d));
//   Threshold
     case  SpectrParams.flgType of
   I_Z:  params[3]:=(round((SpectrParams.LevelIZ)*0.01*round(abs(ApproachParams.SetPoint*TransformUnit.nA_d))));//stop approach i-z
   A_Z:  params[3]:=(round((100-SpectrParams.LevelSFM)*0.01*ApproachParams.UAMMax));//stop approach SFM  ampl-z
         end;
   params[4]:=(SpectrParams.T);
   params[5]:=(integer(flgmode));
 //  params[6]:=(apiType(round(SpectrParams.BiasV*TransformUnit.BiasV_d))); // add 22/10/25
*/

//  Z<=0 сканнер втянут
//  32767 - вытянут
// const int16_t SFM=0;
// const int16_t STM=1;
// const int16_t SICMDC=3;      
 int16_t   NPoints=(int16_t )Vector[1]; // n точек
 int16_t   NCurves=(int16_t )Vector[2]; // отход на  abs(ZStart)
 int16_t    ZStart=(int16_t )Vector[3]; // отход на  abs(ZStart)
 int16_t     ZStep=(int16_t )Vector[4]; // ZStep>0
 int16_t Threshold=(int16_t )Vector[5]; // Threshold
 int16_t     delay=(int16_t )Vector[6]; // delay
 int16_t   flgModa=(int16_t )Vector[7]; // flgmode stm,sfm;

 int16_t SignalValue;
 int16_t dir,dlt,deltaZ;
 int16_t Z0,Zt;

 int16_t MicrostepDelay=3;
 if (flgDebug)
 {  
  for (int j = 0; j <= 7; ++j)
  {
    debugdata.emplace_back(Vector[j]);
  }
 // sendStrData(code+std::to_string(DEBUG)+" AI_Z parameters",debugdata,100,true);
 } 
 //start
  SignalValue=0;
  int16_t k=0;
  vectorA_Z.clear();
  if (!flgVirtual)
  {
    hardware->getValuesFromAdc();
    Z0=(int16_t) spiBuf[ZPin];
    hardware->retract();
    sleep_ms(50);
    deltaZ=DACZMove(0,Z0-abs(ZStart),-10,delay);  //z=0 втянут tested 250423
    debugdata.emplace_back(Z0);
    debugdata.emplace_back(Z0-abs(ZStart));
    sendStrData(code+std::to_string(DEBUG)+" AI_Z parameters",debugdata,100,true);
  }
//////////////////////////////////////
  sleep_ms(200);      
 for(int16_t j=0; j<NCurves; j++)    
 {
  Zt=-abs(ZStart);
  for(int16_t i=0; i<NPoints; i++)     //сближение
  {
   sleep_ms(delay);  
   if (!flgVirtual) 
   {  
      hardware->getValuesFromAdc();   
        switch (flgModa)
    {
     case SFM:     { SignalValue=(int16_t)spiBuf[AmplPin]; break;}  
     case STM:
     case SICMDC:  { SignalValue=(int16_t)spiBuf[IPin];    break;}  
    }
   }
   else
   {
           switch (flgModa)
    {
     case SFM:     { SignalValue=ZMaxValue-abs(deltaZ); break;}  
     case STM:   
     case SICMDC:  { SignalValue=ZMaxValue-abs(deltaZ); break;}  
    } 
   }
     vectorA_Z.emplace_back(SignalValue); 
     vectorA_Z.emplace_back(Zt);
     vectorA_Z.emplace_back(1);
     k+=3;       	
   if (flgModa==STM) 
   {
    if ((abs(SignalValue)>abs(Threshold)) and (i<=(NPoints-1)))
     {
       break;
     }
   };
   if  ((flgModa==SICMDC) || (flgModa==SFM) )
   {
     if (abs(SignalValue)<abs(Threshold) and (i<=(NPoints-1)))
     {
      break; 
     }
   };
    deltaZ = DACZMove(deltaZ, ZStep, -1, MicrostepDelay); //-1
    Zt=Zt+ZStep;
  }  // for    i
    NPoints= k / 3;
    sleep_ms(300);

  for(int16_t i=NPoints; i>=1; i--)  // отвод
  {
    sleep_ms(delay); 
   if (!flgVirtual)
   {
      hardware->getValuesFromAdc(); 
      switch (flgModa)
    {
     case SFM:    { SignalValue=(int16_t)spiBuf[AmplPin]; break;}  
     case STM: 
     case SICMDC: { SignalValue=(int16_t)spiBuf[IPin];    break;}  
    }
   }
   else
   {
      switch (flgModa)
    {
     case SFM:    { SignalValue=ZMaxValue-abs(deltaZ)+100; break;}  
     case STM:    
     case SICMDC: { SignalValue=ZMaxValue-abs(deltaZ+100); break;}  
    }
   }
     vectorA_Z.emplace_back(SignalValue);
     vectorA_Z.emplace_back(Zt);
     vectorA_Z.emplace_back(-1);
     deltaZ = DACZMove( deltaZ, ZStep, 1, MicrostepDelay); 
     Zt=Zt-ZStep;
  } //i
  sendStrData(code+std::to_string(SPECTROSOPY_AIZ),vectorA_Z,100,true); 
  //move to DACZ zero point 
  sleep_ms(300);
  dlt=abs(deltaZ);
 // if (deltaZ>0) dir=-10; //-1
 // else
  dir= 10;  //1
  deltaZ = DACZMove(deltaZ, dlt, dir, MicrostepDelay );
 } //j
 /////////////////////////////////////////  
 // разморозка состояния pid
  if(!flgVirtual) 
  {
    hardware->protract();
    sleep_ms(400);
    DACZMove(deltaZ,abs(deltaZ),10,delay);
  }
 //////////////////////////////////////////
    int16_t count = 0;
  while ((!TheadDone) || (count<20) )//ожидание ответа ПК для синхронизации
  {
    sleep_ms(100);
    count++;
  } 
  TheadDone = false;
  sendStrData(code+std::to_string(END)+"end");
}

void Scanner::spectroscopyIV(std::vector<int32_t> &Vector)
{
    int i,j;
    int16_t  Z0;
    int16_t  UBackup;
    int16_t  delay;
    int16_t  UPoints;
    int16_t  UCurves;
    int32_t  UStep;
    int32_t  UStart;
    int8_t   flgDev;
    int32_t  dacU;
    int32_t  start_step;
    int32_t  step;

    UPoints         = (int16_t) Vector[1]; // n точек
    UCurves         = (int16_t) Vector[2]; // m кривых
		UStart	      	=           Vector[3]; // V начальное значение    
		UStep		        =           Vector[4]; // V шаг 
  	delay           = (int16_t) Vector[5]; // задержка в точке измерения
    flgDev          = (int8_t)  Vector[6]; // прибор not use
    UBackup         = (int16_t) Vector[7]; // V текущее значение напряжения 
//start
 if (flgDebug)
 {  
  for (int j = 0; j <= 7; ++j)
  {
    debugdata.emplace_back(Vector[j]);
  }
  sendStrData(code+std::to_string(DEBUG)+" I_V parameters",debugdata,100,true);
 } 
///////////////////////////////////////////////////  
// if(!flgVirtual)  freezeLOOP(200); 240325
  if (!flgVirtual)
  {
    hardware->getValuesFromAdc();
    Z0=(int16_t) spiBuf[ZPin];
    hardware->retract();
    sleep_ms(50);
    DACZMove(0,Z0,-10,delay);
  }
////////////////////////////////////////////////////
  sleep_ms(300);
// установка начального значения напряжения
      int16_t kk;
      int32_t dlt;
      int16_t nstep;
      int16_t rest;    
      start_step=100;
      dacU=UBackup; //+ShiftDAC; //240206
      UStart=UStart;//+ShiftDAC;
      step=-start_step;
//  снятие ВАХ
 for (j=0; j<UCurves; j++)
 {
      vectorI_V.clear();
      step=-start_step;
      dlt=(dacU-UStart);
      if (dlt<0)
      {
        step=start_step;
	      dlt=-dlt;
      }
      nstep=dlt / start_step;
      rest=dlt%start_step;
      for (kk=0; kk<nstep; kk++)
      {
       if (!flgVirtual) hardware->set_BiasV(dacU);    
       sleep_ms(10);   
       dacU+=step;
      } 
      dacU+=rest;
      if (!flgVirtual) hardware->set_BiasV(dacU);         
      sleep_ms(10);  
      for(i=0; i<UPoints; i++)
      {
       if (!flgVirtual) {hardware->set_BiasV(dacU); }
       sleep_ms(delay);
       if (!flgVirtual)
       {
        hardware->getValuesFromAdc();
        SignalValue = (int16_t)spiBuf[IPin];
        vectorI_V.emplace_back(UStart+i*UStep);
        vectorI_V.emplace_back(SignalValue);
       }
       else
       {
        vectorI_V.emplace_back(UStart+i*UStep);
        vectorI_V.emplace_back(dacU);
       }
       dacU+=UStep;
      }
      sleep_ms(100);
      sendStrData(code+std::to_string(SPECTROSOPY_IV),vectorI_V,100,true); //65 
  //move to start point
  }// j Curves  
    //возврат к исходному напряжению
     dacU-=UStep;
     step=-start_step;
     dlt=(dacU-UBackup);
   if (dlt<0)
   {
     step=start_step;
	   dlt=-dlt;
   }     
  rest=dlt%start_step ;
  nstep=dlt/start_step;
  for (kk=0; kk<nstep; kk++)
  {
   if (!flgVirtual) hardware->set_BiasV(dacU);  
   sleep_ms(10);
   dacU+=step;
  }
    dacU+=rest;
  if (!flgVirtual) hardware->set_BiasV(dacU);  
  sleep_ms(10);  
  if (!flgVirtual) hardware->set_BiasV(UBackup);  
  sleep_ms(10);
 ///////////////////////////////////////////////
 //  if(!flgVirtual)  unfreezeLOOP(500); //240322
  if (!flgVirtual)
  {
    hardware->protract();
    sleep_ms(400);
    DACZMove(-Z0,abs(Z0),10,delay);
  }
/////////////////////////////////////////////  
   int16_t count = 0;
  while ((!TheadDone) || (count<20) )//ожидание ответа ПК для синхронизации
  {
    sleep_ms(10);
    count++;
  } 
  TheadDone = false;
  sendStrData(code+std::to_string(END)+"end");  
}

void Scanner::approacphm(std::vector<int32_t> &Vector) 
{
  const int none = 30;
  const int ok = 3;
  const int touch = 2;
  const int stopdone = 1;
 // uint16_t ZMaxValue = 32767;
 // uint16_t SignalMaxValue =32767;
  int16_t  SET_POINT;
  int16_t  GATE_Z_MAX, GATE_Z_MIN;
  int16_t  freq, duty;//
  uint16_t GAIN;
  int16_t  NSTEPS;
  uint16_t INTDELAY, SCANNERDECAY;
  int16_t  flgDev;
  int16_t  BiasV;
  bool     DIR;
  bool     APPROACHDIR ;
  // SET VALUE FROM RX_CORE
  SET_POINT      =(int16_t) Vector[1]; // set point
  GATE_Z_MAX     =(int16_t) Vector[2]; // max
  GATE_Z_MIN     =(int16_t) Vector[3]; // min
  NSTEPS         =(int16_t) Vector[4]; // steps 
  DIR            =(bool)    Vector[5]; //  int dir
  APPROACHDIR    =(bool)    Vector[6]; //  APPRoach dir read from ini file
  INTDELAY       =(uint16_t)Vector[7]; // initdelay
  GAIN           =(uint32_t)Vector[8]; // gain  //240320
  SCANNERDECAY   =(uint16_t)Vector[9]; // scannerDelay 
  freq           =(int16_t) Vector[10]; // freq
  duty           =(int16_t) Vector[11]; // scv
  flgDev         =(int16_t) Vector[12];//  0= SFM, 1=STM ;SICMAC-2; SICMDC-3;  device type
  BiasV          =(int16_t) Vector[13];// Voltage need for STM,SICM
 //need to add channel Bias ????
 //need to add channel SetPoint ????
 // ZValue=-1000;
 if (flgDebug)
 {
  for (size_t j = 0; j < 11; j++)     // send info
  {
    debugdata.emplace_back(Vector[j]);
  } 
  sendStrData(code+std::to_string(DEBUG)+" approach parameters  ",debugdata,100,true);
 } 
  hardware->set_SetPoint(SET_POINT); 
  if (flgDev!=SFM) hardware->set_BiasV(BiasV);  
  hardware->set_GainPID((uint32_t)GAIN); 
  if (!flgVirtual)
  {
    hardware->getValuesFromAdc(); 
    ZValue = (int16_t)spiBuf[ZPin];
    switch (flgDev)
   {
      case SFM: 
     {
      SignalValue = (int16_t)spiBuf[AmplPin];
      break;  
     } 
      case STM:
   case SICMDC:  
     {
      SignalValue = (int16_t)spiBuf[IPin];
      break;  
     } 
   }
  }
  std::vector<int16_t> buf_status;
  buf_status.push_back(none);
  buf_status.push_back(ZValue);
  buf_status.push_back(SignalValue);

  sendStrData( code+std::to_string(APPROACH),buf_status,100,false);
  while (true)
  { 
    sleep_ms(INTDELAY);
    buf_status[0] = none;
    if (STOP)
    {
      STOP=false;
      buf_status[0] = stopdone;
      buf_status[1] = ZValue;
      buf_status[2] = SignalValue;
      sleep_ms(200);
      sendStrData(code+std::to_string(STOPPED)+" stopped");
    //  sleep_ms(200); //24/02/02
      break;
    }
    if (CONFIG_UPDATE)
    {
      CONFIG_UPDATE = false;
      SET_POINT    = Vupdateparams[1];
      GATE_Z_MAX   = Vupdateparams[2];
      GATE_Z_MIN   = Vupdateparams[3];
      NSTEPS       = Vupdateparams[4];
      DIR          =(bool)Vupdateparams[5]; //  int dir
      INTDELAY     = Vupdateparams[6];
      GAIN         =(uint32_t) Vupdateparams[7];
      SCANNERDECAY = Vupdateparams[8];
     
     // if (flgDev!=SFM) set_Bias(1,Bias);  240211
      hardware->set_SetPoint(SET_POINT); 
      sleep_ms(100);  // need for virtual для разделение afc //240314
      hardware->set_GainPID(GAIN); //240316
      sleep_ms(100);  // need for virtual для разделение afc //240314
      if (flgDebug)
      {  
       for (int j = 0; j <= 7; ++j)
       {
        debugdata.emplace_back(Vupdateparams[j]);
       }
       sendStrData(code+std::to_string(DEBUG)+" parameters update",debugdata,200,true);
      } 
     if (flgСritical_section)  critical_section_enter_blocking(&criticalSection); 
      Vupdateparams.clear();
     if (flgСritical_section)  critical_section_exit(&criticalSection); 
    }
 
    if (!flgVirtual) // get values
    {
      hardware->getValuesFromAdc();
      ZValue = (int16_t)spiBuf[ZPin];
     switch (flgDev)
     {
      case SFM: 
      {
       SignalValue = (int16_t)spiBuf[AmplPin];
       break;  
      } 
         case STM:
      case SICMDC:  
      {
       SignalValue = (int16_t)spiBuf[IPin];
      break;  
      } 
     }     
   // if (flgDebugLevel <= DEBUG_LEVEL);//log("Z = " + std::to_string(Z) + '\n',flgDebugLevel);
      buf_status[1] = ZValue;
      buf_status[2] = SignalValue;    
    }
    else //virtual
    {
      if(APPROACHDIR==DIR)
      {
            ZValue -= 500;
        SignalValue-= 500;
      }
      else
      {
        if ((ZMaxValue - ZValue) > 0) { ZValue += 500; }
                                 else { ZValue  = maxint16_t; }
        if ((SignalMaxValue - SignalValue) > 0) { SignalValue += 500; }
                                           else { SignalValue  = maxint16_t; }                                 
      }
      buf_status[1] = ZValue;
      buf_status[2] = SignalValue;
    }
    if(APPROACHDIR==DIR)
    {
      if (ZValue <= GATE_Z_MIN)
      {
        buf_status[0] = touch;
        buf_status[1] = ZValue;
        buf_status[2] = SignalValue;
        break;
      }
      if (ZValue <= GATE_Z_MAX)
      {
        int k = 0;
        for (int i = 0; i < 10; ++i)
        {
          if (!flgVirtual)
          {
             hardware->getValuesFromAdc();
            ZValue = (int16_t)spiBuf[ZPin];
          }
          if (ZValue <= GATE_Z_MAX) k++;
          if (k == 3)
          {
            buf_status[0] = ok;
            buf_status[1] = ZValue;
            buf_status[2] = SignalValue;
            if (flgDebugLevel <= DEBUG_LEVEL);// log("success\n",flgDebugLevel);
            break;
          }
          sleep_ms(10);
        }
        if (buf_status[0] == ok)  { break; }
      }
    } //NSTEPS>0
    sendStrData( code+std::to_string(APPROACH),buf_status,100,false);//75
    /**/
    if (!flgVirtual)
    {
      hardware->retract();  //втянуть сканнер
      sleep_ms(SCANNERDECAY);
      hardware->linearDriver->move(AxisZ, freq, duty, std::abs(NSTEPS), DIR);
      hardware->protract(); //вытянуть
    }
  } //while
  sendStrData( code+std::to_string(APPROACH),buf_status,100,false);
  if (!flgVirtual)
  {
    hardware->protract();//вытянуть
    sleep_ms(INTDELAY);
  }
 //  sendStrData("stopped");
  int16_t count = 0;
  while ((!TheadDone) || (count<20) )//ожидание ответа ПК для синхронизации
  {
    sleep_ms(100);
    count++;
  } 
  TheadDone = false;
  sendStrData(code+std::to_string(END)+"end");
}

void Scanner::testpiezomover(std::vector<int32_t> &Vector)
{
  int8_t   flgstop;  //=1  stop
  int16_t  step,Z0;
  int16_t  GATE_Z_MAX, GATE_Z_MIN;
  int16_t  freq, scv;//
  int16_t  NSTEPS,NCYCLES;
  uint16_t INTDELAY, SCANNERDECAY;
  // SET VALUE FROM RX_CORE
         GATE_Z_MAX     =(int16_t) Vector[1]; // max
         GATE_Z_MIN     =(int16_t) Vector[2]; // min
         NSTEPS         =abs((int16_t) Vector[3]); // steps 
         NCYCLES        =(int16_t) Vector[4]; // ncycles 
         INTDELAY       =(uint16_t)Vector[5]; // initdelay
         SCANNERDECAY   =(uint16_t)Vector[6]; // scannerDelay 
         freq           =(int16_t) Vector[7]; // freq
         scv            =(int16_t) Vector[8]; // scv  
    //     flgDev         =(int16_t) Vector[9];//  0= SFM, 1=STM ;SICMAC-2; SICMDC-3;  device type
 if (flgDebug)
 {
   for (size_t j = 0; j < 9; j++)     // send info
  {
    debugdata.emplace_back(Vector[j]);
  } 
  sendStrData(code+std::to_string(DEBUG)+" test motor steps parameters  ",debugdata,300,true);
 }
  flgstop=0;
  if (!flgVirtual)
  {
    hardware->getValuesFromAdc(); 
    ZValue = (int16_t)spiBuf[ZPin];
  } 
  else
  { 
    ZValue= GATE_Z_MIN+1000;
  }    
     Z0=ZValue;  
     std::vector<int16_t> buf_status;
     buf_status.push_back(ZValue);
     buf_status.push_back(NSTEPS);
     buf_status.push_back(0); //cycle nmb
     buf_status.push_back(0);
     sendStrData( code+std::to_string(TESTMOVER),buf_status,100,true);
    // проверить, в воротах ли Z
    step = NSTEPS;          // NSTEPS > 0 - сближение
   // Идти вниз до мин. точки
     while ( ZValue > GATE_Z_MIN )          // ZMin < Z < ZMax означает, что Z  в воротах
    	{
        if (STOP)
        {
         STOP=false;
         flgstop=1;
         sleep_ms(200);  
         sendStrData(code+std::to_string(STOPPED)+"stopped");
         break;
        }
          //steps
        if (!flgVirtual)
        {
          hardware->retract();  //втянуть сканнер
          sleep_ms(SCANNERDECAY);
          hardware->linearDriver->move(AxisZ, freq, scv, std::abs(step), step > 0);
          hardware->protract(); //вытянуть
          sleep_ms(INTDELAY);
        }  
        if (!flgVirtual)
        {
          hardware->getValuesFromAdc(); 
          ZValue = (int16_t)spiBuf[ZPin];
        } 
        else
        {
          sleep_ms(INTDELAY);
         ZValue=ZValue-step*100;
        }     
        buf_status.push_back(ZValue);
        buf_status.push_back(step);
        buf_status.push_back(0); //cycle nmb
        buf_status.push_back(0);
        sendStrData( code+std::to_string(TESTMOVER),buf_status,100,true);
      };
      // циклическое хождение между воротами
      int16_t i=0;
      while(NCYCLES>i)
      {
       if (flgstop==1) break;                 // если "стоп" был прочитан ранее
       step=-NSTEPS;
       while (ZValue <GATE_Z_MAX)                                    // идти вверх до верхних ворот
       {
        if (flgstop==1) break;                 // если "стоп" был прочитан ранее
        if (STOP)
        {
         STOP=false;
         flgstop=1;
         sleep_ms(200); 
         sendStrData(code+std::to_string(STOPPED)+"stopped");
         break;
        }                                    
        if (!flgVirtual)
        {
          hardware->retract();  //втянуть сканнер
          sleep_ms(SCANNERDECAY);
          hardware->linearDriver->move(AxisZ, freq, scv, std::abs(step), step > 0);
          hardware->protract(); //вытянуть
          sleep_ms(INTDELAY);
          hardware->getValuesFromAdc(); 
          ZValue = (int16_t)spiBuf[ZPin];
        }  
        else
        {
         sleep_ms(INTDELAY);
         ZValue=ZValue-step*300;
        }    
        buf_status.push_back(ZValue);
        buf_status.push_back(step);
        buf_status.push_back(i); //cycle nmb
        buf_status.push_back(0);
        sendStrData( code+std::to_string(TESTMOVER),buf_status,100,true);  
       } //while
       step = NSTEPS;
       while (ZValue > GATE_Z_MIN)          // идти до нижних ворот
       {
         if (flgstop==1) break;                 // если "стоп" был прочитан ранее
         if (STOP)
         {
          flgstop=1;
          sleep_ms(200);
          sendStrData(code+std::to_string(STOPPED)+"stopped");
          break;
         }
         if (!flgVirtual)
         {
          hardware->retract();  //втянуть сканнер
          sleep_ms(SCANNERDECAY);
          hardware->linearDriver->move(AxisZ, freq, scv, std::abs(step), step > 0);
          hardware->protract(); //вытянуть
          sleep_ms(INTDELAY);
          hardware->getValuesFromAdc(); 
          ZValue = (int16_t)spiBuf[ZPin];
         }  
         else
         { 
           sleep_ms(INTDELAY);
           ZValue=ZValue-step*300;
         }      
         buf_status.push_back(ZValue);
         buf_status.push_back(step);
         buf_status.push_back(i); //cycle nmb
         buf_status.push_back(0);
         sendStrData( code+std::to_string(TESTMOVER),buf_status,100,true);       
       } // while
     if (CONFIG_UPDATE)
     {
        CONFIG_UPDATE = false;
        NSTEPS       = abs(Vupdateparams[1]);
        INTDELAY     = Vupdateparams[2];   
        freq         = Vupdateparams[3];
        scv          = Vupdateparams[4];
       if (flgDebug)
       {
        for (int j = 0; j <= 4; ++j)
        {
         debugdata.emplace_back(Vupdateparams[j]);
        }
        sendStrData(code+std::to_string(DEBUG)+" test piezo parameters update",debugdata,200,true);
       } 
        Vupdateparams.clear();
       }
       i++;
      }//while cyclecount
//////////////////////////////////////////////////////////////////////////////////////
//  возврат в начальное состояние
////////////////////////////////////////////////////////////////
        if (!flgVirtual)
        {
          hardware->getValuesFromAdc(); 
          ZValue = (int16_t)spiBuf[ZPin];
        } 
        else
        {
        //  sleep_ms(INTDELAY);
        // ZValue=ZValue-step*100;
        }  
        if (flgstop!=1) 
        {     
         if (ZValue < Z0) 
         {
          step= - NSTEPS;
          while (ZValue < Z0)                                    // идти вверх до Z0 (начальной позиции)
  		    {
            if (STOP)
            {
             sleep_ms(200);
             sendStrData(code+std::to_string(STOPPED)+"stopped");
             break;
            }
            if (!flgVirtual)
            {
               hardware->retract();  //втянуть сканнер
               sleep_ms(SCANNERDECAY);
               hardware->linearDriver->move(AxisZ, freq, scv, std::abs(step), step > 0);
               hardware->protract(); //вытянуть
               sleep_ms(INTDELAY);
               hardware->getValuesFromAdc(); 
               ZValue = (int16_t)spiBuf[ZPin];
            }   
            else
            {
              sleep_ms(INTDELAY);
              ZValue=ZValue-step*100;
            }     
              buf_status.push_back(ZValue);
              buf_status.push_back(step);
              buf_status.push_back(i); //cycle nmb
              buf_status.push_back(0);
              sendStrData( code+std::to_string(TESTMOVER),buf_status,100,true); 
            };
          }
          else
          {
            step=   NSTEPS;
            while (ZValue > Z0) // идти вверх до Z0 (начальной позиции)
    		    {
             if (STOP)
             {
              sleep_ms(200);
              sendStrData(code+std::to_string(STOPPED)+"stopped");
              break;
             }
             if (!flgVirtual)
              {
               hardware->retract();  //втянуть сканнер
               sleep_ms(SCANNERDECAY);
               hardware->linearDriver->move(AxisZ, freq, scv, std::abs(step), step > 0);
               hardware->protract(); //вытянуть
               sleep_ms(INTDELAY);
               hardware->getValuesFromAdc(); 
               ZValue = (int16_t)spiBuf[ZPin];
              }
              else
              {
               sleep_ms(INTDELAY);
               ZValue=ZValue-step*100;
              }        
              buf_status.push_back(ZValue);
              buf_status.push_back(step);
              buf_status.push_back(i); //cycle nmb
              buf_status.push_back(0);
              sendStrData( code+std::to_string(TESTMOVER),buf_status,100,true); 
            };
          }     
       }
//  Признак конца
            sleep_ms(300);
            buf_status.push_back(ZValue);
            buf_status.push_back(step);
            buf_status.push_back(i); //cycle nmb
            buf_status.push_back(1); //признак конца
            sendStrData( code+std::to_string(TESTMOVER),buf_status,100,true);    
///////////////////////////////////////////////////////////////////////////////////
  int16_t count = 0;
  while ((!TheadDone) || (count<20) )//ожидание ответа ПК для синхронизации
  {
    sleep_ms(100);
    count++;
  } 
  TheadDone = false;
  sendStrData(code+std::to_string(END)+"end");
 }   //test mover

void Scanner::start_frqscan()
{
  int16_t  SignalValue;
  int32_t  res_freq = 10000;
  double_t a = 10000;
  int16_t  scan_index = 0;
  int32_t  current_freq = 0;
  int32_t freq;
  uint32_t freqstep;
  int16_t  delay;
  int32_t freqstart;
  int16_t  npoint;
    npoint=Vector[1];
 freqstart=(int32_t)Vector[2];
  freqstep=(int32_t)Vector[3];
     delay=Vector[4];
  if (flgDebug)
  {  
   for (int j = 0; j < 5; ++j)
   {
    debugdata.emplace_back(Vector[j]);
   }
   sendStrData(code+std::to_string(DEBUG)+" frq scan parameters ",debugdata,100,true);
  }
  std::vector<int32_t> data;
  freq=freqstart;
  while ((scan_index++ < npoint))
  {
    if (!flgVirtual)
    {
      hardware->set_Freq(freq);
      sleep_ms(delay);
      hardware->getValuesFromAdc();
      SignalValue = (int32_t)spiBuf[AmplPin];
      data.emplace_back(freq);
      data.emplace_back(SignalValue); 
    }
    else
    {
      current_freq =freq;
      sleep_ms(delay);
      SignalValue = (int32_t) std::round(a*(pow(M_E,-pow((current_freq - res_freq),2)/1000000))); 
      data.emplace_back(current_freq);
      data.emplace_back(SignalValue);
    }
    sleep_ms(10);
    freq += freqstep;
  }
  sendStrData(code+std::to_string(RESONANCE),data,100,true);
   int16_t count = 0;
  while ((!TheadDone) || (count<20) )//ожидание ответа ПК для синхронизации
  {
    sleep_ms(100);
    count++;
  } 
  TheadDone = false;
  sendStrData(code+std::to_string(END)+"end");
}

