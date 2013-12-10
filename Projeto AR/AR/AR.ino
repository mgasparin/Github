#include <Thermistor.h>
const float CELSIUS_BASE = 0.4887585532746823069403714565; //Base de conversão para Graus Celsius ((5/1023) * 100)
 
Thermistor temp(0);
int thermistorPin = A0; //Thermistor
//int lm35Pin = A0; //Sensor LM35

int potPin = A2; //Potenciometro
int potValor = 0;

int temperatura = 0;
int temperaturaMedia = 10;
int tolerancia = 3;
int coolerPin = 7;

int ledR = 2;
int ledG = 3;
int ledB = 4;

int A = 9;
int B = 10;
int C = 11;
int D = 12;
int ponto = 13; 
int seven_seg_digits[10][4] = { 
{ 0,0,0,0 }, // = 0
{ 0,0,0,1 }, // = 1
{ 0,0,1,0 }, // = 2
{ 0,0,1,1 }, // = 3
{ 0,1,0,0 }, // = 4
{ 0,1,0,1 }, // = 5
{ 0,1,1,0 }, // = 6
{ 0,1,1,1 }, // = 7
{ 1,0,0,0 }, // = 8
{ 1,0,0,1 } // = 9
};


void setup() {                
  pinMode(ledR, OUTPUT);     
  pinMode(ledG, OUTPUT);     
  pinMode(ledB, OUTPUT);     
  pinMode(coolerPin, OUTPUT);     
  pinMode(A, OUTPUT);  
  pinMode(B, OUTPUT);  
  pinMode(C, OUTPUT);  
  pinMode(D, OUTPUT);  
  pinMode(ponto, OUTPUT);  
  digitalWrite(ponto, 0);   // Ponto desligado
  
  Serial.begin(9600);
}

void loop() {
  //LER VALORES
  temperaturaMedia = getTemperaturaMedia();

  temperatura = getTempThermistor();

  //MOSTRAR VALOR DA TEMPERATURA
  int pisca = 1000;
  int dec = (temperatura/10);
  Serial.print("dec: ");  
  Serial.println(dec);    
  int num = temperatura-(dec*10);
  Serial.print("num: ");  
  Serial.println(num);    

  //NUMERO
  for(int i=0; i<=dec; i++) {
    digitalWrite(A, seven_seg_digits[num][0]);
    digitalWrite(B, seven_seg_digits[num][1]);
    digitalWrite(C, seven_seg_digits[num][2]);
    digitalWrite(D, seven_seg_digits[num][3]);
  }  
  //PONTO
  if(dec > 0) {
    if(dec > 1)
    for(int j=0; j<dec; j++) {
      digitalWrite(ponto, 0);
      delay(pisca/dec);
      digitalWrite(ponto, 1);
      delay(pisca/dec);
      digitalWrite(ponto, 0);
      //delay(pisca/dec);      
    }
    digitalWrite(ponto, 1);
  } else {
    digitalWrite(ponto, 0);     
  }
  
  
  
  if (temperatura <= temperaturaMedia) {
    Serial.println("*** FRIO ***\n");
    digitalWrite(coolerPin, LOW); 
    //BLUE
    digitalWrite(ledB, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);               // wait for a second
    digitalWrite(ledB, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);    
  }

  if (temperatura > temperaturaMedia && temperatura < temperaturaMedia+tolerancia) {
    Serial.println("### AGRADAVEL ###\n");
    //digitalWrite(coolerPin, LOW); 
    //GREEN
    digitalWrite(ledG, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(500);               // wait for a second
    digitalWrite(ledG, LOW);    // turn the LED off by making the voltage LOW
    delay(500);     
  }

  if (temperatura >= temperaturaMedia+tolerancia) {
    Serial.println("$$$ CALOR $$$\n");
    //RED
    digitalWrite(ledR, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(100);               // wait for a second
    digitalWrite(ledR, LOW);    // turn the LED off by making the voltage LOW
    delay(100);               // wait for a second
    
    //LIGAR COOLER
//    if(coollerPin == LOW)
      digitalWrite(coolerPin, HIGH); 
      Serial.println("COOLER Ligado!");
  }

  //delay(1000);
}//FIM LOOP



//Funcoes
int getTempThermistor(){
//  //Ler termistor  
//  int thermistorReading = analogRead(thermistorPin); 
//  Serial.println(thermistorReading);
//  delay(500); //just here to slow down the output for easier reading

//  //Ler Sensor LM35
//  int tempLocal = analogRead(lm35Pin) * CELSIUS_BASE;
  
  //Leitura pela biblioteca
  int tempLocal = temp.getTemp();

  Serial.print("Temperatura no Sensor eh: ");
  Serial.print(tempLocal);
  Serial.println("*C");
  Serial.println("");  
  
  return(tempLocal);
}  

int getTemperaturaMedia(){
  int potValorLocal = analogRead(potPin);      
  int tempMediaLocal = (potValorLocal/25); //convertendo o valor do POT para Temperatura. MAX 40o.C
  Serial.print("Temperatura Media: ");
  Serial.println(tempMediaLocal);
  Serial.println("");
  //Serial.println(potValor);
  
  return(tempMediaLocal);
}

