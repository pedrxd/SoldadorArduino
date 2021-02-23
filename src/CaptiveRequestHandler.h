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

#include "ESPAsyncWebServer.h"
#include "Configuracion.h"

void(* resetFunc) (void) = 0;

class CaptiveRequestHandler : public AsyncWebHandler {
public:
  CaptiveRequestHandler(Configuracion *config) : _config(config) {}

  virtual ~CaptiveRequestHandler() {}

  bool canHandle(AsyncWebServerRequest *request){
    //request->addInterestingHeader("ANY");
    return true;
  }

  void handleRequest(AsyncWebServerRequest *request) {
    AsyncResponseStream *response = request->beginResponseStream("text/html");
    String url = request->url();
    if(url.equals("/"))
    {
      //ToDo: Añadir la nueva página web
      response->printf("<!DOCTYPE html><html lang=\"es\" dir=\"ltr\"><head><meta charset=\"utf-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><style media=\"screen\">body{padding:0;margin:0;border:0}.config{background-color:#034f84;display:flex;flex-direction:row;flex-wrap:wrap;justify-content:center;color:white}.config-contenedor{margin:20px;padding-bottom:20px;border-style:dashed;border-radius:10px;width:400px}.config-item{text-align:center}.footer-options{position:fixed;display:flex;width:100%;max-width:600px;justify-content:center;bottom:0px}.footer-button{border:none;color:white;padding:15px 32px;width:50%;text-align:center;text-decoration:none;display:inline-block;font-size:16px}.footer-guardar{background-color:#4CAF50}.footer-reset{background-color:#f44336}</style> <script>function cambiarConfig(t,e){var n=new XMLHttpRequest;n.onreadystatechange=function(){4==n.readyState&&200==n.status&&(document.getElementById(\"footer\").style.visibility=\"visible\")},n.open(\"GET\",window.location.origin+\"/cambiarconfig?\"+t+\"=\"+e,!0),n.send(null)}function guardarConfig(){var t=new XMLHttpRequest;t.onreadystatechange=function(){4==t.readyState&&200==t.status&&(document.getElementById(\"footer\").style.visibility=\"hidden\")},t.open(\"GET\",window.location.origin+\"/guardar\",!0),t.send(null)}function resetConfig(){var t=new XMLHttpRequest;t.onreadystatechange=function(){4==t.readyState&&200==t.status&&(document.location.reload(true))},t.open(\"GET\",window.location.origin+\"/reset\",!0),t.send(null)}</script> <title>Configurador</title></head><body><h2>Configuracion</h2><div class=\"config\"><div class=\"config-contenedor\"><div class=\"config-item\"><h2>Tiempo de pulsación</h2> <input type=\"number\" id=\"dpulso\" onchange=\"cambiarConfig('dpulso', this.value)\" value=\"%d\"></div><div class=\"config-item\"><h2>Tiempo entre pulsaciones</h2> <input type=\"number\" id=\"dintermedio\" onchange=\"cambiarConfig('dintermedio', this.value)\" value=\"%d\"></div><div class=\"config-item\"><h2>Modo</h2> <input type=\"range\" id=\"modo\" min=\"0\" max=\"1\" step=\"1\" onchange=\"cambiarConfig('modo', this.value)\" value=\"%d\"></div></div><div class=\"config-contenedor\"><div class=\"config-item\"><h2>SSID</h2> <input type=\"text\" id=\"ssid\" onchange=\"cambiarConfig('ssid', this.value)\" value=\"%s\"></div><div class=\"config-item\"><h2>Password</h2> <input type=\"text\" id=\"password\" onchange=\"cambiarConfig('password', this.value)\" value=\"%s\"></div></div><div id=\"footer\" style=\"visibility: hidden;\" class=\"footer-options\"> <button class=\"footer-button footer-guardar\" type=\"button\" onclick=\"guardarConfig()\">Guardar</button> <button class=\"footer-button footer-reset\" type=\"button\" onclick=\"resetConfig()\">Reset</button></div></div></body></html>", _config->getDuracionPulso(), _config->getDuracionIntermedio(), _config->getModoSoldador(), _config->getWifiSsid(), _config->getWifiPassword());
    }else if(url.equals("/cambiarconfig"))
    {
      if(request->params() > 0)
      {
        for (size_t i = 0; i < request->params(); i++) {
          AsyncWebParameter* p = request->getParam(i);
          String name = p->name();
          String value = p->value();
          if(name.equals("dpulso"))
          {
            _config->setDuracionPulso(value.toInt());
            response->print("dpulso=ok,");
          }else if(name.equals("dintermedio"))
          {
            _config->setDuracionIntermedio(value.toInt());
            response->print("dintermedio=ok,");
          }else if(name.equals("ssid"))
          {
            _config->setWifiSsid(value.c_str());
            response->print("ssid=ok,");
          }else if(name.equals("password"))
          {
            _config->setWifiPassword(value.c_str());
            response->print("password=ok,");
          }else if(name.equals("modo"))
          {
            switch(value.c_str()[0])
            {
              case '0':
                _config->setModoSoldador(MODO_SIMPLE);
                response->print("modo=ok,");
              break;
              case '1':
                _config->setModoSoldador(MODO_MULTIPLE);
                response->print("modo=ok,");
              break;
              default:
                response->print("modo=error,");
            }
          }else
          {
            response->printf("%s=error", name.c_str());
          }
        }
      }
    }else if(url.equals("/guardar"))
    {
      _config->guardarConfiguracion();
      response->print("guardar=ok,");
    }else if(url.equals("/reset"))
    {
      _config->cargarConfiguracion();
      response->print("reset=ok,");
    }else if(url.equals("/restart"))
    {
      resetFunc();
    }

    request->send(response);
  }

private:
  Configuracion *_config;
};
