// gdanek 2022
// Библиотека для работы с датчиком DHT
//GitHub: https://github.com/gdanek
// version 1.0
#include "DHTXX.h"
bool DHT::rst() // Сброс датчика
{
  uint8_t c = 0;
  DDRD |= (1<<pin);
  _delay_us(2);
  PORTD &= ~(1<<pin);
  _delay_ms(20);      // Для DHT11 нужна такая большая задержка. Много спит
  DDRD &= ~(1<<pin);
  _delay_us(2);
  while((PIND & (1 << pin)))   // Ждём ответа датчика 
  {
    c++;if(c>100) // Если не отвечает - выходим из цикла с ошибкой
    return false;
  }
  _delay_us(150);
  return true;
}
void DHT::getBits() // Считать все 40 бит датчика
{
  for(uint8_t i = 0;i < 40;i++)
  {
    uint8_t d = 0;
    while((!(PIND & (1 << pin)))&&(d < 255))d++;//ждем, когда датчик отпустит линию
    uint8_t counter = 1; 
    while(((PIND & (1 << pin)))&&(counter>0))counter++;//пока датчик не "прижал" линию - считаем. Как прижал - выходим(на линии ноль)
    data[i] = (counter > 65)? 1 : 0;// запоминаем, сколько насчитали, пока на линии была единица
  }
}
uint8_t DHT::getHum() // Получить влажность с датчика
{
  uint8_t hum = 0;
  //первые 8 бит - влажность. Зачем нам биты с 8 по 15?
  for(int8_t i = 0;i < 8;i++){if(data[i])hum |= (1<<(7-i));} // Читаем и двигаем в соответствии с datasheet
  return hum;
}
int8_t DHT::getTmp() // Получить температуру с датчика
{
  int8_t tmp = 0;
  for(int8_t i = 16;i > 8;i--){if(data[i])tmp |= (1<<(7-(i-16)));}
  return tmp;
}
void DHT::getData(int &hum, int &temp) 
{ 
  hum = 0;
  temp = 0;
  rst();
  getBits();
  hum = (int)getHum();  // Результат записан по ссылке
  temp = (int)getTmp(); // Результат записан по ссылке
}
void getData(uint8_t &hum) 
{
  rst();
  getBits();
  hum = getHum();
}
//gdanek2022