int valSin; //Almacena el valor de la senal de salida, en valores de 0 a 255 (8bit)

void setup() {
  Serial.begin(230400);
  DDRD = B11111111; //El registro DDR, determina si el pin es una entrada o una salida (1 salida, 0 entrada)
}
 
void loop() {
  
    for (int i = 0; i < 360; i++) { //i representa un angulo que varia entre 0 y 360.
    valSin = ((((0.494*sin(9.998 * i * DEG_TO_RAD)+       //Componente senodial 1
                (0.164*sin(29.995 *i * DEG_TO_RAD))+    //Componente senodial 2
                (0.099*sin(49.992 *i * DEG_TO_RAD))) + 1) * 255) / 2);  //Componente senodial 3
     
    PORTD = valSin;
      
    Serial.println(analogRead(A1));
    }
}
