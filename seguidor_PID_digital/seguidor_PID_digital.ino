#include <TimerOne.h>

#define KP 2.9  
#define KD 5	

#define KP2 1  
#define KD2 0  

#define VEL_MAX 200
#define VEL VEL_MAX/3*2// con 200
//#define DT 1000  // Cada 1 ms accion de control.

#define M1PWM 3
#define M1A 4
#define M2PWM 10
#define M2A 7
#define LED1 5
#define LED2 12
#define M1E 2
#define M2E 13

#define S1 A1
#define S2 A2
#define S3 A3
#define S4 A4
#define S5 A5
#define LEDON 11


#define DT2 50000 // 200000

#define BOTON 9

// TIME 
unsigned long dt = 0; 
unsigned long Now = 0;
unsigned long LastTime[1] = {0};
unsigned long LastTime2[1] = {0};

// CONTROLLER
float LastError[1] = {0};
float Error[1] = {0};
int Vel = VEL;
int Vel_filtrada[1] = {0};

// SENSORES
boolean Ledon_state = false;

// STATES
int Flags[1] = {0};
boolean Flag = false;

void setup(){
  
  Serial.begin(115200);
  Serial.println("/*** sigue_linea_PD ***/");
  Serial.println("*** INITS ***");
  LEDsInit();
  MotoresInit(M1A, M2A, M1E, M2E);
  ApagarMotores(M1PWM, M1A, M2PWM, M2A);
  pinMode(BOTON,INPUT_PULLUP); // Boton para programa
  pinMode(LEDON, OUTPUT); 	   // Salida para encender CNY

  while(digitalRead(BOTON) == 1)
  { 
	    LEDsBlink(1, 1, 1, 100);
  }

  ApagarMotores(M1PWM, M1A, M2PWM, M2A);
  LEDsDrive(1,1);
  
  while(digitalRead(BOTON) == 1){}
  
  Serial.println("*** GO! ***");
  LEDsDrive(0,0);

  Timer1.initialize(20000); // set a timer of length 20000 microseconds (or 0.02 sec - or 50Hz)
  Timer1.attachInterrupt(control_loop); // attach the service routine here
  
}

void loop(){
  
  if(digitalRead(BOTON) == 0)
  {
   Flag = !Flag;
   Ledon_state = !Ledon_state;
   digitalWrite(LEDON,!Ledon_state); // Apago Sensores si Ledon_state = 0 !
   delay(200); 
  }
    
}

void control_loop() {
    if(Flag == true)
    {
        //Vel_filtrada[0] = LecturaSensores2(1, 0, Error, LastError, LastTime2, Flags);
        SeguirLinea(M1PWM, M1A, M2PWM, M2A, M1E, M2E, LecturaSensores2(1, 0, Error, LastError, LastTime2, Flags), Vel, 0);  // Probar varios delays
    } 
    else {
        LEDsDrive(0,0);
        ApagarMotores(M1PWM, M1A, M2PWM, M2A);
    }

}

