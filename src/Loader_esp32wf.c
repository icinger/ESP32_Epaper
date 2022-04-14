/**
  ******************************************************************************
  * @file    Loader.h
  * @author  Waveshare Team
  * @version V1.0.0
  * @date    23-January-2018
  * @brief   The main file.
  *          This file provides firmware functions:
  *           + Initialization of Serial Port, SPI pins and server
  *           + Main loop
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <WiFi.h>
#include <PubSubClient.h>
const char* mqtt_server = "192.168.1.1";
WiFiClient espClient;
PubSubClient mqclient(espClient);
char msg[50];
int value = 0;

#include "srvr.h" // Server functions
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  10*60        /* Time ESP32 will go to sleep (in seconds) */
RTC_DATA_ATTR int bootCount = 0;



/* Entry point ----------------------------------------------------------------*/
void setup()
{
    // Serial port initialization
    Serial.begin(115200);
    delay(10);
  delay(1000); //Take some time to open up the Serial Monitor

  //Increment boot number and print it every reboot
  ++bootCount;
    Serial.println("Boot number: " + String(bootCount));
    // Server initialization
    Srvr__setup();

    mqclient.setServer(mqtt_server, 1883);
    mqclient.connect("WetterDisplay",NULL,NULL,mqTopic, 0,true,"offline");
    while (!mqclient.connected()) {
        Serial.println("Connecting to MQTT...");

        if (mqclient.connect("WetterDisplay" )) {

          Serial.println("connected");

        } else {

          Serial.print("failed with state ");
          Serial.print(mqclient.state());
          delay(2000);

        }
    }
    Serial.println("connected");

    // Icinger: Send "online"-Message to MQTT

    mqclient.publish(mqTopic, "online");
    // SPI initialization
    EPD_initSPI();

    // Initialization is complete
    Serial.print("\r\nOk!\r\n");

    // Icinger: Init deepsleep-timer

    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
}

/* The main loop -------------------------------------------------------------*/
void loop()
{
    // The server state observation
    Srvr__loop();
}
