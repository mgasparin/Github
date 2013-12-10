//Projeto Utiliza Arduino MEGA 2560
#include <Thermistor.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 37, 35, 33, 31); //Define os pinos que serão ligados ao LCD
byte a[8]= {B00110,B01001,B00110,B00000,B00000,B00000,B00000,B00000,}; //Array que desenha o simbolo de grau
 
Thermistor temp(0);
int thermistorPin = A0; //Thermistor
int LDRPin = A2; //Porta analógica utilizada pelo LDR  
 
int temperatura = 0; //TEMPERATURA AMBIENTE
int temperaturaMedia = 0; //TEMPERATURA DE ACIONAMENTO
int tolerancia = 2; //EM GRAUS
int coolerPin = 7;
int buzzerPin = 8;

int ledR = 4;
int ledG = 3;
int ledB = 2;

int bt1Pin = 41;
int bt2Pin = 43;
int bt3Pin = 45;

void setup() {                
  pinMode(ledR, OUTPUT);     
  pinMode(ledG, OUTPUT);     
  pinMode(ledB, OUTPUT);     
  pinMode(coolerPin, OUTPUT);     
  pinMode(buzzerPin, OUTPUT);     
  
  pinMode(bt1Pin, INPUT);    // declare pushbutton as input
  pinMode(bt2Pin, INPUT);
  pinMode(bt3Pin, INPUT);
  
  //LCD
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  lcd.begin(16, 2); //Inicializa 
  lcd.print("Temperatura: ");
  lcd.createChar(1, a); //Atribui a "1" o valor do array "A", que desenha o simbolo de grau
  lcd.setCursor(15,0);
  lcd.write(1);
  
  Serial.begin(9600); //Serial Monitor
  Serial1.begin(9600); //BT
//  String command = "";
  
  temperaturaMedia = getTempThermistor(); //inicializa a temperatura media com a temperatura ambiente
}

void loop() {
  //LER VALORES
  char valBT = 0; //BlueTooth
  if (Serial1.available() > 0) {
    valBT = Serial1.read();
    //Serial.println(valBT);
      if (valBT != '0') {
        Serial.print("BT: ");      
        Serial.println(valBT);
      }
  }  
  if((digitalRead(bt3Pin) == HIGH && digitalRead(bt2Pin) == HIGH) || (valBT == '1')) {         // check if the input is HIGH (button released)
    //Confgura a tolerancia
    tolerancia = getTolerancia(tolerancia);
    delay(500);
  }
  if((digitalRead(bt1Pin) == HIGH) || (valBT == '2')) {         // check if the input is HIGH (button released)
    //Configura a tolerancia media para acionar o sistema
    temperaturaMedia = getTemperaturaMedia(temperaturaMedia);
    delay(500);
  }
  
  temperatura = getTempThermistor(); //LE a temperatura atual.
  
  if (temperatura <= temperaturaMedia) {
    Serial.println("*** FRIO ***\n");
    lcd.setCursor(13,0);
    lcd.print(temperatura);
    lcd.setCursor(0,1);
    lcd.print("** FRIO **      ");
    //BLUE
//    digitalWrite(ledB, HIGH);
//    digitalWrite(ledR, LOW);
//    digitalWrite(ledG, LOW);
    analogWrite(ledB, map(analogRead(LDRPin), 0, 1023, 0, 255));
    analogWrite(ledR, 0);
    analogWrite(ledG, 0);
    digitalWrite(buzzerPin, LOW);    
    digitalWrite(coolerPin, LOW);     
  }

  if (temperatura > temperaturaMedia && temperatura < temperaturaMedia+tolerancia) {
    Serial.println("### AGRADAVEL ###\n");    
    lcd.setCursor(13,0);
    lcd.print(temperatura);
    lcd.setCursor(0,1);
    lcd.print("## AGRADAVEL ## ");
    //GREEN
//    digitalWrite(ledG, HIGH);
//    digitalWrite(ledR, LOW);
//    digitalWrite(ledB, LOW);
    analogWrite(ledG, map(analogRead(LDRPin), 0, 1023, 0, 255));
    analogWrite(ledR, 0);
    analogWrite(ledB, 0);
    digitalWrite(buzzerPin, LOW);    
  }

  if (temperatura >= temperaturaMedia+tolerancia) {
    Serial.println("$$$ CALOR $$$\n");
    lcd.setCursor(13,0);
    lcd.print(temperatura);    
    lcd.setCursor(0,1);
    lcd.print("$$ CALOR $$     ");
    //RED
//    digitalWrite(ledR, HIGH);
//    digitalWrite(ledB, LOW);
//    digitalWrite(ledG, LOW);
    analogWrite(ledR, map(analogRead(LDRPin), 0, 1023, 0, 255));
    analogWrite(ledB, 0);
    analogWrite(ledG, 0);
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(coolerPin, HIGH); 
    Serial.println("COOLER Ligado!");
  }

}//FIM LOOP



