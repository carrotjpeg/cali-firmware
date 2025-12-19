#include "ble_data.h"
#include "ble.h"
#include "ble_srv_common.h"
#include "nrf_sdh_ble.h"
#include <string.h>

static uint16_t m_service_handle;
static ble_gatts_char_handles_t m_char_handle;
static uint16_t m_conn_handle = BLE_CONN_HANDLE_INVALID;

/* ---------- BLE EVENT HANDLER ---------- */

static void ble_evt_handler(ble_evt_t const *p_ble_evt, void *p_context)
{
    switch (p_ble_evt->header.evt_id)
    {
    case BLE_GAP_EVT_CONNECTED:
        m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
        break;

    case BLE_GAP_EVT_DISCONNECTED:
        m_conn_handle = BLE_CONN_HANDLE_INVALID;
        break;

    default:
        break;
    }
}

NRF_SDH_BLE_OBSERVER(ble_data_obs, 3, ble_evt_handler, NULL);

/* ---------- BLE INIT ---------- */

void ble_data_init()
{
    ble_uuid_t service_uuid;
    ble_uuid128_t base_uuid = {
        {0x23, 0x15, 0xAD, 0xE0, 0x8F, 0x44, 0x4A, 0xB6,
         0xA1, 0x9E, 0xC3, 0x01, 0x00, 0x00, 0x40, 0x6E}};

    sd_ble_uuid_vs_add(&base_uuid, &service_uuid.type);
    service_uuid.uuid = 0x0001;

    sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                             &service_uuid,
                             &m_service_handle);

    ble_gatts_char_md_t char_md = {};
    char_md.char_props.notify = 1;

    ble_gatts_attr_md_t cccd_md = {};
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    cccd_md.vloc = BLE_GATTS_VLOC_STACK;
    char_md.p_cccd_md = &cccd_md;

    ble_uuid_t char_uuid;
    char_uuid.type = service_uuid.type;
    char_uuid.uuid = 0x0002;

    ble_gatts_attr_md_t attr_md = {};
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    attr_md.vloc = BLE_GATTS_VLOC_STACK;

    ble_gatts_attr_t attr_char_value = {};
    attr_char_value.p_uuid = &char_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len = sizeof(float);
    attr_char_value.max_len = sizeof(float);

    sd_ble_gatts_characteristic_add(m_service_handle,
                                    &char_md,
                                    &attr_char_value,
                                    &m_char_handle);
}

/* ---------- NOTIFY ---------- */

void ble_data_notify_float(float value)
{
    if (m_conn_handle == BLE_CONN_HANDLE_INVALID)
        return;

    uint16_t len = sizeof(float);
    uint8_t data[4];
    memcpy(data, &value, sizeof(float));

    ble_gatts_hvx_params_t params = {};
    params.handle = m_char_handle.value_handle;
    params.type = BLE_GATT_HVX_NOTIFICATION;
    params.p_len = &len;
    params.p_data = data;

    sd_ble_gatts_hvx(m_conn_handle, &params);
}
