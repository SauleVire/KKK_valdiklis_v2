/* @@@@@@@@@@@@@@@@@@@@@@ testavimui @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
 #define DEBUG 
 #define SetWaitForConversionFALSE // pagreitintas ds18b20 temperatūrų matavimas
// #define DEBUGakumuliacine //akumuliacines testavimui
 #define DEBUGboileris // boilerio testavimui
// #define DEBUGkolektorius // saulės kolektoriaus testavimui
// #define DEBUGpv // pamaišymo vožtuvo testavimui
 #define DEBUGds18b20 // temperatūrų matavimo testavimui 
// #define DEBUGbusena

/* ********************** Laikai *************************************** */
unsigned long Meniu_praleistas_neaktyvus_laikas;
#define neaktyvus_meniu 20000
unsigned long temperaturu_matavimo_laikas_1 = 0;
unsigned long temperaturu_matavimo_laikas_2 = 0;
unsigned long temperaturu_matavimo_laikas_3 = 0;
unsigned long SK_siurblio_junginejimas = 0;
#define temperaturu_matavimo_pertrauka_1 5000
#define temperaturu_matavimo_pertrauka_2 7000
#define temperaturu_matavimo_pertrauka_3 9000
#define moto_voztuvu_uzdarymo_fpv_reziflaikas 5000

unsigned long Ekrano_pasvietimo_ijungimo_laikas;
unsigned long  Ekrano_pasvietimo_pertrauka = 600000;
unsigned long Ekrano_rodmenu_atnaujinimo_laikas = 0;
#define Ekrano_rodmenu_atnaujinimo_pertrauka 5000
unsigned long AT_rankinio_ijungimo_laikas =0;
unsigned long AT_rankinio_ijungimo_trukme = 300000;
unsigned long B_rankinio_ijungimo_laikas =0;
unsigned long B_rankinio_ijungimo_trukme = 300000;
unsigned long SK_rankinio_ijungimo_laikas =0;
unsigned long SK_rankinio_ijungimo_trukme = 300000;
unsigned long PV_pauze =20000;
unsigned long PV_pauzes_pertrauka =20000;
unsigned long PV_atidarinejimo_laikas =0;
unsigned long PV_uzdarinejimo_laikas =0;
unsigned long PV_atidarinejimo_pertrauka =6000;
unsigned long PV_uzdarinejimo_pertrauka =6000;
unsigned long Boilerio_siurblio_ijungimo_laikas;
unsigned long Boilerio_siurblio_pertrauka =60000;
byte PV_darinejimas = 60;
#define MV_judejimas 5500

/* ************** RTC KINTAMIEJI ************************************* */
int Sekundes; 
int Minutes; 
int Valandos;
int SavaitesDiena;
int MenesioDiena;
int Menuo;
int Metai;
/* ************** LKLAVIATUROS KINTAMIEJI ************************************* */
#define Key_Pin 3 //Analogines klaviaturos pajungimo i6vadas A3
volatile int klaviaturos_pasikeitimas =-1;               // Tikrinti ar pasikeitė klaviatūros reikšmė
volatile int x=-1;                 // 
volatile int stan_Analog;          // 
/* ********** LCD aprasymas ******************* */
#define BackLight_Pin 2 //LCD pasvietimo isvadas (standart LCD KeeyPad use pin 10)
byte lcd_pasvietimas = 10; // lcd_pasvietimas * 25 = MAX LCD apsviestumas
boolean Backlighting = true; // zyme ekrano pasvietimui
//Siurblio įjungimo simbolis (rodyklė į viršų)
byte arrow_up[8]={ B00100,B01110,B11111,B00100,B00100,B00100,B00100,B00100};
//Siurblio įšungimo simbolis (rodyklė į žemyn)
byte arrow_down[8]={ B00100,B00100,B00100,B00100,B00100,B11111,B01110,B00100};

/* ******************** reles *********************************** */
#define Relay12 31
#define B_termostatas 33 // boilerio termostatas
#define MV_K_21 35 // motorizuotas vožtuvas katiui, 2.1 Uždarymas
#define MV_K_11 37 // motorizuotas vožtuvas katiui, 1.1 Atidarymas
#define MV_A_21 39 // motorizuotas vožtuvas akumuliacinei 2.1 Uždarymas
#define MV_A_11 41 // motorizuotas vožtuvas akumuliacinei 1.1 Atidarymas
#define PVsiurblys 43 // PV siurblys
#define PVuzdarymas 45 // PV uždarymas Relė 5
#define PVatidarymas 47 // PV atidarymas Relė 4
#define BSiurblys 49 // B siurblio junginėjimui Relė 3
#define SKSiurblys 51 // SK siurblio junginėjimui Relė 2
#define ATSiurblys 53 // AT siurblio junginėjimui Relė 1

