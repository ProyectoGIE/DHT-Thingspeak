#include <DHT.h>
#include <ESP8266WiFi.h>
 

/*-----------------ESP8266 Configuración WiFi---------------*/
String apiKey = "APIKEY";                   // Sustituir con tu API del canal de thingspeak
const char* ssid = "SSID";                    //Poner el SSID de la WiFi
const char* password = "PASSWORD";  //Sustituir por el password de la WiFi
const char* server = "api.thingspeak.com";
/*----------------------------------------------------------*/

/*------------------------DHT SENSOR------------------------*/
#define DHTPIN D4           //Pin al que se encuentra conectado el pin de datos del DHT
#define DHTTYPE DHT22       //Si usamos el DHT11 lo cambiaremos aquí
DHT dht(DHTPIN, DHTTYPE);   //Inicializa el sensor DHT
/*----------------------------------------------------------*/

//En el setup vamos a conectar a la WiFi

WiFiClient client;
 
void setup() 
{
Serial.begin(115200);
delay(10);
dht.begin();
 
WiFi.begin(ssid, password);
 
Serial.println();
Serial.println();
Serial.print("Connecting to ");
Serial.println(ssid);
 
WiFi.begin(ssid, password);
 
while (WiFi.status() != WL_CONNECTED) 
{
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("WiFi connected");
 
}
 
void loop() 
{
 
float h = dht.readHumidity();
float t = dht.readTemperature();
if (isnan(h) || isnan(t)) 
{
Serial.println("Failed to read from DHT sensor!");
return;
}
 
if (client.connect(server,80)) {
String postStr = apiKey;
postStr +="&field1=";
postStr += String(t);
postStr +="&field2=";
postStr += String(h);
postStr += "\r\n\r\n";
 
client.print("POST /update HTTP/1.1\n");
client.print("Host: api.thingspeak.com\n"); 
client.print("Connection: close\n");
client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
client.print("Content-Type: application/x-www-form-urlencoded\n");
client.print("Content-Length: ");
client.print(postStr.length());
client.print("\n\n");
client.print(postStr);
 
Serial.print("Temperatura: ");
Serial.print(t);
Serial.print(" ºC ");
Serial.println("");
Serial.print("Humedad: ");
Serial.print(h);
Serial.print("%");
Serial.println("");
Serial.print("Enviando datos a Thingspeak");
Serial.println("");
}
client.stop();
 
Serial.println("Esperando 20 secs");
// ThingSpeak necesita al menos 15 segundos entre cada subida
// Ponemos 20 para estar más seguros
delay(20000);
}
