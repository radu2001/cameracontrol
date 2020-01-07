#define Joystick1ControlX 0
#define Joystick1ControlY 1
#define Joystick2ControlX 2
#define Joystick2ControlY 3
#define JoystickZoom1 4
#define JoystickZoom2 5
#define keyboard_debounce 75  //cat de des citim tastatura; o data la 75ms

typedef struct {
  byte speed_pin;
  byte direction_up;
  byte direction_down;
  byte direction_left;
  byte direction_right;
} MOTOR;

typedef struct {
  byte zoom_in_fast;
  byte zoom_in_slow;
  byte zoom_out_fast;
  byte zoom_out_slow;
} ZOOM;

      //viteza, st, dr, sus, jos
MOTOR M3 = {11, 52, 53, 51, 50}; // M3 = cor
MOTOR M1 = {10, 46, 47, 49, 48}; // M1 = balcon
MOTOR M2 = {12, 42, 43, 44, 45}; // M2 = mijloc
MOTOR M4 = {101, 99, 99, 99, 99};

//z_in_slow_fast__z_out_slow_fast
ZOOM Z1 = {41, 38, 39, 40};  // Z1 = mijloc sala mare
ZOOM Z2 = {36, 37, 35, 34};
ZOOM Z3 = {99, 99, 99, 99};

typedef struct {
  byte t1;  //tasta1
  byte t2;  //tasta2
  byte t3;  //tasta3
  byte t4;  //tasta4
  MOTOR selectat;
  byte led1; //led conectat la pin-ul asta echivalent apasarii tastei 1
  byte led2;
  byte led3;
  byte led4;
  ZOOM zoom_selectat;
} TASTATURA;

        //tasta 1, 2,  3,  4,motor,led,led,led,led,zoom
TASTATURA T = {28, 26, 22, 24, M1, 99, 99, 99, 99, Z1};  //valorile default cu care pornim - motor amvon + balcon
TASTATURA T2= {27, 23, 29, 25, M2, 99, 99, 99, 99, Z2};

void setup() {
  //pinMode(M1.speed_pin, OUTPUT); //teoretic nu avem nevoie, ar trebui sa fie pin PWM
  pinMode(Z1.zoom_in_fast, OUTPUT);
  pinMode(Z1.zoom_in_slow, OUTPUT);
  pinMode(Z1.zoom_out_fast, OUTPUT);
  pinMode(Z1.zoom_out_slow, OUTPUT);
  pinMode(Z2.zoom_in_fast, OUTPUT);
  pinMode(Z2.zoom_in_slow, OUTPUT);
  pinMode(Z2.zoom_out_fast, OUTPUT);
  pinMode(Z2.zoom_out_slow, OUTPUT);
  pinMode(M1.direction_up, OUTPUT);
  pinMode(M1.direction_down, OUTPUT);
  pinMode(M1.direction_left, OUTPUT);
  pinMode(M1.direction_right, OUTPUT);
  pinMode(M2.direction_up, OUTPUT);
  pinMode(M2.direction_down, OUTPUT);
  pinMode(M2.direction_left, OUTPUT);
  pinMode(M2.direction_right, OUTPUT);
  pinMode(M3.direction_up, OUTPUT);
  pinMode(M3.direction_down, OUTPUT);
  pinMode(M3.direction_left, OUTPUT);
  pinMode(M3.direction_right, OUTPUT);
  pinMode(T.t1, INPUT_PULLUP);  //rezistor intern actioneaza ca pull-up ca sa putem conecta pin-ul common de la tastatura la GND in loc de VCC
  pinMode(T.t2, INPUT_PULLUP);
  pinMode(T.t3, INPUT_PULLUP);
  pinMode(T.t4, INPUT_PULLUP);
  pinMode(T2.t1, INPUT_PULLUP);  //rezistor intern actioneaza ca pull-up ca sa putem conecta pin-ul common de la tastatura la GND in loc de VCC
  pinMode(T2.t2, INPUT_PULLUP);
  pinMode(T2.t3, INPUT_PULLUP);
  pinMode(T2.t4, INPUT_PULLUP);
  pinMode(T.led1, OUTPUT);
  pinMode(T.led2, OUTPUT);
  pinMode(T.led3, OUTPUT);
  pinMode(T.led4, OUTPUT);
  pinMode(T2.led1, OUTPUT);
  pinMode(T2.led2, OUTPUT);
  pinMode(T2.led3, OUTPUT);
  pinMode(T2.led4, OUTPUT);
  Serial.begin(9600);
}

