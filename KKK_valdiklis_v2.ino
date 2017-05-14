// Patikrinta su Arduino 1.6.12
// ============= KKK valdinlis v2.1 ===============================================

#include <DS1307RTC.h>
#include <Wire.h>
//#include <DS1307.h>
#include <Time.h>
#include <TimeAlarms.h>
#include "MenuBackend.h"        
 #include <LiquidCrystal.h>         
  #include <OneWire.h>
   #include <DallasTemperature.h>
   // Arduino EEPROMex library
   // https://github.com/PaulStoffregen/Arduino-Libraries/tree/master/EEPROMEx
   #include <EEPROMex.h>
//    #include <EEPROM.h>
//     #include <PID_v1.h>
#include "definitions.h"
#include <EtherCard.h>
#define STATIC 1  // set to 1 to disable DHCP (adjust myip/gwip values below)

#if STATIC
// ethernet interface ip address
static byte myip[] = { 192,168,1,3 };
// gateway ip address
static byte gwip[] = { 192,168,1,254 };
#endif

// ethernet mac address - must be unique on your network
static byte mymac[] = { 0x11,0x22,0x33,0x44,0x55,0x66 };

byte Ethernet::buffer[500]; // tcp/ip send and receive buffer
unsigned long Ethernet_timer;

  #define ONE_WIRE_BUS4 42
//Define Variables we'll be connecting to
double Setpoint, Input, Output;
//Specify the links and initial tuning parameters
//PID PV_PID(&Input, &Output, &Setpoint,6,15,3, DIRECT);
//int WindowSize = 10000;
//unsigned long windowStartTime;

  // --- definiujemy dla LCD własne znaki strzałek: dół, lewo, prawo, gora-dół i powrót ---
uint8_t arrowUpDown[8] = {0x4,0xe,0x15,0x4,0x15,0xe,0x4};
uint8_t arrowDown[8]  = {0x4,0x4,0x4,04,0x15,0xe,0x4};
uint8_t arrowRight[8] = {0x0,0x4,0x2,0x1f,0x2,0x4,0x0};
uint8_t arrowLeft[8] = {0x0,0x4,0x8,0x1f,0x8,0x4,0x0};
uint8_t arrowBack[8] = {0x1,0x1,0x5,0x9,0x1f,0x8,0x4};
uint8_t arrowUp[8]={ B00100,B01110,B11111,B00100,B00100,B00100,B00100,B00100};
    // definicja pinów dla LCD (sprawdź piny w swoim LCD)
LiquidCrystal lcd(26, 24, 22, 23, 25, 27, 29);
/* ------------------ R T C ---------------------- */


/* --------------------- RTC PABAIGA ---------------- */
char *eilute1;                      // pierwsza eilute wyświetlanego tekstu na LCD
char *eilute2;                      // druga eilute wyświetlanego tekstu na LCD
char *eilute3;
    boolean InMenu = false;


/* Przykładowe Menu (podobieństwo do IDE Arduino): --------------------------------------------
// mamy 5 głównych opcji PLIK, EDYCJA, SZKIC, NARZEDZIA, POMOC
// w opcji SZKIC i NARZEDZIA mamy rozbudowane opcje dla 2 i 3 poziomu Menu
// --------------------------------------------------------------------------------------------
      NUSTATYMAI
         Pasvietimas
         Irasymas
         Numatytos reiksmesatymas
      AKUMULIACINE
         Ijungimo temp.
         Isjungimo temp.
         Rankinis vald.
      BOILERIS
         Ijungimo temp.
         Isjungimo temp.         
      SAULES KOLEKTORIUS
         Ijungimo t.skirt
         Isjungimo t.skirt
         Apsauga nuo salc
         Nuorinimas
*/
// --- tworzymy wszystkie opcje Menu: ---------------------------------------
// de facto tworzymy obiekty klasy MenuItem, które dziedziczą po klasie MenuBackend
    void menuChangeEvent(MenuChangeEvent changed);
    void menuUseEvent(MenuUseEvent used);

MenuBackend menu = MenuBackend(menuUseEvent,menuChangeEvent); // konstruktor 
   //                        ("                ")
   MenuItem P1 =  MenuItem("NUSTATYMAI        ",1);
      MenuItem P11 = MenuItem("Irasymas",2);
      MenuItem P12 = MenuItem("Numatytos reiksmes",2);
      MenuItem P13 = MenuItem("Pasvietimas",2);
      MenuItem P14 = MenuItem("Metai",2);
      MenuItem P15 = MenuItem("Menuo",2);
      MenuItem P16 = MenuItem("Diena",2);
      MenuItem P17 = MenuItem("Valanda",2);
      MenuItem P18 = MenuItem("Minute",2);
   MenuItem P2 = MenuItem("AKUMULIACINE      ",1);
      MenuItem P21 = MenuItem("AT ijungimo temp.",2);
      MenuItem P22 = MenuItem("AT isjungimo temp.",2);
      MenuItem P23 = MenuItem("AT rankinis vald.",2);
   MenuItem P3 = MenuItem("SAULES KOLEKTORIUS",1);
      MenuItem P31 = MenuItem("Ijungimo t.skirtum",2);
      MenuItem P32 = MenuItem("Isjungimo t.skirtu",2);
      MenuItem P33 = MenuItem("Apsauga nuo salcio",2);
      MenuItem P34 = MenuItem("SK rankinis vald.",2);
   MenuItem P4 = MenuItem("BOILERIS          ",1);
      MenuItem P41 = MenuItem("B ijungimo temp.",2);
      MenuItem P42 = MenuItem("B isjungimo temp.",2);
      MenuItem P43 = MenuItem("B rankinis vald.",2);
      MenuItem P44 = MenuItem("B termostatas   ",2);
   MenuItem P5 = MenuItem("PAMAISYMO VOZTUVAS",1);
      MenuItem P51 = MenuItem("Darbo Rezimas     ",2);
      MenuItem P52 = MenuItem("PV palaikoma temp.",2);
      MenuItem P53 = MenuItem("PV ijungimo temp. ",2);
      MenuItem P54 = MenuItem("PV isjungimo temp.",2);
      MenuItem P55 = MenuItem("PV palaikoma riba ",2);
      MenuItem P56 = MenuItem("PV pauze          ",2);
      MenuItem P57 = MenuItem("PV veiksmas       ",2);


