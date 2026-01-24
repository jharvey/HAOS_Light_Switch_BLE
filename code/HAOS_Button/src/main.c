#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>

// Register Logging
LOG_MODULE_REGISTER(haos_app, LOG_LEVEL_INF);

// Hardware setup from app.overlay
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(DT_ALIAS(sw0), gpios);

// BTHome V2 Data Packet (Button Press 0x01)
static const struct bt_data ad[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR),
    BT_DATA_BYTES(BT_DATA_UUID16_ALL, 0xD2, 0xFC), // BTHome UUID
    BT_DATA_BYTES(BT_DATA_SVC_DATA16, 
        0xD2, 0xFC, // BTHome Service UUID
        0x40,       // BTHome Device Info (Unencrypted, V2)
        0x3A, 0x01  // Button (0x3A), Event: Press (0x01)
    ),
};

int main(void) {
    int ret;

    // 1. Initialize Hardware
    if (!gpio_is_ready_dt(&led) || !gpio_is_ready_dt(&button)) {
        return 0;
    }
    gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
    gpio_pin_configure_dt(&button, GPIO_INPUT);

    // 2. Initialize Bluetooth
    ret = bt_enable(NULL);
    if (ret) {
        LOG_ERR("Bluetooth init failed (err %d)", ret);
        return 0;
    }

    LOG_INF("System Started - Hello World via SWO!");

    while (1) {
        // Toggle LED (Blinky logic)
        gpio_pin_toggle_dt(&led);

        // Check Button
        if (gpio_pin_get_dt(&button) == 1) {
            LOG_INF("Button Pressed! Sending BLE broadcast...");
            
            // Send BLE BTHome broadcast (Visible to HAOS)
            bt_le_adv_start(BT_LE_ADV_NCONN, ad, ARRAY_SIZE(ad), NULL, 0);
            k_msleep(200); // Broadcast for 200ms
            bt_le_adv_stop();
        }

        // Loop delay (1 second)
        k_msleep(1000);
    }
}
