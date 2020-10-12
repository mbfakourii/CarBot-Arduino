#include <ESP8266WiFi.h>		//کتابخانه مربوط به وای فا
#include "Regexp.h"				//کتابخانه مربوط به عبارت های باقاعده

const char* ssid = "CarBot";		//نام کاربری وای فا
const char* password = "75757575";	//رمز وای فا

WiFiServer server(1337);			//باز کردن پورت 1337 برا تبادل اطلاعات در بستر وای فا برای سوکت
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
//motor_1
int ENA = D8;		//پایه فعال ساز متور 1 و قادر است سرعت هم تغییر بدهد
int IN1 = D6;		//پایه 1 متور 1
int IN2 = D7;		//پایه 2 متور 1

//motor_2
int IN3 = D2;		//پایه 1 متور 2
int IN4 = D3;		//پایه 2 متور 2
int ENB = D4;		//پایه فعال ساز متور 2 و قادر است سرعت هم تغییر بدهد

void setup(void)	//تابع که اولین بار اچرا میشود
{
    Serial.begin(115200);		//باز کردن سریال برای نمایش اطلاعات برای برنامه نویس
    WiFi.softAP(ssid, password);	//ایجاد یک نقطه اتصال به کمک رمز و نام کاربری
    server.begin();				//فعال کردن سرور برای سوکت

    //pinModes
    pinMode(ENA, OUTPUT);		//اماده کردن پایه های برای استفاده
    pinMode(ENB, OUTPUT);		//اماده کردن پایه های برای استفاده

    pinMode(IN1, OUTPUT);		//اماده کردن پایه های برای استفاده
    pinMode(IN2, OUTPUT);		//اماده کردن پایه های برای استفاده
    pinMode(IN3, OUTPUT);		//اماده کردن پایه های برای استفاده
    pinMode(IN4, OUTPUT);		//اماده کردن پایه های برای استفاده

    Start_Pause();		//تابع فعال ساز پایه های فعال و غیر فعال کردن پایه های موتور
}

void loop(void) //تابعی که پشت سر هم اجرا میشود
{
    WiFiClient client = server.available(); //برسی وجود ارتباط بین اندروید و نقطه اتصال

    if (client) {		//برسی وجود ارتباط بین اندروید و نقطه اتصال
        Serial.println("Client connected.");		//برسی وجود ارتباط بین اندروید و نقطه اتصال

        while (client.connected()) {	//برسی وجود ارتباط بین اندروید و نقطه اتصال
            if (client.available()) {	//برسی در دسترس بودن ارتباط
              Check_Send_Data(client.readStringUntil('\n'));	//انتقال اطلاعات دریافتی به تابع مربوط برای پردازش
            }
        }

        Serial.println("Client disconnected."); // اجرای این قسمت بعد تمام شدن ارتباط و نمایش پیام
        Start_Pause();	//تابع فعال ساز پایه های فعال و غیر فعال کردن پایه های موتور
        client.stop();	//پایان دادن به سوکت
    }
}

void Start_Pause()		//تابع فعال ساز پایه های فعال و غیر فعال کردن پایه های موتور
{
    digitalWrite(ENA, HIGH);	//روشن کردن پایه فعال ساز متور 1
    digitalWrite(IN1, LOW);		//غیر فعال مردن پایه 1 موتور 1
    digitalWrite(IN2, LOW);		//غیر فعال مردن پایه 2 موتور 1

    digitalWrite(IN3, LOW);		//غیر فعال مردن پایه 2 موتور 1
    digitalWrite(IN4, LOW);		//غیر فعال مردن پایه 2 موتور 2
    digitalWrite(ENB, HIGH);	//روشن کردن پایه فعال ساز موتور 2
}

void Check_Send_Data(String value) // تابع پردازش اطلاعات دریافتی
{
    char text[200];		//تعریف متغیر برای عمل عبارت های باقاعده
    value.toCharArray(text, 200);	//تبدیل رشته به ارایه ای از رشته ها

    

    MatchState ms;		//تعریف یک نو از عبارت های باقاعده
    ms.Target(text);	//ست کردن متن مورد جستجو
    char result = ms.Match("ENA=(.*)-IN1=(.*)-IN2=(.*)-IN3=(.*)-IN4=(.*)-ENB=(.*)", 0);	//انتقال اطلاعات پیدا شده به متغیر جدید
    switch (result) {	//برسی اطلاعات جدید
      case REGEXP_MATCHED:	//اگر عمل درست انجام شده باشد
          analogWrite(ENA, String(ms.GetCapture(text, 0)).toInt()); 	//جدا کردن اطلاعات ارسال شده پایه فعال ساز موتور 1 و ست کردنش
          digitalWrite(IN1, String(ms.GetCapture(text, 1)).toInt());	//جدا کردن اطلاعات ارسال شده پایه 1 موتور 1 و ست کردنش
          digitalWrite(IN2, String(ms.GetCapture(text, 2)).toInt());	//جدا کردن اطلاعات ارسال شده پایه 2 موتور 1 و ست کردنش
  
          digitalWrite(IN3, String(ms.GetCapture(text, 3)).toInt());	//جدا کردن اطلاعات ارسال شده پایه 1 موتور 2 و ست کردنش
          digitalWrite(IN4, String(ms.GetCapture(text, 4)).toInt());	//جدا کردن اطلاعات ارسال شده پایه 2 موتور 2 و ست کردنش
          analogWrite(ENB, String(ms.GetCapture(text, 5)).toInt());		//جدا کردن اطلاعات ارسال شده پایه فعال ساز موتور 2 و ست کردنش
  
          return;		//خروج از تابع
      case REGEXP_NOMATCH:	//اگر عمل درست انجام نشده باشد
          Serial.println("Error In Regex");	// نمایش پیام عمل درست انجام نشد
          return;		//خروج از تابع
    }
}

