#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <EthernetENC.h>
#include <EthernetUdp.h>

#define PIN_SENSOR 2
#define PIN_RELAY 7
#define INTERVAL 60000  // 1 minute

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress loxone(192, 168, 0, 248);

/// PORTS that it will use
int portLoxone = 60000;    // Arduino -> Loxone (temps)
int portReceive = 60001;   // Loxone -> Arduino (bool)

OneWire oneWire(PIN_SENSOR);
DallasTemperature sensors(&oneWire);
EthernetUDP udp;

unsigned long lastTime = 0;

void setup() {
  Serial.begin(9600);
  sensors.begin();
  pinMode(PIN_RELAY, OUTPUT);
  digitalWrite(PIN_RELAY, LOW);

  Ethernet.begin(mac);
  udp.begin(portReceive);

  Serial.print("IP: ");
  Serial.println(Ethernet.localIP());
  Serial.println("Started!");
}

void loop() {
  // Receive commands from Loxone
  int packetSize = udp.parsePacket();
  if (packetSize) {
    char buf[32];
    memset(buf, 0, sizeof(buf));
    udp.read(buf, sizeof(buf));
    String message = String(buf);
    message.trim();

    Serial.print("Received: ");
    Serial.println(message);

    if (message == "on") {
      digitalWrite(PIN_RELAY, HIGH);
      Serial.println("Relay: on");
    } else if (message == "off") {
      digitalWrite(PIN_RELAY, LOW);
      Serial.println("Relay: off");
    }
  }

  // Send temperature once per minute
  unsigned long now = millis();
  if (now - lastTime >= INTERVAL) {
    lastTime = now;

    sensors.requestTemperatures();
    float temperature = sensors.getTempCByIndex(0);

    char message[32];
    dtostrf(temperature, 4, 1, message);

    udp.beginPacket(loxone, portLoxone);
    udp.print("temperature:");
    udp.print(message);
    udp.endPacket();

    Serial.print("Temperature sent: ");
    Serial.println(temperature);
  }
}