void LCD_T_sablonas(){
  lcd.setCursor(0,0);  lcd.print("KI/  "); 
  lcd.setCursor(5,0);  lcd.print(" SK"); if (SK_siurblys == false) lcd.write(5); 
                                         else lcd.write(1);   lcd.print("  ");
  lcd.setCursor(11,0);  lcd.print(" PV");if (PV_stop == true) lcd.print("/");
                                           else if (PV_atidarinejamas == true) lcd.write(1);
                                                  else lcd.write(5); lcd.print("     ");
  
  lcd.setCursor(0,1);  lcd.print("AV"); if (AT_siurblys == false) lcd.write(5); 
                                        else lcd.write(1);   lcd.print("  ");
  lcd.setCursor(5,1);  lcd.print(" AA/  ");
  lcd.setCursor(11,1); lcd.print(" KT/      ");
  
  lcd.setCursor(0,2);  lcd.print("BV");if (B_siurblys == false) lcd.write(5);
                                          else lcd.write(1);   lcd.print("  ");
  lcd.setCursor(5,2);  lcd.print(" BA/  ");
  lcd.setCursor(11,2); lcd.print(" LT/     ");
}

void Temperaturu_vaizdavimas(){
lcd.setCursor(3,0);  if (KI == -127) { lcd.print("!!");}
  else { lcd.print(KI,0 );  }
lcd.setCursor(8,0); if (SK_siurblys == false) lcd.write(5);  
  else lcd.write(1);
if (SK == -127) { lcd.print("!!");}
  else { lcd.print(SK,0);  }
lcd.setCursor(14,0); if (PV_siurblys == false) lcd.write(5);  
  else lcd.write(1);
lcd.setCursor(15,0); if (PV == -127) {lcd.print("!!");}
  else { lcd.print(PV,0);  } 
lcd.setCursor(2,1); if (AT_siurblys == false) lcd.write(5);  
  else lcd.write(1);
if (AV == -127) { lcd.print("!!");}
  else { lcd.print(AV,0); }// lcd.print((char)223);  }
lcd.setCursor(9,1); if (AA == -127) { lcd.print("!!");}
  else { lcd.print(AA,0);  }
lcd.setCursor(15,1); if (KT == -127) { lcd.print("!!");}
  else { lcd.print(KT,0);  }  
lcd.setCursor(2,2);  if (B_siurblys == false) lcd.write(5);
  else lcd.write(1);   lcd.print("  ");  
lcd.setCursor(3,2); if (BV == -127) {  lcd.print("!!");}
  else { lcd.print(BV,0);  }
lcd.setCursor(9,2); if (BA == -127) { lcd.print("!!");}
  else { lcd.print(BA,0);  }
lcd.setCursor(15,2); if (LT == -127) { lcd.print("!!");}
  else { lcd.print(LT,0);  }
}



