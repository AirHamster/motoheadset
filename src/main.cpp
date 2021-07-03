// /*
//  * Copyright (C) Emlid Limited - All Rights Reserved
//  * All information contained in this file is the property of Emlid Limited.
//  * The intellectual and technical concepts contained herein are proprietary to
//  * Emlid Limited and are protected by copyright law. Distribution of this
//  * information or reproduction of this material is strictly forbidden without
//  * written permission obtained from Emlid Limited.
//  *
//  * Written by Ilia Zarubin <ilia.zarubin@emlid.com>, 2020
//  */

// #include <stdio.h>
// #include "sdkconfig.h"
// #include "esp_system.h"
// #include "esp_spi_flash.h"
// #include "esp_log.h"
// #include "console.h"
// #include "i2s_driver.h"

// // extern "C" {
// // void app_main();
// // }

// // constexpr uint32_t main_task_delay_ms = 10;

// // void app_main(void)
// // {
// //     cli_start();
// //     i2s_test_start();
// //     /* Sheduler started automatically on boot,
// //         so we don't need to call vTaskStartScheduler()*/
// // }

// /* Play music from Bluetooth device

//    This example code is in the Public Domain (or CC0 licensed, at your option.)

//    Unless required by applicable law or agreed to in writing, this
//    software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
//    CONDITIONS OF ANY KIND, either express or implied.
// */
// #include <string.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "esp_log.h"
// #include "nvs_flash.h"
// #include "audio_element.h"
// #include "audio_pipeline.h"
// #include "audio_event_iface.h"
// #include "audio_common.h"
// #include "i2s_stream.h"
// #include "esp_peripherals.h"
// #include "periph_touch.h"
// #include "periph_adc_button.h"
// #include "periph_button.h"
// #include "board.h"
// #include "filter_resample.h"
// #include "audio_mem.h"
// #include "bluetooth_service.h"
// #define I2S_BCK_IO (GPIO_NUM_13)
// #define I2S_WS_IO (GPIO_NUM_21)
// #define I2S_DO_IO (GPIO_NUM_15)
// #define I2S_DI_IO (-1)
// static const char *TAG = "BLUETOOTH_EXAMPLE";

// extern "C" {
// void app_main();
// }

// void app_main(void)
// {
//     cli_start();
//    // i2s_test_start();

//     audio_pipeline_handle_t pipeline;
//     audio_element_handle_t bt_stream_reader, i2s_stream_writer;

//     esp_err_t err = nvs_flash_init();
//     if (err == ESP_ERR_NVS_NO_FREE_PAGES) {
//         // NVS partition was truncated and needs to be erased
//         // Retry nvs_flash_init
//         ESP_ERROR_CHECK(nvs_flash_erase());
//         err = nvs_flash_init();
//     }

//     esp_log_level_set("*", ESP_LOG_INFO);
//     esp_log_level_set(TAG, ESP_LOG_DEBUG);

//     ESP_LOGI(TAG, "[ 1 ] Create Bluetooth service");
//     bluetooth_service_cfg_t bt_cfg = {
//         .device_name = "ESP-ADF-SPEAKER",
//         .mode = BLUETOOTH_A2DP_SINK,
//     };
//     bluetooth_service_start(&bt_cfg);

//     ESP_LOGI(TAG, "[ 2 ] Start codec chip");
//     //audio_board_handle_t board_handle = audio_board_init();
//     //audio_hal_ctrl_codec(board_handle->audio_hal, AUDIO_HAL_CODEC_MODE_DECODE, AUDIO_HAL_CTRL_START);

//     ESP_LOGI(TAG, "[ 3 ] Create audio pipeline for playback");
//     audio_pipeline_cfg_t pipeline_cfg = DEFAULT_AUDIO_PIPELINE_CONFIG();
//     pipeline = audio_pipeline_init(&pipeline_cfg);

//     ESP_LOGI(TAG, "[3.1] Create i2s stream to write data to codec chip");
//     i2s_stream_cfg_t i2s_cfg
// };// = I2S_STREAM_CFG_DEFAULT();
//     //i2s_cfg.i2s_port
//     // i2s_config_t i2sConfig = {
//     //     .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
//     //     .sample_rate = m_sample_generator->sampleRate(),
//     //     .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
//     //     .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
//     //     .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S),
//     //     .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
//     //     .dma_buf_count = 4,
//     //     .dma_buf_len = 64};

//     i2s_cfg.type = AUDIO_STREAM_WRITER;
//     i2s_stream_writer = i2s_stream_init(&i2s_cfg);

