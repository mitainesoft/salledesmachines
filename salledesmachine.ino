/*
Salle des machine du 482 Pierre Boileau
Arret de lèechangeur d'air si la temperature du tuyau de secheuse 
est superieure a la temperature ambiante

Rev: 03 2015-12-27 JLC

Pour modifier:
- Brancher le arduino au port Serie
- CTRL-U pour uploader
- CTRKL-SHIFT-M pour voir les print outs pour debug

 */
 
 #include <dht.h>

dht DHT;

#define RELAY_ON LOW
#define RELAY_OFF HIGH

// PIN Definition
int testModePushButton = 6;
int relayIN1 = 2;
int relayIN2 = 3;
int relayIN3 = 7;
int relayIN4 = 8;
int relayIN5 = 9;
int relayIN6 = 10;
int relayIN7 = 11;
int relayIN8 = 12;

//// PIN Definition Relay #8 = RelayIN1
//int testModePushButton = 6;
//int relayIN1 = 12;
//int relayIN2 = 11;
//int relayIN3 = 10;
//int relayIN4 = 9;
//int relayIN5 = 8;
//int relayIN6 = 7;
//int relayIN7 = 3;
//int relayIN8 = 2;

// Attention aux commentaires c'est un define sti !!!
// Quabnd le delta de temp entre tuyau et ambiant sera de TEMP_THRESH_DELTA_TUYAU_AMBIANT, l'echangeur arrete
#define TEMP_THRESH_DELTA_TUYAU_AMBIANT 1.5
#define TIME_MIN_AIREX_CMD 20000
#define DHT11_PIN 13
#define DHT21_PIN 5

float dht21_temp_tuyau = 0;       // Mettee le sensor sur le tuyau de secheuse
float dht11_temp_ambiante=100;    // Etre sure que le tuyau est chaud en partant pour eviter un arret. Mettre ce sensor a 1 pied de l'autre
float delta_temp_tuyau_ambiante=dht21_temp_tuyau-dht11_temp_ambiante;



int prev_IN1_status=LOW;
boolean testModeEnabled=false;

//int tempPin = 0;
//int ledPin = 9;           // the pin that the LED is attached to

int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by

long mydelay = 2000; //2000 at least needed by DHT


// the setup routine runs once when you press reset:
void setup()  { 
  
//Turn off all relays by default. 
// Must be done before pinMode to avoid relay
// HIGH = OFF !

digitalWrite(testModePushButton,LOW);
digitalWrite(relayIN1,RELAY_OFF);
digitalWrite(relayIN2,RELAY_OFF);
digitalWrite(relayIN3,RELAY_OFF);
digitalWrite(relayIN4,RELAY_OFF);
digitalWrite(relayIN5,RELAY_OFF);
digitalWrite(relayIN6,RELAY_OFF);
digitalWrite(relayIN7,RELAY_OFF);
digitalWrite(relayIN8,RELAY_OFF);

//Digital
pinMode(relayIN1, OUTPUT);
pinMode(relayIN2, OUTPUT);
pinMode(relayIN3, OUTPUT);
pinMode(relayIN4, OUTPUT);
pinMode(relayIN5, OUTPUT);
pinMode(relayIN6, OUTPUT);
pinMode(relayIN7, OUTPUT);
pinMode(relayIN8, OUTPUT);
pinMode(testModePushButton, INPUT);

//Analog
//pinMode(tempPin, INPUT);

// initialize serial communication at 9600 bits per second:
Serial.begin(9600);
Serial.println("Apps pour Arret Echangeur d'air !");
Serial.print("LIBRARY VERSION: ");
Serial.println(DHT_LIB_VERSION);
Serial.println();

//Lire le sensor pour etablir la comm serie qui est bizarre au demarrage
int prechk = DHT.read11(DHT11_PIN);
delay(500);
prechk = DHT.read11(DHT21_PIN);
delay(300);
//prechk = DHT.read11(DHT11_PIN);
//prechk = DHT.read11(DHT21_PIN);
Serial.println();
Serial.println();

} 

