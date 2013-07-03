#include <SPI.h>
#include <Ethernet.h>
#include <MemoryFree.h>

byte mac[] = {<your-ethernet-shield-mac>};
IPAddress ip(192,168,1,20);

EthernetClient client;
char serverName[] = "<your-jenkins-proxy>";

String currentLine = "";

char eor[] = {'\r', '\n', '\r', '\n', '>', '\0'};

boolean capture = false;

int r = 2;
int o = 7;
int g = 8;

void setup()
{
  pinMode(r, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(o, OUTPUT);

  Serial.begin(9600);

  Serial.println(F("new IP address using DHCP"));
  if(!Ethernet.begin(mac)){
    Serial.println(F("DHCP failure, trying manually"));
    Ethernet.begin(mac, ip);
  }
  Serial.print(F("My address:"));
  Serial.println(Ethernet.localIP());
  connectToServer();
}

void loop()
{
  if(client.connected() && client.available()){
    char inChar = client.read();

    if(capture){
      Serial.println(inChar);
      capture = false;
      Serial.println(client.available());
      switch(inChar){
      case 'R':
        digitalWrite(r, HIGH);
        digitalWrite(o, LOW);
        digitalWrite(g, LOW);
        Serial.println("Red");
        break;
      case 'O':
        digitalWrite(r, LOW);
        digitalWrite(o, HIGH);
        digitalWrite(g, LOW);
        Serial.println("Orange");
        break;
      case 'G':
        digitalWrite(r, LOW);
        digitalWrite(o, LOW);
        digitalWrite(g, HIGH);
        Serial.println("Green");
        break;
      default:
        digitalWrite(r, LOW);
        digitalWrite(o, LOW);
        digitalWrite(g, LOW);
        Serial.println("All Off");
        break;
      }
    }

    currentLine += inChar;
    if(currentLine.endsWith(eor)){
      Serial.println(F("CAPTURE"));
      capture = true;
      Serial.println(client.available());
    }

    if(!client.connected() && !client.available()){
      Serial.println();

      Serial.print(F("Free Memory: "));
      Serial.println(freeMemory());

      client.stop();

      currentLine = "";
      capture = false;

      delay(5000);

      connectToServer();
    }
  }
}

void connectToServer(){
  Serial.println(F("Connecting to server..."));
  if(client.connect(serverName, 1983)){
    Serial.println(F("HTTP request..."));

    client.println(F("GET / HTTP/1.1"));
    client.println(F("HOST: <your-jenkins-host>"));
    client.println();
  }
}
