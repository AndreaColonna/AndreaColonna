/* Link between Arduino Nano 33 BLE Sense and phyphox app vAD1
   Allows to choose which experiment the Arduino Nano 33 BLE Sense should execute, using Phyphox.
   by Gautier Creutzer and Frédéric Bouquet, La Physique Autrement / Physics Reimagined, Laboratoire de Physique des Solides / Laboratory of Solid state Physics, Université Paris-Saclay / Paris-Saclay University
   Check our other projects in English and French: www.physicsreimagined.com
   This work is based on the phyphox Arduino library developed by the phyphox team at the RWTH Aachen University, which is released under the GNU Lesser General Public Licence v3.0 (or newer).
   This work is released under the GNU Lesser General Public Licence v3.0 (or newer).
*/
/*
 File modificato per il progetto MOBILE da Colonna Andrea nella acquisizione delle temperature e della pressione del sensore interno
 include anche la possibilità di utilizzare il sensore di temperatura digitale DHT11 con ingresso 3.3V ed segnale di uscita su PIN 2 
*/
#include <phyphoxBle.h>         // libreria per la comunicazione tramite bluetooth con phyphox
#include <Arduino_LSM9DS1.h>    // libreria per accelerometro, giroscospio e magnetometro
#include <Arduino_LPS22HB.h>    // libreria per il sensore di pressione 
#include <Arduino_HTS221.h>     // libreria per il sensore di temperatura e umidità
#include <Arduino_APDS9960.h>   // libreria per il sensore di prossimità, luce e colore

#include <DHT.h>                // libreria sensore esterno temperatura DHT          
#define DHTPIN 2                // porta su cui viene preso il segnale digitale nel nostro caso 2
#define DHTTYPE DHT11           //  specifica del modello nel nostro caso DHT11  
DHT dht(DHTPIN, DHTTYPE);       // si definisce la funzione di acquisizione dht

const float pi=3.14;
char board_name[] = "Andrea"; // to change the name displayed by the board using BLE: no space and no special character
float choice = 0.0;             // variabile per la scelta del segnale del sensore da inviare trtamite bluetooth


float old_temp = 0.0;          // variabili di comodo per la temperatura e umidità
float old_hum = 0.0;
float temperature, humidity;

float old_pres = 0.0;          // variabili di comodo per la pressione
float pressure;

float accx, accy, accz, acc, first_acc;   // variabili di comodo per acc, gir e magnetometro
float gyrx, gyry, gyrz, gyr2, gyr;
float magx, magy, magz, magn;

int red, green, blue, ambient;
float red_float, green_float, blue_float, ambient_float; // variabili di comodo per colori e luce

const int analogInPin1 = A0; // change here to read another analog input
const int analogInPin2 = A1;
const int analogInPin3 = A2;
float voltage1, voltage2, voltage3;  // variabili di comodo per il segnale analogico

unsigned long initial_time, first_time, fourth_time;
float first_difference_float, fourth_difference_float;   // variabili di comodo per il tempo

unsigned int period = 100;      // periodo in millisecondi

const int ledPin = 22;         // variabili per controllare i LED della scheda
const int ledPin2 = 23;
const int ledPin3 = 24;

void receivedData(); // see Phyphox Arduino Library example

void setup()
{
  PhyphoxBLE::minConInterval = 6; //6 = 7.5ms       // impostazioni per il bluetooth
  PhyphoxBLE::maxConInterval = 24; //6 = 7.5ms
  PhyphoxBLE::slaveLatency = 0; //
  PhyphoxBLE::timeout = 50; //10 = 100ms

  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);
  pinMode(24, OUTPUT);

  digitalWrite(ledPin, LOW);
  digitalWrite(ledPin2, HIGH);
  digitalWrite(ledPin3, HIGH);

  if (!IMU.begin()) { // starting all useful sensors
    while (1);
  }
  if (!BARO.begin()) {
    while (1);
  }
  if (!HTS.begin()) {
    while (1);
  }
  if (!APDS.begin()) {
    while (1);
  }
  PhyphoxBLE::start(board_name); // the name of the board can be changed at the beginning of the program
  PhyphoxBLE::configHandler = &receivedData; // see Phyphox Arduino library example
}

void loop() // depending on the config parameter sent by Phyphox, an experiment is chosen
{
  if (PhyphoxBLE::currentConnections > 0) {
    digitalWrite(ledPin, HIGH);
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin3, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
    digitalWrite(ledPin2, HIGH);
    digitalWrite(ledPin3, HIGH);
  }

  if (choice == 1.0) {
    accelerometerChoice();   
  } else if (choice == 2.0) {
    gyroscopeChoice();
  } else if (choice == 3.0) {
    magnetometerChoice();
  } else if (choice == 4.0) {
    pressureChoice();
  } else if (choice == 5.0) {
    temperatureChoice();
  } else if (choice == 6.0) {
    lightChoice();
  }else if (choice == 7.0) {
    temperatureChoiceDHT();
  }else if (choice == 8.0) {
    accelerazioneCentripeta();
  } else if (choice == 9.0) {
    analogChoice();
  } else {
  }
}

void receivedData() { // see Phyphox Arduino Library example
  if (PhyphoxBLE::currentConnections == 1) {
    PhyphoxBLE::read(choice); // the "choice" variable is written by our Phyphox experiments
    initial_time = millis();
    IMU.readAcceleration(accx, accy, accz);
    first_acc = sqrt(pow(accx, 2) + pow(accy, 2) + pow(accz, 2)); 
  }
}