/* --- Teraz pozycjonujemy  menu ( zgodnie z ustawieniem podanym powyżej) ------------
add - dodaje w pionie, addRight - dodaje w poziomie z prawej , addLeft dodaje z lewej
*/
void menuSetup()                       // funkcja klasy MenuBackend 
{
      menu.getRoot().add(P4);          // 
      P1.add(P11);
        P11.add(P12);P11.addLeft(P1);  //  
        P12.add(P13);P12.addLeft(P1);  // 
        P13.add(P14);P13.addLeft(P1);
        P14.add(P15);P14.addLeft(P1);
        P15.add(P16);P15.addLeft(P1);
        P16.add(P17);P16.addLeft(P1);
        P17.add(P18);P17.addLeft(P1);
        P18.add(P11);P18.addLeft(P1);
      menu.getRoot().add(P2);
      P1.addRight(P2);                 //
      
      P2.add(P21);                     // 
        P21.add(P22);P21.addLeft(P2);  // 
        P22.add(P23);P22.addLeft(P2);  // 
        P23.add(P21);P23.addLeft(P2);
      menu.getRoot().add(P3);
      P2.addRight(P3);                 //
      
      P3.add(P31);                     // 
        P31.add(P32);P31.addLeft(P3);  //  
        P32.add(P33);P32.addLeft(P3);
        P33.add(P34);P33.addLeft(P3);
        P34.add(P31);P34.addLeft(P3);    //        
      menu.getRoot().add(P5);
      P3.addRight(P4);
      
      P4.add(P41);
        P41.add(P42);P41.addLeft(P4);
        P42.add(P43);P42.addLeft(P4);
        P43.add(P44);P43.addLeft(P4);
        P44.add(P41);P44.addLeft(P4);
      menu.getRoot().add(P1);
      P4.addRight(P5);
      
      P5.add(P51);
        P51.add(P52);P51.addLeft(P5);
        P52.add(P53);P52.addLeft(P5);
        P53.add(P54);P53.addLeft(P5);
        P54.add(P55);P54.addLeft(P5);
        P55.add(P56);P55.addLeft(P5);
        P56.add(P57);P56.addLeft(P5);
        P57.add(P51);P57.addLeft(P5);
      menu.getRoot().add(P5);
      P5.addRight(P1);
}
// ----------- uff... nareszcie :-) -----------------------------------------------------------------------
void menuUseEvent(MenuUseEvent used)      // funkcja klasy MenuBackend - reakcja na wciśnięcie OK
                                          // tutaj właśnie oddajemy menu na rzecz akcji obsługi klawisza OK
{
   Serial.print("pasirinkta:  "); Serial.println(used.item.getName()); // do testów, potem niepotrzebne
   // --- ponizej kilka przykładów obsługi  opcji -----------
   // przykładowa reakcja na wcisnięcie klawisza OK w opcji Otworz :
/* __________________________ NUSTATYMAI Pasvietimas____________________________________ */
  if (used.item.getName() == "Pasvietimas")
  {
  lcd.setCursor(0,2);lcd.write(7);     // simbolis aukštyn/žemyn
  lcd.print("                  ");lcd.setCursor(1,2);lcd.print("Sviesumas"); // keičiamos reikšmės pavadinimas
  lcd.setCursor(13,2);lcd.print(lcd_pasvietimas);lcd.print("0% ");                        // dabartinė reikšmė
  int  veiksmas=-1;delay(1000);                                             // pagalbinis kintamasis, kontroliuojantis while ciklą
                                                                         // jei jums nereikia keisti, spauti OK po 1 sek. ir grįžti į meniu  
  while(veiksmas!=4)                   // Šis ciklas bus kartojamas, kol paspausite mygtuką OK
         {
           klaviaturos_pasikeitimas=-1; 
           veiksmas=Klaviaturos_skaitymas(Key_Pin);//delay(300);   // odczyt stanu klawiatury - funkcja Klaviaturos_skaitymas lub czytaj_2 lub czytaj_3
                                            // opis poniżej przy 3 różnych definicjach funkcji czytaj
           if(klaviaturos_pasikeitimas!=veiksmas)                    // ruszamy do pracy tylko wtedy gdy klaviaturos_pasikeitimasienił sie stan klawiatury
             {lcd.setCursor(13,2);
             if (veiksmas==1) {lcd_pasvietimas++; analogWrite(BackLight_Pin,lcd_pasvietimas*25);delay(300);}
               // jesli akcja=1 (czyli wciśnieto klawisz w górę to zwiększono temperaturę
               // ustawiono max próg i wyświetlono obecną temperaturę
             if(veiksmas==2)  {lcd_pasvietimas--;analogWrite(BackLight_Pin,lcd_pasvietimas*25);delay(300);}
            if ((lcd_pasvietimas == 255) || (lcd_pasvietimas == 0)) lcd_pasvietimas = 0;
            if (lcd_pasvietimas > 10) lcd_pasvietimas = 10;
            if (lcd_pasvietimas < 10) lcd.print(" ");
            lcd.print(lcd_pasvietimas);
            if (lcd_pasvietimas == 0) lcd.print("");
            lcd.print("0% ");

               // jesli akcja=2 (czyli wciśnieto klawisz w dół to mniejszono temperaturę
               // ustawiono min próg i wyświetlono obecną temperaturę
             if(veiksmas==4) // jeśli wciśnieto OK 
               {
                 lcd.setCursor(0,2);lcd.print(">Sviesumas OK");delay(2000); // pokazujemy OK przez 2 sek.
                 lcd.setCursor(0,2);lcd.print("                    "); // czyścimy linię
                 menu.moveDown();
               //  lcd.setCursor(1,0);lcd.print(eilute1);           // odtwarzamy poprzedni stan na LCD
               }
             } 
         } klaviaturos_pasikeitimas=veiksmas;  // aktualizacja klaviaturos_pasikeitimasiennej klaviaturos_pasikeitimas, po to aby reagować tylko na klaviaturos_pasikeitimasiany stanu klawiatury
         // tu WAŻNY MOMENT - kończy się pętla while i zwracamy sterowanie do głównej pętli loop()
      } 
/* __________________________ NUSTATYMAI Irasymas____________________________________ */
     if (used.item.getName() == "Irasymas")   // dokładnie taki sam ciąg " Temperatura"
      {
                 SaveConfig();
                 lcd.setCursor(0,2);lcd.print(">Irasyta OK        ");delay(2000); // pokazujemy OK przez 2 sek.
                 lcd.setCursor(0,2);lcd.print("                    "); // czyścimy linię
                 menu.moveDown();
                // lcd.setCursor(0,0);lcd.print("*");lcd.print(eilute1);           // odtwarzamy poprzedni stan na LCD
                // lcd.setCursor(19,0);lcd.print("*");

      }
/* __________________________________________________________________________ */

/* __________________________ NUSTATYMAI Irasymas____________________________________ */
     if (used.item.getName() == "Numatytos reiksmes")   // dokładnie taki sam ciąg " Temperatura"
      {
       k_skirtumas_OFF = 3; k_skirtumas_ON = 5;
       at_OFF_T = 89; at_ON_T = 90;
       b_OFF_T = 65; b_ON_T = 45;
       pv_palaikoma_T = 40;  // PV palaikoma temperatūra
       pv_ON_T = 35; pv_OFF_T = 28; // PV atidarymo temperatūra // PV atidarymo temperatūra
       pv_palaikoma_riba_T = 1.5; // Pamaišymo vožtuvo palaikomos temperatūros riba
       PV_pauzes_pertrauka = 200;
       PV_darinejimas = 60;
       SaveConfig();
                 lcd.setCursor(0,2);lcd.print("Pradines reiksmes OK");delay(2000); // pokazujemy OK przez 2 sek.
                 lcd.setCursor(0,2);lcd.print("                    "); // czyścimy linię
                 menu.moveDown();
                 //lcd.setCursor(0,0);lcd.print("*");lcd.print(eilute1);           // odtwarzamy poprzedni stan na LCD
                 //lcd.setCursor(19,0);lcd.print("*");

      }
/* __________________________________________________________________________ */

/* _____________________ NUSTATYMAI Laikrodis Metai ____________ */
time_t t;
    tmElements_t tm;
RTC.read(tm);
     if (used.item.getName() == "Metai")   // dokładnie taki sam ciąg " Temperatura"
      {
        lcd.setCursor(0,2);lcd.write(7);     // wyswietlamy nasz symbol strzałki góra-dół
 //       lcd.setCursor(1,2);lcd.print("Metai         =   "); // tekst dla użytkownika
        lcd.setCursor(0,3);lcd.print(tm.Year+1970); lcd.blink();
        
        int  veiksmas=-1;delay(1000);         // klaviaturos_pasikeitimasienna pomocnicza, sterująca dla petli while
                                           // jesli nie puścisz klawisza OK w ciągu 1 sek. to powrót do menu    
        while(veiksmas!=4)                   // ta pętla trwa tak długo aż wciśniesz klawisz OK  
         {
           klaviaturos_pasikeitimas=-1; 
           veiksmas=Klaviaturos_skaitymas(Key_Pin); //delay(300);   // odczyt stanu klawiatury - funkcja Klaviaturos_skaitymas lub czytaj_2 lub czytaj_3
                                            // opis poniżej przy 3 różnych definicjach funkcji czytaj
           if(klaviaturos_pasikeitimas!=veiksmas)                    // ruszamy do pracy tylko wtedy gdy klaviaturos_pasikeitimasienił sie stan klawiatury
             {
             if (veiksmas==1) {1970+tm.Year++;lcd.setCursor(0,3);lcd.print(tm.Year+1970);delay(200);}
             if(veiksmas==2)  {1970+tm.Year--;if (Metai<2014) Metai=2014;lcd.setCursor(0,3);lcd.print(tm.Year+1970);delay(200);}
             if(veiksmas==4) // jeśli wciśnieto OK 
               {
                 lcd.noBlink();lcd.setCursor(0,2);lcd.print(">Metai            OK");delay(2000); // pokazujemy OK przez 2 sek.
                 lcd.setCursor(0,2);lcd.print("                    "); 
                 t = makeTime(tm);
            RTC.write(tm);        //use the time_t value to ensure correct weekday is set
            setTime(t);    
                 lcd.setCursor(1,0);lcd.print(eilute1);           // odtwarzamy poprzedni stan na LCD
                 menu.moveDown();
               }
             } 
         } klaviaturos_pasikeitimas=veiksmas;  // aktualizacja klaviaturos_pasikeitimasiennej klaviaturos_pasikeitimas, po to aby reagować tylko na klaviaturos_pasikeitimasiany stanu klawiatury
      }
/* _____________________ NUSTATYMAI Laikrodis Menuo ____________ */
              if (used.item.getName() == "Menuo")   // dokładnie taki sam ciąg " Temperatura"
      { lcd.setCursor(0,2);lcd.write(7); 
 //       lcd.setCursor(1,2);lcd.print("Menuo         -   "); // tekst dla użytkownika
        lcd.setCursor(5,3);  if(tm.Month < 10) lcd.print('0');lcd.print(tm.Month); lcd.blink();
        
        int  veiksmas=-1;delay(1000);         // klaviaturos_pasikeitimasienna pomocnicza, sterująca dla petli while
                                           // jesli nie puścisz klawisza OK w ciągu 1 sek. to powrót do menu    
        while(veiksmas!=4)                   // ta pętla trwa tak długo aż wciśniesz klawisz OK  
         {
           klaviaturos_pasikeitimas=-1; 
           veiksmas=Klaviaturos_skaitymas(Key_Pin); //delay(300);   // odczyt stanu klawiatury - funkcja Klaviaturos_skaitymas lub czytaj_2 lub czytaj_3
                                            // opis poniżej przy 3 różnych definicjach funkcji czytaj
           if(klaviaturos_pasikeitimas!=veiksmas)                    // ruszamy do pracy tylko wtedy gdy klaviaturos_pasikeitimasienił sie stan klawiatury
             {
             if (veiksmas==1) {tm.Month++;if (tm.Month>12) tm.Month=1;lcd.setCursor(5,3);if(tm.Month < 10) lcd.print('0');lcd.print(tm.Month);delay(200);}
             if(veiksmas==2)  {tm.Month--;if (tm.Month<1) tm.Month=12;lcd.setCursor(5,3);if(tm.Month < 10) lcd.print('0');lcd.print(tm.Month);delay(200);}
             if(veiksmas==4) // jeśli wciśnieto OK 
               {
                 lcd.noBlink();lcd.setCursor(0,2);lcd.print(">Menuo            OK");delay(2000); // pokazujemy OK przez 2 sek.
                 lcd.setCursor(0,2);lcd.print("                    "); 
                 t = makeTime(tm);
            RTC.write(tm);        //use the time_t value to ensure correct weekday is set
            setTime(t);    
                 lcd.setCursor(1,0);lcd.print(eilute1);           // odtwarzamy poprzedni stan na LCD
                 menu.moveDown();
               }
             } 
         } klaviaturos_pasikeitimas=veiksmas;  // aktualizacja klaviaturos_pasikeitimasiennej klaviaturos_pasikeitimas, po to aby reagować tylko na klaviaturos_pasikeitimasiany stanu klawiatury
         
      }
/* _____________________ NUSTATYMAI Laikrodis Diena ____________ */
              if (used.item.getName() == "Diena")   // dokładnie taki sam ciąg " Temperatura"
      { lcd.setCursor(0,2);lcd.write(7); 
 //       lcd.setCursor(1,2);lcd.print("Diena         -   "); // tekst dla użytkownika
        lcd.setCursor(8,3);if(tm.Day < 10) lcd.print('0');lcd.print(tm.Day); lcd.blink();
        
        int  veiksmas=-1;delay(1000);         // klaviaturos_pasikeitimasienna pomocnicza, sterująca dla petli while
                                           // jesli nie puścisz klawisza OK w ciągu 1 sek. to powrót do menu    
        while(veiksmas!=4)                   // ta pętla trwa tak długo aż wciśniesz klawisz OK  
         {
           klaviaturos_pasikeitimas=-1; 
           veiksmas=Klaviaturos_skaitymas(Key_Pin); //delay(300);   // odczyt stanu klawiatury - funkcja Klaviaturos_skaitymas lub czytaj_2 lub czytaj_3
                                            // opis poniżej przy 3 różnych definicjach funkcji czytaj
           if(klaviaturos_pasikeitimas!=veiksmas)                    // ruszamy do pracy tylko wtedy gdy klaviaturos_pasikeitimasienił sie stan klawiatury
             {
             if (veiksmas==1) {tm.Day++;if (tm.Day>31) tm.Day=1;lcd.setCursor(8,3);if(tm.Day < 10) lcd.print('0');lcd.print(tm.Day);delay(200);}
             if(veiksmas==2)  {tm.Day--;if (tm.Day<1) tm.Day=31;lcd.setCursor(8,3);if(tm.Day < 10) lcd.print('0');lcd.print(tm.Day);delay(200);}
             if(veiksmas==4) // jeśli wciśnieto OK 
               {
                 lcd.noBlink();lcd.setCursor(0,2);lcd.print(">Diena            OK");delay(2000); // pokazujemy OK przez 2 sek.
                 lcd.setCursor(0,2);lcd.print("                    "); 
                 t = makeTime(tm);
            RTC.write(tm);        //use the time_t value to ensure correct weekday is set
            setTime(t);    
                 lcd.setCursor(1,0);lcd.print(eilute1);           // odtwarzamy poprzedni stan na LCD
                 menu.moveDown();
               }
             } 
         } klaviaturos_pasikeitimas=veiksmas;  // aktualizacja klaviaturos_pasikeitimasiennej klaviaturos_pasikeitimas, po to aby reagować tylko na klaviaturos_pasikeitimasiany stanu klawiatury
         
      }
/* _____________________ NUSTATYMAI Laikrodis Valanda ____________ */
              if (used.item.getName() == "Valanda")   // dokładnie taki sam ciąg " Temperatura"
      { lcd.setCursor(0,2);lcd.write(7); 
 //       lcd.setCursor(1,2);lcd.print("Valanda       -   "); // tekst dla użytkownika
        lcd.setCursor(11,3);if(tm.Hour < 10) lcd.print('0');lcd.print(tm.Hour); lcd.blink();
        
        int  veiksmas=-1;delay(1000);         // klaviaturos_pasikeitimasienna pomocnicza, sterująca dla petli while
                                           // jesli nie puścisz klawisza OK w ciągu 1 sek. to powrót do menu    
        while(veiksmas!=4)                   // ta pętla trwa tak długo aż wciśniesz klawisz OK  
         {
           klaviaturos_pasikeitimas=-1; 
           veiksmas=Klaviaturos_skaitymas(Key_Pin); //delay(300);   // odczyt stanu klawiatury - funkcja Klaviaturos_skaitymas lub czytaj_2 lub czytaj_3
                                            // opis poniżej przy 3 różnych definicjach funkcji czytaj
           if(klaviaturos_pasikeitimas!=veiksmas)                    // ruszamy do pracy tylko wtedy gdy klaviaturos_pasikeitimasienił sie stan klawiatury
             {
             if (veiksmas==1) {tm.Hour++;if (tm.Hour>23) tm.Hour=0;lcd.setCursor(11,3);if(tm.Hour < 10) lcd.print('0');lcd.print(tm.Hour);delay(200);}
             if(veiksmas==2)  {tm.Hour--;if (tm.Hour>24) tm.Hour=23;lcd.setCursor(11,3);if(tm.Hour < 10) lcd.print('0');lcd.print(tm.Hour);delay(200);}
             if(veiksmas==4) // jeśli wciśnieto OK 
               {
                lcd.noBlink(); lcd.setCursor(0,2);lcd.print(">Valanda          OK");delay(2000); // pokazujemy OK przez 2 sek.
                 lcd.setCursor(0,2);lcd.print("                    "); 
                 t = makeTime(tm);
            RTC.write(tm);        //use the time_t value to ensure correct weekday is set
            setTime(t);    
                 lcd.setCursor(1,0);lcd.print(eilute1);           // odtwarzamy poprzedni stan na LCD
                 menu.moveDown();
               }
             } 
         } klaviaturos_pasikeitimas=veiksmas;  // aktualizacja klaviaturos_pasikeitimasiennej klaviaturos_pasikeitimas, po to aby reagować tylko na klaviaturos_pasikeitimasiany stanu klawiatury
         
      }

/* _____________________ NUSTATYMAI Laikrodis Minute ____________ */
              if (used.item.getName() == "Minute")   // dokładnie taki sam ciąg " Temperatura"
      { lcd.setCursor(0,2);lcd.write(7); 
//        lcd.setCursor(1,2);lcd.print("Minute        -   "); // tekst dla użytkownika
        lcd.setCursor(14,3);if(tm.Minute < 10) lcd.print('0');lcd.print(tm.Minute); lcd.blink();
        
        int  veiksmas=-1;delay(1000);         // klaviaturos_pasikeitimasienna pomocnicza, sterująca dla petli while
                                           // jesli nie puścisz klawisza OK w ciągu 1 sek. to powrót do menu    
        while(veiksmas!=4)                   // ta pętla trwa tak długo aż wciśniesz klawisz OK  
         {
           klaviaturos_pasikeitimas=-1; 
           veiksmas=Klaviaturos_skaitymas(Key_Pin); //delay(300);   // odczyt stanu klawiatury - funkcja Klaviaturos_skaitymas lub czytaj_2 lub czytaj_3
                                            // opis poniżej przy 3 różnych definicjach funkcji czytaj
           if(klaviaturos_pasikeitimas!=veiksmas)                    // ruszamy do pracy tylko wtedy gdy klaviaturos_pasikeitimasienił sie stan klawiatury
             {
             if (veiksmas==1) {tm.Minute++;if (tm.Minute>59) tm.Minute=0;lcd.setCursor(14,3);if(tm.Minute < 10) lcd.print('0');lcd.print(tm.Minute);delay(200);}
             if(veiksmas==2)  {tm.Minute--;if (tm.Minute>60) tm.Minute=59;lcd.setCursor(14,3);if(tm.Minute < 10) lcd.print('0');lcd.print(tm.Minute);delay(200);}
             if(veiksmas==4) // jeśli wciśnieto OK 
               {
                 lcd.noBlink();lcd.setCursor(0,2);lcd.print(">Minute           OK");delay(2000); // pokazujemy OK przez 2 sek.
                 lcd.setCursor(0,2);lcd.print("                    "); 
                 t = makeTime(tm);
            RTC.write(tm);        //use the time_t value to ensure correct weekday is set
            setTime(t);    
                 lcd.setCursor(1,0);lcd.print(eilute1);           // odtwarzamy poprzedni stan na LCD
                 menu.moveDown();
               }
             } 
         } klaviaturos_pasikeitimas=veiksmas;  // aktualizacja klaviaturos_pasikeitimasiennej klaviaturos_pasikeitimas, po to aby reagować tylko na klaviaturos_pasikeitimasiany stanu klawiatury
         
      } 
 
/* _____________________ AKUMULIACINES TALPOS SIURBLIO IJUNGIMAS ____________ */
     if (used.item.getName() == "AT ijungimo temp.")   // dokładnie taki sam ciąg " Temperatura"
     at_ON_T = MeniuFunkcija ("Nustatyta.temp=   ", at_ON_T, 99, 20, ">Temperatura OK");
 ///////////////////////////////////////////////////////////////////////////////////////////////////////
/* _____________________ AKUMULIACINES TALPOS SIURBLIO ISJUNGIMAS ____________ */
     if (used.item.getName() == "AT isjungimo temp.")   // dokładnie taki sam ciąg " Temperatura"
         at_OFF_T = MeniuFunkcija ("Nustatyta.temp=   ", at_OFF_T, 99, 20, ">Temperatura OK");
         /////////////////////////////////////////////////////////////////////////
/* _____________________ AKUMULIACINES TALPOS RANKINIS VALDYMAS ____________ */
     if (used.item.getName() == "AT rankinis vald.")   // dokładnie taki sam ciąg " Temperatura"
     {}
     /*
      {
        lcd.setCursor(0,2);lcd.print(">Isjungti");lcd.write(5);lcd.print(" Ijungti");lcd.write(1);AT_rankinis_ijungimas = false;
int  veiksmas=-1;delay(1000);         // klaviaturos_pasikeitimasienna pomocnicza, sterująca dla petli while
                                           // jesli nie puścisz klawisza OK w ciągu 1 sek. to powrót do menu    
        while(veiksmas!=4)                   // ta pętla trwa tak długo aż wciśniesz klawisz OK  
         {
           klaviaturos_pasikeitimas=-1; 
           veiksmas=Klaviaturos_skaitymas(Key_Pin); //delay(300);   // odczyt stanu klawiatury - funkcja Klaviaturos_skaitymas lub czytaj_2 lub czytaj_3
                                            // opis poniżej przy 3 różnych definicjach funkcji czytaj
           if(klaviaturos_pasikeitimas!=veiksmas)                    // ruszamy do pracy tylko wtedy gdy klaviaturos_pasikeitimasienił sie stan klawiatury
             {
             if (veiksmas==1) {AT_rankinis_ijungimas = true;  lcd.setCursor(0,2);lcd.print(">Ijungta           ");delay(200);}
             if (veiksmas==2) {AT_rankinis_ijungimas = false; lcd.setCursor(0,2);lcd.print(">Isjungta          ");delay(200);}
             if (veiksmas==4) // jeśli wciśnieto OK 
               {
                 lcd.setCursor(10,2);lcd.print("Irasyta OK");delay(2000); // pokazujemy OK przez 2 sek.
                 lcd.setCursor(0,2);lcd.print("                    "); // czyścimy linię
                 AT_rankinio_ijungimo_laikas = millis();
#ifdef DEBUGakumuliacine
Serial.print("AT_rankinis_ijungimas- ");  Serial.println(AT_rankinis_ijungimas); delay(5000);
#endif
             }
             } 
         } klaviaturos_pasikeitimas=veiksmas;  // aktualizacja klaviaturos_pasikeitimasiennej klaviaturos_pasikeitimas, po to aby reagować tylko na klaviaturos_pasikeitimasiany stanu klawiatury
         
      }
 */     
/* _____________________ SAULES KOLEKTORIAUS SIURBLIO IJUNGIMAS ____________ */
     if (used.item.getName() == "Ijungimo t.skirtum")   // dokładnie taki sam ciąg " Temperatura"
              k_skirtumas_ON = MeniuFunkcija ("Nustatyta.temp=   ", k_skirtumas_ON, 25, 1, ">Temperatura OK");
     ////////////////////////////////////////////////////////////////////////////////////
/* _____________________ SAULES KOLEKTORIAUS SIURBLIO ISJUNGIMAS ____________ */
     if (used.item.getName() == "Isjungimo t.skirtu")   // dokładnie taki sam ciąg " Temperatura"
                  k_skirtumas_OFF = MeniuFunkcija ("Nustatyta.temp=   ", k_skirtumas_OFF, 24, 0, ">Temperatura OK");
                  //////////////////////////////////////////////////////////////////////////
/* _____________________ SAULES KOLEKTORIAUS APSAUGA NUO UŽŠALIMO ____________ */
     if (used.item.getName() == "Apsauga nuo salcio")   // 
      {
        lcd.setCursor(0,2);
        if (SK_apsauga_nuo_salcio == 1) lcd.println(">Ijungta           ");
        if (SK_apsauga_nuo_salcio == 2) lcd.println(">Isjungta          ");
 
int  veiksmas=-1;delay(1000);         // klaviaturos_pasikeitimasienna pomocnicza, sterujaca dla petli while
                                           // jesli nie puscisz klawisza OK w ciagu 1 sek. to powrót do menu    
        while(veiksmas!=4)                   // ta petla trwa tak dlugo az wcisniesz klawisz OK  
         {
           klaviaturos_pasikeitimas=-1; 
           veiksmas=Klaviaturos_skaitymas(Key_Pin); //delay(300);   // odczyt stanu klawiatury - funkcja Klaviaturos_skaitymas lub czytaj_2 lub czytaj_3
                                            // opis ponizej przy 3 róznych definicjach funkcji czytaj
           if(klaviaturos_pasikeitimas!=veiksmas)                    // ruszamy do pracy tylko wtedy gdy klaviaturos_pasikeitimasienil sie stan klawiatury
             {
             if (veiksmas==1) {SK_apsauga_nuo_salcio++; lcd.setCursor(0,2);if (SK_apsauga_nuo_salcio > 2) 
                                                                            {SK_apsauga_nuo_salcio = 2; 
                                                                             lcd.print(">Isjungta          ");
                                                                             delay(200);
                                                                            }
                               }
             if (veiksmas==2) {SK_apsauga_nuo_salcio--; lcd.setCursor(0,2);if (SK_apsauga_nuo_salcio < 1) 
                                                                            {SK_apsauga_nuo_salcio = 1; 
                                                                             lcd.print(">Ijungta           ");
                                                                             delay(200);
                                                                            }
                              }
             if (veiksmas==4) // jesli wcisnieto OK 
               {
                 lcd.setCursor(10,2);lcd.print("Irasyta OK");delay(2000); // pokazujemy OK przez 2 sek.
                 lcd.setCursor(0,2);lcd.print("                    "); // czyscimy linie
                 menu.moveDown();
#ifdef DEBUGakumuliacine
Serial.print("Apsauga nuo salcio- ");  Serial.println(SK_apsauga_nuo_salcio); delay(5000);
#endif
             }
             } 
         } klaviaturos_pasikeitimas=veiksmas;  // aktualizacja klaviaturos_pasikeitimasiennej klaviaturos_pasikeitimas, po to aby reagowac tylko na klaviaturos_pasikeitimasiany stanu klawiatury
         
      }     

/* _____________________ BOILERIO SIURBLIO IJUNGIMAS ____________ */
     if (used.item.getName() == "B ijungimo temp.")   // dokładnie taki sam ciąg " Temperatura"
            b_ON_T = MeniuFunkcija ("Nustatyta.temp=   ", b_ON_T, 99, 20, ">Temperatura OK"); 
/////////////////////////////////////////////////////////////////////////////////////////////////////
/* _____________________ BOILERIO SIURBLIO ISJUNGIMAS ____________ */
     if (used.item.getName() == "B isjungimo temp.")   // dokładnie taki sam ciąg " Temperatura"
       b_OFF_T = MeniuFunkcija ("Nustatyta.temp=   ", b_OFF_T, 99, 25, ">Temperatura OK");     
     ////////////////////////////////////////////////////////////////////////////////////////////////
/* _____________________ BOILERIO RANKINIS VALDYMAS ____________ */
     if (used.item.getName() == "B rankinis vald.")   // dokładnie taki sam ciąg " Temperatura"
      {
        lcd.setCursor(0,2);lcd.print(">Isjungti");lcd.write(5);lcd.print(" Ijungti");lcd.write(1);B_rankinis_ijungimas = false;
int  veiksmas=-1;delay(1000);         // klaviaturos_pasikeitimasienna pomocnicza, sterująca dla petli while
                                           // jesli nie puścisz klawisza OK w ciągu 1 sek. to powrót do menu    
        while(veiksmas!=4)                   // ta pętla trwa tak długo aż wciśniesz klawisz OK  
         {
           klaviaturos_pasikeitimas=-1; 
           veiksmas=Klaviaturos_skaitymas(Key_Pin); //delay(300);   // odczyt stanu klawiatury - funkcja Klaviaturos_skaitymas lub czytaj_2 lub czytaj_3
                                            // opis poniżej przy 3 różnych definicjach funkcji czytaj
           if(klaviaturos_pasikeitimas!=veiksmas)                    // ruszamy do pracy tylko wtedy gdy klaviaturos_pasikeitimasienił sie stan klawiatury
             {
             if (veiksmas==1) {B_rankinis_ijungimas = false; lcd.setCursor(0,2);lcd.print(">Ijungta           ");delay(200);}
             if (veiksmas==2) {B_rankinis_ijungimas = true;  lcd.setCursor(0,2);lcd.print(">Isjungta          ");delay(200);}
             if (veiksmas==4) // jeśli wciśnieto OK 
               {
                 lcd.setCursor(10,2);lcd.print("Irasyta OK");delay(2000); // pokazujemy OK przez 2 sek.
                 lcd.setCursor(0,2);lcd.print("                    "); // czyścimy linię
                 menu.moveDown();
                 B_rankinio_ijungimo_laikas = millis();
#ifdef DEBUGboileris
Serial.print("B_rankinis_ijungimas- ");  Serial.println(B_rankinis_ijungimas); delay(3000);
#endif
             }
             } 
         } klaviaturos_pasikeitimas=veiksmas;  // aktualizacja klaviaturos_pasikeitimasiennej klaviaturos_pasikeitimas, po to aby reagować tylko na klaviaturos_pasikeitimasiany stanu klawiatury
         
      } 
     ////////////////////////////////////////////////////////////////////////////////////////////////
/* _____________________ BOILERIO TERMOSTATAS ____________ */
     if (used.item.getName() == "B termostatas   ")   // dokładnie taki sam ciąg " Temperatura"
      {
        lcd.setCursor(0,2);lcd.print(">Isjungti");
        lcd.write(5);lcd.print(" Ijungti");lcd.write(1);
        B_termostat_status = false;    // termostato elektrinis tenas isjungtas
int  veiksmas=-1;delay(1000);         // klaviaturos_pasikeitimasienna pomocnicza, sterująca dla petli while
                                           // jesli nie puścisz klawisza OK w ciągu 1 sek. to powrót do menu    
        while(veiksmas!=4)                   // ta pętla trwa tak długo aż wciśniesz klawisz OK  
         {
           klaviaturos_pasikeitimas=-1; 
           veiksmas=Klaviaturos_skaitymas(Key_Pin); //delay(300);   // odczyt stanu klawiatury - funkcja Klaviaturos_skaitymas lub czytaj_2 lub czytaj_3
                                            // opis poniżej przy 3 różnych definicjach funkcji czytaj
           if(klaviaturos_pasikeitimas!=veiksmas)                    // ruszamy do pracy tylko wtedy gdy klaviaturos_pasikeitimasienił sie stan klawiatury
             {
             if (veiksmas==1) {B_termostat_ON = true; lcd.setCursor(0,2);lcd.print(">Ijungta           ");delay(200);} // termostato elektrinis tenas isjungtas
             if (veiksmas==2) {B_termostat_ON = false;  lcd.setCursor(0,2);lcd.print(">Isjungta          ");delay(200);} // termostato elektrinis tenas ijungtas
             if (veiksmas==4) // jeśli wciśnieto OK 
               {
                 lcd.setCursor(10,2);lcd.print("Irasyta OK");delay(2000); // pokazujemy OK przez 2 sek.
                 lcd.setCursor(0,2);lcd.print("                    "); // czyścimy linię
                 menu.moveDown();
                 B_rankinio_ijungimo_laikas = millis();
#ifdef DEBUGboileris
Serial.print("B_rankinis_sildymas ");  
if (B_termostat_ON == true) Serial.println("I_J_U_N_G_T_A_S"); else Serial.println("I*S*J*U*N*G*T*A*S"); 
#endif
             }
             } 
         } klaviaturos_pasikeitimas=veiksmas;  // aktualizacja klaviaturos_pasikeitimasiennej klaviaturos_pasikeitimas, po to aby reagować tylko na klaviaturos_pasikeitimasiany stanu klawiatury
         
      } 
         
/* _____________________ PAMAIŠYMO VOŽTUVO PALAIKOMA TEMPERATURA ____________ */
///////
if (used.item.getName() == "Darbo Rezimas     ") 
 {       
        lcd.setCursor(0,0);lcd.write(7);     
        lcd.setCursor(1,1);lcd.print("Darbo Rezimas     "); 
        lcd.setCursor(8,2);
        if (PV_rezimas == 1) lcd.print("isjungta OFF");
        if (PV_rezimas == 2) lcd.print("ijungta   ON");
//        if (PV_rezimas == 3) lcd.print("isjungta    ");
        int  veiksmas=-1; delay(1000);         // 
                                           
        while(veiksmas!=4)                   // 
         {
           klaviaturos_pasikeitimas=-1; 
           veiksmas=Klaviaturos_skaitymas(Key_Pin); //delay(300);  
                                            
           if(klaviaturos_pasikeitimas!=veiksmas)           
             {
             if (veiksmas==1) {PV_rezimas++; if(PV_rezimas>2) 
                                               {PV_rezimas=2; 
                                                lcd.setCursor(8,2); 
                                                lcd.print("ijunta    ON");
                                                delay(200);
                                              }
                               }
             if(veiksmas==2)  {PV_rezimas--; if(PV_rezimas<1) 
                                               {PV_rezimas=1; 
                                                lcd.setCursor(8,2); 
                                                lcd.print("isjungta OFF");
                                               delay(200);
                                              }
                               }
             if(veiksmas==4) // 0
               {
                 lcd.setCursor(0,2); lcd.print(">Darbo Rezimas    OK"); delay(2000); // 0
                 lcd.setCursor(0,2); lcd.print("                    "); // 0
//                 lcd.setCursor(1,0);lcd.print(eilute1);           // 0
                 menu.moveDown();
               }
             } 
         } klaviaturos_pasikeitimas=veiksmas;
 }
//////
     if (used.item.getName() == "PV palaikoma temp.")   // dokladnie taki sam ciag " Temperatura"
  pv_palaikoma_T = MeniuFunkcija ("Nustatyta.temp=   ", pv_palaikoma_T, 70, 20, ">Temperatura OK");     
     //////////////////////////////////////////////////////////////////////////////
 /* _____________________ PAMAIŠYMO VOŽTUVO ĮJUNGIMO TEMPERATURA ____________ */
     if (used.item.getName() == "PV ijungimo temp. ")   // dokladnie taki sam ciag " Temperatura"
  pv_ON_T = MeniuFunkcija ("Nustatyta.temp=   ", pv_ON_T, 90, 20, ">Temperatura OK");     
     ////////////////////////////////////////////////////////////////////////////////////
/* _____________________ PAMAIŠYMO VOŽTUVO IŠJUNGIMO TEMPERATURA ____________ */
     if (used.item.getName() == "PV isjungimo temp.")   // dokladnie taki sam ciag " Temperatura"
pv_OFF_T =  MeniuFunkcija ("Nustatyta.temp=", pv_OFF_T, 90, 20, ">Temperatura OK"); 
     ///////////////////////////////////////////////////////////////////
/* _____________________ PAMAIŠYMO VOŽTUVO PALAIKOMOS TEMPERATUROS RIBA ____________ */
     if (used.item.getName() == "PV palaikoma riba ")   // dokladnie taki sam ciag " Temperatura"
      {
        lcd.setCursor(0,2);lcd.write(7);     // wyswietlamy nasz symbol strzalki góra-dól
        lcd.setCursor(1,2);lcd.print("Palaikoma riba=   "); // tekst dla uzytkownika
        lcd.setCursor(16,2);lcd.print(pv_palaikoma_riba_T);lcd.setCursor(19,2);lcd.print(" "); // wyswietlamy akt. temperature
        int  veiksmas=-1;delay(1000);         // klaviaturos_pasikeitimasienna pomocnicza, sterujaca dla petli while
                                           // jesli nie puscisz klawisza OK w ciagu 1 sek. to powrót do menu    
        while(veiksmas!=4)                   // ta petla trwa tak dlugo az wcisniesz klawisz OK  
         {
           klaviaturos_pasikeitimas=-1; 
           veiksmas=Klaviaturos_skaitymas(Key_Pin); //delay(300);   // odczyt stanu klawiatury - funkcja Klaviaturos_skaitymas lub czytaj_2 lub czytaj_3
                                            // opis ponizej przy 3 róznych definicjach funkcji czytaj
           if(klaviaturos_pasikeitimas!=veiksmas)                    // ruszamy do pracy tylko wtedy gdy klaviaturos_pasikeitimasienil sie stan klawiatury
             {
             if (veiksmas==1) {pv_palaikoma_riba_T=pv_palaikoma_riba_T+0.1;if(pv_palaikoma_riba_T>10)pv_palaikoma_riba_T=10;lcd.setCursor(16,2);
                                                 lcd.print(pv_palaikoma_riba_T);lcd.setCursor(19,2);lcd.print(" ");delay(200);}
             if(veiksmas==2)  {pv_palaikoma_riba_T=pv_palaikoma_riba_T-0.1;if(pv_palaikoma_riba_T<0.1)pv_palaikoma_riba_T=0.1;lcd.setCursor(16,2);
                                                 lcd.print(pv_palaikoma_riba_T);lcd.setCursor(19,2);lcd.print(" ");delay(200);}
             if(veiksmas==4) // jesli wcisnieto OK 
               {
                 lcd.setCursor(0,2);lcd.print(">Riba        OK");delay(2000); // pokazujemy OK przez 2 sek.
                 lcd.setCursor(0,2);lcd.print("                    "); // czyscimy linie
                 menu.moveDown();
                 //lcd.setCursor(1,0);lcd.print(eilute1);           // odtwarzamy poprzedni stan na LCD
               }
             } 
         } klaviaturos_pasikeitimas=veiksmas;  // aktualizacja klaviaturos_pasikeitimasiennej klaviaturos_pasikeitimas, po to aby reagowac tylko na klaviaturos_pasikeitimasiany stanu klawiatury
         // tu WAZNY MOMENT - konczy sie petla while i zwracamy sterowanie do glównej petli loop()
      }
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* _____________________ PAMAIŠYMO VOŽTUVO PAUZĖ ____________ */
     if (used.item.getName() == "PV pauze          ")   // dokladnie taki sam ciag " Temperatura"
     PV_pauzes_pertrauka =  MeniuFunkcija ("Pauze         =   ", PV_pauzes_pertrauka, 999, 50, ">Pauze       OK"); 
     //////////////////////////////////////////////////////////////////////////////////////
/* _____________________ PAMAIŠYMO VOŽTUVO DARINEJIMAS ____________ */
     if (used.item.getName() == "PV veiksmas       ")   // dokladnie taki sam ciag " Temperatura"
     PV_darinejimas =  MeniuFunkcija ("Veiksmas      =   ", PV_darinejimas, PV_pauzes_pertrauka-20, 30, ">Veiksmas    OK"); 
          //////////////////////////////////////////////////////////////////////////////////////////
 // ...
}
// --- Reakcja na wciśnięcie klawisza -----------------------------------------------------------------
void menuChangeEvent(MenuChangeEvent changed)  // funkcja klasy MenuBackend 
{
  if(changed.to.getName()==menu.getRoot()){InMenu =false;
                                           Serial.println("Dabar esame MenuRoot");
                                           LCD_T_sablonas();
                                           Temperaturu_vaizdavimas();}
  /* tak naprawdę to tylko tutaj przydaje się ów shortkey i służy przede wszystkim do wzbogacenia menu
     o symbole strzałek w zależności co wybrano. Wszystko co tutaj się wyprawia jest pokazywane na LCD. 
  */
  int c=changed.to.getShortkey();                         // pobieramy shortkey (1,2,3, lub4)
  lcd.setCursor(0,0); 
  if(c==1)                                                // jeśli to menu głowne (shortkey=1) to:
    {InMenu =true;
    lcd.write(3);                                         // strzałka w lewo
    strcpy(eilute1,changed.to.getName());                  // tworzymy napis w pierwszej linii
    lcd.print(eilute1);                                    // wyświetlamy ją
    lcd.setCursor(19,0);lcd.write(4);                     // strzałka w prawo
    lcd.setCursor(0,1);lcd.write(5); lcd.print("                  ");   
    lcd.setCursor(19,1);lcd.write(5);                     // strzałka w dół
    lcd.setCursor(0,2);lcd.print("                    "); // išvaloma 3 eilutė, kai atsiduriama meniu nr.1
    }
    if(c==2)                                              // jeśli to podmenu dla dziecka - (shortkey=2) to:
    {InMenu =true;
    lcd.print("*");                                       // rysujemy gwiazdkę
    strcpy(eilute2,changed.to.getName());                  // tworzymy napis w pierwszej linii
    lcd.print(eilute1);                                    // wyświetlamy ją
    lcd.setCursor(19,0);lcd.print("*");                   // gwiazdka 
    lcd.setCursor(0,1);lcd.write(6); lcd.print("                  ");                     // druga eilute i strzałka powrotu (arrowBack)
    lcd.setCursor(1,1);lcd.print(changed.to.getName());                      // wyświetlamy nazwe "dziecka"
    lcd.setCursor(19,1);lcd.write(7);                     // strzałka góra-dół
    }
    
    if(c==3)                                              // jeśli dziecko  ma dziecko - (shortkey =3) to:
    {InMenu =true;
    lcd.print("*");                                       // gwiazdka
    strcpy(eilute2,changed.to.getName());                  // kopiujemy akt. nazwe opcji menu do klaviaturos_pasikeitimasiennej eilute2
    lcd.print(eilute1);                                    // i wyświetlamy pierwszą linię
    lcd.setCursor(19,0);lcd.print("*");                   // gwiazdka
    lcd.setCursor(0,1);lcd.write(6);                      // druga eilute i strzałka arrowBack
    lcd.print(changed.to.getName());                      // wyświetlamy wnuka w drugiej linii
    lcd.setCursor(19,1);lcd.write(4);                     // strzałka w prawo bo są wnuki
    }
    
    if(c==4)                                              // jeśli to wnuk  (shortkey =4) to:
    {InMenu =true;
    lcd.print("*");                                       // gwaizdka
    lcd.print(eilute2);                                    // w pierwszej linii wyświetlamy dziecko ( czyli rodzica wnuka) 
    lcd.setCursor(19,0);lcd.print("*");                   // gwaizdka
    lcd.setCursor(0,1);lcd.write(6);                      // druga eilute i strzałka arrowBack
    lcd.print(changed.to.getName());                      // wyświetlamy wnuka
    lcd.setCursor(19,1);lcd.write(7);                     // strzałka góra-dół 
    } 
}

