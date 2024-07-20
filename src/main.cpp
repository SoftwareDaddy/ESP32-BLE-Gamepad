#include "BleGamepad.h"
#include <esp_log.h>

static const char* LOG_TAG = "main";

extern "C" {void app_main(void);}

void taskServer(void *pvParameter)
{
    BleGamepad *pGamepad = (BleGamepad *)pvParameter; // static_cast<BleGamepad *>(pvParameter);
    u_int8_t originalBatteryLevel = pGamepad->batteryLevel;
    ESP_LOGI(LOG_TAG, "***Original Battery Level: %d", originalBatteryLevel);
    bool connected = false;

    while(true){
        if (pGamepad->isConnected()){
            if (!connected){
                printf("Connected!\n");
                connected = true;
            }
            
            ESP_LOGI(LOG_TAG, "Press buttons 5, 16 and start. Move all enabled axes to max. Set DPAD (hat 1) to down right.");
            pGamepad->press(BUTTON_5);
            pGamepad->press(BUTTON_16);
            pGamepad->pressStart();
            pGamepad->setAxes(32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767);
            pGamepad->setHat1(HAT_DOWN_RIGHT);
            // All axes, sliders, hats etc can also be set independently. See the IndividualAxes.ino example
            
            vTaskDelay(250);

            ESP_LOGI(LOG_TAG, "Release button 5 and start. Move all axes to min. Set DPAD (hat 1) to centred.");
            pGamepad->release(BUTTON_5);
            pGamepad->releaseStart();
            pGamepad->setHat1(HAT_CENTERED);
            pGamepad->setAxes(0, 0, 0, 0, 0, 0, 0, 0);

            vTaskDelay(250);

        } else {
            if (connected){
                printf("Disconnected!\n");
                connected = false;
            }
        }
        vTaskDelay(1);
    }
}

void app_main(void){
    BleGamepad *pGamepad = new BleGamepad("Riggio Controller", "Riggio Games, Inc", 42U);
    pGamepad->begin();

    xTaskCreate(&taskServer, "server", 20000, pGamepad, 1, NULL);
}