#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include <zephyr/bluetooth/bluetooth.h>

LOG_MODULE_REGISTER(haos_app, LOG_LEVEL_INF);

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const struct gpio_dt_spec btn_up = GPIO_DT_SPEC_GET(DT_ALIAS(sw0), gpios);
static const struct gpio_dt_spec btn_dn = GPIO_DT_SPEC_GET(DT_ALIAS(sw1), gpios);

/* Sends BTHome v2 packet for button events (Object ID 0x3A) */
void send_bthome_event(uint8_t event_id) {
    static uint8_t packet_id = 0;
    
    struct bt_data ad[] = {
        BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR),
        BT_DATA_BYTES(BT_DATA_UUID16_ALL, 0xD2, 0xFC), // BTHome Service UUID
        BT_DATA_BYTES(BT_DATA_SVC_DATA16, 
            0xD2, 0xFC, 
            0x40,       // Device Info: No encryption, V2
            0x3A,       // Object ID: Button event
            event_id    // Value: 0x01 (Press) or 0x02 (Double/Down example)
        )
    };

    bt_le_adv_start(BT_LE_ADV_NCONN, ad, ARRAY_SIZE(ad), NULL, 0);
    k_msleep(150); // Broadcast for 150ms to ensure HAOS hears it
    bt_le_adv_stop();
    LOG_INF("BLE Event 0x%02X sent (Packet %d)", event_id, ++packet_id);
}

int main(void) {
    if (!gpio_is_ready_dt(&led) || !gpio_is_ready_dt(&btn_up) || !gpio_is_ready_dt(&btn_dn)) {
        LOG_ERR("Hardware not ready");
        return 0;
    }

    gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
    gpio_pin_configure_dt(&btn_up, GPIO_INPUT);
    gpio_pin_configure_dt(&btn_dn, GPIO_INPUT);

    if (bt_enable(NULL)) {
        LOG_ERR("Bluetooth failed");
        return 0;
    }

    LOG_INF("Dual Button Switch Ready - P0.11 (Up), P0.12 (Down)");

    while (1) {
        // Toggle LED to show system heartbeat
        gpio_pin_toggle_dt(&led);

        // Check Up Button (P0.11)
        if (gpio_pin_get_dt(&btn_up) == 1) {
            LOG_INF("UP Pressed");
            send_bthome_event(0x01); // Standard "Press"
            while(gpio_pin_get_dt(&btn_up) == 1) k_msleep(10); // Simple debounce
        }

        // Check Down Button (P0.12)
        if (gpio_pin_get_dt(&btn_dn) == 1) {
            LOG_INF("DOWN Pressed");
            send_bthome_event(0x02); // Sending 0x02 to distinguish from Up
            while(gpio_pin_get_dt(&btn_dn) == 1) k_msleep(10); // Simple debounce
        }

        k_msleep(250); // General polling interval
    }
}
