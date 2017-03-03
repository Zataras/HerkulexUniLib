#include <iostream>
#include <string>     // string, stoi
#include <iomanip>
#include <fstream> // pliki
#include <windows.h> // system start
#include <sstream>
#include <stdlib.h>     //for using the function sleep

using namespace std;

//==============DEKLARACJE FUNKCJI============
bool generowanie(void);//menu
int checksum(int no);//generowanie checksum
string gen_kom(void);//generowanie komendy
bool menu3(void);//menu dla typu 3. CMD
bool menu4(void);//menu dla typu 4. CMD
bool menu5(void);//menu dla typu 5. CMD
bool menu6(void);//menu dla typu 6. CMD
bool menu7(void);//menu dla typu 7. CMD
void wyslij_komende(bool q_uit);
int ID_plik(bool typOP/*0 - odczyt, 1 - zapis*/, string nazwaPliku, int ID = 0);
bool moveSpeedOne(int Goal, int pTime, int iLed);
bool moveOne(int Goal, int pTime, int iLed);
bool changeID(int newID);

//----------------DANE SERWA------------------
const int header = 0xFF;
int pID = 0xFE;
int size = 0;//min 7
int CMD = 10; 
int data[216] = {};
int checkSum1 = checksum(1); 
int checkSum2 = checksum(2);
//--------------------------------------------

int main(){

 cout << "\n\t\t\t ==WITAJ== \n"
  "Program generuje skrypt do programu Terminal dla serwa " 
  "Herkulex.\n\n";
  
 bool koniec = 0;
 char wybor = '0';
 string tempID;
 int itempID;
 
 {int temp = ID_plik(0, "ID.txt");
 if(temp != -1) pID = temp;}
 
 while(!koniec){  

    cout << "\n\n\t\t\t--Jestes w MENU glownym--\n"
    "Co chcesz zrobic? Opcje:\n\n"
    "\t 1: Chce wygenerowac komende\n"
    "\t 2: Informacje o programie\n"
    "\t 3: Podac/sprawdzic ID serwa(domyslnie wspolne)\n"
    "\t 4: Potwierdzic blad serwa\n"
    "\t 5: Wygenerowac sume kontrolna\n"
    "\t q: Aby zakonczyc\n";
    cin >> wybor;
  switch(wybor){
   case '1':
      koniec = generowanie();
   break;
   case '2':
      cout << "\n\t:Autor: Michal Tatara\n";
   break;
   case '3':
      cout << "\n\t:Akualne ID = "
      << pID
      <<"\n\t:Podaj nowe ID(HEX, anuluj[can]): ";
      cin >> tempID;
      if(tempID.compare("can") != 0 && tempID.compare("g") < 0){
         pID = stoi (tempID,nullptr,16);
         ID_plik(1, "ID.txt", pID);
      }
      else{
         cout << "anulowane lub bledny format liczby";
      }
      cout << "\n\t:Akualne ID = " << pID;
   break;
   case '4':
      CMD = 3;
      size = 11;
      data[0] = 48;
      data[1] = 2;
      data[2] = 0;
      data[3] = 0;
      checkSum1 = checksum(1);
      checkSum2 = checksum(2);     
      wyslij_komende(1);
   break;
   case '5':
      cout << "\n\t:Wprowadz decymalnie: \n\t\t:CMD = ";
      cin >> CMD;
      cout << "\n\t\t:size = ";
      cin >> size;
      cout << "\n\t\t:pID = ";
      itempID = pID;
      cin >> pID;
      for(int i = 0;i < size - 7; i++){
         cout << "\n\t\t:data["<< i <<"] = ";
         cin >> data[i];
      }
      cout << "\n\t:CHKS1 = " << checksum(1);
      cout << "\n\t:CHKS2 = " << checksum(2);   
      pID = itempID;      
   break;
   case 'q':
    cout << "\n\t:Zegnaj\n";
    koniec = 1;
    break;
    default:
     cout << "\n\t:Niewlasciwa komenda\n";
  }
 }
 //Przed zakończeniem programu:
 cout << "\n\n\tDo nastepnego razu!";
 Sleep( 800 );
 return 0;
}

//=====================DEFINICJE FUNKCJI=====================

