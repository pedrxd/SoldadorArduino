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
      response->addHeader("Cache-Control", "no-cache, must-revalidate");
      response->addHeader("Pragma", "no-cache");

      response->printf("<!doctype html><html lang=es dir=ltr><meta http-equiv=cache-control content=\"no-cache, no-store, must-revalidate\"><meta http-equiv=pragma content=no-cache><meta http-equiv=expires content=0><meta charset=utf-8><meta name=viewport content=\"width=device-width,initial-scale=1\"><style media=screen>body{padding:0;margin:0;border:0}#loading{position:fixed;top:5px;right:5px}.lds-dual-ring{display:inline-block;width:60px;height:60px}.lds-dual-ring:after{content:\" \";display:block;width:32px;height:32px;margin:8px;border-radius:50%%;border:6px solid #aaa;border-color:#f92e53 #0000 #f92e53 #0000;animation:lds-dual-ring 1.2s linear infinite}@keyframes lds-dual-ring{0%%{transform:rotate(0deg)}100%%{transform:rotate(360deg)}}.config{background-color:#034f84;display:flex;flex-direction:row;flex-wrap:wrap;justify-content:center;color:#fff}.config-contenedor{margin:20px;padding-bottom:20px;border-style:dashed;border-radius:10px;width:400px}.config-item{text-align:center}.footer-options{position:fixed;display:flex;width:100%%;max-width:800px;justify-content:center;bottom:0}.footer-button{border:none;color:#fff;padding:15px 32px;width:50%%;text-align:center;text-decoration:none;display:inline-block;font-size:16px}.footer-guardar{background-color:#4caf50}.footer-reset{background-color:#f44336}</style><title>Configurador</title><div id=loading class=lds-dual-ring style=visibility:hidden></div><div id=header class=header><h2 id=title>Configuracion: %s</h2></div><div id=config class=config><div id=config-default class=config-contenedor><div id=config-pulse class=config-item><h2>Tiempo de pulsación</h2><input type=number id=pulse value=%d></div><div id=config-intermediate class=config-item><h2>Tiempo entre pulsaciones</h2><input type=number id=intermediate value=%d></div><div class=config-item><h2>Modo: <span id=config-mode></span></h2><input type=range id=mode min=0 max=2 step=1 value=%d></div></div><div id=config-wifi class=config-contenedor style=visibility:hidden><div class=config-item><h2>SSID</h2><input id=ssid value=%s></div><div class=config-item><h2>Password</h2><input id=password value=%s></div></div><div id=footer class=footer-options style=visibility:hidden><button id=buttonsave class=\"footer-button footer-guardar\" type=button>Guardar</button>\n\
      <button id=buttonreset class=\"footer-button footer-reset\" type=button>Reset</button></div></div><script>var firstPress=false;function clickDown(e){var configwifi=document.getElementById(\"config-wifi\");if(firstPress===true){if(configwifi.style.visibility==\"hidden\")\n\
      configwifi.style.visibility=\"visible\";else\n\
      configwifi.style.visibility=\"hidden\";}else{firstPress=true;window.setTimeout(function(){firstPress=false;},200);}};function changeMode(value){var cdpulso=document.getElementById(\"config-pulse\");var cdintermedio=document.getElementById(\"config-intermediate\")\n\
      var modo=document.getElementById(\"config-mode\");if(value===\"0\"){cdpulso.style.visibility=\"visible\";cdintermedio.style.visibility=\"hidden\";modo.innerHTML=\"Unico\";}else if(value===\"1\"){cdpulso.style.visibility=\"visible\";cdintermedio.style.visibility=\"visible\";modo.innerHTML=\"Multiple\";}else{cdpulso.style.visibility=\"hidden\";cdintermedio.style.visibility=\"hidden\";modo.innerHTML=\"Continuo\";}}\n\
      function sendCommand(command,key=null,value=null){var petition=new XMLHttpRequest();petition.onreadystatechange=function(){if(petition.readyState==4&&petition.status==200)\n\
      loadFinish();}\n\
      var url=window.location.origin+\"/\"+command;if(key!=null){url+=\"?\"+key+\"=\"+value;}\n\
      petition.open(\"GET\",url,true);petition.send(null);}\n\
      function loadFinish()\n\
      {document.getElementById(\"loading\").style.visibility=\"hidden\";}\n\
      function loadStart(){document.getElementById(\"loading\").style.visibility=\"visible\";}\n\
      function onChangedValue(e){var target=e.target;loadStart();sendCommand(\"changeconfig\",target.id,target.value);if(target.id===\"mode\"){changeMode(target.value);}\n\
      document.getElementById(\"footer\").style.visibility=\"visible\";}\n\
      function onClickButton(e){var target=e.target;loadStart();if(target.id===\"buttonsave\")\n\
      {sendCommand(\"save\");}\n\
      else\n\
      {sendCommand(\"reset\");location.reload();return false;}\n\
      document.getElementById(\"footer\").style.visibility=\"hidden\";}\n\
      document.getElementById(\"config\").addEventListener(\"change\",onChangedValue);document.getElementById(\"config\").addEventListener(\"keyup\",onChangedValue);document.getElementById(\"buttonsave\").addEventListener(\"click\",onClickButton);document.getElementById(\"buttonreset\").addEventListener(\"click\",onClickButton);document.getElementById(\"title\").addEventListener(\"click\",clickDown);changeMode(document.getElementById(\"mode\").value);</script>", _config->getWifiSsid(), _config->getDuracionPulso(), _config->getDuracionIntermedio(), _config->getModoSoldador(), _config->getWifiSsid(), _config->getWifiPassword());
    }else if(url.equals("/changeconfig"))
    {
      if(request->params() > 0)
      {
        for (size_t i = 0; i < request->params(); i++) {
          AsyncWebParameter* p = request->getParam(i);
          String name = p->name();
          String value = p->value();
          if(name.equals("pulse"))
          {
            _config->setDuracionPulso(value.toInt());
            response->print("pulse=ok,");
          }else if(name.equals("intermediate"))
          {
            _config->setDuracionIntermedio(value.toInt());
            response->print("intermediate=ok,");
          }else if(name.equals("ssid"))
          {
            _config->setWifiSsid(value.c_str());
            response->print("ssid=ok,");
          }else if(name.equals("password"))
          {
            _config->setWifiPassword(value.c_str());
            response->print("password=ok,");
          }else if(name.equals("mode"))
          {
            switch(value.c_str()[0])
            {
              case '0':
                _config->setModoSoldador(MODO_SIMPLE);
                response->print("mode=ok,");
              break;
              case '1':
                _config->setModoSoldador(MODO_MULTIPLE);
                response->print("mode=ok,");
              break;
              case '2':
                _config->setModoSoldador(MODO_CONTINUO);
                response->print("mode=ok,");
                break;
              default:
                response->print("mode=error,");
            }
          }else
          {
            response->printf("%s=error", name.c_str());
          }
        }
      }
    }else if(url.equals("/save"))
    {
      _config->guardarConfiguracion();
      response->print("save=ok,");
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
