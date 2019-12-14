// Загрузка данных сохраненных в файл  config.json
bool loadConfig() {
  // Открываем файл для чтения
  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
  // если файл не найден 
    Serial.println("Failed to open config file");
  // Создаем файл запиав в него данные по умолчанию
    saveConfig();
    configFile.close();
    return false;
  }
  // Проверяем размер файла, будем использовать файл размером меньше 2048 байта
  size_t size = configFile.size();
  if (size > 2048) {
    Serial.println("Config file size is too large");
    configFile.close();
    return false;
  }

//загружаем файл конфигурации в глобальную переменную
  jsonConfig = configFile.readString();
  configFile.close();
  //Резервируем память для json обекта буфер может рости по мере необходимости предпочтительно 
  DynamicJsonBuffer jsonBuffer;
  //  вызовите парсер JSON через экземпляр jsonBuffer
  //  строку возьмем из глобальной переменной String jsonConfig
  JsonObject& root = jsonBuffer.parseObject(jsonConfig);
  //Теперь можно получить значения из root  
    _ssidAP = root["ssidAPName"].as<String>(); //Так получаем строку
    _passwordAP = root["ssidAPPassword"].as<String>();
    timezone = root["timezone"];               //Так получаем число
    SSDP_Name = root["SSDPName"].as<String>();
    _ssid = root["ssidName"].as<String>();
    _password = root["ssidPassword"].as<String>();
    _ip0 = root["ip0"];
    _ip1 = root["ip1"];
    _ip2 = root["ip2"];
    _ip3 = root["ip3"];
    _ip4 = root["ip4"];
    _IPaddress = root["IPaddress"].as<String>();
//    _ssHaus = root["ssHaus"];
//    _tmHaus = root["tmHaus"];
//    _phHaus = root["phHaus"];
//    _phDHT = root["phDHT"];
    return true;
}

// Запись данных в файл config.json
bool saveConfig() {
  // Резервируем память для json обекта буфер может рости по мере необходимти предпочтительно для ESP8266
  DynamicJsonBuffer jsonBuffer;
  // вызовите парсер JSON через экземпляр jsonBuffer 
  JsonObject& json = jsonBuffer.parseObject(jsonConfig);
  // Заполняем поля  json 
  json["SSDPName"] = SSDP_Name;
  json["ssidAPName"] = _ssidAP;
  json["ssidAPPassword"] = _passwordAP;
  json["ssidName"] = _ssid;
  json["ssidPassword"] = _password;
  json["ip0"] = _ip0;
  json["ip1"] = _ip1;
  json["ip2"] = _ip2;
  json["ip3"] = _ip3;
  json["ip4"] = _ip4;
  json["timezone"] = timezone;
  json["IPaddress"] = _IPaddress;
//  json["ssHaus"] = _ssHaus;
//  json["tmHaus"] = _tmHaus;
//  json["phHaus"] = _phHaus;
//  json["phDHT"] = _phDHT;
  
  // Помещаем созданный json в глобальную переменную json.printTo(jsonConfig);
  json.printTo(jsonConfig);
  // Открываем файл для записи
  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    //Serial.println("Failed to open config file for writing");
    configFile.close();
    return false;
  }
  // Записываем строку json в файл 
  json.printTo(configFile);
  configFile.close();
  return true;
  }