//FUNCOES DO SISTEMA
int getTempThermistor() {
//  //Ler termistor  
//  int thermistorReading = analogRead(thermistorPin); 
//  Serial.println(thermistorReading);
//  delay(500); //just here to slow down the output for easier reading

  //Leitura pela biblioteca
  int tempLocal = temp.getTemp();

  Serial.print("Temperatura: ");
  Serial.print(tempLocal);
  Serial.println("*C");
  Serial.println("");  
  
  return(tempLocal);
}//getTempThermistor  


int getTemperaturaMedia(int tempMedia) {
  int tempMedLocal = tempMedia;
  
  Serial.print("Temperatura Media: ");
  Serial.println(tempMedLocal);    
  
  lcd.setCursor(0,1);
  lcd.print("Temp. Media: ");
  lcd.setCursor(13,1);
  lcd.print(tempMedLocal);  

  int val = 1;  // read input value
  Serial.print(val);

  char valBTLocal = 0;

  delay(500);
  while(val == HIGH) {
    if (Serial1.available() > 0) {
      valBTLocal = Serial1.read();
      if (valBTLocal != '0') {
        Serial.print("BT: ");      
        Serial.println(valBTLocal);
      }
    }  
    if((digitalRead(bt2Pin) == HIGH) || (valBTLocal == '3')) {
      tempMedLocal--;
      Serial.println(tempMedLocal);
      lcd.setCursor(13,1);
      lcd.print(tempMedLocal);        
      delay(200);        
    } 
    if((digitalRead(bt3Pin) == HIGH) || (valBTLocal == '4')) {
      tempMedLocal++;
      Serial.println(tempMedLocal);
      lcd.setCursor(13,1);
      lcd.print(tempMedLocal);                
      delay(200);
    }
    if((digitalRead(bt1Pin) == HIGH) || (valBTLocal == '9')) {
      val = LOW;
    }      
  }//while

  Serial.println("*C");
  Serial.println("");            
  
  return(tempMedLocal);
}//getTemperaturaMedia


int getTolerancia(int tol) {
  int tolLocal = tol;
  int val2 = 1;
  
  Serial.print("Tolerancia: ");
  Serial.println(tolLocal);    
  
  lcd.setCursor(0,1);
  lcd.print("Tolerancia: ");
  lcd.setCursor(13,1);
  lcd.print(tolLocal);  
  
  Serial.print(val2);

  char valBTLocal2 = 0;

  delay(500);
  while(val2 == 1) {
    if (Serial1.available() > 0) {
      valBTLocal2 = Serial1.read();
      if (valBTLocal2 != '0') {
        Serial.print("BT: ");      
        Serial.println(valBTLocal2);
      }
    }  
    if((digitalRead(bt2Pin) == HIGH) || (valBTLocal2 == '3')) {
      tolLocal--;
      Serial.println(tolLocal);
      lcd.setCursor(13,1);
      lcd.print(tolLocal);        
      delay(200);        
    } 
    if((digitalRead(bt3Pin) == HIGH) || (valBTLocal2 == '4')) {
      tolLocal++;
      Serial.println(tolLocal);
      lcd.setCursor(13,1);
      lcd.print(tolLocal);                
      delay(200);
    }
    if((digitalRead(bt1Pin) == HIGH) || (valBTLocal2 == '9')) {
      val2 = 0;
    }      
  }//while
  
  Serial.println("*C");
  Serial.println("");            
  
  return(tolLocal);
}//getTolerancia