// the loop routine runs over and over again forever:
void loop()  { 

  Serial.println("--- OVERALL STATUS           ----------");
  int testModePushButtonStatus=digitalRead(testModePushButton);
  if (testModePushButtonStatus==LOW) {
    testModeEnabled=false;
    Serial.println("Test Mode: OFF");
  } else {
    testModeEnabled=true;
    Serial.println("Test Mode: ON");
  }
  
  Serial.println("--- DHT21 Digital Thermo Hum ----------");
  // READ DATA
  Serial.print("DHT21, \t");
  int chk = DHT.read21(DHT21_PIN);
  switch (chk)
  {
    case DHTLIB_OK:  
		Serial.print("OK,\t"); 
		break;
    case DHTLIB_ERROR_CHECKSUM: 
		Serial.print("Checksum error,\t"); 
		break;
    case DHTLIB_ERROR_TIMEOUT: 
		Serial.print("Time out error,\t"); 
		break;
    default: 
		Serial.print("Unknown error,\t"); 
		break;
  }
  // DISPLAY DATA
  dht21_temp_tuyau=DHT.temperature;
  Serial.print(dht21_temp_tuyau, 1);
  Serial.print(",\t");
  Serial.println(DHT.humidity, 1);
  
  Serial.println("--- DHT11 Digital Temp Ambiante ----------");

  // READ DATA
  Serial.print("DHT11, \t");
  chk = DHT.read11(DHT11_PIN);
  switch (chk)
  {
    case DHTLIB_OK:  
		Serial.print("OK,\t"); 
		break;
    case DHTLIB_ERROR_CHECKSUM: 
		Serial.print("Checksum error,\t"); 
		break;
    case DHTLIB_ERROR_TIMEOUT: 
		Serial.print("Time out error,\t"); 
		break;
    default: 
		Serial.print("Unknown error,\t"); 
		break;
  }
  // DISPLAY DATA
  dht11_temp_ambiante=DHT.temperature;
  Serial.print(dht11_temp_ambiante,1);
  Serial.print(",\t");
  Serial.println(DHT.humidity,1);
   
 delta_temp_tuyau_ambiante=dht21_temp_tuyau-dht11_temp_ambiante;
 
 Serial.print("Delta Temperature Tuyau vs Ambiante\t");
 Serial.println(delta_temp_tuyau_ambiante,2);
 Serial.println("--------------------------------------------\n\n");
    
    
 if (!testModeEnabled ) {
   if ( delta_temp_tuyau_ambiante > TEMP_THRESH_DELTA_TUYAU_AMBIANT ){
        Serial.println("Relay IN#1:   ON");
        digitalWrite(relayIN1,RELAY_ON);
        delay(TIME_MIN_AIREX_CMD);
    } else {
        Serial.println("Relay IN#1:   OFF");
        digitalWrite(relayIN1,RELAY_OFF);
   }
   digitalWrite(relayIN2,RELAY_OFF);
   digitalWrite(relayIN3,RELAY_OFF);
   digitalWrite(relayIN4,RELAY_OFF);
   digitalWrite(relayIN5,RELAY_OFF);
   digitalWrite(relayIN6,RELAY_OFF);
   digitalWrite(relayIN7,RELAY_OFF);
   digitalWrite(relayIN8,RELAY_OFF);
 } else {
    // wait for x milliseconds to see the dimming effect 
    Serial.println("----------- Test Mode Start -----------");
    
    Serial.println("Relay IN#1:   ON");  
    digitalWrite(relayIN1,RELAY_ON); 
    delay(500);
    digitalWrite(relayIN2,RELAY_ON); 
    delay(500);
    digitalWrite(relayIN3,RELAY_ON); 
    delay(500);
    digitalWrite(relayIN4,RELAY_ON); 
    delay(500);
    digitalWrite(relayIN5,RELAY_ON); 
    delay(500);
    digitalWrite(relayIN6,RELAY_ON); 
    delay(500);
    digitalWrite(relayIN7,RELAY_ON); 
    delay(500);
    digitalWrite(relayIN8,RELAY_ON); 
    
    delay(10000);
    
    Serial.println("Relay IN#1:   OFF");
    //Turn off all relays by default !
    digitalWrite(relayIN1,RELAY_OFF);
    delay(500);
    digitalWrite(relayIN2,RELAY_OFF);
    delay(500);
    digitalWrite(relayIN3,RELAY_OFF);
    delay(500);
    digitalWrite(relayIN4,RELAY_OFF);
    delay(500);
    digitalWrite(relayIN5,RELAY_OFF);
    delay(500);
    digitalWrite(relayIN6,RELAY_OFF);
    delay(500);
    digitalWrite(relayIN7,RELAY_OFF);
    delay(500);
    digitalWrite(relayIN8,RELAY_OFF);
    Serial.println("---------- Test Mode Stop-------------");
  }  // Test mode

  Serial.println("----------------------------------------");
  delay(mydelay); 

}

