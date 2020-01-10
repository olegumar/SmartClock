void WIFIinit() {
  //  Попытка подключения к точке доступа
  IPAddress IPlocal;
  IPAddress IPgete;
  if (_ip0 != 0 && _ip3 != 0){   //Приметивная проверка на не существующий адрес
  IPlocal[0] = _ip0;             //Формируем наш IP желаемый (стат) адрес
  IPlocal[1] = _ip1;
  IPlocal[2] = _ip2;
  IPlocal[3] = _ip3;
  IPgete[0] = _ip0;             //Формируем IP адрес шлюза
  IPgete[1] = _ip1;
  IPgete[2] = _ip2;
  IPgete[3] = 1;
  WiFi.config(IPlocal, IPgete , IPAddress(255, 255, 255, 0));
  } else {
    IPAddress IPgete(0, 0, 0, 0);
  }
  WiFi.mode(WIFI_STA);
  byte tries = 11;
  WiFi.begin(_ssid.c_str(), _password.c_str());
  // Делаем проверку подключения до тех пор пока счетчик tries
  // не станет равен нулю или не получим подключение
  while (--tries && WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
  if (WiFi.status() != WL_CONNECTED)
  {
  // Если не удалось подключиться запускаем в режиме AP
    Serial.println("");
    Serial.println("WiFi up AP");
    StartAPMode();
  }
  else {
   //Иначе удалось подключиться отправляем сообщение
   // о подключении и выводим адрес IP 
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("IP gateway: ");
    Serial.println(WiFi.gatewayIP());
  }
}

bool StartAPMode() {
  IPAddress apIP(192, 168, 4, 1);
  // Отключаем WIFI
  WiFi.disconnect();
  // Меняем режим на режим точки доступа
  WiFi.mode(WIFI_AP);
  // Задаем настройки сети
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  // Включаем WIFI в режиме точки доступа с именем и паролем
  // хронящихся в переменных _ssidAP _passwordAP
  WiFi.softAP(_ssidAP.c_str(), _passwordAP.c_str());
  return true;
}

void SSDP_init(void) {
  // SSDP дескриптор
  HTTP.on("/description.xml", HTTP_GET, []() {
    SSDP.schema(HTTP.client());
  });
  //Если версия  2.0.0 закоментируйте следующую строчку
  SSDP.setDeviceType("upnp:rootdevice");
  SSDP.setSchemaURL("description.xml");
  SSDP.setHTTPPort(80);
  SSDP.setName(SSDP_Name);
  SSDP.setSerialNumber("001");
  SSDP.setURL("/");
  SSDP.setModelName("Gumarov Oleg");
  SSDP.setModelURL("https://github.com/olegumar");
  SSDP.setModelNumber("000000000001");
  SSDP.setManufacturer("Tretyakov Sergey");
  SSDP.setManufacturerURL("http://www.esp8266-arduinoide.ru");
  SSDP.begin();
}