// --- analoginė 5 mygtukų klaviatūros nuskaitymo versija, DFRobot --------------------------------------
volatile int Klaviaturos_skaitymas(int analog)
{
   int stan_Analog = analogRead(analog);delay(30);//Serial.println(stan_Analog); 
   if (stan_Analog > 1000) return -1; // riba
   if (stan_Analog < 50)   return 0;  // Į dešinę 
   if (stan_Analog < 200)  return 1;  // Viršun 
   if (stan_Analog < 400)  return 2;  // Žemyn‚ 
   if (stan_Analog < 600)  return 3;  // Į kairę  
   if (stan_Analog < 800)  return 4;  // OK 
   return -1;                         // Nepaspaustas
}
// ============================================================================================
// 
void setup()
{
  

  /* ********************************************************* */
   LoadConfig(); 
  pinMode(BackLight_Pin, OUTPUT);
    analogWrite(BackLight_Pin,lcd_pasvietimas*25);
  eilute1=new char[20];  // zainicjowanie dynamicznego wskaźnika do tekstu 
  eilute2=new char[20];  // to BARDZO WAŻNE, bo wskażnik dynamiczny musi wskazywać na 
  eilute3=new char[20];   // z góry określone miejsce w pamieci. Gdybyśmy tego nie zrobili
                        // to wcześniej czy później programik mógłby wskoczyć w nieokreślony 
                        //  bliżej obszar pamięci, co może skutkować nieodwracalnymi konsekwencjami
                        // łącznie z przestawieniem Fuse Bitów !!!  
                        // Proszę uważać na wszystkie dynamiczne wskaźniki, TAKA DOBRA RADA :-)
  Serial.begin(9600);   // inicjacja Seriala, głównie do testów 
  lcd.begin(20, 4);     // inicjacja LCD
  Wire.begin();



  lcd.createChar(3,arrowLeft);    // LCD ženklas kairėn
  lcd.createChar(4,arrowRight);
  lcd.createChar(5,arrowDown);
  lcd.createChar(6,arrowBack);
  lcd.createChar(7,arrowUpDown);
  lcd.createChar(1,arrowUp);


    pinMode(Relay12, OUTPUT);digitalWrite(Relay12, HIGH);
    pinMode(B_termostatas, OUTPUT);digitalWrite(B_termostatas, HIGH); //boilerio termostatas
    pinMode(MV_K_21, OUTPUT);digitalWrite(MV_K_21, HIGH); // MotoVožtuvas katilui, kontaktas 21
    pinMode(MV_K_11, OUTPUT);digitalWrite(MV_K_11, HIGH); // MotoVožtuvas katilui, kontaktas 11
    pinMode(MV_A_21, OUTPUT);digitalWrite(MV_A_21, HIGH); // MotoVožtuvas akumuliacinei, kontaktas 21
    pinMode(MV_A_11, OUTPUT);digitalWrite(MV_A_11, HIGH); // MotoVožtuvas akumuliacinei, kontaktas 11
    pinMode(PVsiurblys, OUTPUT);digitalWrite(PVsiurblys, HIGH); // Pamaisymo vožtuvo siurblio Relė 6
    pinMode(PVuzdarymas, OUTPUT);digitalWrite(PVuzdarymas, HIGH); // Pamaisymo vožtuvo uždarymas Relė 5
    pinMode(PVatidarymas, OUTPUT);digitalWrite(PVatidarymas, HIGH); // Pamaisymo vožtuvo atidarymas Relė 4
    pinMode(BSiurblys, OUTPUT);digitalWrite(BSiurblys, HIGH); // Boilerio siurblio junginėjimui Relė 3
    pinMode(SKSiurblys, OUTPUT);digitalWrite(SKSiurblys, HIGH); // Saulės kolektorius
    pinMode(ATSiurblys, OUTPUT);digitalWrite(ATSiurblys, HIGH); // Akumuliacinės talpos įjungimas
   KI_sensor.begin();SK_sensor.begin();
   BV_sensor.begin();BA_sensor.begin();
   AV_sensor.begin();AA_sensor.begin();
   KT_sensor.begin();LT_sensor.begin();
   PV_sensor.begin();

   
  pinMode(13,OUTPUT);digitalWrite(13,LOW); // tik testas 
  menuSetup();          // funkcja klasy MenuBackend - tu tak naprawdę tworzymy nasze menu 
//  menu.moveDown();      // idziemy do pierwszej opcji - PLIK, moveDown bo pierwotnie byliśmy w root
  Temperaturu_matavimas_1();
  Temperaturu_matavimas_2();
  Temperaturu_matavimas_3();
    lcd.setCursor(0,0); 
    lcd.print("www.SauleVire.lt");
    lcd.setCursor(0,2);  
    lcd.print("KKK valdiklis v2.1"); delay(999);
#ifdef DEBUG
Serial.print("k_skirtumas_OFF-    ");Serial.print(k_skirtumas_OFF);Serial.print(char(186));Serial.println("C");
Serial.print("k_skirtumas_ON-     ");Serial.print(k_skirtumas_ON);Serial.print(char(186));Serial.println("C");
Serial.print("at_ON_T-            ");Serial.print(at_ON_T);Serial.print(char(186));Serial.println("C");
Serial.print("at_OFF_T-           ");Serial.print(at_OFF_T);Serial.print(char(186));Serial.println("C");
Serial.print("b_ON_T-             ");Serial.print(b_ON_T);Serial.print(char(186));Serial.println("C");
Serial.print("b_OFF_T-            ");Serial.print(b_OFF_T);Serial.print(char(186));Serial.println("C");
Serial.print("k_uzsalimas-        ");Serial.print(k_uzsalimas);Serial.print(char(186));Serial.println("C");
Serial.print("lcd_pasvietimas-    ");Serial.print(lcd_pasvietimas);Serial.print(char(186));Serial.println("C");
Serial.print("pv_palaikoma_T-     "); Serial.print(pv_palaikoma_T);Serial.print(char(186));Serial.println("C");// PV palaikoma temperatūra
Serial.print("pv_ON_T-            "); Serial.print(pv_ON_T);Serial.print(char(186));Serial.println("C"); // PV atidarymo temperatūra
Serial.print("pv_palaikoma_riba_T-"); Serial.print(pv_palaikoma_riba_T);Serial.print(char(186));Serial.println("C"); // PV atidarymo temperatūra
Serial.print("PV_darinejimas-     "); Serial.print(PV_darinejimas);Serial.println(" s*100"); // Pamaišymo vožtuvo palaikomos temperatūros riba
Serial.print("PV_pauzes_pertrauka-"); Serial.print(PV_pauzes_pertrauka);Serial.println(" s*100"); // Pamaišymo vožtuvo pertraukos laikas po judesio
Serial.print("PV_rezimas-"); if (PV_rezimas == 1) Serial.println(" Rankinis rezimas"); else Serial.println(" Automatinis rezimas");// Pamaišymo vožtuvo rezimo spausdinimas
Serial.print("SK_apsauga nuo uzsalimo-"); if (SK_apsauga_nuo_salcio == 1) Serial.println(" Apsauga ijungta"); else Serial.println(" Apsauga isjungta"); // Pamaišymo vožtuvo rezimo spausdinimas
Serial.println("--------------------");
#endif  

  LCD_T_sablonas();
  Temperaturu_vaizdavimas();
    Ekrano_pasvietimo_ijungimo_laikas = millis();
    Boilerio_siurblio_ijungimo_laikas = millis();
    temperaturu_matavimo_laikas_1 = millis();
    delay(8000);
    PV_stop = true;
    PV_uzdarinejamas = false;
    PV_atidarinejamas = false;
 //////////////////////// LAIKRODIS ///////////////////////////
setSyncProvider(RTC.get); 
  }  // setup() ...************ PABAIGA **************...
  // ************************ PROGRAMOS PRADZIA void loop() *******************************
