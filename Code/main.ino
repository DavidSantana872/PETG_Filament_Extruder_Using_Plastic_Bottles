/*
  * This is a program for an extructor filament created in ULSA university for
 * re-use bottle plastic material (PETG) to then use in a 3D printed.
 *
 * Autors: Ana Arce
 *         Daniel Centeno
 *         David Santana
 * 
 * Version: 1.1.0
 *
 * Pin used:
 *
 * KeyPad Pines:
 *    for rows =  13, 12, 11, 10
 *    for column = 9, 8, 7, 6
 *
 * LCD 16x2 with I2C communication model pines:
 *    for SDA = A4
 *    for SCL = A5
 *
 *Big Steeper motor (Extructor motor)
 *
 *  for motor direction = 2
 *  for signal motor = 3
 *  for Enable = 4
 *
 *Termistor
 *
 *  for termistor = A0
 *
 *PWM Output signal pin
 *
 *  for PWM_pin = 5
 *
 *Indication Pin_Leds
 *
 *  for Led_On = 1  
 *  for Led_Off = 0
 *
 *Fan Pins
 *
 *  for Fan1 = 15
 *  for fan2 = 16
 *  
 *Pot Pin for the velocity of the stepper motor  
 *
 *  for Velocity = A3
 *  
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <AccelStepper.h>

//Display configuration
//LiquidCrystal_I2C lcd(0x3c,16,2); //0x3F  0x27  0x20  0x3c

#include <Wire.h>
#include "DFRobot_LCD.h"
  const int colorR = 255;
  const int colorG = 0;
  const int colorB = 0;
DFRobot_LCD lcd(16,2);  //16 characters and 2 lines of show


//Keypad configuration
const int row_num = 4;
const int column_num = 4;

char keys[row_num][column_num] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'.', '0', '#', 'D'}};

int line = 1;
char TECLA;
boolean state = false;

byte pin_rows[row_num] = {13, 12, 11, 10};
byte pin_column[column_num] = {9, 8, 7, 6};
Keypad teclado = Keypad(makeKeymap(keys), pin_rows, pin_column, 4, 4);


//Variable to control temperature
// Set temperatura for the PID
int set_temperature = 200;
// Variable to get the temperature
int temperature = 0;

//Values to calculate temperature 
const int pinTermistor = A0; // número de pin analógico del termistor
const int Rfixed = 10000; // resistencia fija en ohmios
const float T0 = 25; // temperatura nominal del termistor en grados Celsius
const float B = 3950; // constante de temperatura del termistor
const float R0 = 100000; // resistencia nominal del termistor a la temperatura nominal T0

//*********************************************************************************************************************************
// PID Values
float PID_error = 0;
float previous_error = 0;
float elapsedTime, Time, timePrev;
float PID_Signal = 0;
float last_set_temperature = 0;
int max_PWM = 255;
int PWM_pin = 5;

//***********************************************************************************************************************************
// PID contants
float Kp = 90, P = 0;
float Ki = 30, I = 0;
float Kd = 80, D = 0;
float last_kp = 0;
float last_ki = 0;
float last_kd = 0;

//************************************************************************************************************************************
//Stepper motor configuration 

#define dirPin 2
#define stepPin 3
#define enPin 4
int potPin = A3;
int motorSpeed = 0;
int MotorSpeedPercentage = 0;
AccelStepper stepper(AccelStepper::DRIVER, stepPin, dirPin);

//************************************************************************************************************************************
//Fan's and Led's asignations
int In1 = 15;
int In2 = 16;
int LedOn = 1;
int LedOff = 0;

//************************************************************************************************************************************

void setup()
{  
  lcd.init();
  // Turn on background light
  //lcd.backlight();

  lcd.setRGB(colorR, colorG, colorB);//If the module is a monochrome screen, you need to shield it
  
  //Serial.begin(9600);
  Wire.begin();

  //Motor setup configuration
  pinMode(enPin, OUTPUT);
  digitalWrite(enPin, LOW);
  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(500);

  //Leds and fan configuration

  pinMode(In1, OUTPUT);
  pinMode(In2, OUTPUT);
  digitalWrite(In1, LOW);
  digitalWrite(In2, LOW);

  pinMode(LedOn, OUTPUT);
  pinMode(LedOff, OUTPUT);
  digitalWrite(LedOn, LOW);
  digitalWrite(LedOff, HIGH);
  
}

void loop()
{
  TECLA = teclado.getKey();
  Serial.println(TECLA);
  if(TECLA == 'B' && line < 3){
    line = line + 1;
    lcd.clear();

  }
  if(TECLA == 'A' && line > 1){
    line = line - 1;
    lcd.clear();
  }

  switch(line){
    case 1:
        lcd.setCursor(15,0);
        lcd.print("<");
        lcd.setCursor(0,0);
        if(state == true){
            lcd.print("1- OFF");
            if (TECLA == '#'){
                lcd.clear();
                function_state();
            }
        }
        else{
            lcd.print("1- ON");
            if (TECLA == '#'){
                lcd.clear();
                function_state();
            }
        }
        lcd.setCursor(0,1);
        lcd.print("2- Information");
        
        break;

    case 2:
        lcd.setCursor(0,0);
        if(state == true){
            lcd.print("1- OFF");
        }
        else{
          	lcd.print("1- ON");
        }        
    	lcd.setCursor(15,1);
        lcd.print("<");
        lcd.setCursor(0,1);
        lcd.print("2- Information");
        if (TECLA == '#'){
            lcd.clear();
            function_information();
        }
        break;

     case 3:
        lcd.setCursor(15,0);
        lcd.print("<");
        lcd.setCursor(0,0);
        lcd.print("3- Edit");
        if(TECLA == '#'){
            lcd.clear();
            function_edit();
        }
        break;
  }

  start_Off();
}

void function_state(){
    //State of the process
    if(state == false){
        state = true;
    }
    else{
        state = false;
    } 
}

void function_information(){
    
    while(1){
        TECLA = teclado.getKey();
        lcd.setCursor(0,0);
        lcd.print("Temp: "); lcd.print(analogRead(temperature)); lcd.print("/"); lcd.print(set_temperature);
        lcd.setCursor(0,1);

        MotorSpeedPercentage = map(motorSpeed, 0, 1000, 0, 100);
        
        lcd.print("Velocity: "); lcd.print(MotorSpeedPercentage); lcd.print("%"); 

        // Agregar pines a leer para mostrar la informacion     

        if(TECLA == 'D'){
            lcd.clear();
            break;
        }

        start_Off();

    }
}

void function_edit(){
    int i = 0;
    char valor_char[16];
    while(1){
            TECLA = teclado.getKey();
            lcd.setCursor(0,0);
            lcd.print("Temperature:");
            if (isdigit(TECLA) && i < 15 || TECLA == '.'){
                valor_char[i] = TECLA;
                lcd.setCursor(i, 1);
                lcd.print(valor_char[i]);
                i = i + 1;
            }
            if(TECLA == '#'){
                lcd.clear();
                set_temperature = atof(valor_char);
                memset(valor_char, ' ', 16);
                break;
            }
            if(TECLA == 'D'){
                lcd.clear();
                memset(valor_char, ' ', 16);
                break;
            }

        start_Off();    
    }
}

void start_Off(){

  //Lecture of the pot to set the velocity of the motor
  int potVal = analogRead(potPin);
  motorSpeed = map(potVal, 0, 1023, 0, 1000);
  stepper.setSpeed(motorSpeed);
  stepper.runSpeed();

  //Calcular temperatura del termistor 
  // Leer el valor de la resistencia del termistor
  int lectura = analogRead(pinTermistor);
  float Rterm = Rfixed / (1023.0 / lectura - 1.0);

  // Calcular la temperatura
  float lnR = log(Rterm/R0);
  float invT = 1.0/T0 + (1.0/B)*lnR;
  temperature = 1.0/invT - 273.15;
  

  if (state == true)
  {

    //Turn the LedON and turn off the LedOff
    digitalWrite(LedOn, HIGH);
    digitalWrite(LedOff, LOW);
    //Turn on the Fans
    digitalWrite(In1, HIGH);
    digitalWrite(In2, HIGH);

    // Calculate the error between the setpoint and the real value
    PID_error = set_temperature - temperature + 6;
    // Calculate the P value
    P = 0.01 * Kp * PID_error;
    // Calculate the I value in a range on +-6
    I = 0.01 * I + (Ki * PID_error);
    // For derivative we need real time to calculate speed change rate
    timePrev = Time;
    Time = millis();
    elapsedTime = (Time - timePrev) / 1000;
    // Now we can calculate the D Value
    D = 0.01 * Kd * ((PID_error - previous_error) / elapsedTime);
    // Calculate the final PID value is the sum of P+I+D
    PID_Signal = P + I + D;
    // We define PWM range between 0 and 255
    if (PID_Signal < 0)
    {
      PID_Signal = 0;
    }
    if (PID_Signal > max_PWM)
    {
      PID_Signal = max_PWM;
    }
    // Now we can write the PWM signal to the mosfet on digital pin D5
    analogWrite(PWM_pin, PID_Signal);
    previous_error = PID_error; // Remember to store the previous error for next loop.
    
  }
  else
  {
    // Apagar todo
    PID_Signal = 0;
    analogWrite(PWM_pin, PID_Signal);
    digitalWrite(LedOff, HIGH);
    digitalWrite(LedOn, LOW);
    digitalWrite(In1, LOW);
    digitalWrite(In2, LOW);
  }
}
