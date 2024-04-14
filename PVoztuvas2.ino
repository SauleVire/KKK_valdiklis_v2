/* ********** Variables for mixing valve ****** kintamieji pamaišymo vožtuvui ************
// MV- Mixing valve
// KI- boiler output temperature
// AV- Accumulation capacity top temperature  (Storage tank)
// pv_palaikoma_T- MV maintained temperature
// pv_palaikoma_riba_T- MV acceptable temperature 
// PV_uzdarinejamas- MV closing
// PV_atidarinejimo_laikas- MV opening time
// pv_klaida- MV error
// PV_pauze- MV pause
// PV_darinejimas- MV is moving




byte pv_palaikoma_T = 40; // Temperature maintained by the mixing valve .. Pamaišymo vožtuvo palaikoma temperatūra
byte pv_atidarymo_T = 30; // Temperature to open the mixing valve .. Temperatūra pamaišymo vožtuvui atidaryti
byte PV_rankinis_ijungimas = 0; // Tag for manual control of the mixing valve .. Žymė rankiniam pamaišymo vožtuvo valdymui
boolean PV_uzdarytas = true; // MV_closed
boolean PV_atidarytas = false; // MV_open
boolean PV_atidarinejamas = false; // MV_opening
boolean PV_uzdarinejamas = false; // MV_closing
boolean PV_stop = true; // MV_stop
************************************************************************
#define PV_pilno_atidarymo_trukme 2500 // MV_full_opening_duration
unsigned long PV_pilno_atidarymo_laikas; //MV_full_opening_time
unsigned long PV_darinejimo_laikas; // MV_opening_time
/* ---ACTIVATION OF THE MIXING VALVE PUMP ..PAMAISYMO VOZTUVO SIURBLIO ĮJUNGIMAS------- */
void PamaisymoVoztuvoSiusblys(){ // subroutine Mixing Valve Pump 
if ((KI > pv_ON_T || AV > pv_ON_T) && (PV_siurblys == false)){ // If the boiler outlet temperature rises to the set temperature .. Jei katilo išėjime temperatūra pakyla iki užduotos
    digitalWrite(PVsiurblys, LOW); // the pump is switched on .. siurblys įjungiamas
    PV_siurblys = true; //  (MV_pump) indicates that the pump is running .. pažymima, kad siurblys veikia
#ifdef DEBUGpv
Serial.print("MV switch-on temperature- ");Serial.print(pv_ON_T);Serial.print(char(186));Serial.println("C");
Serial.println("Mixing valve pump ON *** ON ***");
#endif
}
    if ((KI < pv_OFF_T) && (AV < pv_OFF_T) && (PV_siurblys == true)){ // If the boiler outlet temperature falls below the set temperature .. Jei katilo išėjime temperatūra nukrenta žemiau užduotos
        digitalWrite(PVsiurblys, HIGH); // the pump is switched off .. siurblys išjungiamas
        PV_siurblys = false; // (MV_pump) indicates that the pump is not running .. pažymima, kad siurblys neveikia
#ifdef DEBUGpv
Serial.print("MV cut-off temperature- ");Serial.print(pv_OFF_T);Serial.print(char(186));Serial.println("C");
Serial.println("Mixing valve pump OFF *** OFF ***");
#endif
    }
}