//WriteToFile('input.txt', 'test2'); 

bool generowanie(void){
 bool koniec = 0;
 char wybor = '0';
 ofstream myfile;
 string str; 
 
 while(!koniec){
   cout << "\n\n\t\t\t__MENU generowania__\n"
      "Jaka czynnosc chcesz zadac? Opcje:\n\n"
      "\t 1: Nieaktywne\n"
      "\t 2: Nieaktywne\n"
      "\t 3: RAM_WRITE(e.g. LEDs)\n"
      "\t 4: RAM_READ(e.g. Temperature)\n"
      "\t 5: I_JOG\n"
      "\t 6: S_JOG\n"
      "\t 7: STAT\n"
      "\t b: Aby powrocic\n"
      "\t q: Aby zakonczyc\n";
   cin >> wybor;
   
   switch(wybor){
      case '1':
         cout << "\n\tNieaktywne\n";
         break;
      case '3':
         koniec = menu3();
         break;
      case '4':
         koniec = menu4();
         break;
      case '5':
         koniec = menu5();
         break;
      case '6':
         koniec = menu6();
         break;
      case '7':
         koniec = menu7();
         break;
      case 'b':
         koniec = 1;
         cout << "\n\t:powrot\n";
         return 0;
         break;
      case 'q':
         koniec = 1;
         return 1;
         break;
      
      default:
         cout << "\n\t:Niewlasciwa komenda\n";
  }
 }
}


bool menu3(void){
   bool koniec = 0;
   char wybor = '0';
   
   while(!koniec){
   
      cout << "\n\n\t\t\t__MENU RAM_WRITE__"
         "\n Jaka czynnosc chcesz zadac?"
         "Mozesz wygenerowac polecenia, ktore spowoduja:\n"
         "\n\t 1: Zapalenie zielonej diody"
         "\n\t 2: Zgaszenie zielonej diody"
         "\n\t 3: Zalaczenie momentu"
         "\n\t 4: Zapalenie niebieskiej diody"
         "\n\t 5: Wylaczenie momentu"
         "\n\t 6: Zmiana ID"
         "\n\t 7: Ustaw punkt 0"
         "\n\t c: Sprawdz wygenerowane polecenie"
         "\n\t e: Wyslanie polecenia do serwa"
         "\n\t b: Aby powrocic"
         "\n\t q: Aby zakonczyc\n";
      cin >> wybor;
      
      switch(wybor){
         case '1':
            CMD = 3;
            size = 10;
            data[0] = 53;//adres w pamieci RAM
            data[1] = 1;//ilosc bajtow
            data[2] = 1;//kolor; 0 - off, 1 - zielony
            checkSum1 = checksum(1);
            checkSum2 = checksum(2); 
            wyslij_komende(1);
            break;
         case '2':
            CMD = 3;
            size = 10;
            data[0] = 53;//adres w pamieci RAM
            data[1] = 1;//ilosc bajtow
            data[2] = 0;//kolor; 0 - off, 1 - zielony
            checkSum1 = checksum(1);
            checkSum2 = checksum(2);     
            wyslij_komende(1);
            break;
         case '3':
            //cout << "/n/tNieaktywne";   
            CMD = 3;
            size = 10;
            data[0] = 0x34;//adres w pamieci RAM
            data[1] = 1;//ilosc bajtow wystepujacych od data[2]
            data[2] = 0x60;//zalaczenie momentu
            checkSum1 = checksum(1);
            checkSum2 = checksum(2);     
            wyslij_komende(1);
            break;
         case '4':
            //cout << "/n/tNieaktywne"; 
            CMD = 3;
            size = 10;
            data[0] = 53;//adres w pamieci RAM
            data[1] = 1;//ilosc bajtow
            data[2] = 2;//kolor; 0 - off, 1 - zielony
            checkSum1 = checksum(1);
            checkSum2 = checksum(2); 
            wyslij_komende(1);
            break;
         case '5':
            //cout << "/n/tNieaktywne";   
            CMD = 3;
            size = 10;
            data[0] = 0x34;//adres w pamieci RAM
            data[1] = 1;//ilosc bajtow wystepujacych od data[2]
            data[2] = 0x00;//zalaczenie momentu
            checkSum1 = checksum(1);
            checkSum2 = checksum(2);     
            wyslij_komende(1);
         break;
         case '6':   
            cout << "Podaj nowe ID[dec]: ";
            int newID;
            cin >> newID;
            changeID(newID);
            wyslij_komende(1);
            pID = newID;
            ID_plik(1, "ID.txt", pID);
         break;
         case '7':
            //cout << "/n/tNieaktywne";   
            CMD = 3;
            size = 10;
            data[0] = 50;//adres w pamieci RAM
            data[1] = 1;//ilosc bajtow wystepujacych od data[2]
            data[2] = 0x04;//zalaczenie momentu
            checkSum1 = checksum(1);
            checkSum2 = checksum(2);     
            wyslij_komende(1);
         break;
         //---------pozostale---------- 
         case 'c':
            //str = gen_kom();
            cout << "\n\tKomenda : " + gen_kom();
            break;
         case 'e':
            wyslij_komende(1);
            cout << "/n/tWyslano polecenie";
            break;
         case 'b':
            koniec = 1;
            cout << "\n\t:powrot\n";
            return 0;
            break;
         case 'q':
            koniec = 1;
            return 1;
            break;
          default:
           cout << "\n\t:Niewlasciwa komenda\n";
      }
   }
}

