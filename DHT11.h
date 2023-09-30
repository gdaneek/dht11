// gdanek 2022
// Библиотека для работы с DHT11
//GitHub: https://github.com/gdanek
#ifndef DHTXX_H
#define DHTXX_H

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
  private:  // Для класса можно не писать
  bool use_dht11 = true;
  uint8_t pin; // пин датчика
  bool data[40];// Тяжёлый массив. 
  //Тип данных можно поменять на uint8_t, уменьшив размер памяти для массива в два раза
  bool rst() // Сброс датчика
  {
    uint8_t c = 0;
    DDRD |= (1<<pin);
    _delay_us(2);
    PORTD &= ~(1<<pin);
    if(use_dht11)_delay_ms(20);      // Для DHT11 нужна такая большая задержка. Много спит
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
  void getBits() // Считать все 40 бит датчика
  {
     // Обновляем массив. Можно и не делать 
    for(uint8_t i = 0;i < 40;i++)
    {
	    uint8_t d = 0;
      while((!(PIND & (1 << pin)))&&(d < 255))d++;//ждем, когда датчик отпустит линию
      uint8_t counter = 1; 
      while(((PIND & (1 << pin)))&&(counter>0))counter++;//пока датчик не "прижал" линию - считаем. Как прижал - выходим(на линии ноль)
      data[i] = (counter > 65)? 1 : 0;// запоминаем, сколько насчитали, пока на линии была единица
     }
  }
  public:
  DHT(uint8_t pNum){pin = pNum;} // Конструктор
  DHT(uint8_t pNum,bool __dht11){pin = pNum;use_dht11 = __dht11;} // Конструктор
  uint8_t getHum() // Получить влажность
  {
    uint8_t hum = 0;
    //первые 8 бит - влажность. Зачем нам биты с 8 по 15?
    for(int8_t i = 0;i < 8;i++){if(data[i])hum |= (1<<(7-i));} // Читаем и двигаем в соответствии с datasheet
    return hum;
  }
  int8_t getTmp() // Получить температуру
  {
    int8_t tmp = 0;
    for(int8_t i = 16;i > 8;i--){if(data[i])tmp |= (1<<(7-(i-16)));}
    return tmp;
  }
  void getData(uint8_t &hum, int8_t &temp) // Получить влажность и температуру. 
  { 
    hum = 0;
    temp = 0;
    rst();
    getBits();
    hum = getHum();  // Результат записан по ссылке
    temp = getTmp(); // Результат записан по ссылке
  }
  void getData(uint8_t &hum, uint8_t &temp) // Перегрузка на uint8_t
  { 
    hum = 0;
    temp = 0;
    rst();
    getBits();
    hum = (uint8_t)getHum();  // Результат записан по ссылке
    temp = (uint8_t)getTmp(); // Результат записан по ссылке
  }
  void getData(int &hum, int &temp) // Перегрузка на самый популярный int
  { 
    hum = 0;
    temp = 0;
    rst();
    getBits();
    hum = (int)getHum();  // Результат записан по ссылке
    temp = (int)getTmp(); // Результат записан по ссылке
  }
   void getData(uint8_t &hum) // Перегрузка функции getData. Аналог getHum
  {
    rst();
    getBits();
    hum = getHum();
  }
};// Всё !
#endif 
// danek 2022
