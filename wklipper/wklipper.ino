// socat pty,link=/tmp/virtualcom0,ispeed=b250000,ospeed=b250000,raw,echo=0 tcp:192.168.4.1:8888,nodelay

/*
  wklipper - Transparent UART to Telnet Server for esp8266

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
#include <ESP8266WiFi.h>

#ifndef APSSID
#define APSSID "ESPap"
#define APPSK  "thereisnospoon"
#endif

#define BAUD_SERIAL 115200
#define BAUD_LOGGER 115200
#define RXBUFFERSIZE 8192

#include <SoftwareSerial.h>
SoftwareSerial* logger = nullptr;

#define MAX_SRV_CLIENTS 1
const char *ssid = APSSID;
const char *password = APPSK;
const int port = 8888;

WiFiServer server(port);
WiFiClient serverClients[MAX_SRV_CLIENTS];

void setup() {
  delay(1000);

  Serial.begin(BAUD_SERIAL);
  Serial.setRxBufferSize(RXBUFFERSIZE);

  Serial.swap();
  logger = new SoftwareSerial(3, 1);
  logger->begin(BAUD_LOGGER);
  logger->enableIntTx(false);
  logger->println("\n\nUsing SoftwareSerial for logging");

  logger->print("Configuring access point...\n");
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  logger->print("AP IP address: ");
  logger->println(myIP);

  server.begin();
  server.setNoDelay(true);

  logger->print("Ready! Use 'telnet ");
  logger->print(WiFi.localIP());
  logger->printf(" %d' to connect\n", port);
}

void loop() {
  if (server.hasClient()) {
    int i;
    for (i = 0; i < MAX_SRV_CLIENTS; i++)
      if (!serverClients[i]) {
        serverClients[i] = server.available();
        logger->print("New client: index ");
        logger->print(i);
        break;
      }

    if (i == MAX_SRV_CLIENTS) {
      server.available().println("busy");
      logger->printf("server is busy with %d active connections\n", MAX_SRV_CLIENTS);
    }
  }

  if (serverClients[0] && serverClients[0].connected()){
    if(serverClients[0].available()) {
      while(serverClients[0].available()) {
        Serial.write(serverClients[0].read());
      }
    }
  }

  if(Serial.available()){
    size_t len = Serial.available();
    uint8_t sbuf[len];
    Serial.readBytes(sbuf, len);
    if (serverClients[0] && serverClients[0].connected()){
      serverClients[0].write(sbuf, len);
    }
  }
}
