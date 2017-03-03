//Servo.h
#ifndef UARTRPI_H
#define UARTRPI_H

#define SHOW(obj) cout << #obj << " = " << (obj) << endl;

#include "Servo.h"

class UartRpi : public Servo::UartComm {   //UartRpi
   public:
      virtual bool SendBytes(unsigned char *data_array, unsigned char array_size);
      UartRpi(const char *port_name); //constructor; id e <0; 255>
      static bool debug;//set 1 to display additional information
      
   private:
      char *port_name;
};

#endif //UARTRPI_H