#include <Dictionary.h> //Biblioteca que permite el uso de diccionarios

int myInts[64];     //Array para almacenar 64 muestras de la señal de entrada.
int j = 0;          //Variable de control para el bucle de muestro.
int a = 7;          //Variable auxiliar para el bucle de muestro.
int b = 0;          //Variable utilizada para controlar el flujo del programa. Inicia en 0, permitiendo la ejecucion del bloque de muestreo.
                    //Una vez finalizado el muestreo se coloca en 1 y permite la ejecuccion del bloque que realiza la FFT y la transmision de la señal.


int valSin;         //Variable donde se almacena la señal reconstruida con sus tres primeras componentes, obtenidas de la funcion FFT.
                    //Representa la señal en valores del rango 0-255, para poder utilizar el PORTD(Registro 8bit)

byte sine_data [91]={
0,  
4,    9,    13,   18,   22,   27,   31,   35,   40,   44, 
49,   53,   57,   62,   66,   70,   75,   79,   83,   87, 
91,   96,   100,  104,  108,  112,  116,  120,  124,  127,  
131,  135,  139,  143,  146,  150,  153,  157,  160,  164,  
167,  171,  174,  177,  180,  183,  186,  189,  192,  195,       
198,  201,  204,  206,  209,  211,  214,  216,  219,  221,  
223,  225,  227,  229,  231,  233,  235,  236,  238,  240,  
241,  243,  244,  245,  246,  247,  248,  249,  250,  251,  
252,  253,  253,  254,  254,  254,  255,  255,  255,  255};

float f_peaks[5];                      //Array donde se almacenan las 5 principales frecuencias resultado de la FFT.
Dictionary<float, float> MyData;       //Diccionario donde se almacenan todas las frecuencias obtenidas con la FFT, junto con su valor de amplitud.

void setup() {
  Serial.begin(230400);
  DDRD = B11111111; 
}
 
