int ir=16;
int led1=4;
int led2=5;

void setup(){
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(ir,INPUT);
  Serial.begin(115200);
}

void loop(){
  int value = digitalRead(ir);
  Serial.println(value);
  if (value == 1){
    digitalWrite(led1,0);
    digitalWrite(led2,1);
  }
  else{
    digitalWrite(led1,1);
    digitalWrite(led2,0);
  }
}