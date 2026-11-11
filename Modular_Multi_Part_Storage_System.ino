#include <WiFi.h>
#include <time.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//wifi credentials
const char* ssid      = "Highschoolish";
const char* password  = "Raharuh1";

//NTP server settings for time sync
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -28800; //pacific time
const int   daylightOffset_sec =3600; //daylight savings offset

//.91 inch OLED display specifics
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(115200);

  //intialize 12C
  Wire.begin();

  //intialize OLED display
  if (!display.begin (SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed :("));
    for(;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.print(F("Connecting WIFI..."));
  display.display();

  //connect to wifi
  WiFi.begin(ssid, password);
  while (WiFi.status()  != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //init and retrieve time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  display.clearDisplay();
  display.setCursor(0,0);
  display.print(F("Time Synced!"));
  display.display();
  delay(1000);
}

void loop() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }

  display.clearDisplay();

  //draw an undertale-style headerline
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print(F("HP 20/20 LV 1"));

  //draw a dividing line
  display.drawFastHLine(0, 10, 128, SSD1306_WHITE);

  //format and display the time in large text
  display.setTextSize(2);
  display.setCursor(0,14);

  char timeBuffer[10];
  //note to self: change "%I:%M %p" to "%H:%M:%S" for 24hr time with seconds
  strftime(timeBuffer, sizeof(timeBuffer), "%I:%M %p", &timeinfo);

  display.print(timeBuffer);
  display.display();

  //update every second
  delay(1000);
}