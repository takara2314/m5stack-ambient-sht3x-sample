#include "M5_ENV.h"
#include <Ambient.h>
#include <M5Stack.h>
#include <WiFi.h>

// SHT30 (温度、湿度センサ)
SHT3X sht30;

// Wi-Fi認証情報
const char *ssid = "xxxxx";
const char *password = "xxxxx";

WiFiClient client;
Ambient ambient;

// Ambient認証情報
unsigned int channelId = 99999;
const char *writeKey = "xxxxx";

// 温度
float temperature = 0.0;
// 湿度
float humidity = 0.0;

void setup()
{
    // M5Stack
    M5.begin();
    M5.Power.begin();
    // I2C
    Wire.begin();
    // 液晶
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextDatum(4);

    // Wi-FiとAmbientクライアントの初期化 ここから
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        // Wi-Fiアクセスポイントへの接続待ち
        delay(500);
        Serial.print(".");
    }
    Serial.println("Wi-Fi connected");
    // チャネルIDとライトキーを指定してAmbientの初期化
    ambient.begin(channelId, writeKey, &client);
}

// 温度と湿度を取得
void getEnvirons()
{
    if (sht30.get() == 0)
    {
        temperature = sht30.cTemp;
        humidity = sht30.humidity;
    }
    else
    {
        temperature = 0;
        humidity = 0;
    }
}

void loop()
{
    // 温度と湿度を取得
    getEnvirons();

    // 液晶に表示
    M5.lcd.setCursor(5, 50);
    M5.Lcd.printf("Temperature: %2.1f'C", temperature);
    M5.lcd.setCursor(5, 80);
    M5.Lcd.printf("Humidity: %2.0f%%", humidity);

    // Ambientに気温と湿度を送信
    ambient.set(1, temperature);
    ambient.set(2, humidity);
    ambient.send();

    // Ambientの3000件/日の制限を超えないようにスリープ
    delay(30 * 1000);
    M5.Lcd.clear(BLACK);
}
