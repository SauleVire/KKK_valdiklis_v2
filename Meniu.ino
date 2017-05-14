	   int MeniuFunkcija (String tekstas1, int KeiciamaReiksme, int MaxReiksme, int MinReiksme, String tekstas2)
	        {
        lcd.setCursor(0,2);lcd.write(7);     // wyswietlamy nasz symbol strzalki g�ra-d�l
        lcd.setCursor(1,2);lcd.print(tekstas1); //("Nustatyta.temp=   "); // tekst dla uzytkownika
        lcd.setCursor(16,2);lcd.print(KeiciamaReiksme); // wyswietlamy akt. temperature
        int  veiksmas=-1; delay(333);         //buvo delay (1000) klaviaturos_pasikeitimas ienna pomocnicza, sterujaca dla petli while
                                           // jesli nie puscisz klawisza OK w ciagu 1 sek. to powr�t do menu    
        while(veiksmas!=4)                   // ta petla trwa tak dlugo az wcisniesz klawisz OK  
         {
           klaviaturos_pasikeitimas=-1; 
           veiksmas=Klaviaturos_skaitymas(Key_Pin); //delay(300);   // odczyt stanu klawiatury - funkcja Klaviaturos_skaitymas lub czytaj_2 lub czytaj_3
                                            // opis ponizej przy 3 r�znych definicjach funkcji czytaj
           if(klaviaturos_pasikeitimas!=veiksmas)                    // ruszamy do pracy tylko wtedy gdy klaviaturos_pasikeitimasienil sie stan klawiatury
             {
             if (veiksmas==1) {KeiciamaReiksme++; if(KeiciamaReiksme>MaxReiksme) KeiciamaReiksme=MaxReiksme; lcd.setCursor(16,2);
                                                 lcd.print(KeiciamaReiksme); delay(200);}
             if(veiksmas==2)  {KeiciamaReiksme--; if(KeiciamaReiksme<MinReiksme) KeiciamaReiksme=MinReiksme; lcd.setCursor(16,2);
                                                 lcd.print(KeiciamaReiksme); delay(200);}
             if(veiksmas==4) // jesli wcisnieto OK 
               {
                 lcd.setCursor(0,2); lcd.print(tekstas2); delay(2000); // pokazujemy OK przez 2 sek.
                 lcd.setCursor(0,2); lcd.print("                    "); // czyscimy linie
                 menu.moveDown();
                 //lcd.setCursor(1,0);lcd.print(eilute1);           // odtwarzamy poprzedni stan na LCD
               }
             } 
         } klaviaturos_pasikeitimas=veiksmas;  // aktualizacja klaviaturos_pasikeitimasiennej klaviaturos_pasikeitimas, po to aby reagowac tylko na klaviaturos_pasikeitimasiany stanu klawiatury
         // tu WAZNY MOMENT - konczy sie petla while i zwracamy sterowanie do gl�wnej petli loop()
         return KeiciamaReiksme;
      }
