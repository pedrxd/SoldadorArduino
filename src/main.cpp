/*
  This file is part of SoldadorArduino.

  SoldadorArduino is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version.

  SoldadorArduino is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with SoldadorArduino.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <DNSServer.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>

#include "CaptiveRequestHandler.h"

DNSServer dnsServer;
AsyncWebServer server(80);
Configuracion *config;

unsigned long esperaocupada = 0;
bool unicoUsado = false;

//Utilizado para limpiar impurezas en la entrada
unsigned long pulsadoHasta = 0;
unsigned long siguientePulso = 0;
bool pulsado = false;
bool input = false;

int pinpulsacion = 4;
int pinrele = 12;
int pinled = 13;



void setup() {
  //Iniciamos la configuracion
  config = new Configuracion(WiFi.macAddress().c_str());
  //Inicializamos el wifi
  WiFi.mode(WIFI_AP);
  WiFi.softAP(config->getWifiSsid(), config->getWifiPassword(), 1, false, 1);
  //Iniciamos servidor dns para responder siempre con nuestra IP
  dnsServer.start(53, "*", WiFi.softAPIP());
  //Inicializamos el servidor web
  server.addHandler(new CaptiveRequestHandler(config)).setFilter(ON_AP_FILTER);
  server.begin();

  //Inicializamos los pines
  pinMode(pinpulsacion, INPUT);
  pinMode(pinrele, OUTPUT);
  pinMode(pinled, OUTPUT);

  //Lo ponemos como low (enciende la led azul y apaga el rele)
  digitalWrite(pinrele, LOW);
  digitalWrite(pinled, LOW);

}

void darPunto()
{
  esperaocupada=millis()+config->getDuracionPulso();
  digitalWrite(pinrele, HIGH);
  digitalWrite(pinled, HIGH);
  /*
   * Realizamos una espera ocupada
   */
  while(esperaocupada>millis())
  {
    yield();
  }
  digitalWrite(pinrele, LOW);
  digitalWrite(pinled, LOW);
}

inline void comprobarInput()
{
  input = !digitalRead(pinpulsacion);

  if(input)
  {
    pulsado = true;
    pulsadoHasta = millis() + 100;
  }else
  {
    if(pulsadoHasta < millis())
    {
      pulsado = false;
      unicoUsado = false;
    }
  }
}

void loop() {
  //Respondemos las peticiones DNS
  dnsServer.processNextRequest();

  MODO_SOLDADOR modo = config->getModoSoldador();

  comprobarInput();


  /*
    Si esta en modo multiple. Se quedara a la escucha del pulsador y estara dando chispazos
    hasta que se suelte.
  */
  if(pulsado)
  {
    if(modo == MODO_MULTIPLE)
    {
      while(pulsado)
      {
        yield();
        comprobarInput();
        if(siguientePulso < millis())
        {
          darPunto();
          siguientePulso = millis() + config->getDuracionIntermedio();
        }
      }
    }
    else if(modo == MODO_SIMPLE)
    {
      if(!unicoUsado)
      {
        darPunto();
        unicoUsado = true;
      }
    }
  }

}
