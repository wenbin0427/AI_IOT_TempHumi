#include <WiFi.h>          // 引入Wifi
#include <PubSubClient.h>  // 引入 PubSubClient使用 MQTT
#include <HTTPClient.h>
#include <DHT.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>

#define DHT11PIN 16

LiquidCrystal_I2C lcd(0x27, 16, 2);

DHT dht(DHT11PIN, DHT11);
//red 15
//green 2
//blue 4

// WiFi 配置信息
const char* ssid = "KS_3F-3B_2.4G";         // WiFi SSID
const char* password = "12345678900000";  // WiFi密碼

// MQTT 配置信息
const char* mqtt_server = "34.81.46.57";  // MQTT 地址
const int mqtt_port = 1883;               // MQTT 端口
const char* mqtt_user = "a79899569";      // MQTT帳號
const char* mqtt_pass = "a79899578";      // MQTT密碼
// const char* mqtt_topic = "test/temp";        // 主題
const char* serverName = "http://34.81.46.57/insert_data.php";  // 伺服器及插入資料庫php位置

WiFiClient espClient;            // WiFi
PubSubClient client(espClient);  // MQTT

// 連接至WiFi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

//連接到MQTT
void reconnect() {
  // 不斷嘗試連線
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");

    // 嘗試連接，傳入 ID、帳號和密碼
    if (client.connect("ESP32Client", mqtt_user, mqtt_pass)) {
      Serial.println("Connected to MQTT");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" Trying again in 5 seconds...");
      delay(5000);
    }
  }
}


const int redPin = 15;
const int greenPin = 2;
const int bluePin = 4;

const int humi_redPin = 27;
const int humi_greenPin = 26;
const int humi_bluePin = 25;

float humi;
float temp;

// 初始化
void setup() {
  Serial.begin(115200);
  dht.begin();

  // lcd設置
  lcd.init();       // LCD初始化
  lcd.backlight();  // 開啟背光
  lcd.clear();      // 清除先前畫面

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  pinMode(humi_redPin, OUTPUT);
  pinMode(humi_greenPin, OUTPUT);
  pinMode(humi_bluePin, OUTPUT);

  setup_wifi();                              // 連接WiFi
  client.setServer(mqtt_server, mqtt_port);  //連接至MQTT
}

void humi_temp() {
  humi = dht.readHumidity();
  temp = dht.readTemperature();
  Serial.print("-------------------------------------\n");
  Serial.print("溫度：");
  Serial.print(temp);
  Serial.print("濕度：");
  Serial.println(humi);

  // lcd上顯示溫溼度
  lcd.clear();  // 清除先前畫面
  // 設定游標位置在第0列第0行
  lcd.setCursor(0, 0);
  // 顯示相對濕度字串
  lcd.print("RH: ");
  lcd.print(humi);
  lcd.print("%");
  // 設定游標位置在第1列第0行
  lcd.setCursor(0, 1);
  // 顯示溫度字串
  lcd.print("Temp: ");
  lcd.print(temp);
  lcd.print(F("\xdf"));
  lcd.print("C");
}

void countdown(int seconds) {
  for (int i = seconds; i >= 0; i--) {
    lcd.setCursor(15, 1);  // LCD第1列第14行
    lcd.print("  ");       // 清除前一個數字
    lcd.setCursor(14, 1);
    lcd.print(i);          // 顯示倒數數字
    delay(1000);
  }
}


void temp_color (unsigned char red, unsigned char green, unsigned char blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin,green);
  analogWrite(bluePin, blue);
}
void humi_color (unsigned char red, unsigned char green, unsigned char blue) {
  analogWrite(humi_redPin, red);
  analogWrite(humi_greenPin,green);
  analogWrite(humi_bluePin, blue);
}
void loop() {

  humi_temp();

  if (temp > 35) {
    Serial.print("溫度超過35，");
    temp_color(255,0,0);
  }
  if (temp >= 20 && temp <35) {
    Serial.print("溫度超過20且小於35，");
    temp_color(0,255,0);
  }
  if (temp < 20) {
    Serial.print("溫度低於20，");
    temp_color(0,0,255);
  }

  if (humi > 70) {
    Serial.print("濕度超過70\n");
    humi_color(255,0,0);
  }
  if (humi >= 60 && humi <70) {
    Serial.print("濕度超過60且小於70\n");
    humi_color(0,255,0);
  }
  if (humi < 60) {
    Serial.print("低於60\n");
    humi_color(0,0,255);
  }

  // 連接到MQTT伺服器
  if (!client.connected()) {
    reconnect();
  }

  // 啟動MQTT
  client.loop();
  StaticJsonDocument<200> jsonDoc;  //傳送MQTT JSON
  jsonDoc["temperature"] = temp;
  jsonDoc["humidity"] = humi;

  //String tempStr = String(temp, 2); // 保留2位小数
  char buffer[256];
  size_t n = serializeJson(jsonDoc, buffer);

  client.publish("test/temp", buffer, n);
  // 向伺服器傳送HTTP
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String(serverName) + "?temperature=" + String(temp) + "&humidity=" + String(humi);

    http.begin(url);            // 指定的伺服器
    int httpCode = http.GET();  // GET

    if (httpCode > 0) {  // 檢查是否有錯
      Serial.println("資料已傳送");
      Serial.print("-------------------------------------\n");
    } else {
      Serial.println("Error sending data");
    }
    http.end();  // 結束
  } else {
    Serial.println("WiFi not connected");
  }
  countdown(3);  // 每次偵測完後倒數10秒
}
