//UartRpi.cpp

/////////////////////////////////////////////////////////////////////
//===================UART port class for RPI 2=====================//
//====================by Michal Tatara 2016========================//
/////////////////////////////////////////////////////////////////////
/* 
   To use 

*/

#include <iostream> //cout
  
#include <wiringSerial.h>//requires -lwiringPi while compiling

#include "UartRpi.h"
#include "Servo.h"      //Servo DRS class/library with abstract UartComm class

using namespace std;

UartRpi::UartRpi(const char *port_name){

   UartRpi::port_name = const_cast<char *>(port_name);
   
   if(debug){
      cout<<"Port name is: ";
      char *port_name_temp = UartRpi::port_name;
      while(*port_name_temp)
         cout << *(port_name_temp++);
      cout << endl;
   }
   
}

bool UartRpi::SendBytes(unsigned char *data_array, unsigned char array_size){

   int handle = serialOpen (const_cast<const char *>(port_name), 115200) ;// /dev/ttyAMA0 / /dev/ttyUSB0
   
   if(debug) {cout << "In SendBytesRpi "; SHOW(handle);}
   
   for(int i = 0; i < array_size; i++){
      serialPutchar (handle, data_array[i]) ;
   }
   serialClose (handle) ;
   
   return 0; 
}