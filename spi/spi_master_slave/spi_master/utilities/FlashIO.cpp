#include <iostream>
#include "pico_flash.h"
int test_flash()
{

  // Запись
  // Любая структура размером меньше 256 байт: sizeof(data) <= 256!!!
  Data_example data{};
  data.a = 8213;
  data.b = 551;
  data.c = 4;
  data.d = 7499;
  // При записи передать страницу (FLASH_TARGET_OFFSET_0 - FLASH_TARGET_OFFSET_9) на которую будет записана data
  // WARNING !!! После завершнения функции программа завершается. Ищу как поправить.
  pico_flash_write<Data_example>(FLASH_TARGET_OFFSET_8, &data);

  // Чтение
  // Считывание с указанной страницы
  data = pico_flash_read<Data_example>(FLASH_TARGET_OFFSET_8);
  std::cout << data.a << '\n';
  std::cout << data.b << '\n';
  std::cout << data.c << '\n';
  std::cout << data.d << '\n';

  std::cout << "\n All done..............\n";
  return 0;
}
