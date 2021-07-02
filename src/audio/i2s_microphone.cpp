#include <cstring>
#include <driver/i2s.h>
#include "i2s_microphone.h"

// you shouldn't need to change these settings
#define SAMPLE_BUFFER_SIZE 512
#define SAMPLE_RATE 16000
// most microphones will probably default to left channel but you may need to tie the L/R pin low
#define I2S_MIC_CHANNEL I2S_CHANNEL_FMT_ONLY_LEFT
// either wire your microphone to the same pins or change these to match your wiring
#define I2S_MIC_SERIAL_CLOCK GPIO_NUM_26
#define I2S_MIC_LEFT_RIGHT_CLOCK GPIO_NUM_25
#define I2S_MIC_SERIAL_DATA GPIO_NUM_27

// don't mess around with this
i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = 1024,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0};

// and don't mess around with this
i2s_pin_config_t i2s_mic_pins = {
    .bck_io_num = I2S_MIC_SERIAL_CLOCK,
    .ws_io_num = I2S_MIC_LEFT_RIGHT_CLOCK,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_MIC_SERIAL_DATA};

int16_t raw_samples[SAMPLE_BUFFER_SIZE];
void i2sMicrophoneTask(void *param)
{
    //setup();
    while (true)
    {
        // read from the I2S device
        size_t bytes_read = 0;
        i2s_read(I2S_NUM_1, raw_samples, sizeof(int16_t) * SAMPLE_BUFFER_SIZE, &bytes_read, portMAX_DELAY);
        int samples_read = bytes_read / sizeof(int16_t);
        // dump the samples out to the serial channel.
        for (int i = 0; i < samples_read; i++)
        {
            printf("%d\n", raw_samples[i]);
        }
    }
}

void micropone_read(int16_t *buff, size_t num)
{
    size_t bytes_read = 0;
    int32_t byte;
    uint8_t buffer32[num * 4] = {0};
    i2s_read(I2S_NUM_1, buff, sizeof(int16_t) * num, &bytes_read, portMAX_DELAY);
    //i2s_read(I2S_NUM_1, &byte, 4, &bytes_read, portMAX_DELAY);
    return;
    i2s_read(I2S_NUM_1, &buffer32, sizeof(buffer32), &bytes_read, 100);
    int samplesRead = bytes_read / 4;
    
    int16_t buffer16[num] = {0};
    for (int i = 0; i < samplesRead; i++)
    {
        uint8_t mid = buffer32[i * 4 + 2];
        uint8_t msb = buffer32[i * 4 + 3];
        uint16_t raw = (((uint32_t)msb) << 8) + ((uint32_t)mid);
        memcpy(&buffer16[i], &raw, sizeof(raw)); // Copy so sign bits aren't interfered with somehow.
        // int32_t k = (int32_t)buffer16[i];
        // k += 0x8000;
        // //k = 1000;
        buff[i] = buffer16[i];
        
        // if (buffer16[i] < 0){
        //     buffer16[i] = -1 * buffer16[i];
        // }
        // buff[i] = (uint16_t)buffer16[i];
    }
}

void microphone_init()
{
    // we need serial output for the plotter
    //Serial.begin(115200);
    // start up the I2S peripheral
    i2s_driver_install(I2S_NUM_1, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM_1, &i2s_mic_pins);
    //    xTaskCreate(i2sMicrophoneTask, "i2s Microphone Task", 4096, nullptr, 1, nullptr);
}
