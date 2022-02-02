// socat pty,link=/tmp/virtualcom0,ispeed=b250000,ospeed=b250000,raw,echo=0 tcp:192.168.4.1:8888,nodelay

/*
  wklipper - Transparent UART to Telnet Server for esp32

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

#ifndef APSSID
#define APSSID "ESPap"
#define APPSK  "thereisnospoon"
#endif

#define BAUD_SERIAL 115200
#define BAUD_LOGGER 115200
#define RXBUFFERSIZE 1024

const char *ssid = APSSID;
const char *password = APPSK;

const int port = 8888;

WiFiServer server(port);
WiFiClient serverClients;

void setup() {
  Serial2.begin(BAUD_SERIAL);
  Serial2.setRxBufferSize(RXBUFFERSIZE);

  Serial.begin(BAUD_LOGGER);
  Serial.println("\n\nUsing SoftwareSerial for logging");
  Serial.printf("Serial baud: %d (8n1: %d KB/s)\n", BAUD_SERIAL, BAUD_SERIAL * 8 / 10 / 1024);
  Serial.printf("Serial receive buffer size: %d bytes\n", RXBUFFERSIZE);

  Serial.print("Configuring access point...\n");
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  server.begin();
  server.setNoDelay(true);

  Serial.print("Ready! Use 'telnet ");
  Serial.print(WiFi.localIP());
  Serial.printf(" %d' to connect\n", port);
}

void loop() {
  uint8_t i;

  if (server.hasClient()){
    if (!serverClients || !serverClients.connected()){
      if(serverClients) serverClients.stop();
      serverClients = server.available();
      if (!serverClients) Serial.println("available broken");
      Serial.print("New client: ");
      Serial.print(i); Serial.print(' ');
      Serial.println(serverClients.remoteIP());
    }
  }

  if (serverClients && serverClients.connected()){
    if(serverClients.available()) {
      while(serverClients.available()) {
        Serial2.write(serverClients.read());
      }
    }
  }

  if(Serial2.available()){
    size_t len = Serial2.available();
    uint8_t sbuf[len];
    Serial2.readBytes(sbuf, len);
    if (serverClients && serverClients.connected()){
      serverClients.write(sbuf, len);
    }
  }
}
