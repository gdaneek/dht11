# dht11
Library for working with the DHT11/DHT22 sensor

For any ATmega and ATtiny series microcontrollers

Takes up very little memory (less than 1KB)

Hardware: 

-> use one sensor per pin of the microcontroller

Software: 

-> use the include directive to connect the library

-> create an object of the dht11 class

-> use the constructor with a parameter indicating the pin on which the sensor is located

-> use the getData(&int,&int) method to get the temperature and humidity(the method does not return anything, the values ​​are written by reference)

-> use the getData(&int) to get only humidity

-> see example.ino
