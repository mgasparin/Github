//Projeto Utiliza Arduino MEGA 2560
#include <Thermistor.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); //Define os pinos que serão ligados ao LCD
byte a[8]= {B00110,B01001,B00110,B00000,B00000,B00000,B00000,B00000,}; //Array que desenha o simbolo de grau
 
Thermistor temp(0);
int thermistorPin = A0; //Thermistor
int LDRPin = A2; //Porta analógica utilizada pelo LDR  
 
int temperatura = 0; //TEMPERATURA AMBIENTE
int temperaturaMedia = 0; //TEMPERATURA DE ACIONAMENTO
int tolerancia = 2; //EM GRAUS
int coolerPin = 7;
int buzzerPin = 8;

int ledR = 31;
int ledG = 33;
int ledB = 35;

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
  
  Serial.begin(9600);
  
  temperaturaMedia = getTempThermistor(); //inicializa a temperatura media com a temperatura ambiente
}

void loop() {
  //LER VALORES
  if(digitalRead(bt3Pin) == HIGH && digitalRead(bt2Pin) == HIGH) {         // check if the input is HIGH (button released)
    //Confgura a tolerancia
    tolerancia = getTolerancia(tolerancia);
    delay(500);
  }
  if(digitalRead(bt1Pin) == HIGH) {         // check if the input is HIGH (button released)
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
    digitalWrite(coolerPin, LOW); 
    //BLUE
    digitalWrite(ledB, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);               // wait for a second
    digitalWrite(ledB, LOW);    // turn the LED off by making the voltage LOW
    digitalWrite(buzzerPin, LOW);    
    delay(1000);    
  }

  if (temperatura > temperaturaMedia && temperatura < temperaturaMedia+tolerancia) {
    Serial.println("### AGRADAVEL ###\n");    
    lcd.setCursor(13,0);
    lcd.print(temperatura);
    lcd.setCursor(0,1);
    lcd.print("## AGRADAVEL ## ");
    //GREEN
    digitalWrite(ledG, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(500);               // wait for a second
    digitalWrite(ledG, LOW);    // turn the LED off by making the voltage LOW
    digitalWrite(buzzerPin, LOW);
    delay(500);     
  }

  if (temperatura >= temperaturaMedia+tolerancia) {
    Serial.println("$$$ CALOR $$$\n");
    lcd.setCursor(13,0);
    lcd.print(temperatura);    
    lcd.setCursor(0,1);
    lcd.print("$$ CALOR $$     ");
    //RED
    digitalWrite(ledR, HIGH);   // turn the LED on (HIGH is the voltage level)

    digitalWrite(buzzerPin, HIGH); //Liga o BUZZER
//    int LDRval = analogRead(LDRPin); //LE valor do LDR(sensor de LUZ)
//    analogWrite(buzzerPin, LDRval); //liga o buzzer com base no valor do LDR
    delay(100);               // wait for a second
    digitalWrite(ledR, LOW);    // turn the LED off by making the voltage LOW
    delay(100);               // wait for a second
    
    //LIGAR COOLER
//    if(coollerPin == LOW)
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

  delay(500);
  while(val == HIGH) {
    if(digitalRead(bt2Pin) == HIGH) {
      tempMedLocal--;
      Serial.println(tempMedLocal);
      lcd.setCursor(13,1);
      lcd.print(tempMedLocal);        
      delay(200);        
    } 
    if(digitalRead(bt3Pin) == HIGH) {
      tempMedLocal++;
      Serial.println(tempMedLocal);
      lcd.setCursor(13,1);
      lcd.print(tempMedLocal);                
      delay(200);
    }
    if(digitalRead(bt1Pin) == HIGH) {
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
    
  delay(500);
  while(val2 == 1) {
    if(digitalRead(bt2Pin) == HIGH) {
      tolLocal--;
      Serial.println(tolLocal);
      lcd.setCursor(13,1);
      lcd.print(tolLocal);        
      delay(200);        
    } 
    if(digitalRead(bt3Pin) == HIGH) {
      tolLocal++;
      Serial.println(tolLocal);
      lcd.setCursor(13,1);
      lcd.print(tolLocal);                
      delay(200);
    }
    if(digitalRead(bt1Pin) == HIGH) {
      val2 = 0;
    }      
  }//while
  
  Serial.println("*C");
  Serial.println("");            
  
  return(tolLocal);
}//getTolerancia
