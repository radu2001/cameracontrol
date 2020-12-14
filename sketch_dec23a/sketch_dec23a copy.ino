#define JOYSTICK_1_CONTROL_X 1  // joystick stanga
#define JOYSTICK_1_CONTROL_Y 0
#define JOYSTICK_2_CONTROL_X 2  // joystick dreapta
#define JOYSTICK_2_CONTROL_Y 3
#define JOYSTICK_Zoom_1 4
#define JOYSTICK_Zoom_2 5
#define KEYBOARD_DEBOUNCE 75  // cat de des citim Tastatura: o data la 75ms

typedef struct {
  // digital pin addresses for zoom in/out fast/slow
  byte in_fast;
  byte in_slow;
  byte out_fast;
  byte out_slow;
} Zoom;

typedef struct {
  // PWM pin address for speed
  byte speed;

  // digital pin address for up, down, left, right directions
  byte up;
  byte down;
  byte left;
  byte right;
} Motor;

typedef struct {
  // analog pin addresses for direction joystick
  byte X;
  byte Y;

  // analog pin addresses for zoom in/out joystick
  byte ZOOM;

  // direction thresholds in volts
  byte X_LOWER_BOUND;
  byte X_HIGHER_BOUND;
  byte Y_LOWER_BOUND;
  byte Y_HIGHER_BOUND;
  // zoom thresholds in volts
  byte ZOOM_LOWER_BOUND;
  byte ZOOM_HIGHER_BOUND;
} Joystick;

Zoom ZOOM_BALCON = {41, 38, 39, 40, 4};  // balcon
Zoom ZOOM_MIJLOC = {36, 37, 35, 34, 5};  // sala mare
Zoom ZOOM_AMVON = {99, 99, 99, 99};

Motor MOTOR_BALCON = {10, 46, 47, 48, 49}; // balcon
Motor MOTOR_MIJLOC = {12, 42, 43, 44, 45}; // sala mare
Motor MOTOR_AMVON = {11, 52, 53, 51, 50}; // amvon
Motor M4 = {101, 99, 99, 99, 99}; // ??

Joystick _LEFT_JOYSTICK = {1, 0, 4, 487, 535, 487, 535};
Joystick _LEFT_JOYSTICK = {1, 0, 4, 487, 535, 487, 535};

typedef struct {
  // digital pin addresses for buttons 1-4
  byte key1;
  byte key2;
  byte key3;
  byte key4;

  // associated motor controls
  Motor motor_ctrl;

  // associated zoom controls
  Zoom zoom_ctrl;

  // analog pin addresses for direction joystick
  byte JOYSTICK_X;
  byte JOYSTICK_Y;
  // analog pin address for zoom in/out joystick
  byte JOYSTICK_ZOOM;

  // TODO: Instaleaza leduri, pana atunci N/A
  byte led1;  // led conectat la pin-ul echivalent apasarii tastei 1
  byte led2;  // led conectat la pin-ul echivalent apasarii tastei 2
  byte led3;  // led conectat la pin-ul echivalent apasarii tastei 3
  byte led4;  // led conectat la pin-ul echivalent apasarii tastei 4
} Tastatura;

Tastatura TLEFT = {28, 26, 22, 24, MOTOR_BALCON, ZOOM_BALCON, 1, 0, 4, 99, 99, 99, 99};  // valorile default cu care pornim - Motor balcon + amvon
Tastatura TRIGHT = {27, 23, 29, 25, MOTOR_MIJLOC, ZOOM_MIJLOC, 2, 3, 5, 99, 99, 99, 99};

