#include <ESP8266WiFi.h>              //https://github.com/esp8266/Arduino
#include <ESP8266WebServer.h>
#include <ESP8266SSDP.h>
#include <FS.h>                       // Файловая система
#include <ArduinoJson.h>              // Не выще версии 5
#include <time.h>
#include <WiFiUdp.h>
#include <NTPClient.h>                // Библиотека ддля определения времени через инет 
#include <ESP8266HTTPUpdateServer.h>  // Для обновления по "воздуху"
#include <WebSocketsServer.h>         // Библиотека для отправки данных на HTML
#include <Wire.h>                     // Библиотека для работы по I2C
#include <Adafruit_Sensor.h>          // Библиотеку для работы с датчиками
#include <Adafruit_BME280.h>

// Web интерфейс для устройства
ESP8266WebServer HTTP(80);
// Объект для обнавления с web страницы
ESP8266HTTPUpdateServer httpUpdater;
// Объект позволяющий отправлять и получать пакеты по UDP
WiFiUDP ntpUDP;
// Объект позволяющий получать время с NTP по UDP, задаем параметры
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 5*3600, 6000);
// Для файловой системы 
File fsUploadFile;
// Объект для  webSocket
WebSocketsServer webSocket = WebSocketsServer(81);
// Датчик BME280
#define SEALEVELPRESSURE_HPA (1013.25)                 // Задаем высоту
 Adafruit_BME280 bme;                                  // Cоздаем объект 

// Определяем переменные
String _ssid     = "";            // Для хранения SSID "RTK70"
String _password = "";            // Для хранения пароля "77596501392"
String _ssidAP = "WiFi";          // SSID AP точка доступа
String _passwordAP = "";          // Пароль точки доступа
String SSDP_Name = "Smart";       // Имя нашего устройства SSDP
unsigned char _ip0 = 0;           // Для хранения определения нашего (желаемого)адресса
unsigned char _ip1 = 0;           // Для хранения определения нашего (желаемого)адресса
unsigned char _ip2 = 0;           // Для хранения определения нашего (желаемого)адресса
unsigned char _ip3 = 0;           // Для хранения определения нашего (желаемого)адресса
unsigned char _ip4 = 0;           // Для хранения настройки адресса клиента
String _IPaddress = "";           // Для хранения адресса клиента
signed char timezone = 5;         // Часовой пояс GTM
String _time = "{}";              // Для хранения текущ время
String _date = "{}";              // Для хранения текущ даты
String jsonConfig = "{}";         // данные для config.json
String jsonSetup = "{}";          // данные для setup.json
String jsonData = "{}";           // данные для data.json

int D1_pin = D1;               // Пины для реле 1
int D2_pin = D2;               // Пины для реле 2
int stat[4] = {1,2,3,4};       // Массив для счетчиков
int _ssHaus = 1;              // Для хранения освещения в доме
int _tmHaus = 1;              // Для хранения температуры в доме
int _phHaus = 1;              // Для хранения влажности в доме
int _phDHT = 1;               // Для хранения установки влажности
int _tmOut = 0;               // Для хранения температуры во дворе
int _phOut = 0;               // Для хранения влажности во дворе 
int _atmOut = 0;              // Для хранения давление во дворе

void setup() {
  pinMode(D1_pin, OUTPUT);
  digitalWrite(D1_pin, HIGH);
  pinMode(D2_pin, OUTPUT);
  digitalWrite(D2_pin, HIGH);
  Serial.begin(115200);
  Wire.begin(D3, D4);                 // порты для работы датчика (SDA_PIN, SCL_PIN)
  //Wire.setClock(100000);
  Serial.println("Start - FS");       // Запускаем файловую систему
  FS_init();
  Serial.println("Start FileConfig"); // Загружаем данные из config.json в глобальные переменные
  loadConfig();
  Serial.println("Start WIFI");       // Запускаем WIFI
  WIFIinit();
  Serial.println("Start SSDP");       // Настраиваем и запускаем SSDP интерфейс
  SSDP_init();
  Serial.println("Start WebServer");  // Загружаем сервер
  HTTP_init();
  Serial.println("Start WebSoket");   // Настраиваем и запускаем webSoket интерфейс
  webSoket_init();
  Serial.println("Start Time");       // Получаем время из сети
  Time_init();
  timeClient.begin();
  Serial.println("Start BME280");     // Инициализация датчика BME280
  bme_init();
}
void loop() {
  HTTP.handleClient();
  webSocket.loop();     // Работа webSocket
  bme_load();      // Опрос датчика BME280
  clock_load();    // Опрос времени
  data_print();     // Выводим инфу в Serial порт
  delay(1);
}
