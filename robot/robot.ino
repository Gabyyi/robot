#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define enA 9   // motor A
#define in1 4
#define in2 5
#define enB 10  // motor B
#define in3 6
#define in4 7

RF24 radio(8, 3); // CE, CSN
const byte address[6] = "00001";

int data[2]={505,516};
int xAxis = 505, yAxis = 516;

int motorSpeedA = 0;
int motorSpeedB = 0; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();

  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  analogWrite(enA, 0);
  analogWrite(enB, 0);

  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  if(radio.available()){
    radio.read(&data, sizeof(data));
    xAxis=data[0];
    yAxis=data[1];
    digitalWrite(A0, HIGH);
    digitalWrite(A1, HIGH);
    digitalWrite(A2, HIGH);
    digitalWrite(A3, HIGH);
    Serial.print(data[0]);
    Serial.print(" and ");
    Serial.println(data[1]);
  }
  else{
    xAxis=512;
    yAxis=512;
    motorSpeedA=0;
    motorSpeedB=0;
    Serial.println("no signal");
    digitalWrite(A0, LOW);
    digitalWrite(A1, LOW);
    digitalWrite(A2, LOW);
    digitalWrite(A3, LOW);
  }

  // Y Axis for forward/backward direction

  if(yAxis<470){  // go bacward
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);

    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);

    motorSpeedA=map(yAxis, 470, 0, 0, 255);
    motorSpeedB=map(yAxis, 470, 0, 0, 255);    
  }

  else if(yAxis>550){ // go forward
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);

    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);

    motorSpeedA=map(yAxis, 550, 1023, 0, 255);
    motorSpeedB=map(yAxis, 550, 1023, 0, 255);
  }
  else{
    motorSpeedA=0;
    motorSpeedB=0;
  }

  // X Axis for left/right

  if(xAxis<470){ // turn left
    int xMapped=map(xAxis, 470, 0, 0, 255);

    if(yAxis>=470 && yAxis<=550){
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);

      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);

      motorSpeedA=xMapped;
      motorSpeedB=xMapped;
    }
    else{
      motorSpeedA=motorSpeedA-xMapped;
      motorSpeedB=motorSpeedB+xMapped;
    }
    if(motorSpeedA<0){
      motorSpeedA=0;
    }
    if(motorSpeedB>255){
      motorSpeedB=255;
    }
  }

  if(xAxis>550){ //turn right
    int xMapped=map(xAxis, 550, 1023, 0, 255);

    if(yAxis>=470 && yAxis<=550){
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);

      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);

      motorSpeedA=xMapped;
      motorSpeedB=xMapped;
    }
    else{
      motorSpeedA=motorSpeedA+xMapped;
      motorSpeedB=motorSpeedB-xMapped;
    }
    if(motorSpeedA>255){
      motorSpeedA=255;
    }
    if(motorSpeedB<0){
      motorSpeedB=0;
    }
  }

  if(motorSpeedA<70){
    motorSpeedA=0;
  }
  if(motorSpeedB<70){
    motorSpeedB=0;
  }

  analogWrite(enA, motorSpeedA);
  analogWrite(enB, motorSpeedB);

  delay(100);
}
