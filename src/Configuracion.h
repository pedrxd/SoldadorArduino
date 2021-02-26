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

#include <EEPROM.h>

#ifndef CONFIGURACION_H
#define CONFIGURACION_H

enum MODO_SOLDADOR
{
  MODO_SIMPLE,
  MODO_MULTIPLE,
  MODO_CONTINUO,
};

struct MEM_CONFIG
{
  char _mac[8];

  int _duracion_pulso;
  int _duracion_intermedio;

  MODO_SOLDADOR _modo;

  char _wifi_ssid[64];
  char _wifi_password[64];
};

class Configuracion
{
public:
  Configuracion(const char *mac);

  void setModoSoldador(MODO_SOLDADOR modo);
  void setDuracionPulso(int dpulsos);
  void setDuracionIntermedio(int dintermedio);
  void setWifiSsid(const char *ssid);
  void setWifiPassword(const char *password);

  MODO_SOLDADOR getModoSoldador();
  int getDuracionPulso();
  int getDuracionIntermedio();
  char* getWifiSsid();
  char* getWifiPassword();


  void cargarConfiguracion();
  void guardarConfiguracion();

private:

  struct MEM_CONFIG config;

};

#endif
