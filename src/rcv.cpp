#include <iostream> //cout
#include <stdio.h> //printf
#include <cstdlib> //atoi
#include <unistd.h> //Used for UART, sleep
#include <fcntl.h>  //Used for UART
#include <termios.h>//Used for UART

#include <wiringSerial.h>

#define SHOW(obj) cout << #obj << " = " << (obj) << endl;//nice function for debug

using namespace std;

int SetUpUART(/*string Porte.g."/dev/ttyAMA0", "/dev/ttyUSB0"*/){
   //-------------------------
	//----- SETUP USART 0 -----
	//-------------------------
	//At bootup, pins 8 and 10 are already set to UART0_TXD, UART0_RXD (ie the alt0 function) respectively
	int uart0_filestream = -1;
	
	//OPEN THE UART
	//The flags (defined in fcntl.h):
	//	Access modes (use 1 of these):
	//		O_RDONLY - Open for reading only.
	//		O_RDWR - Open for reading and writing.
	//		O_WRONLY - Open for writing only.
	//
	//	O_NDELAY / O_NONBLOCK (same function) - Enables nonblocking mode. When set read requests on the file can return immediately with a failure status
	//											if there is no input immediately available (instead of blocking). Likewise, write requests can also return
	//											immediately with a failure status if the output can't be written immediately.
	//
	//	O_NOCTTY - When set and path identifies a terminal device, open() shall not cause the terminal device to become the controlling terminal for the process.
	uart0_filestream = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode
	if (uart0_filestream == -1)
	{
		//ERROR - CAN'T OPEN SERIAL PORT
		printf("Error - Unable to open UART.  Ensure it is not in use by another application\n");
      return -1;
	}
	
	//CONFIGURE THE UART
	//The flags (defined in /usr/include/termios.h - see http://pubs.opengroup.org/onlinepubs/007908799/xsh/termios.h.html):
	//	Baud rate:- B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200, B230400, B460800, B500000, B576000, B921600, B1000000, B1152000, B1500000, B2000000, B2500000, B3000000, B3500000, B4000000
	//	CSIZE:- CS5, CS6, CS7, CS8
	//	CLOCAL - Ignore modem status lines
	//	CREAD - Enable receiver
	//	IGNPAR = Ignore characters with parity errors
	//	ICRNL - Map CR to NL on input (Use for ASCII comms where you want to auto correct end of line characters - don't use for bianry comms!)
	//	PARENB - Parity enable
	//	PARODD - Odd parity (else even)
	struct termios options;
	tcgetattr(uart0_filestream, &options);
	options.c_cflag = B1152000 | CS8 | CLOCAL | CREAD;		//<Set baud rate
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(uart0_filestream, TCIFLUSH);
	tcsetattr(uart0_filestream, TCSANOW, &options);
   
   return uart0_filestream;
}

int ReceiveBytes(int uart0_filestream){
   //----- CHECK FOR ANY RX BYTES -----
	if (uart0_filestream != -1)
	{
		// Read up to 255 characters from the port if they are there
		unsigned char rx_buffer[256];
		int rx_length = read(uart0_filestream, (void*)rx_buffer, 255);		//Filestream, buffer to store in, number of bytes to read (max)
		if (rx_length < 0)
		{
			//An error occured (will occur if there are no bytes)
         //cout << "e" << endl;
		}
		else if (rx_length == 0) 
		{
			//No data waiting
         cout << "n" << endl;
		}
		else
		{
			//Bytes received
			//rx_buffer[rx_length] = '\0';
			//printf("%i bytes read : %d\n", rx_length, rx_buffer);
         for(int i = 0; i < rx_length; i++)
            SHOW(+rx_buffer[i]);
		}
	}
}

int CloseUART(int uart0_filestream){
   //----- CLOSE THE UART -----
	close(uart0_filestream);
   return 0;
}
/*This command will set read and write access permissions for all users on the UART – it shouldn't be needed but can be used just to be sure there is not a permissions problem:
sudo chmod a+rw /dev/ttyAMA0*/
//http://www.tldp.org/HOWTO/Serial-Programming-HOWTO/x115.html

int main (int argc, char *argv[]){ 
   /*long int i = 0;
   cout << "Setting up uart" << endl;
   int uart0_filestream = SetUpUART();
   SHOW(uart0_filestream);
   //SHOW(sizeof(i));
        
   while(i<10000000){
      i++;
      if(i%100000 == 0) SHOW(i);
      ReceiveBytes(uart0_filestream);
   }
   
   CloseUART(uart0_filestream);*/
   int data[20];
   int handle = serialOpen ("/dev/ttyAMA0", 115200) ;
   unsigned char c = 219;
   
   
   
   for(int i = 0; i < 15; i++){
      sleep(1);
      serialPutchar (handle, c) ;
      //data[i] = serialGetchar (handle);
      //cout << "For i = " << i << " "; 
      
      SHOW(data[i]);
   }
   
   serialClose (handle) ;
   
   return 0;
}