// gdanek 2022
// Библиотека для работы с DHT11
//GitHub: https://github.com/gdanek
#ifndef DHTXX_H
#define DHTXX_H
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#ifdef P_DHT_REG               // Если нужно поменять регистр
#if P_DHT_REG == 1             // 1 для А
#define ONW_PIN_REG_DDR DDRA
#define ONW_PIN_REG_PORT PORTA
#define ONW_PIN_REG_PIN PINA
#elif P_DHT_REG == 2           // 2 для B
#define ONW_PIN_REG_DDR DDRB
#define ONW_PIN_REG_PORT PORTB
#define ONW_PIN_REG_PIN PINB
#elif P_DHT_REG == 3           // 3 для C
#define ONW_PIN_REG_DDR DDRC
#define ONW_PIN_REG_PORT PORTC
#define ONW_PIN_REG_PIN PINC
#else                           // Любое другое значение - по умолчанию D
#define ONW_PIN_REG_DDR DDRD
#define ONW_PIN_REG_PORT PORTD
#define ONW_PIN_REG_PIN PIND
#endif
#else                            // По умолчанию D
#define ONW_PIN_REG_DDR DDRD
#define ONW_PIN_REG_PORT PORTD
#define ONW_PIN_REG_PIN PIND
#endif
// Всего одна структура. Больше не надо
class DHT
{
  uint8_t pin; // пин датчика
  bool data[40];// Тяжёлый массив. 
  bool rst(); // Сброс датчика
  void getBits(); // Считать все 40 бит датчика
  public:
  DHT(uint8_t pin){this->pin = pin;} // Конструктор
  uint8_t getHum(); // Получить влажность с датчика
  int8_t getTmp(); // Получить температуру с датчика
  void getData(int &hum, int &temp); // Обработать и вернуть температуру с влажностью
  void getData(uint8_t &hum); // Перегрузка функции getData. Если указан один параметр - передается только влажность
};// Всё !
#endif 
// danek 2022