void accelerometerChoice() { // if the sensor is available, the acceleration and a timestamp are written to the BLE server, and then to the Phyphox app
  if (IMU.accelerationAvailable()) {
    first_time = millis();
    IMU.readAcceleration(accx, accy, accz);
    first_difference_float = ((float)first_time - (float)initial_time) / 1000;
    acc = sqrt(pow(accx, 2) + pow(accy, 2) + pow(accz, 2));
    PhyphoxBLE::write(first_difference_float, accx, accy, accz, acc);
    delay(60);
  }
}

void gyroscopeChoice() {
  if (IMU.gyroscopeAvailable()) {
    first_time = millis();
    IMU.readGyroscope(gyrx, gyry, gyrz);
    first_difference_float = ((float)first_time - (float)initial_time) / 1000;
    gyr = sqrt(pow(gyrx, 2) + pow (gyry, 2) + pow(gyrz, 2));
    PhyphoxBLE::write(first_difference_float, gyrx, gyry, gyrz, gyr);
    delay(60);
  }
}

void magnetometerChoice() {
  if (IMU.magneticFieldAvailable()) {
    first_time = millis();
    IMU.readMagneticField(magx, magy, magz);
    first_difference_float = ((float)first_time - (float)initial_time) / 1000;
    magn = sqrt(pow(magx, 2) + pow (magy, 2) + pow(magz, 2));
    PhyphoxBLE::write(first_difference_float, magx, magy, magz, magn);
    delay(60);
  }
}


void pressureChoice() {
   IMU.end();
    first_time = millis();
    pressure = 1000*BARO.readPressure();
    first_difference_float = ((float)first_time - (float)initial_time) / 1000;
      PhyphoxBLE::write(first_difference_float, pressure);
    delay(60);
}


//void pressureChoice() {
//    first_time = millis();
//    pressure = BARO.readPressure();
//    if (abs(old_pres-pressure)>=1)
//    {
//      old_pres = pressure;
//      first_difference_float = ((float)first_time - (float)initial_time) / 1000;
//      PhyphoxBLE::write(first_difference_float, pressure);
//    }
//    first_difference_float = ((float)first_time - (float)initial_time) / 1000;
//    PhyphoxBLE::write(first_difference_float, pressure);
//    delay(300);
//}

void temperatureChoice() {
    first_time = millis();
    temperature = HTS.readTemperature();
    humidity = HTS.readHumidity();
    if (abs(old_temp-temperature)>=0.5 || abs(old_hum-humidity)>=1)
    {
      old_temp = temperature;
      old_hum = humidity;
      first_difference_float = ((float)first_time - (float)initial_time) / 1000;
      PhyphoxBLE::write(first_difference_float, temperature, humidity);
    }
    first_difference_float = ((float)first_time - (float)initial_time) / 1000;
    PhyphoxBLE::write(first_difference_float, temperature, humidity);
    delay(100); 
}

void temperatureChoiceDHT() {
    first_time = millis();
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();
    if (abs(old_temp-temperature)>=0.5 || abs(old_hum-humidity)>=1)
    {
      old_temp = temperature;
      old_hum = humidity;
      first_difference_float = ((float)first_time - (float)initial_time) / 1000;
      PhyphoxBLE::write(first_difference_float, temperature, humidity);
    }
    first_difference_float = ((float)first_time - (float)initial_time) / 1000;
    PhyphoxBLE::write(first_difference_float, temperature, humidity);
    delay(100); 
}

void lightChoice() {
  if (APDS.colorAvailable()) {
    first_time = millis();
    APDS.readColor(red, green, blue, ambient);
    red_float = (float)red;
    green_float = (float)green;
    blue_float = (float)blue;
    ambient_float = (float)ambient;
    first_difference_float = ((float)first_time - (float)initial_time) / 1000;
    PhyphoxBLE::write(first_difference_float, ambient_float, red_float, green_float, blue_float);
    delay(60);
  }
}

void accelerazioneCentripeta(){
 if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable())
     first_time = millis();
    IMU.readAcceleration(accx, accy, accz);
    acc=abs(sqrt(pow(accx,2)+pow(accy,2)+pow(accz,2))-1);
    
    IMU.readGyroscope(gyrx, gyry, gyrz);
    gyrx = pi*gyrx/180;
    gyry = pi*gyry/180;
    gyrz = pi*gyrz/180;
    gyr2=pow(gyrx,2)+pow(gyry,2)+pow(gyrz,2);
    gyr = sqrt(gyr2);
    first_difference_float = ((float)first_time - (float)initial_time) / 1000;
    PhyphoxBLE::write(first_difference_float, acc, gyr, gyr2);
    delay(300); 
}
void analogChoice() { // reads the analog input selected at the beginning
  first_time = millis();
  voltage1 = analogRead(analogInPin1)*3.3/1023.;
  voltage2 = analogRead(analogInPin2)*3.3/1023.;
  voltage3 = analogRead(analogInPin3)*3.3/1023.;
  first_difference_float = ((float)first_time - (float)initial_time) / 1000;
  PhyphoxBLE::write(first_difference_float, voltage1, voltage2, voltage3);
  delay(60);
}