void loop() {
  
  if(b == 0){                              //Inicio BLOQUE DE MUESTREO
  
  if(j>6 && j<71){                        //Se omiten las primeras 6 lecturas de la señal
    myInts[j-a] = analogRead(A1);         //Se almacenan 64 lecturas de la señal en el array myInts[]
  }
  j = j +1;
  delay(10);
      if(j == 71){                       //Una vez completado el muestreo, la variable de control b = 1, entramos en el bloque de FFT.
        b = 1;
      }
  }                                       //Fin BLOQUE DE MUESTREO
  
  if(b == 1){
    FFT(myInts,64,100);                  //Llamamos a la funcion FFT

    float a_peak1 = MyData.get(f_peaks[0])/27000;      //Con las frecuencias que nos otorga la funcion FFT,
    float a_peak2 = MyData.get(f_peaks[1])/27000;      //buscamos en nuestro diccionario sus amplitudes correspondientes y las almacenamos.
    float a_peak3 = MyData.get(f_peaks[2])/27000;      //Aplicamos un factor de correcion para expresar la amplitud en Volts y disminuirla, para no
                                                       //provocar un desbordamiento sobre los 8 bits de la salida
    
    for (int i = 0; i < 360; i++) {   //Con los datos obtenidos, reconstruimos la señal en valores entre 0 y 255, y la enviamos al PORTD
    valSin = ((((a_peak1*sin(f_peaks[0] * i * DEG_TO_RAD)+(a_peak2*sin(f_peaks[1] *i * DEG_TO_RAD))+(a_peak3*sin(f_peaks[2] *i * DEG_TO_RAD))) + 1) * 255) / 2);  //El maximo valor posible representable es 255, porque tenemos 8bits.
    PORTD = valSin;  
    Serial.println(analogRead(A0)); // Realimentamos Arduino con la señal del R2R, para visualizar en el monitor serie.
    }                               // Se podria conectar la señal del R2R a un osciloscopio, para visualizarla.
  }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float FFT(int in[],int N,float Frequency)
{

unsigned int data[13]={1,2,4,8,16,32,64,128,256,512,1024,2048};
int a,c1,f,o,x;
a=N;  
                                 
      for(int i=0;i<12;i++)                 //Calculando los niveles.
         { if(data[i]<=a){o=i;} }
      
int in_ps[data[o]]={};     //input for sequencing
float out_r[data[o]]={};   //Parte real de la transformada
float out_im[data[o]]={};  //Parte imaginaria
           
x=0;  
      for(int b=0;b<o;b++)                     //Inversión de bits.
         {
          c1=data[b];
          f=data[o]/(c1+c1);
                for(int j=0;j<c1;j++)
                    { 
                     x=x+1;
                     in_ps[x]=in_ps[j]+f;
                    }
         }

 
      for(int i=0;i<data[o];i++)            //Actualizar el array de entrada según el orden inverso de bits.
         {
          if(in_ps[i]<a)
          {out_r[i]=in[in_ps[i]];}
          if(in_ps[i]>a)
          {out_r[i]=in[in_ps[i]-a];}      
         }


int i10,i11,n1;
float e,c,s,tr,ti;

    for(int i=0;i<o;i++)                                    //fft
    {
     i10=data[i];              // valores globales de seno/coseno:
     i11=data[o]/data[i+1];    // bucle con seno/coseno similar:
     e=360/data[i+1];
     e=0-e;
     n1=0;

          for(int j=0;j<i10;j++)
          {
          c=cosine(e*j);
          s=sine(e*j);    
          n1=j;
          
                for(int k=0;k<i11;k++)
                 {
                 tr=c*out_r[i10+n1]-s*out_im[i10+n1];
                 ti=s*out_r[i10+n1]+c*out_im[i10+n1];
          
                 out_r[n1+i10]=out_r[n1]-tr;
                 out_r[n1]=out_r[n1]+tr;
          
                 out_im[n1+i10]=out_im[n1]-ti;
                 out_im[n1]=out_im[n1]+ti;          
          
                 n1=n1+i10+i10;
                  }       
             }
     }



//---> De aca en adelante out_r contiene amplitud y out_in contiene frecuencia (Hz)
    for(int i=0;i<data[o-1];i++)               // Se obtiene la amplitud desde el numero complejo.
        {
         out_r[i]=sqrt(out_r[i]*out_r[i]+out_im[i]*out_im[i]); 
         out_im[i]=i*Frequency/N;
	   
         MyData.set(out_im[i], out_r[i]); // Cargamos el diccionario con los pares frecuencia-amplitud
        }




x=0;       // Detección de picos
   for(int i=1;i<data[o-1]-1;i++)
      {
      if(out_r[i]>out_r[i-1] && out_r[i]>out_r[i+1]) 
      {in_ps[x]=i;    //in_ps array usado para almacenar los valores pico.
      x=x+1;}    
      }


s=0;
c=0;
    for(int i=0;i<x;i++)             // Reordenar por magnitud.
    {
        for(int j=c;j<x;j++)
        {
            if(out_r[in_ps[i]]<out_r[in_ps[j]]) 
                {s=in_ps[i];
                in_ps[i]=in_ps[j];
                in_ps[j]=s;}
        }
    c=c+1;
    }



    for(int i=0;i<5;i++)     // Aactualizando f_peak array (variable global) en orden descendiente.
    {
    f_peaks[i]=out_im[in_ps[i]];
    }



}
    

float sine(int i)
{
  int j=i;
  float out;
  while(j<0){j=j+360;}
  while(j>360){j=j-360;}
  if(j>-1   && j<91){out= sine_data[j];}
  else if(j>90  && j<181){out= sine_data[180-j];}
  else if(j>180 && j<271){out= -sine_data[j-180];}
  else if(j>270 && j<361){out= -sine_data[360-j];}
  return (out/255);
}

float cosine(int i)
{
  int j=i;
  float out;
  while(j<0){j=j+360;}
  while(j>360){j=j-360;}
  if(j>-1   && j<91){out= sine_data[90-j];}
  else if(j>90  && j<181){out= -sine_data[j-90];}
  else if(j>180 && j<271){out= -sine_data[270-j];}
  else if(j>270 && j<361){out= sine_data[j-270];}
  return (out/255);
}