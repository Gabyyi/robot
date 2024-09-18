#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(8, 9);
const byte address[6] = "00001";

// int xAxis=0, yAxis=0;
int data[2];

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening();
}

int i=0;

void loop() {
  int xAxis = analogRead(A0);
  int yAxis = analogRead(A1);

  data[0] = xAxis;
  data[1] = yAxis;

  Serial.print(i);
  i++;
  Serial.print("xAxis: ");
  Serial.print(data[0]);
  Serial.print(" yAxis: ");
  Serial.println(data[1]);
  // delay(1000);

  radio.write(&data, sizeof(data));
  // delay(200);
  bool success = radio.write(&data, sizeof(data));
  if(success) {
    Serial.println("Data sent successfully.");
  } else {
    Serial.println("Failed to send data.");
  }
  delay(100);
}
