const int SAMPLE_SIZE = 10;
const int IR_PIN_0 = A0;
const int IR_PIN_1 = A1;
const int IR_PIN_2 = A2;
const int DUCK_OUTPUT_D0 = D0;
const int JUMP_OUTPUT_D1 = D1;
const int BEAM_PIN_D2 = D2;
const int SW_PIN_D3 = D3;
const int BUTTON_DUCK_D4 = D4;
const int BUTTON_JUMP_D5 = D5;
const int IR_0_THRESHOLD = 500;
const int IR_1_THRESHOLD = 500;
const int IR_2_THRESHOLD = 500;

bool prev_foot_detected = false; //no foot there initially -> for jumping
bool jump = false;
bool duck = false;

void setup() {
  Serial.begin(9600);
  pinMode(BEAM_PIN_D2, INPUT_PULLUP); //for jumping
  pinMode(SW_PIN_D3, INPUT_PULLUP); //for switch
  pinMode(DUCK_OUTPUT_D0, OUTPUT);
  pinMode(JUMP_OUTPUT_D1, OUTPUT);
  pinMode(BUTTON_DUCK_D4, INPUT_PULLUP);
  pinMode(BUTTON_JUMP_D5, INPUT_PULLUP);
}

void loop() {
  // print_val();
  //print_min_of_samples();

  //check pressed of third button
  if(!digitalRead(SW_PIN_D3)){ //D3 pressed means we care about buttons
    //do button stuff
    button_input();

  } else {
    detect_ducking();
    detect_jumping();
  }

  delay(5);
}

void button_input(){
  if(!digitalRead(BUTTON_DUCK_D4)){
    duck = true;
  } else {
    duck = false;
  }

  digitalWrite(DUCK_OUTPUT_D0, duck); //write duck val
  Serial.print("Duck: ");
  Serial.println(duck);

  if(!digitalRead(BUTTON_JUMP_D5)){
    jump = true;
  } else {
    jump = false;
  }

  digitalWrite(JUMP_OUTPUT_D1, jump); //write jump val
  Serial.print("Jump: ");
  Serial.println(jump);

}

void print_min_of_samples() {
  int min0 = min_of_samples(IR_PIN_0); 
  char buf[32] = {0};
  sprintf(buf, "0:%d", min0);
  Serial.println(buf);

  if (min0 > IR_0_THRESHOLD) {
    Serial.println("H0");
  } else {
    Serial.println("L0");
  }

  int min1 = min_of_samples(IR_PIN_1); 
  sprintf(buf, "1:%d", min1);
  Serial.println(buf);

  if (min1 > IR_1_THRESHOLD) {
    Serial.println("H1");
  } else {
    Serial.println("L1");
  }

  int min2 = min_of_samples(IR_PIN_2); 
  sprintf(buf, "2:%d", min2);
  Serial.println(buf);

  if (min2 > IR_2_THRESHOLD) {
    Serial.println("H2");
  } else {
    Serial.println("L2");
  }
}

void detect_ducking() {
  int min1 = min_of_samples(IR_PIN_1); 
  int min2 = min_of_samples(IR_PIN_2); 

  if (min1 < IR_1_THRESHOLD && min2 < IR_2_THRESHOLD) {
    Serial.println("D"); //ducked
    duck = true;
  } else {
    duck = false;
  }
  
  digitalWrite(DUCK_OUTPUT_D0, duck); //write duck val
  Serial.print("Duck: ");
  Serial.println(duck);
}

int min_of_samples(const int pin) {
  uint32_t min = 0xFFFFFFFF; 

  for (int i = 0; i < SAMPLE_SIZE; i++) {
    int val = analogRead(pin);
    if (min > val) {
      min = val;
    }
  }

  return min;
}


void print_val() {
  // analogRead takes ~100us
  int read_val = analogRead(IR_PIN_1);
  char buf[32] = {0};
  sprintf(buf, "1:%d", read_val);
  Serial.println(buf);

  read_val = analogRead(IR_PIN_2);
  sprintf(buf, "2:%d", read_val);
  Serial.println(buf);
}

void detect_jumping() {
  // put your main code here, to run repeatedly:
  if(digitalRead(BEAM_PIN_D2)){//reads HIGH means beam unbroken
    if(prev_foot_detected){//foot there before
       //foot not there anymore
      Serial.println("J");
      jump = true;
    } else {
      jump = false;
    }
    prev_foot_detected = false;
    
    // Serial.println("Beam Ok!");

  } else {//reads LOW means beam broken
     prev_foot_detected = true;
    //Serial.println("Beam Broken!");
    jump = false;
    
  }
  //Serial.println(digitalRead(PIN_D2));
  digitalWrite(JUMP_OUTPUT_D1, jump); //write jump val
  Serial.print("Jump: ");
  Serial.println(jump);

}