//     i2s_pin_config_t i2sPins = {
//     .bck_io_num = I2S_BCK_IO,
//     .ws_io_num = I2S_WS_IO,
//     .data_out_num = I2S_DO_IO,
//     .data_in_num = -1};
//     i2s_set_pin(i2s_cfg.i2s_port, &i2sPins);

//     ESP_LOGI(TAG, "[3.2] Get Bluetooth stream");
//     bt_stream_reader = bluetooth_service_create_stream();

//     ESP_LOGI(TAG, "[3.2] Register all elements to audio pipeline");
//     audio_pipeline_register(pipeline, bt_stream_reader, "bt");
//     audio_pipeline_register(pipeline, i2s_stream_writer, "i2s");

//     ESP_LOGI(TAG, "[3.3] Link it together [Bluetooth]-->bt_stream_reader-->i2s_stream_writer-->[codec_chip]");

// #if (CONFIG_ESP_LYRATD_MSC_V2_1_BOARD || CONFIG_ESP_LYRATD_MSC_V2_2_BOARD)
//     rsp_filter_cfg_t rsp_cfg = DEFAULT_RESAMPLE_FILTER_CONFIG();
//     rsp_cfg.src_rate = 44100;
//     rsp_cfg.src_ch = 2;
//     rsp_cfg.dest_rate = 48000;
//     rsp_cfg.dest_ch = 2;
//     audio_element_handle_t filter = rsp_filter_init(&rsp_cfg);
//     audio_pipeline_register(pipeline, filter, "filter");
//     i2s_stream_set_clk(i2s_stream_writer, 48000, 16, 2);
//     const char *link_tag[3] = {"bt", "filter", "i2s"};
//     audio_pipeline_link(pipeline, &link_tag[0], 3);
// #else
//     const char *link_tag[2] = {"bt", "i2s"};
//     audio_pipeline_link(pipeline, &link_tag[0], 2);
// #endif
//     ESP_LOGI(TAG, "[ 4 ] Initialize peripherals");
//     esp_periph_config_t periph_cfg = DEFAULT_ESP_PERIPH_SET_CONFIG();
//     esp_periph_set_handle_t set = esp_periph_set_init(&periph_cfg);

//     ESP_LOGI(TAG, "[4.1] Initialize Touch peripheral");
//     //audio_board_key_init(set);

//     ESP_LOGI(TAG, "[4.2] Create Bluetooth peripheral");
//     esp_periph_handle_t bt_periph = bluetooth_service_create_periph();

//     ESP_LOGI(TAG, "[4.2] Start all peripherals");
//     esp_periph_start(set, bt_periph);

//     ESP_LOGI(TAG, "[ 5 ] Set up  event listener");
//     audio_event_iface_cfg_t evt_cfg = AUDIO_EVENT_IFACE_DEFAULT_CFG();
//     audio_event_iface_handle_t evt = audio_event_iface_init(&evt_cfg);

//     ESP_LOGI(TAG, "[5.1] Listening event from all elements of pipeline");
//     audio_pipeline_set_listener(pipeline, evt);

//     ESP_LOGI(TAG, "[5.2] Listening event from peripherals");
//     audio_event_iface_set_listener(esp_periph_set_get_event_iface(set), evt);

//     ESP_LOGI(TAG, "[ 6 ] Start audio_pipeline");
//     audio_pipeline_run(pipeline);

//     ESP_LOGI(TAG, "[ 7 ] Listen for all pipeline events");
//     while (1) {
//         audio_event_iface_msg_t msg;
//         esp_err_t ret = audio_event_iface_listen(evt, &msg, portMAX_DELAY);
//         if (ret != ESP_OK) {
//             ESP_LOGE(TAG, "[ * ] Event interface error : %d", ret);
//             continue;
//         }

//         if (msg.source_type == AUDIO_ELEMENT_TYPE_ELEMENT && msg.source == (void *) bt_stream_reader
//             && msg.cmd == AEL_MSG_CMD_REPORT_MUSIC_INFO) {
//             audio_element_info_t music_info = {0};
//             audio_element_getinfo(bt_stream_reader, &music_info);

//             ESP_LOGI(TAG, "[ * ] Receive music info from Bluetooth, sample_rates=%d, bits=%d, ch=%d",
//                      music_info.sample_rates, music_info.bits, music_info.channels);

//             audio_element_setinfo(i2s_stream_writer, &music_info);
// #if (CONFIG_ESP_LYRATD_MSC_V2_1_BOARD || CONFIG_ESP_LYRATD_MSC_V2_2_BOARD)
// #else
//             i2s_stream_set_clk(i2s_stream_writer, music_info.sample_rates, music_info.bits, music_info.channels);
// #endif
//             continue;
//         }

