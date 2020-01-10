 void bme_init() {
 //if (!bme.begin(&Wire)) {                         // Строка, если не знаем точный адресс
  if (!bme.begin(0x76)) {                                // Проверка инициализации датчика BME280
     Serial.println("Не удалось найти датчик BME280!");  // Печать сообщения об ошибки
     scanner_I2C();                                      // сканируем I2C, ищем адресс
     //while (1);                      // Строка перезагрузит скетч, пока не произойдет инициализация
   }
 }
 void bme_load() {
 if (stat[1] >= 2000) {    // включим задачу через 2 сек
 _tmOut = bme.readTemperature();              // сохраняем температуру в глоб переменную
 _phOut = bme.readHumidity();                 // сохраняем влажность
 _atmOut = bme.readPressure() / 100.0F;       // сохраняем атмосферное давление
     Serial.println("-ПЕРЕДАЕМ ДАННЫЕ Датчика BME280-");       // Новая строка
  SoketData ("tmOut", (String)(_tmOut), jsonRead(jsonData,"tmOut"));
  SoketData ("phOut", (String)(_phOut), jsonRead(jsonData,"phOut"));
  SoketData ("atmOut", (String)(_atmOut), jsonRead(jsonData,"atmOut"));
 stat[1] = 0;                                      // обнуляем счетчик
  } else {
    stat[1] ++;
  }
}
void data_print() {
  if (stat[0] >= 10000) {    // включим задачу через 10 сек
   //  ntp_time();
   Serial.print("\nДанные на ");
   Serial.print(GetDate());
   Serial.print(" в "); 
   Serial.println(GetTime());
 Serial.print("Температура дома = ");                    //
   Serial.print(_tmHaus);                                // Печать температуры
   Serial.println("*C");                                 //
 Serial.print("Влажность дома = ");                      //
   Serial.print(_phHaus);                                // Печать влажности
   Serial.println("%");                                  //
 Serial.print("Температура на улице = ");                //
   Serial.print(_tmOut);                                 // Печать температуры
   Serial.println("*C");                                 //
 Serial.print("Влажность на улице = ");                  //
   Serial.print(_phOut);                                 // Печать влажности
   Serial.println("%");                                  //
 Serial.print("Давление на улице = ");                   //
   Serial.print(_atmOut);                                // Печать атмосферное давление
   Serial.println("hPa");                                //
 Serial.print("Высота над уровнем моря = ");             //
   Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA)); // Вычисление высоты над уровнем моря
   Serial.println("m");                                  //
 Serial.println();                                       // Новая строка
    stat[0] = 0;                                         // обнуляем счетчик
  } else {
    stat[0] ++;
  }
}
void ntp_time() {
  timeClient.update();
  Serial.println("\nВремя через NTP:");
  Serial.println(timeClient.getFormattedTime());
  Serial.println(timeClient.getDay());
}
/*
 Datatypes 
 NTPClient 
 
begin 
end
update
forceUpdate
getDay
getHours
getMinutes
getSeconds
getFormattedTime
getEpochTime
setTimeOffset() -> смещение в секундах (zona)
setUpdateInterval() -> интервал обновленияв миллисекундах
setPoolServerName
*/
// ------------- Сканер для определения адресса I2C модуля -------- //
void scanner_I2C(){
    byte error, address;
    int nDevices;
  Serial.println("Сканируем I2C...");
    nDevices = 0;
    for(address = 8; address < 127; address++ ){
        Wire.beginTransmission(address);
        error = Wire.endTransmission();
        if (error == 0){
            Serial.print("I2C устройство найдено по адресу 0x");
            if (address<16)
                Serial.print("0");
            Serial.print(address,HEX);
            Serial.println(" !");
            nDevices++;
        }
        else if (error==4) {
            Serial.print("Неизвестная ошибка по адресу 0x");
            if (address<16)
                Serial.print("0");
            Serial.println(address,HEX);
        } 
    }
    if (nDevices == 0)
        Serial.println("I2C устройство не найдено\n");
    else
        Serial.println("done\n");
}
