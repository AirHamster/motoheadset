/* I2S Example

    This example code will output 100Hz sine wave and triangle wave to 2-channel of I2S driver
    Every 5 seconds, it will change bits_per_sample [16, 24, 32] for i2s data

    This example code is in the Public Domain (or CC0 licensed, at your option.)

    Unless required by applicable law or agreed to in writing, this
    software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
    CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2s.h"
#include "driver/gpio.h"
#include "esp_system.h"
#include <math.h>
#include "SinWaveGenerator.h"
#include "I2SOutput.h"
#include "i2s_microphone.h"
#include "WAVFileReader.h"
#include "Microphone.h"

#define SAMPLE_RATE (16000)
#define I2S_NUM (0)
#define WAVE_FREQ_HZ (100)
#define PI (3.14159265)
#define I2S_BCK_IO (GPIO_NUM_13)
#define I2S_WS_IO (GPIO_NUM_21)
#define I2S_DO_IO (GPIO_NUM_15)
#define I2S_DI_IO (-1)

i2s_pin_config_t i2sPins = {
    .bck_io_num = I2S_BCK_IO,
    .ws_io_num = I2S_WS_IO,
    .data_out_num = I2S_DO_IO,
    .data_in_num = -1};

#define SAMPLE_PER_CYCLE (SAMPLE_RATE / WAVE_FREQ_HZ)

static void setup_triangle_sine_waves(int bits)
{
    int *samples_data = (int *)malloc(((bits + 8) / 16) * SAMPLE_PER_CYCLE * 4);
    unsigned int i, sample_val;
    double sin_float, triangle_float, triangle_step = (double)pow(2, bits) / SAMPLE_PER_CYCLE;
    size_t i2s_bytes_write = 0;

    printf("\r\nTest bits=%d free mem=%d, written data=%d\n", bits, esp_get_free_heap_size(), ((bits + 8) / 16) * SAMPLE_PER_CYCLE * 4);

    triangle_float = -(pow(2, bits) / 2 - 1);

    for (i = 0; i < SAMPLE_PER_CYCLE; i++)
    {
        sin_float = sin(i * 2 * PI / SAMPLE_PER_CYCLE);
        if (sin_float >= 0)
            triangle_float += triangle_step;
        else
            triangle_float -= triangle_step;

        sin_float *= (pow(2, bits) / 2 - 1);

        if (bits == 16)
        {
            sample_val = 0;
            sample_val += (short)triangle_float;
            sample_val = sample_val << 16;
            sample_val += (short)sin_float;
            samples_data[i] = sample_val;
        }
        else if (bits == 24)
        { //1-bytes unused
            samples_data[i * 2] = ((int)triangle_float) << 8;
            samples_data[i * 2 + 1] = ((int)sin_float) << 8;
        }
        else
        {
            samples_data[i * 2] = ((int)triangle_float);
            samples_data[i * 2 + 1] = ((int)sin_float);
        }
    }

    i2s_set_clk(static_cast<i2s_port_t>(I2S_NUM), SAMPLE_RATE, (i2s_bits_per_sample_t)bits, (i2s_channel_t)2);
    //Using push
    // for(i = 0; i < SAMPLE_PER_CYCLE; i++) {
    //     if (bits == 16)
    //         i2s_push_sample(0, &samples_data[i], 100);
    //     else
    //         i2s_push_sample(0, &samples_data[i*2], 100);
    // }
    // or write
    i2s_write(static_cast<i2s_port_t>(I2S_NUM), samples_data, ((bits + 8) / 16) * SAMPLE_PER_CYCLE * 4, &i2s_bytes_write, 100);

    free(samples_data);
}

void i2s_test_start(void)
{
    //for 36Khz sample rates, we create 100Hz sine wave, every cycle need 36000/100 = 360 samples (4-bytes or 8-bytes each sample)
    //depend on bits_per_sample
    //using 6 buffers, we need 60-samples per buffer
    //if 2-channels, 16-bit each channel, total buffer is 360*4 = 1440 bytes
    //if 2-channels, 24/32-bit each channel, total buffer is 360*8 = 2880 bytes
    //loop();
    microphone_init();
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX), // Only TX
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = (i2s_bits_per_sample_t)16,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT, //2-channels
        .communication_format = I2S_COMM_FORMAT_PCM,
        .dma_buf_count = 6,
        .dma_buf_len = 60,
        .use_apll = false
        //.intr_alloc_flags = ESP_INTR_FLAG_LEVEL1 //Interrupt level 1
    };
    i2s_pin_config_t pin_config = {
        .bck_io_num = I2S_BCK_IO,
        .ws_io_num = I2S_WS_IO,
        .data_out_num = I2S_DO_IO,
        .data_in_num = I2S_DI_IO //Not used
    };
    //i2s_driver_install(static_cast<i2s_port_t>(I2S_NUM), &i2s_config, 0, NULL);
    //i2s_set_pin(static_cast<i2s_port_t>(I2S_NUM), &pin_config);

    // int test_bits = 16;
    // while (1)
    // {
    //     setup_triangle_sine_waves(test_bits);
    //     vTaskDelay(5000 / portTICK_RATE_MS);
    //     test_bits += 8;
    //     if (test_bits > 32)
    //         test_bits = 16;
    // }
    
    //SampleSource *sampleSource = new SinWaveGenerator(40000, 10000, 0.75);
    //SampleSource *sampleSource = new WAVFileReader("/spiffs/sample.wav");
    SampleSource *sampleSource = new Microphone();
    I2SOutput *output = new I2SOutput();
    output->start(I2S_NUM_0, i2sPins, sampleSource);
    //output->pump();
    while (1){
        vTaskDelay(5000 / portTICK_RATE_MS);
    }
}
