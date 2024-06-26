#include <WiFi.h>
#include <WiFiUdp.h>

#include "NTPAsyncClient.h"

#define SSID "<your-ssid>"
#define PASSWORD "<your-password>"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "0.asia.pool.ntp.org", 0, 1500);

void setup(){
  Serial.begin(115200);

  WiFi.begin(SSID, PASSWORD);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  timeClient.begin();
}

void loop() {
  long int loopMillis = millis();
  static int prevRes = 0;
  // with update() method loop will be blocked until time is updated
  // timeClient.update();
  int res = timeClient.asyncUpdate();

  if (res != prevRes) {
    Serial.printf("Result: %d\n", res);
    prevRes = res;
  }

  static long int lm = 0;

  if (millis() - lm > 1000) {
    lm = millis();
    Serial.printf("%s.%d\n", timeClient.getFormattedTime().c_str(), int(timeClient.getEpochTimeMillis()%1000));
  }

  long int dd = millis() - loopMillis;
  if (dd > 10) {
    Serial.printf("Loop took %d ms...\n", dd);
  }
}