bool menu4(void){
   bool koniec = 0;
   char wybor = '0';
   
   while(!koniec){
      cout << "\n\n\t\t\t__MENU RAM_READ__"
         "\n Jaka czynnosc chcesz zadac?"
         "Mozesz wygenerowac polecenia, ktore spowoduja:\n"
         "\n\t 1: Odczyt pID"
         "\n\t 2: Odczyt temperatury"
         "\n\t 3: Sprawdzenie stanu diody"
         "\n\t 4: Odczyt napiecia"
         "\n\t 5: Odczyt rejestru Torque"
         "\n\t 6: Odczyt rejestru Absolute position"
         "\n\t 7: Odczyt rejestru Calibrated position"
         "\n\t c: Sprawdz wygenerowane polecenie"
         "\n\t e: Wyslanie polecenia do serwa"
         "\n\t b: Aby powrocic"
         "\n\t q: Aby zakonczyc\n";
      cin >> wybor;
      
      switch(wybor){
         case '1':
            CMD = 4;
            size = 9;
            data[0] = 0;//adres w pamieci RAM
            data[1] = 1;//ilosc bajtow
            checkSum1 = checksum(1);
            checkSum2 = checksum(2);    
            wyslij_komende(0);            
            break;
         case '2':
            CMD = 4;
            size = 9;
            data[0] = 55;//adres w pamieci RAM
            data[1] = 1;//ilosc bajtow
            checkSum1 = checksum(1);
            checkSum2 = checksum(2); 
            wyslij_komende(0);
         break;
         case '3':
            //cout << "/n/tNieaktywne";  
            CMD = 4;
            size = 9;
            data[0] = 0x35;//adres w pamieci RAM
            data[1] = 1;//ilosc bajtow
            checkSum1 = checksum(1);
            checkSum2 = checksum(2); 
            wyslij_komende(0);
            break;
         case '4':
            CMD = 4;
            size = 9;
            data[0] = 54;//adres w pamieci RAM
            data[1] = 1;//ilosc bajtow
            checkSum1 = checksum(1);
            checkSum2 = checksum(2); 
            wyslij_komende(0);   
         break;
         case '5':
            CMD = 4;
            size = 9;
            data[0] = 52;//adres w pamieci RAM
            data[1] = 1;//ilosc bajtow
            checkSum1 = checksum(1);
            checkSum2 = checksum(2); 
            wyslij_komende(0);   
         break;
         case '6':
            CMD = 4;
            size = 9;
            data[0] = 60;//adres w pamieci RAM
            data[1] = 2;//ilosc bajtow
            checkSum1 = checksum(1);
            checkSum2 = checksum(2); 
            wyslij_komende(0);
         break;
         case '7':
            CMD = 4;
            size = 9;
            data[0] = 58;//adres w pamieci RAM
            data[1] = 2;//ilosc bajtow
            checkSum1 = checksum(1);
            checkSum2 = checksum(2); 
            wyslij_komende(0);
         break;
         //---------pozostale---------- 
         case 'c':
            //str = gen_kom();
            cout << "\n\tKomenda : " + gen_kom();
            break;
         case 'e':
            wyslij_komende(0);
            cout << "/n/tWyslano polecenie";
            break;
         case 'b':
            koniec = 1;
            cout << "\n\t:powrot\n";
            return 0;
            break;
         case 'q':
            koniec = 1;
            return 1;
            break;
          default:
           cout << "\n\t:Niewlasciwa komenda\n";
      }
   }
}

