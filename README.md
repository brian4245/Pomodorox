# Pomodorox
Pomodorox is a project that helps to the high school´s students for studying without distractions
# Motivation
Well, I did this project because I realized that many people try to study but many times they don´t study, and it is because I use a laptop or a smarthphone to use a technique for study, but many times these technology products give us many distractions, and the better option to use could be an analogyc product that you can use to study.
![image alt](https://github.com/brian4245/Pomodorox/blob/08eb7784531f2145c11373bd9231dea07e133b54/3dDesign.png)
# Instructions to build it
After all I know you the functions that I used for the circuit, I used the default functions like setup and loop. In adittion I use these functions:
void ultrasonico(); this function is to start the work of ultrasonic distance sensor, it starts when you press the red button in the IR remote, and it detects if you are near or far, and if you near the circuit start to works as a pomodoro.
``` C++
void reset(); //This one is to reset the values of time at the pomodoro (twenty five minutes).
void reloj(); //This another one is to start the clock pomodoro for study time.
void descanso(); //This one is to start the break (five minutes).
void start(); //This one is to start the words that going to give you a gretting.
void plazos(); // With this one you can decides if you wanna continue with the break or with the focus time.
```

Well, first you need to configure you ultrasonic distance sensor for detects the distance in a well way. I this step I use this code to configure it: 
``` C++
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
```
I use a function to become the code more versatil. Oh, before it you need placed the pins at setup, like this:
```C++
pinMode(trigpin, OUTPUT);
pinMode(echopin, INPUT);
Variables:
int trigpin = 4;
int echopin = 5;
```
Well after it, now you need to configure the IR remote and the IR sensor to POMODORO going to able to detect you position in yout desktop and If you really study. For it, I use this variable:
```C++
int IRD = 3;
```
Then I placed it at setup to begin the IR sensor:
```C++
  IrReceiver.begin(IRD);
```
Well, after it the third step is to configure the button that you use to start the system of POMODORO, I placed the code in loop:
```C++
 int IRcode = 0; 
  if(IrReceiver.decode()){
    IRcode = IrReceiver.decodedIRData.command; 
    Serial.println(IRcode);
    IrReceiver.resume();
  }
```
Well I declared " int IRcode = 0 " to avoid some mistkae with the control at the moment I don´t need it more, and I place the conditional to start the IR sensor for the code, IRcode is the Data number that the IRremote give us, Then I print it in monitor serial, after it I reset the IRcode, to start all again.
```C++
 if (IRcode == 162){
    startcontrol = true; <= this function is to start the greeting at the display
    ultrasonicocontrol = true; <= this one is to start the ultrasonic distance to POMODORO can see if you are near (1m).
    yasepuede = true; <= this one is for the POMODORO can understand that you are ready.
  }
```

### ¿How I use it?
It´s very easy to use, the only thing that you do is press the red button in the IR remote control when you are near (1m) to pomodorox. After it, the scren give you some instructions to continue the pomodoro (the button green is for continue studying and the other button green is for the break).
## Materials ##
- 1 Arduino uno
- 1 resistor
- 1 led
- 2 push buttons 
- 1 Ultrasonic distance sensor
-  1 IR sensor
-  1 IR remote
## Schematic ##
![image alt](https://github.com/brian4245/Pomodorox/blob/78f34b7c5a06dd28907c09dd0d82df8b5d0ea1a8/SCHEMATICCIRCUIT.png)
## Magazine ##
![image alt](https://github.com/brian4245/Pomodorox/blob/c3ee0a976004a3a642ec7369a8749fe598ded15f/Magazine.png)