unsigned long startTime = 0;

bool operator==(MOTOR &M1, MOTOR &M2)  //definim operatorul '==' pt structura MOTOR ca sa putem compara daca avem deja selectat un motor pe o tastatura
{
    if (M1.speed_pin == M2.speed_pin) {
      return true;
    }
    return false;
}

void loop() {
  int X, Y, Z;
/*
  if ( (millis() - startTime) > keyboard_debounce ) {  //millis face rollover o data la ~49 de zile, deci (millis-startTime)>0 tot timpul
    M1 = T.selectat;
    M2 = T2.selectat;
    read_key(T);
    read_key(T2);
    if (T.selectat == T2.selectat) { //sa avem intotdeauna taste diferite selectate ca sa nu controlam acelasi motor de la 2 joystick-uri
      T.selectat = M2;  //emulam switch asincron   --------------->>>>> de verificat daca nu se switchuie intre ele aiurea
      T2.selectat = M1;
    }
    startTime = millis();
  } */
  read_key(T);
  read_key(T2);
  X = analogRead(Joystick1ControlX);
  Y = analogRead(Joystick1ControlY);
  Z = analogRead(JoystickZoom1);
  do_zoom(Z, T);
  //Serial.println(Z);
  muta_motor(X, Y, T.selectat);
  X = analogRead(Joystick2ControlX);
  Y = analogRead(Joystick2ControlY);
  Z = analogRead(JoystickZoom2);
  do_zoom(Z, T2);
  //Serial.println(Z);
  muta_motor(X, Y, T2.selectat);
  //Serial.println(X);
  //Serial.println(Y);

}  

void muta_motor(int X1, int Y1, MOTOR M) {
  if (X1 > 535) {  //stanga - axa X  //citim intre 487 < X < 535 sa eliminam eroarea joystick-ului in punctul 511 (1023/2)
    motor(M.speed_pin, M.direction_left, M.direction_right, 1, X1, true);
  }

  else if (X1 < 487) { //dreapta
    motor(M.speed_pin, M.direction_left, M.direction_right, 2, X1, false);
  }

  else { //cand revine joystick-ul la 0 pe axa stanga/dreapta
    motor(M.speed_pin, M.direction_left, M.direction_right, 3, X1, false);
  }

  if (Y1 < 487) {  //sus - axa Y
    motor(M.speed_pin, M.direction_up, M.direction_down, 1, Y1, false);
  }

  else if (Y1 > 535) { //jos
    motor(M.speed_pin, M.direction_up, M.direction_down, 2, Y1, true);
  }

  else { //cand revine joystick-ul la 0 pe axa sus/jos
    motor(M.speed_pin, M.direction_up, M.direction_down, 3, Y1, false);
  }  
  return;
}

void motor(byte pwm_port, byte direction_port1, byte direction_port2, byte dir, int spd, bool is_inverted) {
  switch (dir) {
    case 1: //left or up (dat de parametrul byte dir)
      //calculam viteza cu care mergem
      if (is_inverted) {
        analogWrite(pwm_port, map(spd, 536, 1023, 0, 255));
        //Serial.println(map(spd, 536, 1023, 0, 255));
      }
      else {
        analogWrite(pwm_port, map(spd, 486, 0, 0, 255));
        //Serial.println(map(spd, 486, 0, 0, 255));
      }
      
      //
      digitalWrite(direction_port1, HIGH);
      digitalWrite(direction_port2, LOW);
      break;
      
    case 2: //right or down
      if (is_inverted) {
        analogWrite(pwm_port, map(spd, 536, 1023, 0, 255));
        //Serial.println(map(spd, 536, 1023, 0, 255));
      }
      else {
        analogWrite(pwm_port, map(spd, 486, 0, 0, 255));
        //Serial.println(map(spd, 486, 0, 0, 255));
      }

      //
      digitalWrite(direction_port1, LOW);
      digitalWrite(direction_port2, HIGH);
      break;
      
    case 3: //oprim motorul pe directia data
      analogWrite(pwm_port, 0);
      
      //
      digitalWrite(direction_port1, HIGH);
      digitalWrite(direction_port2, HIGH);
      break;
  }
}

