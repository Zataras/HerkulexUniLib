//servo.cpp

/////////////////////////////////////////////////////////////////////
//===============Servo class of Herkulex DRS-0602==================//
//================author: Michal Tatara, Poland====================//
//====================created in 2016, v 1.0=======================//
/////////////////////////////////////////////////////////////////////
/* 
   To use library just create Servo object passing it's ID as 
argument. It will create it's inside objects as e.g. Led object.
   E.g. if You want to light green LED of your created Servo use: 
your_serwo.led.greenOn(); 
   Example for error acknowledgement: 
your_serwo.errorAck();

*/

#include <iostream> //cout
  
#include "Servo.h"

using namespace std;

/////////////////////////////////////////////////////////////////////
//=================================================================//
//===============Servo class functions - general===================//
//=================================================================//

//constructor
Servo::Servo(unsigned char id, UartComm *uart_port_ptr){

   Servo::id = id;
   if(uart_port_ptr)
      Servo::uart_port_ptr = uart_port_ptr;
   else{
      Servo::uart_port_ptr =  0;
      cout << "Pointer to port is 0." << endl;
   }
   //debug
   if (debug){
      cout<<"Utworzono obiekt serwo, "; 
      SHOW(+id);
   }
   
   //init clear array:
   for (int i = 0; i < MAX_PACKET_SIZE; i++)
      data_bytes[i] = 0;
      
   led = Led(this);//create Led object
}

//0 - error, any other value is checksum
unsigned char Servo::checksum(int no){
//no only 1 or 2    
   int sum = 0;
   int data_amount = command_data.size - MIN_PACKET_SIZE;
   
   if((no == 1) || (no == 2))
      sum = command_data.size^id^command_data.cmd;
   else 
      return 0; //wrong input

   for(int i = 0; i < data_amount; i++){
      sum = sum^command_data.data[i];
   }
   
   if(no == 2){
      sum = ~sum;
   }
   
   sum = sum & 0xFE;
   
   return sum; 
}

//Generates command in format: bytes
void Servo::genCommand(){

   const unsigned char HEADER = 0xFF;

   if(debug) cout << "Generating command" << endl;
   
   data_bytes[0] = HEADER ;
   data_bytes[1] = HEADER;
   data_bytes[2] = command_data.size;
   data_bytes[3] = id;
   data_bytes[4] = command_data.cmd;
   data_bytes[5] = checksum(1);
   data_bytes[6] = checksum(2);
   //rest:
   for(int i = 0; i < (command_data.size - MIN_PACKET_SIZE); i++){
      data_bytes[i + MIN_PACKET_SIZE] =  command_data.data[i];
   }
   
   if(debug){
      for(int i = 0; i < command_data.size; i++){
         cout << "For i = " << i << " "; SHOW(+data_bytes[i]);
      }
   }
}

void Servo::sendCommand(void){
   const unsigned char SIZE_BYTE = 2;
   (*uart_port_ptr).SendBytes(data_bytes, data_bytes[SIZE_BYTE]);
}

void Servo::genAndSendCommand(void){
   genCommand();
   sendCommand();
}

//=================================================================//
//=====================Servo class functions=======================//
//=================================================================//

void Servo::errorAck(void){
   if(debug) cout << "Error Ack will be performed" << endl;
   
   command_data.cmd = 3;
   command_data.size = 11;
   command_data.data[0] = 48;
   command_data.data[1] = 2;
   command_data.data[2] = 0;
   command_data.data[3] = 0;
   
   genAndSendCommand();
   
}


/////////////////////////////////////////////////////////////////////
//=================================================================//
//==============Servo:Led class functions - general================//
//=================================================================//

Servo::Led::Led(const Servo *servo_ptr)
{
   Led::servo_ptr = const_cast<Servo *>(servo_ptr);
}
Servo::Led::Led(void){
   //for declaration
}

//=================================================================//
//===================Servo:Led class functions=====================//
//=================================================================//
void Servo::Led::off(void){
   
   if(debug) cout << "Turning Led off" << endl;
   
   (*servo_ptr).command_data.cmd = 3;
   (*servo_ptr).command_data.size = 10;
   (*servo_ptr).command_data.data[0] = 53;//adres w pamieci RAM
   (*servo_ptr).command_data.data[1] = 1; //amount of bytes
   (*servo_ptr).command_data.data[2] = 0; //color: 0 - off, 1 - green 
   
   (*servo_ptr).genAndSendCommand();
}

void Servo::Led::greenOn(void){
   
   if(debug) cout << "Turning green Led on" << endl;
   
   (*servo_ptr).command_data.cmd = 3;
   (*servo_ptr).command_data.size = 10;
   (*servo_ptr).command_data.data[0] = 53;//RAM address
   (*servo_ptr).command_data.data[1] = 1; //amount of bytes
   (*servo_ptr).command_data.data[2] = 1; //color: 0 - off, 1 - green   
   
   (*servo_ptr).genAndSendCommand();
}

void Servo::Led::blueOn(void){
   
   if(debug) cout << "Turning blue Led on" << endl;
   
   (*servo_ptr).command_data.cmd = 3;
   (*servo_ptr).command_data.size = 10;
   (*servo_ptr).command_data.data[0] = 53;//RAM address
   (*servo_ptr).command_data.data[1] = 1; //amount of bytes
   (*servo_ptr).command_data.data[2] = 2; //color: 0 - off, 1 - green   
   
   (*servo_ptr).genAndSendCommand();
}