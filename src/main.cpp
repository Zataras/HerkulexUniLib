//main.c

/////////////////////////////////////////////////////////////////////
//================Program handling functions of====================//
//===================servo Herkulex DRS-0602=======================//
//===================by Michał Tatara 2016=========================//
/////////////////////////////////////////////////////////////////////

#include <iostream> //cout
#include <cstdlib> //atoi
#include <string> //string, stoi
#include "stdint.h" // int8_t

#define SHOW(obj) cout << #obj << " = " << (obj) << endl;//nice function for debug

#include <boost/program_options.hpp> //input flags handle
#include <iterator> //?

//#include "servo_comm.h" //uart communication stuff
#include "Servo.h"      //Servo DRS class/library
#include "UartRpi.h" //uart communication stuff

//bool debug = 0; //1 for displaying values of variables and other

struct RetCheckArgs{
   int status;
   unsigned char id;
   bool debug;
};

int checkArgs(int argc, char *argv[]/*, int mode, Servo *servo = 0*/);

using namespace std;

bool UartRpi::debug = 0;
bool Servo::debug = 0;

int main (int argc, char *argv[]){
   
   /*struct RetCheckArgs ret_struct;
   
   const int MODE_INIT_DEBUG_HELP_ID_PORT = 1;
   const int MODE_REST = 2;
   const int HELP_CALLED = 256;
   const int RET_STS_CONTINUE = 0;*/
   
   /*ret_struct = */checkArgs(argc, argv);//, MODE_INIT_DEBUG_HELP_ID_PORT);
   
   /*if(ret_struct.status == RET_STS_CONTINUE){
      UartRpi::debug = ret_struct.debug;
      Servo::debug = ret_struct.debug;
      
      UartRpi uart_port("/dev/ttyUSB0");
      Servo servo(ret_struct.id, &uart_port);//create servo object 
      
   
      ret_struct = checkArgs(argc, argv, MODE_REST, &servo);
      return ret_struct.status;
   }*/
   //else return 1;
   return 0;
}

int checkArgs(int argc, char *argv[]/*, int mode, Servo *servo*/){
   
   //new
   const char RET_HELP = 1;
   const char RET_OK = 0;
   const char RET_ERR_EXCEPTION_DEF = -1;
   const char RET_ERR_EXCEPTION_UNDEF = -2;
   const char RET_ERR_WRONG_INPUT = -3;
   const char RET_ERR_NO_ACTION = -4;
   
   const unsigned char DEFAULT_ID = 219;
   const unsigned char ID_MIN = 0;
   const unsigned char ID_MAX = 255;
   
   //old
   const int CONTINUE = 0;
   const int MODE_INIT_DEBUG_HELP_ID_PORT = 1;
   
   //struct RetCheckArgs ret_struct = {ACTION_NOT_DEFINED, DEFAULT_ID, 0};
   int ret_val = RET_ERR_NO_ACTION;
   int id = DEFAULT_ID;
   
   try {
      // Declare the supported options.
      namespace po = boost::program_options;
      
      static po::options_description desc("Allowed options");
      static po::variables_map vm;
      
      //if(mode == MODE_INIT_DEBUG_HELP_ID_PORT){
      
         
      desc.add_options()
          ("port,p", po::value<string>(), "specify serial port name to communicate with servo [/dev/ttyUSB0]")
          ("id", po::value<int8_t>(), "set servo ID <0 - 254> [219]")
          ("errAck,a", "acknowledge error on servo")
          ("led,l", po::value<string>(), "command for LED, options:\noff(o) \ngreen(g) \nblue(b)")
          ("debug,d", "Enable additional information during program processing")
          ("help,h", "produce help message and exit");

      po::store(po::parse_command_line(argc, argv, desc), vm);
      po::notify(vm); 
      
      if (vm.count("help")) {
         cout << desc << "\n";
         return RET_HELP;
      }
      
      if (vm.count("debug")){
         UartRpi::debug = true;
         Servo::debug = true;
      }
      
      if (vm.count("id")){
         int8_t temp_id = vm["id"].as<int8_t>();
         if(temp_id <= ID_MAX && temp_id >= ID_MIN)
            id = temp_id;
         else{
            cout << "specified ID not in valid range, check '-h'" << endl;
            return RET_ERR_WRONG_INPUT;
         }
         cout << "Servo's ID will be set to " << id << ".\n";
         //return CONTINUE;
      }
      
      //===============================================
      //========Options requiring servo object=========
      //===============================================
      if (vm.count("errAck")){
         ret_val = RET_OK;
      }
      
      if (vm.count("led")){
         string input = vm["led"].as<string>();
         if(input == "off"  || input == "green")
            ret_val = RET_OK;
         if(input == "blue" || input == "g")
            ret_val = RET_OK;
         if(input == "b" || input == "o")
            ret_val = RET_OK;
         if(ret_val != RET_OK){
            cout << "Command for LED mistaken, use --help.\n";
            return RET_ERR_WRONG_INPUT;
         }
      }
      
      if(ret_val == RET_ERR_NO_ACTION){
         cout << "There was no action defined for servo, use '-h' option for help" << endl;
         return RET_ERR_NO_ACTION;
      }
      
      //servo object and port create if any action specified:
      UartRpi uart_port("/dev/ttyUSB0");
      Servo servo(id, &uart_port);//create servo object 
      
      if (vm.count("errAck")){
         servo.errAck;
      }
      
      if (vm.count("led")) {
         //cout << "Led action will be performed";
         string input = vm["led"].as<string>();
         if     (input == "off" || input == "o")
            (servo).led.off();
         else if(input == "green" || input == "g")
            (servo).led.greenOn();
         else if(input == "blue" || input == "b")
            (servo).led.blueOn();
      }
   }
   catch(exception& e) {
      cerr << "error: " << e.what() << "\n" << "For help use '-h'.\n";
      return RET_ERR_EXCEPTION_DEF;
   }
   catch(...) {
      cerr << "Exception of unknown type!\n";
      return RET_ERR_EXCEPTION_UNDEF;
   }
}