bool menu5(void){
   bool koniec = 0;
   char wybor = '0';
   
   while(!koniec){
         cout << "\n\n\t\t\t__MENU I_JOG__"
         "\n Jaka czynnosc chcesz zadac?"
         "Mozesz wygenerowac polecenia, ktore spowoduja:\n"
         "\n\t 1: Nieskonczony ruch z predkoscia 320, \n\tniebieska dioda i okreslonym czasem"
         "\n\t 3: Kontrola pozycji, poz. zadana: 512, zapalona zielona dioda"
         "\n\t 3: Nieaktywne"
         "\n\t 4: Nieaktywne"
         "\n\t c: Sprawdz wygenerowane polecenie"
         "\n\t e: Wyslanie polecenia do serwa"
         "\n\t b: Aby powrocic"
         "\n\t q: Aby zakonczyc\n";
      cin >> wybor;
      
      switch(wybor){
         case '1':
            //data[] = {};
            CMD = 5;
            size = 12;
            data[0] = 0;
            data[1] = 10;
            data[2] = 0x0A;
            data[3] = pID;
            data[4] = 0x3C;
            checkSum1 = checksum(1);
            checkSum2 = checksum(2);  
            wyslij_komende(0);            
            break;
         case '2':
            CMD = 5;
            size = 12;
            data[0] = 0;
            data[1] = 2;
            data[2] = 4;
            data[3] = pID;
            data[4] = 0x3C;
            checkSum1 = checksum(1);
            checkSum2 = checksum(2);  
            wyslij_komende(0);      
            break;
         case '3':
            cout << "/n/tNieaktywne";    
            break;
         case '4':
            cout << "/n/tNieaktywne";    
            break;
         //---------pozostale---------- 
         case 'c':
            //str = gen_kom();
            cout << "\n\tKomenda : " + gen_kom();
            break;
         case 'e':
            wyslij_komende(0);
            cout << "/n/tWyslano polecenie";
            break;
         case 'b':
            koniec = 1;
            cout << "\n\t:powrot\n";
            return 0;
            break;
         case 'q':
            koniec = 1;
            return 1;
            break;
          default:
           cout << "\n\t:Niewlasciwa komenda\n";
      }
   }
}

bool menu6(void){
   bool koniec = 0;
   char wybor = '0';
   
   while(!koniec){
         cout << "\n\n\t\t\t__MENU S_JOG__"
         "\n Jaka czynnosc chcesz zadac?"
         "Mozesz wygenerowac polecenia, ktore spowoduja:\n"
         "\n\t 1: Ruch do pozycji"
         "\n\t 2: Ruch z zadana predkoscia"
         "\n\t 3: Nieaktywne"
         "\n\t c: Sprawdz wygenerowane polecenie"
         "\n\t e: Wyslanie polecenia do serwa"
         "\n\t b: Aby powrocic"
         "\n\t q: Aby zakonczyc\n";
      cin >> wybor;
      
      switch(wybor){
         case '1':
            cout << "\nPodaj zadana pozycje(0 - 1023): ";
            int poz;
            cin >> poz;
            cout << "\nPodaj zadany czas ruchu w [ms](0 - 1023): ";
            int czas;
            cin >> czas;
            cout << "\nZapali sie czerwona dioda";
            moveOne(poz, czas, 3); 
            wyslij_komende(0);   
         break;
         /*case '3':
            CMD = 6;
            size = 12;
            data[0] = 60;
            data[1] = 40;
            data[2] = 1;
            data[3] = 0x0A;
            data[3] = 0x0A;
            checkSum1 = checksum(1);
            checkSum2 = checksum(2);  
            wyslij_komende(0);     
            break;*/
         case '2':   
            cout << "\nPodaj zadana predkosc(0 - 1023): ";
            int predkosc;
            cin >> predkosc;
            cout << "\nPodaj zadany czas ruchu w [ms](0 - 1023): ";
            //int czas;
            cin >> czas;
            cout << "\nZapali sie niebieska dioda";
            moveSpeedOne(predkosc, czas, 2);
            wyslij_komende(0);  
         break;
         //---------pozostale---------- 
         case 'c':
            //str = gen_kom();
            cout << "\n\tKomenda : " + gen_kom();
            break;
         case 'e':
            wyslij_komende(0);
            cout << "/n/tWyslano polecenie";
            break;
         case 'b':
            koniec = 1;
            cout << "\n\t:powrot\n";
            return 0;
            break;
         case 'q':
            koniec = 1;
            return 1;
            break;
          default:
           cout << "\n\t:Niewlasciwa komenda\n";
      }
   }
}

