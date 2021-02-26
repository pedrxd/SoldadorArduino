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
      //The chain contains  a minimized version of website/index.html
      response->printf("<!DOCTYPE html><html lang=\"es\" dir=\"ltr\"><head><meta charset=\"utf-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><style media=\"screen\">body{padding:0;margin:0;border:0}.config{background-color:#034f84;display:flex;flex-direction:row;flex-wrap:wrap;justify-content:center;color:white}.config-contenedor{margin:20px;padding-bottom:20px;border-style:dashed;border-radius:10px;width:400px}.config-item{text-align:center}.footer-options{position:fixed;display:flex;width:100%;max-width:600px;justify-content:center;bottom:0px}.footer-button{border:none;color:white;padding:15px 32px;width:50%;text-align:center;text-decoration:none;display:inline-block;font-size:16px}.footer-guardar{background-color:#4CAF50}.footer-reset{background-color:#f44336}</style><title>Configurador</title></head><body><h2 onmousedown=\"pulsar_titulo()\" onmouseup=\"despulsar_titulo()\">Configuracion: %s</h2><div class=\"config\"><div id=\"config-default\" class=\"config-contenedor\"><div id=\"config-dpulso\" class=\"config-item\"><h2>Tiempo de pulsación</h2> <input type=\"number\" id=\"dpulso\" onkeyup=\"cambiarConfig('dpulso', this.value)\" value=\"%d\"></div><div id=\"config-dintermedio\" class=\"config-item\"><h2>Tiempo entre pulsaciones</h2> <input type=\"number\" id=\"dintermedio\" onkeyup=\"cambiarConfig('dintermedio', this.value)\" value=\"%d\"></div><div class=\"config-item\"><h2>Modo: <span id=\"modo-type\"></span></h2> <input type=\"range\" id=\"modo\" min=\"0\" max=\"2\" step=\"1\" onchange=\"cambiarConfig('modo', this.value)\" value=\"%d\"></div></div><div id=\"config-wifi\" class=\"config-contenedor\" style=\"visibility: hidden;\"><div class=\"config-item\"><h2>SSID</h2> <input type=\"text\" id=\"ssid\" onkeyup=\"cambiarConfig('ssid', this.value)\" value=\"%s\"></div><div class=\"config-item\"><h2>Password</h2> <input type=\"text\" id=\"password\" onkeyup=\"cambiarConfig('password', this.value)\" value=\"%s\"></div></div><div id=\"footer\" class=\"footer-options\" style=\"visibility: hidden;\"> <button class=\"footer-button footer-guardar\" type=\"button\" onclick=\"guardarConfig()\">Guardar</button> <button class=\"footer-button footer-reset\" type=\"button\" onclick=\"resetConfig()\">Reset</button></div></div></body> <script>var pressTimer;function pulsar_titulo(){pressTimer=window.setTimeout(function(){\"hidden\"==document.getElementById(\"config-wifi\").style.visibility?document.getElementById(\"config-wifi\").style.visibility=\"visible\":document.getElementById(\"config-wifi\").style.visibility=\"hidden\"},1e3)}function despulsar_titulo(){clearTimeout(pressTimer)}function cambiarModo(e){\"0\"===e?(document.getElementById(\"config-dpulso\").style.visibility=\"visible\",document.getElementById(\"config-dintermedio\").style.visibility=\"hidden\",document.getElementById(\"modo-type\").innerHTML=\"Unico\"):\"1\"===e?(document.getElementById(\"config-dpulso\").style.visibility=\"visible\",document.getElementById(\"config-dintermedio\").style.visibility=\"visible\",document.getElementById(\"modo-type\").innerHTML=\"Multiple\"):(document.getElementById(\"config-dpulso\").style.visibility=\"hidden\",document.getElementById(\"config-dintermedio\").style.visibility=\"hidden\",document.getElementById(\"modo-type\").innerHTML=\"Continuo\")}function cambiarConfig(e,t){\"modo\"===e&&cambiarModo(t);var i=new XMLHttpRequest;i.onreadystatechange=function(){4==i.readyState&&200==i.status&&(document.getElementById(\"footer\").style.visibility=\"visible\")},i.open(\"GET\",window.location.origin+\"/cambiarconfig?\"+e+\"=\"+t,!0),i.send(null)}function guardarConfig(){var e=new XMLHttpRequest;e.onreadystatechange=function(){4==e.readyState&&200==e.status&&(document.getElementById(\"footer\").style.visibility=\"hidden\")},e.open(\"GET\",window.location.origin+\"/guardar\",!0),e.send(null)}function resetConfig(){var e=new XMLHttpRequest;e.onreadystatechange=function(){4==e.readyState&&200==e.status&&(document.getElementById(\"footer\").style.visibility=\"hidden\",document.location.reload(!0))},e.open(\"GET\",window.location.origin+\"/reset\",!0),e.send(null)}cambiarModo(document.getElementById(\"modo\").value);</script> </html>", _config->getWifiSsid(), _config->getDuracionPulso(), _config->getDuracionIntermedio(), _config->getModoSoldador(), _config->getWifiSsid(), _config->getWifiPassword());
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
              case '2':
                _config->setModoSoldador(MODO_CONTINUO);
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
