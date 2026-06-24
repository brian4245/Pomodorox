#include <Arduino.h>
#include <IRremote.h>
#include <LiquidCrystal_I2C.h>

int IRD = 3;
int trigpin = 4;
int echopin = 5;
int LEDAVISADOR = 6;
int despeje = 8;
int focus = 9;
bool ActualFocus = LOW;
bool AnteriorFocus = LOW;
bool ActualDespeje = LOW;
bool AnteriorDespeje = LOW;
unsigned long tiempoinicio = 0;
unsigned long tiempoiniciodescanso = 0;
int distance;
int duration;
bool startcontrol = false;
bool saludo = false;
bool MOTIVACION = false;
bool RELOJ = false;
bool CONTINUAR = false;
bool cronometrofocus = false;
bool cronometrodespeje = false;
bool reiniciarvariable = false;
int SEGUNDOS = 00;
int MINUTOS = 00;
int SEGUNDOSD = 00;
int MINUTOSD = 00;
bool empezar = false;
bool turnon = false;
bool descansocontrol = false;
bool ultrasonicocontrol = false;
bool controlplazos = false;
bool yasepuede = false;

LiquidCrystal_I2C lcd(0x27, 20, 4);
//FUNCIONES :V
void reset(); //This one is to reset the values of time at the pomodoro (twenty five minutes).
void reloj(); //This another one is to start the clock pomodoro for study time.
void descanso(); //This one is to start the break (five minutes).
void start(); //This one is to start the words that going to give you a gretting.
void plazos(); // With this one you can decides if you wanna continue with the break or with the focus time.

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IRD);
  pinMode(despeje, INPUT);
  pinMode(focus, INPUT);
  pinMode(trigpin, OUTPUT);
  pinMode(echopin, INPUT);
  pinMode(LEDAVISADOR, OUTPUT); 
  lcd.init();
  lcd.backlight();
  lcd.clear();
}

void loop() {
 
  // 1. CONTROL DE FIN DE TIEMPOS 
  if (MINUTOS == 25) {
    cronometrofocus = false;
    RELOJ = false;
    controlplazos = true; 
    MINUTOS = 0; // Pa que no se trabe xd
    SEGUNDOS = 0;
  }
  
  if (MINUTOSD == 5) {
    cronometrodespeje = false;
    descansocontrol = false;
    controlplazos = true;
    MINUTOSD = 0; // Pa que no se trabe xd
    SEGUNDOSD = 0;
  }

  // 2. LECTURA DE BOTONES
  ActualFocus = digitalRead(focus);
  if (ActualFocus == HIGH && AnteriorFocus == LOW && yasepuede == true) {
    reset(); // Reseteamos los tiempos justo al empezar el tramo
    RELOJ = true;
    descansocontrol = false;
    cronometrodespeje = false;
    delay(50); // Pa que no se repita
  }
  AnteriorFocus = ActualFocus;

  ActualDespeje = digitalRead(despeje);
  if (ActualDespeje == HIGH && AnteriorDespeje == LOW && yasepuede == true) {
    reset(); 
    controlplazos = false;
    descansocontrol = true;
    RELOJ = false;
    cronometrofocus = false;
    delay(50); // Pa que no se repita
  }
  AnteriorDespeje = ActualDespeje;
  
  // 3. CONTROL REMOTO INFRARROJO
  int IRcode = 0; 
  if(IrReceiver.decode()){
    IRcode = IrReceiver.decodedIRData.command; // Declared it here to avoid scope issues
    Serial.println(IRcode); // To know the code of the buttons, you can remove it later
    IrReceiver.resume(); // Receive the next value, if you don't call this, you won't be able to receive values anymore
  }
  
  if (IRcode == 162){
    startcontrol = true; // it starts the pomodoro when you press the red button at the remote control
    ultrasonicocontrol = true; // it allows the ultrasonic sensor to work when you press the red button at the remote control
    yasepuede = true; // it allows the rest of the program to work when you press the red button at the remote control
  }
  
  // 4. EJECUCIÓN DE FUNCIONES DE CONTROL
  if (controlplazos == true){
    plazos(); // it shows the message to press the buttons when a pomodoro or a break ends
    controlplazos = false; // Se ejecuta una sola vez al terminar un ciclo
  }
  
 
  
  if (startcontrol == true){
    start();
    startcontrol = false;
  }
  
  if (RELOJ == true){
    reloj();
  } 

  if (descansocontrol == true){
    descanso();
  }

  // 5. CRONÓMETROS 
  if (cronometrofocus == true){
    if (millis() - tiempoinicio >= 1000){
      tiempoinicio = millis();
      SEGUNDOS++;
      if (SEGUNDOS == 60){
        SEGUNDOS = 0;
        MINUTOS++;
      }
    }
  }
  
  if (cronometrodespeje == true){
    if (millis() - tiempoiniciodescanso >= 1000){
      tiempoiniciodescanso = millis();
      SEGUNDOSD++;
      if (SEGUNDOSD == 60){
        SEGUNDOSD = 0;
        MINUTOSD++;
      }
    }
  }
}

void reset(){
  MINUTOS = 0;
  SEGUNDOS = 0;
  MINUTOSD = 0;
  SEGUNDOSD = 0;
}

void ultrasonico(){
  digitalWrite(trigpin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigpin, LOW);
  
  duration = pulseIn(echopin, HIGH);
  distance = duration / 58.2;
  Serial.println(distance);
}

void reloj(){
  lcd.setCursor(0, 0);
  lcd.print("POMODORO");
  lcd.setCursor(0, 1);
  
  if (MINUTOS < 10) lcd.print("0"); 
  lcd.print(MINUTOS);
  lcd.print(":");
  if (SEGUNDOS < 10) lcd.print("0"); 
  lcd.print(SEGUNDOS);
  
  cronometrofocus = true;
}
  
void descanso(){
  lcd.setCursor(9, 0);
  lcd.print("DESCANSO");
  lcd.setCursor(9, 1);
  if (MINUTOSD < 10) lcd.print("0"); 
  lcd.print(MINUTOSD);
  lcd.print(":");
  if (SEGUNDOSD < 10) lcd.print("0"); 
  lcd.print(SEGUNDOSD);
  
  cronometrodespeje = true;
}

void start(){
  yasepuede = true;
  ultrasonico(); // Ejecutamos la lectura real del sensor antes de evaluar
  if (distance <= 100 && distance > 0){ 
    digitalWrite(LEDAVISADOR, HIGH);
    lcd.setCursor(0,0);
    lcd.print("EMPEZEMOS");
    delay(1000);
    lcd.clear();
    lcd.print("LET'S MOVE ON");
    delay(1000);
    lcd.clear();
    digitalWrite(bocina, LOW);
  }
}

void plazos(){
  Serial.println("ejecutado");
  lcd.setCursor(0, 2);
  lcd.print("PRESIONA R o L");
}


  