void _stop_motor_complet(TASTATURA &tst) {
  digitalWrite(tst.selectat.direction_up, HIGH);
  digitalWrite(tst.selectat.direction_down, HIGH);
  digitalWrite(tst.selectat.direction_left, HIGH);
  digitalWrite(tst.selectat.direction_right, HIGH);
}

void read_key(TASTATURA &tst) {
    if (digitalRead(tst.t1) == LOW){
      _stop_motor_complet(tst);
      tst.selectat = M1;
      Serial.println("apasat: 1");
      Serial.println(tst.selectat);
      Serial.println(tst.selectat.direction_up);
      Serial.println(tst.selectat.direction_down);
      Serial.println(tst.selectat.direction_left);
      Serial.println(tst.selectat.direction_right);
      //digitalWrite(tst.led1, HIGH);   //-->>> de schimbat cu shiftare pe biti, mult mai rapid
      return;
    }

    if (digitalRead(tst.t2) == LOW){
      _stop_motor_complet(tst);
      tst.selectat = M2;
      Serial.println("apasat: 2");
      Serial.println(tst.selectat.direction_up);
      Serial.println(tst.selectat.direction_down);
      Serial.println(tst.selectat.direction_left);
      Serial.println(tst.selectat.direction_right);
      //digitalWrite(tst.led2, HIGH);  //---->>>> ce aprindem trebuie sa si inchidem...
      return;
    }

    if (digitalRead(tst.t3) == LOW){
      _stop_motor_complet(tst);
      tst.selectat = M3;
      Serial.println("apasat: 3");
      Serial.println(tst.selectat.direction_up);
      Serial.println(tst.selectat.direction_down);
      Serial.println(tst.selectat.direction_left);
      Serial.println(tst.selectat.direction_right);
      //digitalWrite(tst.led3, HIGH);
      return;
    }

    if (digitalRead(tst.t4) == LOW){
      _stop_motor_complet(tst);
      tst.selectat = M4;
      Serial.println("apasat: 4");
      Serial.println(tst.selectat.direction_up);
      Serial.println(tst.selectat.direction_down);
      Serial.println(tst.selectat.direction_left);
      Serial.println(tst.selectat.direction_right);
      //digitalWrite(tst.led4, HIGH);
      return;
    }
}

void do_zoom(int val_joystick, TASTATURA &tst) {

  if (val_joystick > 387 && val_joystick < 635) {
    digitalWrite(tst.zoom_selectat.zoom_out_slow, LOW);
    digitalWrite(tst.zoom_selectat.zoom_in_slow, LOW);
    digitalWrite(tst.zoom_selectat.zoom_in_fast, LOW);
    digitalWrite(tst.zoom_selectat.zoom_out_fast, LOW);
  }
  
  if (val_joystick > 635 && val_joystick < 1015) {
    digitalWrite(tst.zoom_selectat.zoom_out_slow, HIGH);
    digitalWrite(tst.zoom_selectat.zoom_in_slow, LOW);
    digitalWrite(tst.zoom_selectat.zoom_in_fast, LOW);
    digitalWrite(tst.zoom_selectat.zoom_out_fast, LOW);
    return;
  }

  if (val_joystick > 1015) {
    digitalWrite(tst.zoom_selectat.zoom_out_fast, HIGH);
    digitalWrite(tst.zoom_selectat.zoom_in_slow, LOW);
    digitalWrite(tst.zoom_selectat.zoom_in_fast, LOW);
    digitalWrite(tst.zoom_selectat.zoom_out_slow, LOW);
    return;
  }

  if (val_joystick < 387 && val_joystick > 5) {
    digitalWrite(tst.zoom_selectat.zoom_in_slow, HIGH);
    digitalWrite(tst.zoom_selectat.zoom_in_fast, LOW);
    digitalWrite(tst.zoom_selectat.zoom_out_fast, LOW);
    digitalWrite(tst.zoom_selectat.zoom_out_slow, LOW);
    return;
  }

  if (val_joystick < 5) {
    digitalWrite(tst.zoom_selectat.zoom_in_fast, HIGH);
    digitalWrite(tst.zoom_selectat.zoom_in_slow, LOW);
    digitalWrite(tst.zoom_selectat.zoom_out_fast, LOW);
    digitalWrite(tst.zoom_selectat.zoom_out_slow, LOW);
    return;
  }
  return;
}