//         if ((msg.source_type == PERIPH_ID_TOUCH || msg.source_type == PERIPH_ID_BUTTON || msg.source_type == PERIPH_ID_ADC_BTN)
//             && (msg.cmd == PERIPH_TOUCH_TAP || msg.cmd == PERIPH_BUTTON_PRESSED || msg.cmd == PERIPH_ADC_BUTTON_PRESSED)) {

//             if ((int) msg.data == get_input_play_id()) {
//                 ESP_LOGI(TAG, "[ * ] [Play] touch tap event");
//                 periph_bluetooth_play(bt_periph);
//             } else if ((int) msg.data == get_input_set_id()) {
//                 ESP_LOGI(TAG, "[ * ] [Set] touch tap event");
//                 periph_bluetooth_pause(bt_periph);
//             } else if ((int) msg.data == get_input_volup_id()) {
//                 ESP_LOGI(TAG, "[ * ] [Vol+] touch tap event");
//                 periph_bluetooth_next(bt_periph);
//             } else if ((int) msg.data == get_input_voldown_id()) {
//                 ESP_LOGI(TAG, "[ * ] [Vol-] touch tap event");
//                 periph_bluetooth_prev(bt_periph);
//             }
//         }

//         /* Stop when the Bluetooth is disconnected or suspended */
//         if (msg.source_type == PERIPH_ID_BLUETOOTH
//             && msg.source == (void *)bt_periph) {
//             if (msg.cmd == PERIPH_BLUETOOTH_DISCONNECTED) {
//                 ESP_LOGW(TAG, "[ * ] Bluetooth disconnected");
//                 break;
//             }
//         }
//         /* Stop when the last pipeline element (i2s_stream_writer in this case) receives stop event */
//         if (msg.source_type == AUDIO_ELEMENT_TYPE_ELEMENT && msg.source == (void *) i2s_stream_writer
//             && msg.cmd == AEL_MSG_CMD_REPORT_STATUS
//             && (((int)msg.data == AEL_STATUS_STATE_STOPPED) || ((int)msg.data == AEL_STATUS_STATE_FINISHED))) {
//             ESP_LOGW(TAG, "[ * ] Stop event received");
//             break;
//         }
//     }

//     ESP_LOGI(TAG, "[ 8 ] Stop audio_pipeline");
//     audio_pipeline_stop(pipeline);
//     audio_pipeline_wait_for_stop(pipeline);
//     audio_pipeline_terminate(pipeline);

//     audio_pipeline_unregister(pipeline, bt_stream_reader);
//     audio_pipeline_unregister(pipeline, i2s_stream_writer);

//     /* Terminate the pipeline before removing the listener */
//     audio_pipeline_remove_listener(pipeline);

//     /* Stop all peripherals before removing the listener */
//     esp_periph_set_stop_all(set);
//     audio_event_iface_remove_listener(esp_periph_set_get_event_iface(set), evt);

//     /* Make sure audio_pipeline_remove_listener & audio_event_iface_remove_listener are called before destroying event_iface */
//     audio_event_iface_destroy(evt);

//     /* Release all resources */

// #if (CONFIG_ESP_LYRATD_MSC_V2_1_BOARD || CONFIG_ESP_LYRATD_MSC_V2_2_BOARD)
//     audio_pipeline_unregister(pipeline, filter);
//     audio_element_deinit(filter);
// #endif
//     audio_pipeline_deinit(pipeline);
//     audio_element_deinit(bt_stream_reader);
//     audio_element_deinit(i2s_stream_writer);
//     esp_periph_set_destroy(set);
//     bluetooth_service_destroy();
// }
#include <string.h>
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_peripherals.h"
#include "esp_bt_device.h"
#include "esp_bt_main.h"
#include "esp_gap_bt_api.h"
#include "audio_pipeline.h"
#include "audio_event_iface.h"
#include "i2s_stream.h"
#include "board.h"
#include "hfp_stream.h"

#include "esp_hf_client_api.h"
#define I2S_MIC_SERIAL_CLOCK GPIO_NUM_26
#define I2S_MIC_LEFT_RIGHT_CLOCK GPIO_NUM_25
#define I2S_MIC_SERIAL_DATA GPIO_NUM_27

#define I2S_BCK_IO (GPIO_NUM_13)
#define I2S_WS_IO (GPIO_NUM_21)
#define I2S_DO_IO (GPIO_NUM_15)
#define I2S_DI_IO (-1)

#if __has_include("esp_idf_version.h")
#include "esp_idf_version.h"
#else
#define ESP_IDF_VERSION_VAL(major, minor, patch) 1
#endif