/* ************************** davikliai *********************** */
#define ONE_WIRE_BUS1 36
#define ONE_WIRE_BUS2 38
#define ONE_WIRE_BUS3 40
#define ONE_WIRE_BUS4 42
#define ONE_WIRE_BUS5 44
#define ONE_WIRE_BUS6 46
#define ONE_WIRE_BUS7 48
#define ONE_WIRE_BUS8 50
#define ONE_WIRE_BUS9 52

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire KI_t(ONE_WIRE_BUS1); //katilo isejimas
OneWire SK_t(ONE_WIRE_BUS2); //saules kolektorius
OneWire PV_t(ONE_WIRE_BUS3); //pamaisymo voztuvas
OneWire AV_t(ONE_WIRE_BUS4); //akumuliacines virsaus
OneWire AA_t(ONE_WIRE_BUS5); //akumuliacines apacia
OneWire KT_t(ONE_WIRE_BUS6); //kambario temperatura
OneWire BV_t(ONE_WIRE_BUS7); //boilerio virsus
OneWire BA_t(ONE_WIRE_BUS8); //boilerio apacia
OneWire LT_t(ONE_WIRE_BUS9); //lauko temperatura
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature KI_sensor(&KI_t);
DallasTemperature SK_sensor(&SK_t);
DallasTemperature PV_sensor(&PV_t);
DallasTemperature AV_sensor(&AV_t);
DallasTemperature AA_sensor(&AA_t);
DallasTemperature KT_sensor(&KT_t);
DallasTemperature LT_sensor(&LT_t);
DallasTemperature BV_sensor(&BV_t);
DallasTemperature BA_sensor(&BA_t);
//kintamieji saugoti ismatuotoms temperaturu reiksmems
float KI,SK,PV,AV,AA,KT,BV,BA,LT; 
/* ********** kintamieji Kieto kuro katilui ******************* */
/* ********** kintamieji Dujiniam katilui ******************* */
/* ********** kintamieji saulės kolektoriui ******************* */
byte k_skirtumas_ON = 5;
byte k_skirtumas_OFF = 3;
boolean k_uzsalimas = false;
boolean SK_rankinis_ijungimas = false; // Žymė rankiniam SK siurblio valdymui (nuorinimas)
byte SK_apsauga_nuo_salcio = 1; // 1-įjungta, 2- išjungta ,Žymė SK apsaugai nuo šalčio
/* *************************************************************** */
/* ********** kintamieji Akumuliacinei talpai ******************* */
byte at_ON_T = 90; // temperatūra akumuliacines talpos siurbliui įjungti
byte at_OFF_T = 89; // temperatūra akumuliacines talpos siurbliui įšjungti
byte AT_rankinis_ijungimas = 0; // Žymė rankiniam AT siurblio valdymui
/* ********************************************************************** */
/* ********** kintamieji Pamaišymo vožtuvui ******************* */
byte pv_palaikoma_T = 40; // PV palaikoma temperatūra 
float pv_palaikoma_riba_T = 1.5; // PV palaikomos temperatūros riba 
byte pv_ON_T = 35; // PV įjungimo temperatūra 
byte pv_OFF_T = 28; // PV išjungimo temperatūra 
byte PV_rankinis_ijungimas = 0; // Žymė rankiniam PV valdymui
float pv_klaida;
boolean PV_atidarytas = false;
boolean PV_uzdarytas = true;
boolean PV_atidarinejamas = false;
boolean PV_uzdarinejamas = false;
boolean PV_pusiausvyra = false;
boolean PV_stop = true;
byte PV_rezimas = 2; // 1- rankinis, 2- automatinis
/* ********************************************************************** */
/* ********** kintamieji Boileriui ******************* */
byte b_ON_T = 45; // temperatūra boilerio siurbliui įjungti
byte b_OFF_T = 65; // temperatūra boilerio siurbliui įšjungti
boolean B_rankinis_ijungimas = false; // Žymė rankiniam AT siurblio valdymui
boolean B_termostat_ON = false; // Žymė rankiniam termostato įjungimui
boolean B_termostat_status = false; // Žymė termostato busenai
/* ************************************************************************ */
/* ********** kintamieji siurbliams ******************* */
boolean SK_siurblys = false;
boolean PV_siurblys = false;
boolean AT_siurblys = false;
boolean B_siurblys = false;
/* ---------------- kintamieji motorizuotiems vožtuvams -------------------- */
boolean MV_K = true; //true- atidarytas , false- uždarytas
boolean MV_A = true; //true- atidarytas , false- uždarytas
