
#include <WiFi.h>
#include <bigiot.h>
#include <MPython.h>

#define WIFI_TIMEOUT 30000

const char *ssid = "GoodMaker";   //WIFI名称  此处改为您的WIFI名称
const char *passwd = "steam666";  //WIFI密码  此处改为您的WIFI密码

const char *id = "xxxx";              //设备编号ID
const char *apikey = "xxxxx";      //设备APIKEY
const char *usrkey = " ";           //平台用户密钥，如果您不使用加密登录，则可以将其留空

BIGIOT bigiot;

void setup()
{
    Serial.begin(115200);
    mPython.begin();
    delay(100);

    pinMode(P0, OUTPUT);
    digitalWrite(P0, LOW);
    pinMode(P1, OUTPUT);
    WiFi.begin(ssid, passwd);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.print("Connect ssid fail");
        while (1);
    }

    //初始化命令事件处理程序
    bigiot.eventAttach(eventCallback);

    //设备断开处理程序
    bigiot.disconnectAttack(disconnectCallback);

    //设备连接处理程序
    bigiot.connectAttack(connectCallback);

    // 登录bigiot.net
    if (!bigiot.login(id, apikey, usrkey)) {
        Serial.println("Login fail");
        while (1);
    }
   display.fillScreen(0);
   display.setCursorXY(16, 22);
   display.print("connected: OK");
   delay(1000);
}

void loop()
{
    static uint64_t last_upload_time = 0;
    static uint64_t last_wifi_check_time = 0;

    if (WiFi.status() == WL_CONNECTED) {
        //等待平台命令释放
        bigiot.handle();
    } else {
        uint64_t now = millis();
        // Wifi disconnection reconnection mechanism
        if (now - last_wifi_check_time > WIFI_TIMEOUT) {
            Serial.println("WiFi connection lost. Reconnecting...");
            WiFi.reconnect();
            last_wifi_check_time = now;
        }
    }    
}

void eventCallback(const int devid, const int comid, const char *comstr)
{
    //您可以在此处理平台发出的命令。
    Serial.printf("Received[%d] - [%d]:%s \n", devid, comid, comstr);
    switch (comid) {
    case 1:
        digitalWrite(P0, LOW);
        digitalWrite(P1, LOW);
        display.fillScreen(0);
        display.setCursorXY(30, 22);
        display.print("LED: OFF");
        delay(1000);
        break;
    case 0:
        digitalWrite(P0, HIGH);
        digitalWrite(P1, HIGH);
        display.fillScreen(0);
        display.setCursorXY(30, 22);
        display.print("LED: ON");
        delay(1000);
        break;
    case OFFON:
        break;
    case MINUS:
        break;
    case UP:
        break;
    case PLUS:
        break;
    case LEFT:
        break;
    case PAUSE:
        break;
    case RIGHT:
        break;
    case BACKWARD:
        break;
    case DOWN:
        break;
    case FPRWARD:
        break;
    default:
        break;
    }
}

void disconnectCallback(BIGIOT &obj)
{
    // 当设备与平台断开连接时，您可以在此处理外围设备
    Serial.print(obj.deviceName());
    Serial.println("  disconnect");
}

void connectCallback(BIGIOT &obj)
{
    // 当设备连接到平台时，您可以在此处预处理外围设备
    Serial.print(obj.deviceName());
    Serial.println("  connect");
}
