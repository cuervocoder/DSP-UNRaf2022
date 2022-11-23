int muestra;
int tiempo;
int bit8;
void setup(){
   Serial.begin(115200);
   DDRD = B11111111;
}

void loop(){
   
  muestra = analogRead(A1);
  bit8 = muestra/4.012;
  PORTD = bit8;
  Serial.println(analogRead(A0));
  
}
