#include <Wire.h>
#include "DHT.h"
#define SLAVE_ADDRESS 0x04 //Addresse sur bus i2c
#define DHTPIN 10          // what pin DTH22 is connected to
#define DHTTYPE DHT22      // DHT 22  (AM2302)
#define LED_RED   6
#define LED_GREEN 7
#define LED_BLUE  8
int data[4];

// Pour rs485 
#include "RS485_protocol.h"
#include <SoftwareSerial.h>
const byte ENABLE_PIN = 4;
SoftwareSerial rs485 (2, 3);  // receive pin, transmit pin
// callback routines
void fWrite (const byte what)
  {
  rs485.write (what);  
  }
int fAvailable ()
  {
  return rs485.available ();  
  }
int fRead ()
  {
  return rs485.read ();  
  }
byte received = 0;
byte buf [10];

DHT dht(DHTPIN, DHTTYPE);

void setup(){
  Serial.begin(9600);
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);
  dht.begin();
  rs485.begin (28800);
  pinMode (ENABLE_PIN, OUTPUT);  // driver output enable
  pinMode (LED_RED, OUTPUT);
  pinMode (LED_GREEN, OUTPUT);
  pinMode (LED_BLUE, OUTPUT);
}
 
void loop(){
  get_temp_DTH22();
  for (int slave = 1; slave <= 1 ; slave++) { 
    // assemble message
    byte msg [] = { 
      slave, // device
      1,     // get data
    };
    // send to slave
    // Note from Toutou: RGB LED with commun anode is control by reverse LOW HIGH state
    // Turn off RGE LED
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_BLUE, HIGH);
    digitalWrite(LED_BLUE, LOW);
    digitalWrite (ENABLE_PIN, HIGH);  // enable sending
    sendMsg (fWrite, msg, sizeof msg);
    digitalWrite (ENABLE_PIN, LOW);  // disable sending
    // receive response  
    buf[2]=255;
    buf[3]=255;
    received = 0;
    int trial=0;
    while(trial < 20 & received ==0){
       trial=trial+1;
       Serial.print("trial ");
       Serial.println(trial);
       digitalWrite(LED_RED, HIGH);
       digitalWrite(LED_GREEN, HIGH);
       digitalWrite(LED_BLUE, HIGH);
       digitalWrite(LED_BLUE, LOW);
       received = recvMsg (fAvailable, fRead, buf, sizeof buf);
       delay(1000);
    }
    if(received != 0){
      if(buf[3] != 255){
         buf[3]=buf[3]-128;
         digitalWrite(LED_RED, HIGH);
         digitalWrite(LED_GREEN, HIGH);
         digitalWrite(LED_BLUE, HIGH);
         digitalWrite(LED_GREEN, LOW);
      }else{
         digitalWrite(LED_RED, HIGH);
         digitalWrite(LED_GREEN, HIGH);
         digitalWrite(LED_BLUE, HIGH);
         digitalWrite(LED_RED, LOW);
      }
    } else {
       digitalWrite(LED_RED, HIGH);
       digitalWrite(LED_GREEN, HIGH);
       digitalWrite(LED_BLUE, HIGH);
       digitalWrite(LED_RED, LOW);
    }
    data[slave*2]  =buf[3];
    data[slave*2+1]=buf[2];
    
    delay(10000);

  }
  Serial.println("DEBUT MESSAGE");
  Serial.println("Coin SE : temperature "+String(data[0])+" C, humidite "+String(data[1])+"%");
  Serial.println("Coin SO : temperature "+String(data[2])+" C, humidite "+String(data[3])+"%");
  Serial.println("FIN MESSAGE");
  // To debug put  delay(10000);
  delay(60000);
}

void get_temp_DTH22(){
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  int trial=1;
  float hr = dht.readHumidity();
  float tt = dht.readTemperature();
  while(isnan(tt) || isnan(hr) || trial < 10){
    float hr = dht.readHumidity();
    float tt = dht.readTemperature();
    trial++;
  }
  tt=tt;
  data[0]=round(tt);
  data[1]=round(hr);
}
