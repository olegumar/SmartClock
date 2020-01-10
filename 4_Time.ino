void Time_init() {
  HTTP.on("/Time", handle_Time);           // Синхронизировать время устройства по запросу вида /Time
  HTTP.on("/TimeZone", handle_time_zone);  // Установка времянной зоны по запросу вида http://192.168.0.101/TimeZone?timezone=3
  timeSynch(timezone);
}
void timeSynch(int zone){
  if (WiFi.status() == WL_CONNECTED) {
    // Настройка соединения с NTP сервером
    configTime(zone * 3600, 0, "pool.ntp.org" , "ru.pool.ntp.org" , "europe.pool.ntp.org");
    uint8_t i = 0;
    Serial.println("\nОжидаем время из сети 2 сек");
    while (!time(nullptr) && i < 20) {
      Serial.print(".");
      i++;
      delay(100);
    }
    Serial.println("\nITime Ready!");
    Serial.println(GetTime());
    Serial.println(GetDate());
  }
}
// Установка параметров времянной зоны по запросу вида http://192.168.0.101/TimeZone?timezone=5
void handle_time_zone() {
  timezone = HTTP.arg("timezone").toInt(); // Получаем значение timezone из запроса конвертируем в int сохраняем в глобальной переменной
  saveConfig();
  HTTP.send(200, "text/plain", "OK");
}

void handle_Time(){
  timeSynch(timezone);
  HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
  }

// Получение текущего времени
String GetTime() {
 time_t now = time(nullptr);  // получаем время с помощью библиотеки time.h
    Serial.println(now);      // ? врет 5*5760 
 String newTime = "";         // Строка для результатов времени
 newTime += ctime(&now);      // Преобразуем время в строку формата Thu Jan 19 00:55:35 2017
 uint8_t i = newTime.indexOf(":"); //Ишем позицию первого символа :
 newTime = newTime.substring(i - 2, i + 6); // Выделяем из строки 2 символа перед символом : и 6 символов после
 return newTime;              // Возврашаем полученное время
}

// Получение даты
String GetDate() {
 time_t now = time(nullptr); // получаем время с помощью библиотеки time.h
 String newData = ""; // Строка для результатов времени
 newData += ctime(&now); // Преобразуем время в строку формата Thu Jan 19 00:55:35 2017
 newData.replace("\n", ""); // Удаляем символ переноса строки
 uint8_t i = newData.lastIndexOf(" "); //Ишем позицию последнего символа пробел
 String newTime = newData.substring(i - 8, i+1); // Выделяем время и пробел
 newData.replace(newTime, ""); // Удаляем из строки 8 символов времени и пробел
 return newData; // Возврашаем полученную дату
}
// Получение дня недели
String GetWeekday() {
  String Data = GetDate();
  uint8_t i = Data.indexOf(" "); //Ишем позицию первого символа пробел
  String weekday = Data.substring(i - 3, i); // Выделяем время и пробел
  return weekday;
}