bool menu7(void){
   bool koniec = 0;
   char wybor = '0';
   
   while(!koniec){
         cout << "\n\n\t\t\t__MENU STAT__"
         "\n Jaka czynnosc chcesz zadac?"
         "Mozesz wygenerowac polecenia, ktore spowoduja:\n"
         "\n\t 1: Odczyt Statusu"
         "\n\t 2: Nieaktywne"
         "\n\t c: Sprawdz wygenerowane polecenie"
         "\n\t e: Wyslanie polecenia do serwa"
         "\n\t b: Aby powrocic"
         "\n\t q: Aby zakonczyc\n";
      cin >> wybor;
      
      switch(wybor){
         case '1':
            //data[] = {};
            CMD = 7;
            size = 7;
            data[0] = 0;
            data[1] = 0x40;
            checkSum1 = checksum(1);
            checkSum2 = checksum(2);  
            wyslij_komende(0);            
            break;
         case '2':
            cout << "/n/tNieaktywne";      
            break;
         //---------pozostale---------- 
         case 'c':
            //str = gen_kom();
            cout << "\n\tKomenda : " + gen_kom();
            break;
         case 'e':
            wyslij_komende(0);
            cout << "/n/tWyslano polecenie";
            break;
         case 'b':
            koniec = 1;
            cout << "\n\t:powrot\n";
            return 0;
            break;
         case 'q':
            koniec = 1;
            return 1;
            break;
          default:
           cout << "\n\t:Niewlasciwa komenda\n";
      }
   }
}

string gen_kom(void){
   stringstream komenda;
   //cout << "przed";
   //system("pause");
   int checkSum1 = checksum(1); 
   int checkSum2 = checksum(2);
   
   komenda << uppercase << setfill ('0') << hex 
     <<"$" << setw(2) << header
     <<"$" << setw(2) << header
     <<"$" << setw(2) << size
     <<"$" << setw(2) << pID
     <<"$" << setw(2) << CMD
     <<"$" << setw(2) << checkSum1
     <<"$" << setw(2) << checkSum2;
   for(int i = 0; i < (size - 7); i++){
     komenda << uppercase << setfill ('0') 
     << "$" << setw(2) << hex <<data[i];
   }
   
   return komenda.str();
}

void wyslij_komende(bool q_uit){
   ofstream skrypt;
   string str;

   skrypt.open ("skrypt.tsc");
   str = gen_kom();
   skrypt << "program skrypt;\n" 
      "\tconst STRING1 = '"
      + str +
      "'; \n\tbegin"
      "\n\t\tcomsendmacro(STRING1);";
      if(q_uit == 1)
         skrypt << "\n\t Quit();";
      skrypt << "\n\tend;\nend.";
   skrypt.close();
   system ("start terminal.exe skrypt.tsc");
}

int ID_plik(bool typOP/*0 - odczyt, 1 - zapis*/, string nazwaPliku, int ID){
   fstream plik;
   plik.open(nazwaPliku.c_str(), std::ios::in | std::ios::out );
   if( plik.good() == true ){
      //tu operacje na pliku (zapis/odczyt)
      if(typOP){//zapis
         plik << ID;
         plik.close();
         return 1;
      }
      else{//odczyt
         int oID;
         plik >> oID;
         plik.close();
         return oID;
      }
        
   }
   else return -1;
}

