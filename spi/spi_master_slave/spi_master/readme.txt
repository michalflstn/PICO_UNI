//250321
работа с uart в cmakelist.txt добалено pico_sdk_init()
и  в target_link_libraries hardware_uart?
BBFPGA - управление  gain  один ползунок нет addgain
//250221
error set freq>65568 -corrected!
//250116
gain -> unit32_t
uint16_t error
freq -unint8_t-> unit32_t
edited scan, scanlin, hope,hopelin
//250110
fast scan синхронизация с ПК
// 241214
optimize!
getValuesFromAdc() 
repeatTwoTimes()
SetDacXY
//241210
сделать плавное управление усилением ПИДом
//241028
исправлена ошибка при изменении скорости подвода при позиционировании
для WB инверсия BiasV
initcommutation при смене прибора

//241006
управление направлением движения пьезодивижителей

//240620
изменены параметры коммуникации для WB
//240603
  изменения GainPID для MB
  
// 24/06/01
 добавлена Init communication
 и добавлена Default init comm в hardware default setting
 Решить вопрос с портами IO1.0 IO1.1!!!!
//24/05/30
drive работает резонанс - малая амплитуда
проблема может быть с MB setdefault!!
//240528
 в тесте шагов надо подправить  - при смене направления последнюю точку дубрировать и красить  цветом  
 соответвущим след направлению движения!!!
//24/05/22
class lineardrivebase and child 

// 24/05/07
error забыл про busyport +
//24/05/06
проверить управление  ПИД
//240504
 работа с Lineardriver
 перейтить к pointer создавать только в момент использования +
 изучить atomic!!!
//240502
 ввести параметр задержка в начале линии для хоппинга +
 проверить работу ПИД - есть изменения !!!!!?
//240131
MicroController read data from PC to  Vector<int32_t>
loop() if -> switch
some bool flags -> constants