/* ******************************************************************************* */
void PamaisymoVoztuvoDarbas() // subroutine Mixing Valve Operation
  { 
// If the boiler outlet is higher than the setting for maintaining the mixing valve, .. Jei katilo išėjime daugiau, negu nustatyta pamaišymo vožtuvo palaikymui,
// opening or closing the valve .. vožtuvas A_T_I_D_A_R_I_N_E_J_A_M_A_S arba U_Z_D_A_R_I_N_E_J_A_M_A_S

   if (KI > pv_palaikoma_T || AV > pv_palaikoma_T)   { 
//  Serial.println("Pamaisymo voztuvo darbas, nes KI > pv_palaikoma_T || AV > pv_palaikoma_T");    
/* ********* JEI TEMPERATŪRA KRINTA IR VOŽTUVĄ REIKIA ATIDARYTI ***************** */
/* ********* IF THE TEMPERATURE DROPS AND THE VALVE NEEDS TO BE OPENED ***************** */       
    if (PV <= pv_palaikoma_T - pv_palaikoma_riba_T && PV_stop == true && PV_uzdarinejamas == false && millis() > PV_atidarinejimo_laikas){
pv_klaida = pv_palaikoma_T - pv_palaikoma_riba_T - PV;
if (pv_klaida <= 2){ 
        PV_stop = false; // we note that the valve is already moving .. pasižymime, kad vožtuvas jau juda
        PV_atidarinejamas = true; // note that the valve is already opening .. pažymima, kad vožtuvas jau atidarinėjamas
        PV_pauze = millis() + (PV_darinejimas * 100) - 2000 + (pv_klaida * 1000);
//        PV_atidarinejimo_laikas = millis() + PV_atidarinejimo_pertrauka; // remembering the time when the opening is activated .. atsimenamas atidarymo įjungimo laikas
//        PV_atidarytas = true; // indicates that the valve is no longer closed .. pažymima, kad vožtuvas nebeuždarytas
        digitalWrite(PVuzdarymas, HIGH); // signal to close the valve .. signalas vožtuvui uždaryti
        digitalWrite(PVatidarymas, LOW); // signal to open the valve .. signalas vožtuvui atidaryti
#ifdef DEBUGpv
Serial.print("MV_error= ");  Serial.println(pv_klaida);
Serial.print("Time= ");  Serial.print(millis() / 1000); Serial.println(" s.");
Serial.print("MV_opening_time- ");  Serial.print((PV_darinejimas * 100 - 2000 + (pv_klaida * 1000)) / 1000); Serial.println(" s., shortened");
Serial.println("Mixing valve, started OPENING, shortened"); 
#endif
}else{
        PV_stop = false; // we note that the valve is already moving
        PV_pauze = millis() + PV_darinejimas * 100;
//        PV_atidarinejimo_laikas = millis() + PV_atidarinejimo_pertrauka; // remembering the time when the opening is activated
//        PV_atidarytas = true; // indicates that the valve is no longer closed .. pažymima, kad vožtuvas nebeuždarytas
        PV_atidarinejamas = true; // indicates that the valve is now opening .. pažymima, kad vožtuvas jau atidarinėjamas
        digitalWrite(PVuzdarymas, HIGH); // signal to close the valve .. signalas vožtuvui uždaryti
        digitalWrite(PVatidarymas, LOW); // signal to open the valve .. signalas vožtuvui atidaryti
 }
#ifdef DEBUGpv
Serial.print("MV_error= ");  Serial.println(pv_klaida);
Serial.print("Time- ");  Serial.print(millis() / 1000); Serial.println(" s.");
Serial.print("The_pause_will_start_after- "); Serial.print(PV_darinejimas *100 / 1000); Serial.println(" s.");
Serial.println("Mixing valve, started OPENING"); 
#endif
   }
   /* ********* P A U S E ***************** */
// stops the adjustment of the mixing valve after each opening/closing .. sustabdomas pamaisymo vožtuvo reguliavimas po kiekvieno atidarymo/uzdarymo
   if (PV_stop == false && millis() > PV_pauze) { 
if (PV > pv_palaikoma_T + 10) {
      PV_pauze = millis(); // the length of the pause .. tiek laiko turi testis pauzė
      PV_atidarinejimo_laikas = PV_pauze; // the period of time after which the valve can start to open .. laikotarpis, po kurio vožtvas galės pradėti atsidarinėti
      PV_uzdarinejimo_laikas = PV_pauze; // the period of time after which the valve can start to close .. laikotarpis, po kurio vožtvas galės pradėti užsidarinėti
      PV_stop = true; // we note that the valve is already standing .. pasižymime, kad vožtuvas jau stovi
      PV_atidarinejamas = false; // indicates that the valve is no longer opening .. pažymima, kad vožtuvas jau nebeatidarinėjamas
      PV_uzdarinejamas = false; // indicates that the valve is no longer closing .. pažymima, kad vožtuvas jau nebeuždarinėjamas
 //       digitalWrite(PVuzdarymas, HIGH); // signal to close the valve .. signalas vožtuvui uždaryti
 //       digitalWrite(PVatidarymas, HIGH); // signal to open the valve .. signalas vožtuvui atidaryti
#ifdef DEBUGpv
Serial.print("Time- ");Serial.print(millis() / 1000);Serial.println(" s.");
Serial.println("MV PAUSE, will last 0 s., temperature maintained too high");
#endif#endif
} else {
//      PV_pauze = millis() + PV_pauzes_pertrauka; // this is how long the pause has to last .. tiek laiko turi testis pauzė
      PV_atidarinejimo_laikas = millis() + (PV_pauzes_pertrauka * 100); // this is how long the pause has to last .. tiek laiko turi testis pauzė
      PV_uzdarinejimo_laikas = millis() + (PV_pauzes_pertrauka * 100); // this is how long the pause has to last .. tiek laiko turi testis pauzė
      PV_stop = true; // we note that the valve is already standing .. pasižymime, kad vožtuvas jau stovi
      PV_atidarinejamas = false; // indicates that the valve is no longer opening .. pažymima, kad vožtuvas jau nebeatidarinėjamas
      PV_uzdarinejamas = false; // indicates that the valve is no longer closing .. pažymima, kad vožtuvas jau nebeuždarinėjamas
        digitalWrite(PVuzdarymas, HIGH); // signal to stop the valve .. signalas vožtuvui sustabdyti
        digitalWrite(PVatidarymas, HIGH); // signal to stop the valve .. signalas vožtuvui sustabdyti
#ifdef DEBUGpv
Serial.print("Time- ");Serial.print(millis() / 1000);Serial.println(" s.");
Serial.print("valve movement will start after- "); Serial.print(PV_pauzes_pertrauka * 100 / 1000); Serial.println(" s.");
#endif
}

   }
/* ********* JEI TEMPERATŪRA KYLA IR VOŽTUVĄ REIKIA UŽDARYTI ***************** */
   // IF THE TEMPERATURE RISES AND THE VALVE NEEDS TO BE CLOSED
 //Serial.print("pv_palaikoma_T + pv_palaikoma_riba_T= ");  Serial.println(pv_palaikoma_T + pv_palaikoma_riba_T);  
    if (PV >= pv_palaikoma_T + pv_palaikoma_riba_T && PV_stop == true && PV_atidarinejamas == false  && millis() > PV_uzdarinejimo_laikas){
      pv_klaida = PV - pv_palaikoma_T - pv_palaikoma_riba_T;
      if (pv_klaida < 2){
        PV_stop = false; // we note that the valve is already moving .. pasižymime, kad vožtuvas jau juda
        PV_uzdarinejamas = true;
//        PV_uzdarinejimo_laikas = millis() + PV_uzdarinejimo_pertrauka; // remembering the time when the opening is activated .. atsimenamas atidarymo įjungimo laikas
        PV_pauze = millis() + (PV_darinejimas * 100) - 2000 + (pv_klaida * 1000);
        PV_atidarytas = true; // indicates that the valve is no longer closed .. pažymima, kad vožtuvas nebeuždarytas
      digitalWrite(PVatidarymas, HIGH); // stop opening .. stabdomas atidarymas
      digitalWrite(PVuzdarymas, LOW); // closure begins .. pradedamas uždarymas
#ifdef DEBUGpv
Serial.print("pv_klaida= ");  Serial.println(MV_error);
Serial.print("Time- ");  Serial.print(millis() / 1000); Serial.println(" s.");
Serial.print("The pause will start after- ");  Serial.print((PV_darinejimas * 100 - 2000 + (pv_klaida * 1000)) / 1000); Serial.println(" s., Short version");
Serial.println("Mixing valve, started CLOSED, shortened"); 
#endif
      }else{
        PV_stop = false; // we note that the valve is already moving .. pasižymime, kad vožtuvas jau juda
        PV_uzdarinejamas = true;
//        PV_uzdarinejimo_laikas = millis() + PV_uzdarinejimo_pertrauka; // remembering the time when the opening is activated .. atsimenamas atidarymo įjungimo laikas
        PV_pauze = millis() + PV_darinejimas * 100;
        PV_atidarytas = true; // indicates that the valve is no longer closed .. pažymima, kad vožtuvas nebeuždarytas
      digitalWrite(PVatidarymas, HIGH); // stop opening .. stabdomas atidarymas
      digitalWrite(PVuzdarymas, LOW); // closure begins .. pradedamas uždarymas
#ifdef DEBUGpv
Serial.print("pv_klaida= ");  Serial.println(MV_error);
Serial.print("Time- ");  Serial.print(millis() / 1000); Serial.println(" s.");
Serial.print("The pause will start after- ");  Serial.print(PV_darinejimas * 100 / 1000); Serial.println(" s.");
Serial.println("Mixing valve, started CLOSING"); 
#endif
      }

    }
   }
  }