// move one servo with continous rotation
bool moveSpeedOne(int Goal, int pTime, int iLed)
{
  if (Goal > 1023 || Goal < -1023) return 0;              // speed (goal) non correct
  if ((pTime <0) || (pTime > 2856)) return 0;

  int GoalSpeedSign;
  if (Goal < 0) {
    GoalSpeedSign = (-1)* Goal ;
    GoalSpeedSign |= 0x4000;  //bit n°14 
  } 
  else {
    GoalSpeedSign = Goal;
  }
  int speedGoalLSB=GoalSpeedSign & 0X00FF; 		       // MSB speedGoal 
  int speedGoalMSB=(GoalSpeedSign & 0xFF00) >> 8;      // LSB speedGoal 

  bool stop = 0;
  if (Goal == 0) stop = 1;
  
  //led 
  int iBlue=0;
  int iGreen=0;
  int iRed=0;
  switch (iLed) {
  case 1:
    iGreen=1;
    break;
  case 2:
    iBlue=1;
    break;
  case 3:
    iRed=1;
    break;
  }
  int SetValue = stop + 2 +iGreen*4+iBlue*8+iRed*16;		//assign led value 

  int playTime=int((float)pTime/11.2);				// 8. Execution time

  size = 0x0C;              					// 3.Packet size 7-58
  CMD   = 6;      					        // 5. CMD
  data[0] = playTime;  		// Execution time	
  data[1]=speedGoalLSB;            			    // 8. speedLSB
  data[2]=speedGoalMSB;              			// 9. speedMSB
  data[3]=SetValue;                          	// 10. Mode=0;
  data[4]=pID;                    			// 11. ServoID
  
  //pID = pID^playTime;
  checkSum1 = checksum(1);
  checkSum2 = checksum(2);  
  pID = data[4];
}

// move one servo at goal position 0 - 1024
bool moveOne(int Goal, int pTime, int iLed)
{
  if (Goal > 1023 || Goal < 0) return 0;              // speed (goal) non correct
  if ((pTime <0) || (pTime > 2856)) return 0;

  int posLSB = Goal & 0X00FF; 		       // MSB speedGoal 
  int posMSB = (Goal & 0xFF00) >> 8;      // LSB Goal 

  //led 
  int iBlue=0;
  int iGreen=0;
  int iRed=0;
  switch (iLed) {
  case 1:
    iGreen=1;
    break;
  case 2:
    iBlue=1;
    break;
  case 3:
    iRed=1;
    break;
  }
  int SetValue=iGreen*4+iBlue*8+iRed*16;		//assign led value 

  int playTime=int((float)pTime/11.2);				// 8. Execution time

  size = 0x0C;              					// 3.Packet size 7-58
  CMD   = 6;      					        // 5. CMD
  data[0] = playTime;  		// Execution time	
  data[1]=posLSB;            			    // 8. speedLSB
  data[2]=posMSB;              			// 9. speedMSB
  data[3]=SetValue;                          	// 10. Mode=0;
  data[4]=pID;     
   
  //pID = pID^playTime;
  cout << "pid = " << pID;
  // 11. ServoID
  checkSum1 = checksum(1);
  checkSum2 = checksum(2); 
  pID = data[4];
}

int checksum(int no){
   int sum;
   if((no == 1) || (no == 2))
      sum = size^pID^CMD;
   else 
      return -1;
   int dataAm = size - 7;
   //if(CMD == 5 || CMD == 6)
   //   dataAm = size - 8;
   // xor data:
   for(int i = 0; i < dataAm; i++){
      sum = sum^data[i];
   }
   if(no == 2){
      sum = ~sum;
   }
   sum = sum & 0xFE;
   return sum;
}

bool changeID(int newID){
   CMD = 3;
   size = 10;
   data[0] = 0;//adres w pamieci RAM
   data[1] = 1;//ilosc bajtow
   data[2] = newID;
   checkSum1 = checksum(1);
   checkSum2 = checksum(2);    
}