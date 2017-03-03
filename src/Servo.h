//Servo.h
#ifndef SERVO_H
#define SERVO_H

#define SHOW(obj) cout << #obj << " = " << (obj) << endl;

//extern bool debug;

class Servo{   //servo Herkulex DRS-(0602)
   private: //only static
      static const unsigned char MAX_PACKET_SIZE = 219;
      static const unsigned char MIN_PACKET_SIZE = 7;
      static const unsigned char PACKET_DATA_SIZE = MAX_PACKET_SIZE - MIN_PACKET_SIZE;
   
   public:
      struct CommandData{ //Structure with data being sent to servo
         unsigned char size;//Amount of bytes being sent
         unsigned char cmd;//Type of operation
         unsigned char data[PACKET_DATA_SIZE];//additional data
      };//to private??

      static bool debug;
      
      class Led{ //LED
            
         public:
            //vars:
            Servo *servo_ptr;
            //constructors:
            Led(void);//for declaration
            Led(const Servo *servo_ptr);//for definition
            
            //========RAM WRITE Led member functions:=========
            void off(void);//keep it void for user simplicity
            void greenOn(void);
            void blueOn(void);
            //void redOn(void);
            
            //RAM READ:
            //void checkState(void);
            
      };
      
      class UartComm{//virtual
         public:
            virtual bool SendBytes(unsigned char *data_array, unsigned char array_size)  = 0;
      };
      
      Led led; //Servo's object
      //OTHER:
      //id 0 - 254(255 affects all servos), array_size is maximally 219:
      Servo(unsigned char id, UartComm *uart_port_ptr);//Servo's constructor
      unsigned char checksum(int no);//generowanie checksum
      void genCommand(void);//generate command from struct to array ready to be sent
      void sendCommand(void);
      void genAndSendCommand(void);
      
      //==============RAM WRITE Servo member functions:=============
      void errorAck(void);
      
      //int getId(void);
      //void changeId(unsigned char id) {::id = id};
      //may become private, public for debug:

   private: //rest
      unsigned char id;//Servo's unique ID(pID)
      struct CommandData command_data;//data written by member functions
      //currently processed command ready to be send by uart:
      unsigned char data_bytes[MAX_PACKET_SIZE];
      //pointer to function communicating class with real servo:
      UartComm *uart_port_ptr;
};

#endif //SERVO_H