void setup() {
  //pinMode(MOTOR_BALCON.speed, OUTPUT);  // teoretic nu avem nevoie, ar trebui sa fie pin PWM
  pinMode(ZOOM_BALCON.in_fast, OUTPUT);
  pinMode(ZOOM_BALCON.in_slow, OUTPUT);
  pinMode(ZOOM_BALCON.out_fast, OUTPUT);
  pinMode(ZOOM_BALCON.out_slow, OUTPUT);
  pinMode(ZOOM_MIJLOC.in_fast, OUTPUT);
  pinMode(ZOOM_MIJLOC.in_slow, OUTPUT);
  pinMode(ZOOM_MIJLOC.out_fast, OUTPUT);
  pinMode(ZOOM_MIJLOC.out_slow, OUTPUT);
  pinMode(MOTOR_BALCON.up, OUTPUT);
  pinMode(MOTOR_BALCON.down, OUTPUT);
  pinMode(MOTOR_BALCON.left, OUTPUT);
  pinMode(MOTOR_BALCON.right, OUTPUT);
  pinMode(MOTOR_MIJLOC.up, OUTPUT);
  pinMode(MOTOR_MIJLOC.down, OUTPUT);
  pinMode(MOTOR_MIJLOC.left, OUTPUT);
  pinMode(MOTOR_MIJLOC.right, OUTPUT);
  pinMode(MOTOR_AMVON.up, OUTPUT);
  pinMode(MOTOR_AMVON.down, OUTPUT);
  pinMode(MOTOR_AMVON.left, OUTPUT);
  pinMode(MOTOR_AMVON.right, OUTPUT);

  // rezistor intern actioneaza ca pull-up
  // ca sa putem conecta pin-ul common de la Tastatura
  // la GND in loc de VCC
  pinMode(TLEFT.key1, INPUT_PULLUP);
  pinMode(TLEFT.key2, INPUT_PULLUP);
  pinMode(TLEFT.key3, INPUT_PULLUP);
  pinMode(TLEFT.key4, INPUT_PULLUP);

  // rezistor intern actioneaza ca pull-up
  // ca sa putem conecta pin-ul common de la Tastatura
  // la GND in loc de VCC
  pinMode(TRIGHT.key1, INPUT_PULLUP);
  pinMode(TRIGHT.key2, INPUT_PULLUP);
  pinMode(TRIGHT.key3, INPUT_PULLUP);
  pinMode(TRIGHT.key4, INPUT_PULLUP);

  pinMode(TLEFT.led1, OUTPUT);
  pinMode(TLEFT.led2, OUTPUT);
  pinMode(TLEFT.led3, OUTPUT);
  pinMode(TLEFT.led4, OUTPUT);
  pinMode(TRIGHT.led1, OUTPUT);
  pinMode(TRIGHT.led2, OUTPUT);
  pinMode(TRIGHT.led3, OUTPUT);
  pinMode(TRIGHT.led4, OUTPUT);

  Serial.begin(9600);
}

unsigned long startTime = 0;

// definim operatorul '==' pt structura Motor
// ca sa putem compara daca avem deja motor
// un Motor pe o Tastatura
bool operator==(Motor &m1, Motor &m2) {
  return m1.speed == m2.speed;
}

void loop() {
  int X, Y, Z;

  /*if ( (millis() - startTime) > KEYBOARD_DEBOUNCE ) {  //millis face rollover o data la ~49 de zile, deci (millis-startTime)>0 tot timpul
    MOTOR_BALCON = T.motor;
    MOTOR_MIJLOC = TRIGHT.motor;
    motor_changed(T);
    motor_changed(TRIGHT);
    if (T.motor == TRIGHT.motor) { //sa avem intotdeauna taste diferite motore ca sa nu controlam acelasi Motor de la 2 joystick-uri
      T.motor = MOTOR_MIJLOC;  //emulam switch asincron   --------------->>>>> de verificat daca nu se switchuie intre ele aiurea
      TRIGHT.motor = MOTOR_BALCON;
    }
    startTime = millis();
    }*/

  if (motor_changed(TLEFT)) {
    stop_motor(TLEFT.motor_ctrl);
  }
  if (motor_changed(TRIGHT)) {
    stop_motor(TRIGHT.motor_ctrl);
  }

  // hack ca sa evitam sa facem Zoom in acelasi timp de la 2 joystick-uri
  if (TLEFT.motor_ctrl == TRIGHT.motor_ctrl) {
    TRIGHT.motor_ctrl = {};
  }  // TODO: switch asincron sa nu controlam acelasi Motor + Zoom in acelasi timp de la 2 joystick-uri diferite.

  muta_motor_joystick1(
    analogRead(TLEFT.JOYSTICK_X),
    analogRead(TLEFT.JOYSTICK_Y),
    analogRead(TLEFT.JOYSTICK_ZOOM),
    TLEFT.motor_ctrl
  );

  muta_motor_joystick2(
    analogRead(TRIGHT.JOYSTICK_X),
    analogRead(TRIGHT.JOYSTICK_Y),
    analogRead(TRIGHT.JOYSTICK_ZOOM),
    TRIGHT.motor_ctrl
  );
}

