#include "ble_server.h"
#include "blink.h"

#include <string.h>
#include <stdlib.h>

#include "esp_log.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_gatt_common_api.h"

static const char *TAG = "BLE";

static uint16_t service_handle;

#define GATTS_APP_ID 0x55

#define SERVICE_UUID        0x00FF
#define CHARACTERISTIC_UUID 0xFF01

static void gatts_event_handler(
    esp_gatts_cb_event_t event,
    esp_gatt_if_t gatts_if,
    esp_ble_gatts_cb_param_t *param);

static esp_ble_adv_params_t adv_params =
{
    .adv_int_min       = 0x20,
    .adv_int_max       = 0x40,
    .adv_type          = ADV_TYPE_IND,
    .own_addr_type     = BLE_ADDR_TYPE_PUBLIC,
    .channel_map       = ADV_CHNL_ALL,
    .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY
};

static esp_ble_adv_data_t adv_data =
{
    .set_scan_rsp = false,
    .include_name = true,
    .include_txpower = true,
    .appearance = 0x00,
    .manufacturer_len = 0,
    .p_manufacturer_data = NULL,
    .service_data_len = 0,
    .p_service_data = NULL,
    .service_uuid_len = 0,
    .p_service_uuid = NULL,
    .flag = ESP_BLE_ADV_FLAG_GEN_DISC |
            ESP_BLE_ADV_FLAG_BREDR_NOT_SPT
};

static void gap_event_handler(esp_gap_ble_cb_event_t event,
                              esp_ble_gap_cb_param_t *param)
{
    
    switch (event)
    {
        case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT:

            ESP_LOGI(TAG, "Dados de advertising configurados");

            esp_ble_gap_start_advertising(&adv_params);

            break;

        case ESP_GAP_BLE_ADV_START_COMPLETE_EVT:

            if (param->adv_start_cmpl.status == ESP_BT_STATUS_SUCCESS)
            {
                ESP_LOGI(TAG, "Advertising iniciado");
            }
            else
            {
                ESP_LOGE(TAG, "Falha ao iniciar advertising");
            }

            break;
                
        default:
            break;
    }
}

void ble_server_init(void)
{
    esp_err_t ret;

    ret = esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Erro liberando memoria BT Classic: %s",
                 esp_err_to_name(ret));
    }

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();

    ret = esp_bt_controller_init(&bt_cfg);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Erro controller init: %s",
                 esp_err_to_name(ret));
        return;
    }

    ret = esp_bt_controller_enable(ESP_BT_MODE_BLE);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Erro controller enable: %s",
                 esp_err_to_name(ret));
        return;
    }

    ret = esp_bluedroid_init();
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Erro bluedroid init: %s",
                 esp_err_to_name(ret));
        return;
    }

    ret = esp_bluedroid_enable();
    if (ret != ESP_OK)
    {
                ESP_LOGE(TAG, "Erro bluedroid enable: %s",
                 esp_err_to_name(ret));
        return;
    }

    ESP_ERROR_CHECK(
            esp_ble_gatts_register_callback(gatts_event_handler));

    ESP_ERROR_CHECK(
        esp_ble_gatts_app_register(GATTS_APP_ID));

    ESP_ERROR_CHECK(
    esp_ble_gap_register_callback(gap_event_handler));

    ESP_ERROR_CHECK(
        esp_ble_gap_set_device_name("ESP32-BLINK"));

    ESP_ERROR_CHECK(
        esp_ble_gap_config_adv_data(&adv_data));

    ESP_LOGI(TAG, "BLE inicializado com sucesso");
}

static void gatts_event_handler(
    esp_gatts_cb_event_t event,
    esp_gatt_if_t gatts_if,
    esp_ble_gatts_cb_param_t *param)
{

    switch (event)
    {
        case ESP_GATTS_CONNECT_EVT:

            ESP_LOGI(TAG,
                    "CLIENTE CONECTOU conn_id=%d",
                    param->connect.conn_id);

            break;

        case ESP_GATTS_DISCONNECT_EVT:

            ESP_LOGI(TAG, "CLIENTE DESCONECTOU");

            esp_ble_gap_start_advertising(&adv_params);

            break;

        case ESP_GATTS_REG_EVT:

            ESP_LOGI(TAG,
                     "GATT registrado. app_id=%d",
                     param->reg.app_id);

            esp_gatt_srvc_id_t service_id =
            {
                .is_primary = true,
                .id.inst_id = 0,
                .id.uuid.len = ESP_UUID_LEN_16,
                .id.uuid.uuid.uuid16 = SERVICE_UUID
            };

            esp_ble_gatts_create_service(
                gatts_if,
                &service_id,
                4);

            break;

        case ESP_GATTS_CREATE_EVT:
        {
            ESP_LOGI(TAG, "Servico criado");

            service_handle = param->create.service_handle;

            esp_ble_gatts_start_service(service_handle);

            esp_bt_uuid_t char_uuid =
            {
                .len = ESP_UUID_LEN_16,
                .uuid.uuid16 = CHARACTERISTIC_UUID
            };

            esp_ble_gatts_add_char(
                service_handle,
                &char_uuid,
                ESP_GATT_PERM_WRITE,
                ESP_GATT_CHAR_PROP_BIT_WRITE_NR,
                NULL,
                NULL);

            break;
        }

        case ESP_GATTS_ADD_CHAR_EVT:

            ESP_LOGI(TAG,
                    "Caracteristica criada");

            break;

        case ESP_GATTS_WRITE_EVT:

            ESP_LOGI(TAG,
                    "WRITE conn_id=%d len=%d",
                    param->write.conn_id,
                    param->write.len);

            ESP_LOG_BUFFER_HEX(
                TAG,
                param->write.value,
                param->write.len);

                char buffer[32];

                size_t len =
                    (param->write.len < sizeof(buffer)-1)
                    ? param->write.len
                    : sizeof(buffer)-1;

                memcpy(buffer,
                    param->write.value,
                    len);

                buffer[len] = '\0';

                ESP_LOGI(TAG, "Recebido: %s", buffer);

                char *endptr;

                uint32_t periodo =
                    strtoul(buffer, &endptr, 10);

                if (*endptr == '\0' && periodo > 0)
                {
                    blink_set_period(periodo);

                    ESP_LOGI(TAG,
                            "Novo periodo: %lu ms",
                            (unsigned long)periodo);
                }
                break;
                
        default:
            break;
    }
}