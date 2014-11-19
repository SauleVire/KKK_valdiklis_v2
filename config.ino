
boolean LoadConfig(){
  if ((EEPROM.read(0) == 27) && (EEPROM.read(1) == 28) && 
     (EEPROM.read(2) == 13) && (EEPROM.read(3) == 18)) {
    // Comprobamos que la eeprom tenga una 
    // configuracion valida con numeros concretos
    // solo cargamos el valor de la configuracion si los valores coinciden
    if (EEPROM.read(4) == EEPROM.read(5)) k_skirtumas_OFF = EEPROM.read(4);  
    if (EEPROM.read(6) == EEPROM.read(7)) k_skirtumas_ON = EEPROM.read(6);
    if (EEPROM.read(8) == EEPROM.read(9)) k_uzsalimas = EEPROM.read(8);
    if (EEPROM.read(10) == EEPROM.read(11)) lcd_pasvietimas = EEPROM.read(10);
//_____________ Akumuliacine talpa __________________
    if (EEPROM.read(12) == EEPROM.read(13)) at_ON_T = EEPROM.read(12);
    if (EEPROM.read(14) == EEPROM.read(15)) at_OFF_T = EEPROM.read(14);
//_____________ Boileris __________________
    if (EEPROM.read(16) == EEPROM.read(17)) b_ON_T = EEPROM.read(16);
    if (EEPROM.read(18) == EEPROM.read(19)) b_OFF_T = EEPROM.read(18);
//_____________ Pamaišymo vožtuvas __________________
    if (EEPROM.read(20) == EEPROM.read(21)) pv_palaikoma_T = EEPROM.read(20);  // Pamaišymo vožtuvo palaikoma temperatūra
    if (EEPROM.read(22) == EEPROM.read(23)) pv_ON_T = EEPROM.read(22);  // Pamaišymo vožtuvo palaikoma temperatūra
    if (EEPROM.read(24) == EEPROM.read(25)) pv_OFF_T = EEPROM.read(24);  // Pamaišymo vožtuvo siurblio išjungimo temperatūra
    if (EEPROM.readFloat(26) == EEPROM.readFloat(30)) pv_palaikoma_riba_T = EEPROM.readFloat(26); // Pamaišymo vožtuvo palaikomos temperatūros riba
    if (EEPROM.read(34) == EEPROM.read(35)) PV_pauzes_pertrauka = EEPROM.read(34);  // Pamaišymo vožtuvo pertraukos laikas po judesio
    if (EEPROM.read(36) == EEPROM.read(37)) PV_darinejimas = EEPROM.read(36);  // Pamaišymo vožtuvo pertraukos laikas po judesio
    if (EEPROM.read(38) == EEPROM.read(39)) PV_rezimas = EEPROM.read(38);  //pamaišymo vožtuvo ir siurblio rankinis arba automatinis darbas 
    if (EEPROM.read(40) == EEPROM.read(41)) SK_apsauga_nuo_salcio = EEPROM.read(38);  //SK apsauga nuo užšalimo 
//_____________ Aliarmas 1 __________________   
    if (EEPROM.read(42) == EEPROM.read(43)) Aliarmas_1_hh = EEPROM.read(42);  //Aliarmas 1 valanda ijungimui
    if (EEPROM.read(44) == EEPROM.read(45)) Aliarmas_1_mm = EEPROM.read(44);  //Aliarmas 1 minute ijungimui
    if (EEPROM.read(46) == EEPROM.read(47)) Aliarmas_1_on = EEPROM.read(44);  //Aliarmas 1 ijungtas-1 , isjungtas-2
    
    
    return true;
  }
  return false;
}

void SaveConfig(){
  EEPROM.write(0,27);
  EEPROM.write(1,28);
  EEPROM.write(2,13);
  EEPROM.write(3,18);
  // Ponemos nmeros concretos en el comienzo 
  // de la EEPROM para confirmar que tiene valores correctos.
  EEPROM.write(4,k_skirtumas_OFF);EEPROM.write(5,k_skirtumas_OFF);  // almacenamos los valores 2 veces
  EEPROM.write(6,k_skirtumas_ON); EEPROM.write(7,k_skirtumas_ON);  // almacenamos los valores 2 veces
  EEPROM.write(8,k_uzsalimas); EEPROM.write(9,k_uzsalimas);  // almacenamos los valores 2 veces
  EEPROM.write(10,lcd_pasvietimas); EEPROM.write(11,lcd_pasvietimas);  // almacenamos los valores 2 veces
//_____________ Akumuliacine talpa __________________
  EEPROM.write(12,at_ON_T); EEPROM.write(13,at_ON_T);  // AT siurblio ijungimo temperatura
  EEPROM.write(14,at_OFF_T); EEPROM.write(15,at_OFF_T);  // AT siurblio isjungimo temperatura
//_____________ Boileris __________________
  EEPROM.write(16,b_ON_T); EEPROM.write(17,b_ON_T);  // Boilerio siurblio ijungimo temperatura
  EEPROM.write(18,b_OFF_T); EEPROM.write(19,b_OFF_T);  // Boilerio siurblio isjungimo temperatura
  //_____________ Pamaišymo vožtuvas __________________
  EEPROM.write(20,pv_palaikoma_T); EEPROM.write(21,pv_palaikoma_T);  // Pamaišymo vožtuvo palaikoma temperatūra
  EEPROM.write(22,pv_ON_T); EEPROM.write(23,pv_ON_T);  // Pamaišymo vožtuvo palaikoma temperatūra
  EEPROM.write(24,pv_OFF_T); EEPROM.write(25,pv_OFF_T);  // Pamaišymo vožtuvo siurblio išjungimo temperatūra
  EEPROM.writeFloat(26,pv_palaikoma_riba_T);EEPROM.writeFloat(30,pv_palaikoma_riba_T);  // Pamaišymo vožtuvo palaikomos temperatūros riba
  EEPROM.write(34,PV_pauzes_pertrauka); EEPROM.write(35,PV_pauzes_pertrauka);  // Pamaišymo vožtuvo pertraukos laikas po judesio
  EEPROM.write(36,PV_darinejimas); EEPROM.write(37,PV_darinejimas);  // Pamaišymo vožtuvo pertraukos laikas po judesio
  EEPROM.write(38,PV_rezimas); EEPROM.write(39,PV_rezimas); //pamaišymo vožtuvo ir siurblio rankinis arba automatinis darbas
  EEPROM.write(40,SK_apsauga_nuo_salcio); EEPROM.write(41,SK_apsauga_nuo_salcio); //SK apsauga nuo užšalimo 
//_____________ Aliarmas 1 __________________   
  EEPROM.write(42,Aliarmas_1_hh); EEPROM.write(43,Aliarmas_1_hh);  //Aliarmas 1 valanda ijungimui
  EEPROM.write(44,Aliarmas_1_mm); EEPROM.write(45,Aliarmas_1_mm);  //Aliarmas 1 minute ijungimui
  EEPROM.write(46,Aliarmas_1_on); EEPROM.write(47,Aliarmas_1_on);  //Aliarmas 1 ijungtas-1 , isjungtas-2
  
}