static const char *TAG = "HFP_EXAMPLE";

static audio_element_handle_t hfp_in_stream, hfp_out_stream, i2s_stream_writer, i2s_stream_reader;
static audio_pipeline_handle_t pipeline_in, pipeline_out;
extern "C" {
void app_main(void);
}
static void bt_app_hf_client_audio_open(hfp_data_enc_type_t type)
{
    ESP_LOGI(TAG, "bt_app_hf_client_audio_open type = %d", type);
    int sample_rate = 8000;
    if (type == HF_DATA_CVSD) {
        sample_rate = 8000;
    } else if (type == HF_DATA_MSBC) {
        sample_rate = 16000;
    } else {
        ESP_LOGE(TAG, "error hfp enc type = %d", type);
        }
    i2s_stream_set_clk(i2s_stream_reader, sample_rate, 16, 1);
    i2s_stream_set_clk(i2s_stream_writer, sample_rate, 16, 1);
    audio_pipeline_run(pipeline_out);
    audio_pipeline_resume(pipeline_out);
}

static void bt_app_hf_client_audio_close(void)
{
    ESP_LOGI(TAG, "bt_app_hf_client_audio_close");
    audio_pipeline_pause(pipeline_out);
}

void app_main(void)
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }

    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set(TAG, ESP_LOG_DEBUG);

    ESP_LOGI(TAG, "[ 1 ] init Bluetooth");
    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_BLE));

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    if (esp_bt_controller_init(&bt_cfg) != ESP_OK) {
        AUDIO_ERROR(TAG, "initialize controller failed");
    }

    if (esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT) != ESP_OK) {
        AUDIO_ERROR(TAG, "enable controller failed");
    }

    if (esp_bluedroid_init() != ESP_OK) {
        AUDIO_ERROR(TAG, "initialize bluedroid failed");
    }

    if (esp_bluedroid_enable() != ESP_OK) {
        AUDIO_ERROR(TAG, "enable bluedroid failed");
    }

    esp_bt_dev_set_device_name("ESP-ADF-HFP");
    hfp_open_and_close_evt_cb_register(bt_app_hf_client_audio_open, bt_app_hf_client_audio_close);
    hfp_service_init();
#if (ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(4, 0, 0))
    esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE);
#else
    esp_bt_gap_set_scan_mode(ESP_BT_SCAN_MODE_CONNECTABLE_DISCOVERABLE);