int motor_changed(Tastatura &t) {
  switch (LOW) {
    case digitalRead(t.key1):
      t.motor_ctrl = MOTOR_BALCON;
      return 1;
    case digitalRead(t.key2):
      t.motor_ctrl = MOTOR_MIJLOC;
      return 1;
    case digitalRead(t.key3):
      t.motor_ctrl = MOTOR_AMVON;
      return 1;
    case digitalRead(t.key4):
      t.motor_ctrl = M4;
      return 1;
    default:
      return 0;
  }
}

void stop_motor(Motor &m) {
  digitalWrite(m.up, HIGH);
  digitalWrite(m.down, HIGH);
  digitalWrite(m.left, HIGH);
  digitalWrite(m.right, HIGH);
}

void muta_motor_joystick1(int X1, int Y1, int Z, Motor M) {
  do_zoom_joystick1(Z, M.zoom);

  if (X1 > 535) {  // dreapta - axa X  // citim intre 487 < X < 535 sa eliminam eroarea joystick-ului in punctul 511 (1023/2)
    motor(M.speed, M.up, M.down, 2, X1, true);
  } else if (X1 < 487) { // stanga
    motor(M.speed, M.up, M.down, 1, X1, false);
  } else {  // cand revine joystick-ul la 0 pe axa stanga/dreapta
    motor(M.speed, M.up, M.down, 3, X1, false);
  }

  if (Y1 < 487) {  // sus - axa Y
    motor(M.speed, M.right, M.left, 2, Y1, false);
  } else if (Y1 > 535) {  // jos
    motor(M.speed, M.right, M.left, 1, Y1, true);
  } else {  // cand revine joystick-ul la 0 pe axa sus/jos
    motor(M.speed, M.right, M.left, 3, Y1, false);
  }
}

void muta_motor_joystick2(int X1, int Y1, int Z, Motor M) {

  do_zoom_joystick2(Z, M.zoom);

  if (X1 > 535) {  //sus  //citim intre 487 < X < 535 sa eliminam eroarea joystick-ului in punctul 511 (1023/2)
    motor(M.speed, M.left, M.right, 1, X1, true);
  } else if (X1 < 475) { //jos
    motor(M.speed, M.left, M.right, 2, X1, false);
  } else { //cand revine joystick-ul la 0 pe axa stanga/dreapta
    motor(M.speed, M.left, M.right, 3, X1, false);
  }

  if (Y1 < 487) {  //stanga
    motor(M.speed, M.up, M.down, 1, Y1, false);
  } else if (Y1 > 535) { //dreapta
    motor(M.speed, M.up, M.down, 2, Y1, true);
  } else { //cand revine joystick-ul la 0 pe axa sus/jos
    motor(M.speed, M.up, M.down, 3, Y1, false);
  }
}

