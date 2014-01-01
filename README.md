Atmega8-as-ADC-slave
================================

Use three digital pins on the arduino to read data from upto four analog sensors or devices with Atmega8 as slave.


Arduino----          Atmega8

  2       --------------      PB1
  
  4       --------------      PB3
  
  5       --------------      PB4 
  
  
Atmega8 (analog input pins) :-  PC2, PC3, PC4, PC5 

Note:- ground unused adc pins to avoid noise.
