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

#include "Configuracion.h"

Configuracion::Configuracion(const char *mac)
{
  bool primerInicio = false;

  EEPROM.begin(256);

  cargarConfiguracion();
  for(int i = 0; i < 6; i++)
  {
    if(config._mac[i] != mac[i])
    {
      primerInicio = true;
      break;
    }
  }

  if(primerInicio)
  {
    for(int i = 0; i < 6; i++)
    {
      config._mac[i] = mac[i];
    }

    setDuracionIntermedio(1000);
    setDuracionPulso(50);
    setModoSoldador(MODO_SIMPLE);

    setWifiSsid("Solador\0");
    setWifiPassword("soldador1234\0");

    guardarConfiguracion();
  }
}

void Configuracion::guardarConfiguracion()
{
  EEPROM.put(0, config);
  EEPROM.commit();
}

void Configuracion::cargarConfiguracion()
{
  EEPROM.get(0, config);
}

void Configuracion::setModoSoldador(MODO_SOLDADOR modo)
{
  config._modo = modo;
}

void Configuracion::setDuracionIntermedio(int dintermedio)
{
  config._duracion_intermedio = dintermedio;
}

void Configuracion::setDuracionPulso(int dpulsos)
{
  config._duracion_pulso = dpulsos;
}

void Configuracion::setWifiSsid(const char *ssid)
{
  int i;
  for(i = 0; ssid[i] != '\0'; i++)
  {
    config._wifi_ssid[i] = ssid[i];
  }
  config._wifi_ssid[i++] = '\0';
}

void Configuracion::setWifiPassword(const char *password)
{
  int i;
  for(i = 0; password[i] != '\0'; i++)
  {
    config._wifi_password[i] = password[i];
  }
  config._wifi_password[i++] = '\0';
}

MODO_SOLDADOR Configuracion::getModoSoldador()
{
  return config._modo;
}

int Configuracion::getDuracionPulso()
{
  return config._duracion_pulso;
}

int Configuracion::getDuracionIntermedio()
{
  return config._duracion_intermedio;
}

char* Configuracion::getWifiSsid()
{
  return config._wifi_ssid;
}

char* Configuracion::getWifiPassword()
{
  return config._wifi_password;
}