void loop()    
{
  // jei ekranas, nespaudant mygtukų, šviečia ilgiau negu užduota, pašvietimas išjungiamas
      if (millis()- Ekrano_pasvietimo_ijungimo_laikas > Ekrano_pasvietimo_pertrauka) { 
      analogWrite(BackLight_Pin, 0);
      Backlighting = false;
      Ekrano_pasvietimo_ijungimo_laikas = millis();}
      
 // Paspaudus bet kokį klavišą įjungiamas ekrano pašvietimas, kai jis būna išjungtas  
if ((x != -1) && (Backlighting == false)){ analogWrite(BackLight_Pin,lcd_pasvietimas*25);
                                            Backlighting = true;}
// Jei meniu būna neaktyvus kažkiek laiko, tai grįžtama į pagrindinę programą
//if ((x != -1) && (InMenu == true))Meniu_praleistas_neaktyvus_laikas = millis();
//    else {if (millis()- Meniu_praleistas_neaktyvus_laikas > neaktyvus_meniu)
//          menu.toRoot();
//}
  x=Klaviaturos_skaitymas(Key_Pin);delay(30);             // odczytujemy stan klawiatury:
  if(klaviaturos_pasikeitimas!=x)                               // jesli była klaviaturos_pasikeitimasiana stanu to :
    {
      switch(x)                           // sprawdzamy co nacisnieto 
      {
      case 0: menu.moveRight();break;     // jesli naciśnięto klawisz w Prawo to przesuń menu w prawo
      case 1: menu.moveUp();break;        // menu do góry
      case 2: menu.moveDown();break;      // menu w dół
      case 3: menu.moveLeft();break;      // menu w lewo
      case 4: menu.use();break;           // wciśnięto OK więc skok do funkcji menuUseEvent(MenuUseEvend used)
                                          // to w tej funkcji właśnie obsługujemy nasze Menu, tu sprawdzamy
                                          // jaką opcję wybrano i tutaj tworzymy kod do obslugi zdarzenia.
      }
    } klaviaturos_pasikeitimas=x;                               // przypisanie klaviaturos_pasikeitimasiennej klaviaturos_pasikeitimas wartości x po to, aby dluższe wciskanie tego
                                          // samego klawisza nie powodowało ponownej generacji zdarzenia. 
                                          // program reaguje na klaviaturos_pasikeitimasianę stanu klawiatury. 
// Jeigu dabar neesame meniu viduje, tai vykdoma nuolatinė programa
if (InMenu == false){
  // laiko intervalas naudojamas LCD vaizdui atnaujinti
  if (millis() > Ekrano_rodmenu_atnaujinimo_laikas ) { 
Serial.println("************ temperaturu parodymas ********************");

  Ekrano_rodmenu_atnaujinimo_laikas = millis() + Ekrano_rodmenu_atnaujinimo_pertrauka;
  LCD_T_sablonas();
  Temperaturu_vaizdavimas();

  //#ifdef DEBUGds18b20
//Serial.println("Temperaturu_matavimas");
//unsigned long start = millis();
//#endif
  
#ifdef DEBUGds18b20
//unsigned long stop = millis();
//Serial.print("Temperaturu matavimo laikas: ");  Serial.println(stop - start);
Serial.print("KI/ ");Serial.print(KI);Serial.print(" SK/ ");Serial.print(SK);Serial.print(" PV/ ");Serial.println(PV);
Serial.print("AV/ ");Serial.print(AV);Serial.print(" AA/ ");Serial.print(AA);Serial.print(" KT/ ");Serial.println(KT);
Serial.print("BV/ ");Serial.print(BV);Serial.print(" BA/ ");Serial.print(BA);Serial.print(" LT/ ");Serial.println(LT);
Serial.println("----");
Serial.print("millis- ");Serial.println(millis()/1000);
#endif
#ifdef DEBUGbusena
if (PV_uzdarytas == true) {Serial.println("Pamaisymo voztuvas PILNAI UZDARYTAS"); 
   }else {Serial.println("Pamaisymo voztuvas KAZKIEK ATIDARYTAS");}
if (PV_stop == true) {Serial.println("Pamaisymo voztuvas NEJUDA"); 
   }else{ Serial.println("Pamaisymo voztuvas **JUDA");}
//if (PV_atidarinejamas = false) Serial.println("Pamaisymo voztuvas, atidarymas- NEBEJUDA"); else Serial.println("Pamaisymo voztuvas ATIDARINEJAMAS");
//if (PV_uzdarinejamas = false) Serial.println("Pamaisymo voztuvas, uzdarymas- NEBEJUDA"); else Serial.println("Pamaisymo voztuvas UZDARINEJAMAS");

#endif
  }
} 
// Tikrinama ar rankiniu budu neijungtas AKUMULIACINĖS TALPOS ar BOILERIO  siurblys

Akumuliacine_talpa();
      //--------------------------------------------------------------------------
      // boilerio siurblio paleidimas/stabdymas tikrinami kas 1 min (kintamasis Boilerio_siurblio_pertrauka)
   if (millis()> Boilerio_siurblio_ijungimo_laikas ){
Serial.println("************ vykdoma programa Boileris() ********************");
  Boileris();
  Boilerio_termostatas();
  Boilerio_siurblio_ijungimo_laikas=millis() + Boilerio_siurblio_pertrauka;}
  //***********************************
//  if (millis() > temperaturu_matavimo_laikas_3 ) { 
//  temperaturu_matavimo_laikas_3 = millis() + temperaturu_matavimo_pertrauka_3;
//  Temperaturu_matavimas_3();}
  //***********************************
      //--------------------------------------------------------------------------    
 

// --------------------- Saulės kolektorius --------------------- //
if (SK <=0 & SK_apsauga_nuo_salcio == 1)
  {if (SK <=0)
     {digitalWrite(SKSiurblys, LOW);  // Įjungiamas saulės kolektoriaus siurblys (apsauga nuo užšalimo)
     SK_siurblys = true;}
       else
           {digitalWrite(SKSiurblys, HIGH); // Išjungiamas akumuliacinės talpos siurblys (apsauga nuo užšalimo)
           SK_siurblys = false;}
   }
  else
      {Saules_kolektorius();}

// ---------------------- pamaisymo voztuvo darbas ---------------------------- //
// 1- rankinis, 2- automatinis
if (PV_rezimas == 2){
  PamaisymoVoztuvoSiusblys();
  PamaisymoVoztuvoDarbas();
}
  else {
    digitalWrite(PVsiurblys, HIGH); // siurblys išjungiamas
    digitalWrite(PVuzdarymas, HIGH); // vožtuvas nebevaldomas
    digitalWrite(PVatidarymas,HIGH); // vožtuvas nebevaldomas
    PV_siurblys = false;
    }
// -------------------- motorizuoti voztuvai ------------------------------- //
// Jei Katilo moto vožtuvas uždarytas ir katilo išėjime ir akumuliacinės išėjime nebėra šilumos 
// t.y. temperatūra žemesnė negu pv_OFF_T, tai katilo moto vožtuvas atidaromas ir paruošiamas katilas kūrimui.
if (MV_K == false & KI <= pv_OFF_T & AV <= pv_OFF_T) 
  {}
// matuojamos temperatūros nurodytais laiko intervalais (temperaturu_matavimo_pertrauka)
/* +++++++++++++++++++++++++++ PIRMAS LYGIS ++++++++++++++++++++++++++++++++++++ */ 
if (millis() > temperaturu_matavimo_laikas_1 ) { 
  temperaturu_matavimo_laikas_1 = millis() + temperaturu_matavimo_pertrauka_1;
  Temperaturu_matavimas_1();}
/* +++++++++++++++++++++++++++ ANTRAS LYGIS ++++++++++++++++++++++++++++++++++++ */ 
if (millis() > temperaturu_matavimo_laikas_2 ) { 
  temperaturu_matavimo_laikas_2 = millis() + temperaturu_matavimo_pertrauka_2;
  Temperaturu_matavimas_2();}
/* +++++++++++++++++++++++++++ TREČIAS LYGIS ++++++++++++++++++++++++++++++++++++ */ 
if (millis() > temperaturu_matavimo_laikas_3 ) { 
  temperaturu_matavimo_laikas_3 = millis() + temperaturu_matavimo_pertrauka_3;
  Temperaturu_matavimas_3();}

// === LAIKRODIS ===========================================================


//RTC.read(tm);
lcd.setCursor(0,3);

lcd.print(year());
printDigits(month(),'.');
printDigits(day(),'.');

printDigits(hour(),' ');
printDigits(minute(),':');
printDigits(second(),':'); 
Alarm.delay(0);
}// === PABAIGA ===========================================================
