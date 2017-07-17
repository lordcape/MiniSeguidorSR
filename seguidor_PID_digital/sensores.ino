 #define ERROR_CASE_1 0.0
 #define ERROR_CASE_2 10.0
 #define ERROR_CASE_3 20.0
 #define ERROR_CASE_4 30.0
 #define ERROR_CASE_5 50.0
 #define ERROR_CASE_6 -10.0
 #define ERROR_CASE_7 -20.0
 #define ERROR_CASE_8 -30.0
 #define ERROR_CASE_9 -50.0
 #define ERROR_MAX 80.0
 #define ERROR_MIN -80.0
 #define DELTA_ERROR_MAX 10.0
 #define DELTA_ERROR_MIN -10.0


#define DELAY_REVERSA 150 // en ms (probar 100, 50, 25)

int LecturaSensores2(char mode, char serial, float* error, float* error_ant, unsigned long* LastTime2, int* flags)
{
  int sensor[5] = {0};
  byte umbral = 100;
  byte palabra = 0x00;
  float kp = KP;
  float kd = KD;

  sensor[0] = analogRead(S1);
  sensor[1] = analogRead(S2);
  sensor[2] = analogRead(S3);
  sensor[3] = analogRead(S4);
  sensor[4] = analogRead(S5);

  for (int i = 0; i <= 4; i++)
  {
    if (sensor[i] <= umbral)
    {
      palabra = palabra | (0x01 << i);
    }
  }

  switch (palabra)
  {
  case 0B00000100:
    //case 0x04:  // CASO 1
    *flags = *flags - 1;
    *error_ant = *error;
    *error = ERROR_CASE_1;
    if (*flags < 0) {
        Vel = VEL;
        kp = KP;
        kd = KD;
        *flags = 0;
    }
    break;

  case 0B00001100:
    //case 0x0C:  // CASO 2
    *error_ant = *error;
    *error = ERROR_CASE_2;
    break;

  case 0B00001000:
    //case 0x08:  // CASO 3
    *error_ant = *error;
    *error = ERROR_CASE_3;
    break;

  case 0B00011000:
    //case 0x18:  // CASO 4
    *error_ant = *error;
    *error = ERROR_CASE_4;
    if (*flags > 2) {
        MoverMotores(M1PWM, M1A, M2PWM, M2A, -50, -50);
        Vel = 50;
        kp = KP2;
        kd = KD2;
        *flags = 15;
    }
    break;

  case 0B00010000:
    //case 0x10: // CASO 5
    *error_ant = *error;
    *error = ERROR_CASE_5;
    *flags = *flags+1;
    if (*flags > 2) {
       MoverMotores(M1PWM, M1A, M2PWM, M2A, -50, -50);
       Vel = 50;
       kp = KP2;
       kd = KD2;
       *flags = 15;
    }
    break;

  case 0B00000110:	
    //case 0x06: // CASO 6
    *error_ant = *error;
    *error = ERROR_CASE_6;
    break;

  case 0B00000010:	
    //case 0x02: // CASO 7
    *error_ant = *error;
    *error = ERROR_CASE_7;
    break;

  case 0B00000011:
    //case 0x03: // CASO 8
    *error_ant = *error;
    *error = ERROR_CASE_8;
    if (*flags > 2) {
        MoverMotores(M1PWM, M1A, M2PWM, M2A, -50, -50);
        Vel = 50;
        kp = KP2;
        kd = KD2;
        *flags = 15;
    }
    break;

  case 0B00000001:
    //case 0x01: // CASO 9
    *error_ant = *error;
    *error = ERROR_CASE_9;
    *flags = *flags+1;
    if (*flags > 2) {
       MoverMotores(M1PWM, M1A, M2PWM, M2A, -50, -50);
       Vel = 50;
       kp = KP2;
       kd = KD2;
       *flags = 15;
    }
    break;

  case 0x00:  // si me salgo, vuelvo a entrar...
    MoverMotores(M1PWM, M1A, M2PWM, M2A, -50, -50);
    delay(DELAY_REVERSA);
    break;

  case 0x1F:
    *error_ant = *error;
    *error = ERROR_CASE_1;
    break;

  default:
    *error_ant = *error;
    *error = ERROR_CASE_1;
    break;

  }

  float delta_error = *error - *error_ant;
  if (delta_error >= DELTA_ERROR_MAX)delta_error = DELTA_ERROR_MAX; // "bumpless"
  if (delta_error <= DELTA_ERROR_MIN)delta_error = DELTA_ERROR_MIN; // "bumpless"
  float pwm_filtrado = (*error * kp) + delta_error * kd;

  unsigned long Now2 = micros();
  unsigned long dt2 = Now2 - *LastTime2;

  if ((serial == 1) && (dt2 > DT2))
  {
    /*Serial.println("----");
     Serial.println(sensor[0]);
     Serial.println(sensor[1]);
     Serial.println(sensor[2]);
     Serial.println(sensor[3]);
     Serial.println(sensor[4]);*/
    /*Serial.print("Estados sensores     ");
     Serial.println(palabra, BIN);
     Serial.println();*/
    Serial.print("Error[k]:     ");
    Serial.println(*error);
    Serial.println();
    Serial.print("Error[k-1]:     ");
    Serial.println(*error_ant);
    Serial.println();
    Serial.print("PWM filtrado[k]:     ");
    Serial.println(pwm_filtrado);
    Serial.println();
    Serial.println("----");
    *LastTime2 = Now2;
  }

  return (int)pwm_filtrado;

}