void motor(
  byte pwm_port,
  byte direction_porkey1,
  byte direction_porkey2,
  byte dir,
  int spd,
  bool is_inverted
) {
  switch (dir) {
    case 1:  // left or up (dat de parametrul dir)
      // calculam viteza cu care mergem
      if (is_inverted) {
        analogWrite(pwm_port, map(spd, 536, 1023, 0, 255));
        //Serial.println("Case1 inverted");
        //Serial.println(map(spd, 536, 1023, 0, 255));
      }
      else {
        analogWrite(pwm_port, map(spd, 486, 0, 0, 255));
        //Serial.println("Case1");
        //Serial.println(map(spd, 486, 0, 0, 255));
      }

      digitalWrite(direction_porkey1, HIGH);
      digitalWrite(direction_porkey2, LOW);
      break;

    case 2: //right or down
      if (is_inverted) {
        analogWrite(pwm_port, map(spd, 536, 1023, 0, 255));
        //Serial.println("Case2 inverted");
        //Serial.println(map(spd, 536, 1023, 0, 255));
      }
      else {
        analogWrite(pwm_port, map(spd, 486, 0, 0, 255));
        //Serial.println("Case2");
        //Serial.println(map(spd, 486, 0, 0, 255));
      }

      //
      digitalWrite(direction_porkey1, LOW);
      digitalWrite(direction_porkey2, HIGH);
      break;

    case 3: // oprim Motorul pe directia data
      analogWrite(pwm_port, 0);

      //
      //Serial.println("Case3");
      digitalWrite(direction_porkey1, HIGH);
      digitalWrite(direction_porkey2, HIGH);
      break;
  }
}

void do_zoom_joystick1(int val_joystick, Zoom zoom) {
  Serial.println(val_joystick);
  if (val_joystick > 387 && val_joystick < 635) {
    digitalWrite(zoom.out_slow, LOW);
    digitalWrite(zoom.in_slow, LOW);
    digitalWrite(zoom.in_fast, LOW);
    digitalWrite(zoom.out_fast, LOW);
    return;
  }

  if (val_joystick > 635 && val_joystick < 980) {
    digitalWrite(zoom.out_slow, HIGH);
    digitalWrite(zoom.in_slow, LOW);
    digitalWrite(zoom.in_fast, LOW);
    digitalWrite(zoom.out_fast, LOW);
    return;
  }

  if (val_joystick > 980) {
    digitalWrite(zoom.out_fast, HIGH);
    digitalWrite(zoom.in_slow, LOW);
    digitalWrite(zoom.in_fast, LOW);
    digitalWrite(zoom.out_slow, LOW);
    return;
  }

  if (val_joystick < 387 && val_joystick > 5) {
    digitalWrite(zoom.in_slow, HIGH);
    digitalWrite(zoom.in_fast, LOW);
    digitalWrite(zoom.out_fast, LOW);
    digitalWrite(zoom.out_slow, LOW);
    return;
  }

  if (val_joystick < 5) {
    digitalWrite(zoom.in_fast, HIGH);
    digitalWrite(zoom.in_slow, LOW);
    digitalWrite(zoom.out_fast, LOW);
    digitalWrite(zoom.out_slow, LOW);
    return;
  }
  return;
}

void do_zoom_joystick2(int val_joystick, Zoom zoom) {
  Serial.println(val_joystick);
  if (val_joystick > 387 && val_joystick < 635) {
    digitalWrite(zoom.out_slow, LOW);
    digitalWrite(zoom.in_slow, LOW);
    digitalWrite(zoom.in_fast, LOW);
    digitalWrite(zoom.out_fast, LOW);
    return;
  }

  if (val_joystick > 635 && val_joystick < 1015) {
    digitalWrite(zoom.out_slow, HIGH);
    digitalWrite(zoom.in_slow, LOW);
    digitalWrite(zoom.in_fast, LOW);
    digitalWrite(zoom.out_fast, LOW);
    return;
  }

  if (val_joystick > 1015) {
    digitalWrite(zoom.out_fast, HIGH);
    digitalWrite(zoom.in_slow, LOW);
    digitalWrite(zoom.in_fast, LOW);
    digitalWrite(zoom.out_slow, LOW);
    return;
  }

  if (val_joystick < 387 && val_joystick > 5) {
    digitalWrite(zoom.in_slow, HIGH);
    digitalWrite(zoom.in_fast, LOW);
    digitalWrite(zoom.out_fast, LOW);
    digitalWrite(zoom.out_slow, LOW);
    return;
  }

  if (val_joystick < 5) {
    digitalWrite(zoom.in_fast, HIGH);
    digitalWrite(zoom.in_slow, LOW);
    digitalWrite(zoom.out_fast, LOW);
    digitalWrite(zoom.out_slow, LOW);
    return;
  }
  return;
}
