
void Akumuliacine_talpa () {
  // jei katilo išėjime yra daugiau šilumos, negu nustatyta akumuliacinės talpos šildymo įjungimo temperatūra,
  // tai įjungiamas akumuliacinės talpos siurblys
 if ((KI >= at_ON_T) && (AT_siurblys == false))  {
   digitalWrite(ATSiurblys, LOW); // Įjungiamas akumuliacinės talpos siurblys
   AT_siurblys = true;
// jei AT išjungimo temperatūra nustatoma didesnė, negu įjungimo, tai ji keičiama į mažesnę
if (at_OFF_T > at_ON_T) at_OFF_T = at_ON_T - 1;
#ifdef DEBUGakumuliacine
Serial.print("AT ijungimo temperatura- ");Serial.print(at_ON_T);Serial.print(char(186));Serial.println("C");
Serial.println("Akumuliacines talpos siublys IJUNGTAS *** ON ***");
#endif
              }
              // jei katilo išėjime yra mažiau šilumos, negu nustatyta akumuliacinės talpos šildymo išjungimo temperatūra,
              // tai išjungiamas akumuliacinės talpos siurblys
              if ((KI <= at_OFF_T) && (AT_siurblys == true)){
              digitalWrite(ATSiurblys, HIGH); // Išjungiamas akumuliacinės talpos siurblys
              AT_siurblys = false;
              #ifdef DEBUGakumuliacine
              Serial.print("AT isjungimo temperatura- ");Serial.println(at_OFF_T);Serial.print(char(186));Serial.println("C");
              Serial.println("Akumuliacines talpos siublys ISJUNGTAS *** OFF ***");
              #endif
                   } 
}
/*
// Praėjus nustatytam laikui ir jei tenkinama sąlyga išjungimui pagal temperatūras
// įrašoma žymė, kad baigėsi rankinio siurblio įjungimo veikimas
void Akumuliacine_talpa_rankinis (){
  if ((millis() < AT_rankinio_ijungimo_laikas + AT_rankinio_ijungimo_trukme ) && (KI <= at_OFF_T)){
  } else {AT_rankinis_ijungimas = false;}
}
*/
