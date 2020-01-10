// ----------------------------
void webSoket_init() {
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:  // Событие происходит при отключени клиента 
      Serial.println("web Socket disconnected");
      break;
    case WStype_CONNECTED: // Событие происходит при подключении клиента
      {
        Serial.println("web Socket Connected"); 
        webSocket.sendTXT(num, jsonData); // Отправим всю строку с данными используя номер клиента он в num
      }
      break;
    case WStype_TEXT: // Событие происходит при получении данных текстового формата из webSocket
      // webSocket.sendTXT(num, "message here"); // Можно отправлять любое сообщение как строку по номеру соединения
      // webSocket.broadcastTXT("message here");
      break;
    case WStype_BIN:      // Событие происходит при получении бинарных данных из webSocket
      // webSocket.sendBIN(num, payload, length);
      break;
  }
}
// Отправка данных в Socket всем получателям
// Параметры Имя ключа, Данные, Предыдущее значение
void SoketData (String key, String data, String data_old)  {
  if (data_old != data) {                   // сравниваем с преждними данными
    String broadcast = "{}";
    jsonWrite(broadcast, key, data);        // записываем новые данные в broadcast
    webSocket.broadcastTXT(broadcast);      // отправка всем клиентам
  jsonWrite(jsonData, key, data);           // сохраняем новые данные
  saveData();                               // сохраняем данные в файл data.json
     // Serial.println(broadcast);
  }
}

void clock_load() {
 if (stat[2] >= 500) {    // включим задачу через 0,5 сек
  _time = GetTime();
  _date = GetDate();
      Serial.println("-ПЕРЕДАЕМ ДАННЫЕ Время и Дата-");   //
    SoketData ("time", _time, jsonRead(jsonData,"time"));
    SoketData ("date", _date, jsonRead(jsonData,"date"));
  stat[2] = 0;                                          // обнуляем счетчик
  } else {
    stat[2] ++;
  } 
}
