#include "ble_stack.h"

extern "C"
{
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "nrf_ble_gatt.h"
#include "ble_advertising.h"
#include "ble_advdata.h"
#include "app_error.h"
}

#define DEVICE_NAME "CALI"
#define APP_BLE_CONN_CFG_TAG 1

static nrf_ble_gatt_t m_gatt;
static ble_advertising_t m_advertising;

static void gap_params_init()
{
    ble_gap_conn_params_t gap_conn_params = {};
    ble_gap_conn_sec_mode_t sec_mode;
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

    APP_ERROR_CHECK(sd_ble_gap_device_name_set(
        &sec_mode,
        (const uint8_t *)DEVICE_NAME,
        strlen(DEVICE_NAME)));

    gap_conn_params.min_conn_interval = MSEC_TO_UNITS(30, UNIT_1_25_MS);
    gap_conn_params.max_conn_interval = MSEC_TO_UNITS(60, UNIT_1_25_MS);
    gap_conn_params.slave_latency = 0;
    gap_conn_params.conn_sup_timeout = MSEC_TO_UNITS(4000, UNIT_10_MS);

    APP_ERROR_CHECK(sd_ble_gap_ppcp_set(&gap_conn_params));
}

static void gatt_init()
{
    APP_ERROR_CHECK(nrf_ble_gatt_init(&m_gatt, nullptr));
}

static void advertising_init()
{
    ble_advertising_init_t init = {};

    init.advdata.name_type = BLE_ADVDATA_FULL_NAME;
    init.advdata.flags = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;

    init.config.ble_adv_fast_enabled = true;
    init.config.ble_adv_fast_interval = MSEC_TO_UNITS(100, UNIT_0_625_MS);
    init.config.ble_adv_fast_timeout = 0;

    APP_ERROR_CHECK(ble_advertising_init(&m_advertising, &init));
    ble_advertising_conn_cfg_tag_set(&m_advertising, APP_BLE_CONN_CFG_TAG);
}

static void advertising_start()
{
    APP_ERROR_CHECK(
        ble_advertising_start(&m_advertising, BLE_ADV_MODE_FAST));
}

void ble_stack_init_and_start()
{
    APP_ERROR_CHECK(nrf_sdh_enable_request());

    uint32_t ram_start = 0;
    APP_ERROR_CHECK(
        nrf_sdh_ble_default_cfg_set(APP_BLE_CONN_CFG_TAG, &ram_start));
    APP_ERROR_CHECK(nrf_sdh_ble_enable(&ram_start));

    gap_params_init();
    gatt_init();
    advertising_init();
    advertising_start();
}
