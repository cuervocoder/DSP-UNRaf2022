int valSin; //Almacena el valor de la senal de salida, en valores de 0 a 255 (8bit)
float EMA_ALPHA = 0.3;
int EMA_LP = 0;
int values[360];
int valuesLength = sizeof(values) / sizeof(int);
int getMeasure() //Funcion que consigue el valor An-1, del array de muestras de la señal
{
   int static index = 0;
   index++;
   return values[index - 1];
}

void setup() {
  Serial.begin(230400);
  DDRD = B11111111; //El registro DDR, determina si el pin es una entrada o una salida (1 salida, 0 entrada)
}
 
void loop() {
  
    for (int i = 0; i < 360; i++) { //i representa un angulo que varia entre 0 y 360.
    valSin = ((((0.54*sin(6.562 * i * DEG_TO_RAD)+       //Componente senodial 1
                (0.19*sin(19.688 *i * DEG_TO_RAD))+    //Componente senodial 2
                (0.10*sin(32.813 *i * DEG_TO_RAD))) + 1) * 255) / 2);  //Componente senodial 3
    values[i] = valSin;
    }
  
  for (int iCount = 0; iCount < valuesLength; iCount++)
   {
      int value = getMeasure(); //La funcion nos devuelve el valor An-1 de la señal.
      int filteredLP = EMALowPassFilter(value); //Llamamos a los filtros y le pasamos
      //Serial.print(value);
      //PORTD = value;
      PORTD = filteredLP;
      Serial.println(analogRead(A1));
   }
  delay(99999);
}

int EMALowPassFilter(int value)     //Funcion filtro pasa bajo, recibe una muestra de la señal como parametro
{
   EMA_LP = EMA_ALPHA * value + (1 - EMA_ALPHA) * EMA_LP;
   return EMA_LP;
}
