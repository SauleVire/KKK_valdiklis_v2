// Temperatūrų matavimas suskirstytas į tris lygius.
// pirmas lygis bus šilumos generatoriai- Katilo išėjimas, Saulės kolektorius
// ir Pamaišymo vožtuvas
// antras lygis bus šilumos imtuvai- Akumuliacinės viršaus ir Boilerio viršaus davikliai
// trečias lygis bus visi likę davikliai
// Temperatūra matuojama 3 skirtingais laiko intervalais:
// #define temperaturu_matavimo_pertrauka_1 10000
// #define temperaturu_matavimo_pertrauka_2 30000
// #define temperaturu_matavimo_pertrauka_3 60000
/* +++++++++++++++++++++++++++ PIRMAS LYGIS ++++++++++++++++++++++++++++++++++++ */ 
void Temperaturu_matavimas_1(){
  //____________________________ Start Sensor 1 _________________________________
#ifdef SetWaitForConversionFALSE
  KI_sensor.setWaitForConversion(false);  // makes it async
#endif
  KI_sensor.requestTemperatures(); // Send the command to get temperatures
  KI=KI_sensor.getTempCByIndex(0);
//_____________________________ Stop Sensor 1 ___________________________________
  //______________________ Start Sensor 3 ________________________________________
  #ifdef SetWaitForConversionFALSE
  PV_sensor.setWaitForConversion(false);  // makes it async
#endif
  PV_sensor.requestTemperatures(); // Send the command to get temperatures
PV=PV_sensor.getTempCByIndex(0);
//___________________ Stop Sensor 3 ______________________________________________
//__________________________________________ Start Sensor 2 _____________________
#ifdef SetWaitForConversionFALSE
  SK_sensor.setWaitForConversion(false);  // makes it async
#endif
  SK_sensor.requestTemperatures(); // Send the command to get temperatures
  SK=SK_sensor.getTempCByIndex(0);
//_____________________________________ Stop Sensor 2 ____________________________
}
/* +++++++++++++++++++++++++++ ANTRAS LYGIS ++++++++++++++++++++++++++++++++++++ */ 
void Temperaturu_matavimas_2(){
//_______________________________ Start Sensor 4 _________________________________
#ifdef SetWaitForConversionFALSE
  AV_sensor.setWaitForConversion(false);  // makes it async
#endif
  AV_sensor.requestTemperatures(); // Send the command to get temperatures
AV=AV_sensor.getTempCByIndex(0);
//__________________________ Stop Sessor 4 _______________________________________
//____________________________ Start Sensor 7_____________________________________
#ifdef SetWaitForConversionFALSE
  BV_sensor.setWaitForConversion(false);  // makes it async
#endif
  BV_sensor.requestTemperatures(); // Send the command to get temperatures
BV=BV_sensor.getTempCByIndex(0);
//_____________________________ Stop Sensor 7 ____________________________________   
}
/* +++++++++++++++++++++++++++ TREČIAS LYGIS ++++++++++++++++++++++++++++++++++++ */
void Temperaturu_matavimas_3(){
//__________ Start Sensor 5 ______________________________________________________
#ifdef SetWaitForConversionFALSE
  AA_sensor.setWaitForConversion(false);  // makes it async
#endif
  AA_sensor.requestTemperatures(); // Send the command to get temperatures
AA=AA_sensor.getTempCByIndex(0);
//______________ Stop Sensor 5 ___________________________________________________   
//___________________________ Start Sensor 6 _____________________________________
#ifdef SetWaitForConversionFALSE
  KT_sensor.setWaitForConversion(false);  // makes it async
#endif
  KT_sensor.requestTemperatures(); // Send the command to get temperatures
KT=KT_sensor.getTempCByIndex(0);
//________________________ Stop Sensor 6 _________________________________________   
//_______________ Start sensor 8__________________________________________________
#ifdef SetWaitForConversionFALSE
  BA_sensor.setWaitForConversion(false);  // makes it async
#endif
  BA_sensor.requestTemperatures(); // Send the command to get temperatures
BA=BA_sensor.getTempCByIndex(0);
//__________________Stop Sensor 8 _______________________________________________   
//_________ Start sensor 9 ______________________________________________________
#ifdef SetWaitForConversionFALSE
  LT_sensor.setWaitForConversion(false);  // makes it async
#endif
  LT_sensor.requestTemperatures(); // Send the command to get temperatures
LT= LT_sensor.getTempCByIndex(0);
//______________ Stop Sensor 9 _________________________________________________   
}