#endif

    ESP_LOGI(TAG, "[ 2 ] Start codec chip");
    //audio_board_handle_t board_handle = audio_board_init();
    //audio_hal_ctrl_codec(board_handle->audio_hal, AUDIO_HAL_CODEC_MODE_BOTH, AUDIO_HAL_CTRL_START);

    ESP_LOGI(TAG, "[ 3 ] Create audio pipeline for playback");
    audio_pipeline_cfg_t pipeline_cfg = DEFAULT_AUDIO_PIPELINE_CONFIG();
    pipeline_in = audio_pipeline_init(&pipeline_cfg);
    pipeline_out = audio_pipeline_init(&pipeline_cfg);

    ESP_LOGI(TAG, "[3.1] Create i2s stream to write data to codec chip and read data from codec chip");
    i2s_stream_cfg_t i2s_cfg1 = I2S_STREAM_CFG_DEFAULT();
    i2s_cfg1.type = AUDIO_STREAM_READER;

    //i2s_cfg1.i2s_config.use_apll = false;
    i2s_cfg1.i2s_port = I2S_NUM_1;

    i2s_stream_reader = i2s_stream_init(&i2s_cfg1);
    i2s_stream_cfg_t i2s_cfg2 = I2S_STREAM_CFG_DEFAULT();
    i2s_cfg2.type = AUDIO_STREAM_WRITER;
    i2s_cfg1.i2s_port = I2S_NUM_0;
    i2s_stream_writer = i2s_stream_init(&i2s_cfg2);

    i2s_pin_config_t i2sPins1 = {
     .bck_io_num = I2S_MIC_SERIAL_CLOCK,
    .ws_io_num = I2S_MIC_LEFT_RIGHT_CLOCK,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_MIC_SERIAL_DATA};
    i2s_set_pin(i2s_cfg1.i2s_port, &i2sPins1);

    i2s_pin_config_t i2sPins2 = {
    .bck_io_num = I2S_BCK_IO,
    .ws_io_num = I2S_WS_IO,
    .data_out_num = I2S_DO_IO,
    .data_in_num = -1};
    i2s_set_pin(i2s_cfg2.i2s_port, &i2sPins2);

    ESP_LOGI(TAG, "[3.2] Get hfp stream");
    hfp_stream_config_t hfp_config;
    hfp_config.type = OUTGOING_STREAM;
    hfp_out_stream = hfp_stream_init(&hfp_config);
    hfp_config.type = INCOMING_STREAM;
    hfp_in_stream = hfp_stream_init(&hfp_config);

    ESP_LOGI(TAG, "[3.2] Register all elements to audio pipeline");
    audio_pipeline_register(pipeline_in, hfp_in_stream, "incoming");
    audio_pipeline_register(pipeline_in, i2s_stream_writer, "i2s_writer");

    audio_pipeline_register(pipeline_out, i2s_stream_reader, "i2s_reader");
    audio_pipeline_register(pipeline_out, hfp_out_stream, "outgoing");

    ESP_LOGI(TAG, "[3.3] Link it together [Bluetooth]-->hfp_in_stream-->i2s_stream_writer-->[codec_chip]");
    const char *link_in[2] = {"incoming", "i2s_writer"};
    audio_pipeline_link(pipeline_in, &link_in[0], 2);
    const char *link_out[2] = {"i2s_reader", "outgoing"};
    audio_pipeline_link(pipeline_out, &link_out[0], 2);

    ESP_LOGI(TAG, "[ 4 ] Initialize peripherals");
    esp_periph_config_t periph_cfg = DEFAULT_ESP_PERIPH_SET_CONFIG();
    esp_periph_set_handle_t set = esp_periph_set_init(&periph_cfg);

    ESP_LOGI(TAG, "[ 5 ] Set up  event listener");
    audio_event_iface_cfg_t evt_cfg = AUDIO_EVENT_IFACE_DEFAULT_CFG();
    audio_event_iface_handle_t evt = audio_event_iface_init(&evt_cfg);

    ESP_LOGI(TAG, "[5.1] Listening event from all elements of pipeline");
    audio_pipeline_set_listener(pipeline_in, evt);
    audio_pipeline_set_listener(pipeline_out, evt);

    ESP_LOGI(TAG, "[5.2] Listening event from peripherals");
    audio_event_iface_set_listener(esp_periph_set_get_event_iface(set), evt);

    ESP_LOGI(TAG, "[ 6 ] Start audio_pipeline");
    audio_pipeline_run(pipeline_in);

    ESP_LOGI(TAG, "[ 7 ] Listen for all pipeline events");
    while (1) {
        audio_event_iface_msg_t msg;
        esp_err_t ret = audio_event_iface_listen(evt, &msg, portMAX_DELAY);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "[ * ] Event interface error : %d", ret);
            continue;
        }

        /* Stop when the last pipeline element (i2s_stream_writer in this case) receives stop event */
        if (msg.source_type == AUDIO_ELEMENT_TYPE_ELEMENT && msg.source == (void *) i2s_stream_writer
            && msg.cmd == AEL_MSG_CMD_REPORT_STATUS && (int) msg.data == AEL_STATUS_STATE_STOPPED) {
            ESP_LOGW(TAG, "[ * ] Stop event received");
            break;
        }
    }

    ESP_LOGI(TAG, "[ 8 ] Stop audio_pipeline");
    audio_pipeline_stop(pipeline_in);
    audio_pipeline_wait_for_stop(pipeline_in);
    audio_pipeline_terminate(pipeline_in);
    audio_pipeline_stop(pipeline_out);
    audio_pipeline_wait_for_stop(pipeline_out);
    audio_pipeline_terminate(pipeline_out);

    audio_pipeline_unregister(pipeline_in, hfp_in_stream);
    audio_pipeline_unregister(pipeline_in, i2s_stream_writer);

    audio_pipeline_unregister(pipeline_out, i2s_stream_reader);
    audio_pipeline_unregister(pipeline_out, hfp_out_stream);

    /* Terminate the pipeline before removing the listener */
    audio_pipeline_remove_listener(pipeline_in);
    audio_pipeline_remove_listener(pipeline_out);

    /* Stop all peripherals before removing the listener */
    esp_periph_set_stop_all(set);
    audio_event_iface_remove_listener(esp_periph_set_get_event_iface(set), evt);

    /* Make sure audio_pipeline_remove_listener & audio_event_iface_remove_listener are called before destroying event_iface */
    audio_event_iface_destroy(evt);

    /* Release all resources */
    audio_pipeline_deinit(pipeline_in);
    audio_pipeline_deinit(pipeline_out);
    audio_element_deinit(hfp_in_stream);
    audio_element_deinit(i2s_stream_writer);
    audio_element_deinit(i2s_stream_reader);
    audio_element_deinit(hfp_out_stream);
    esp_periph_set_destroy(set);
}