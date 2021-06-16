/*
 * Copyright (C) Emlid Limited - All Rights Reserved
 * All information contained in this file is the property of Emlid Limited.
 * The intellectual and technical concepts contained herein are proprietary to
 * Emlid Limited and are protected by copyright law. Distribution of this
 * information or reproduction of this material is strictly forbidden without
 * written permission obtained from Emlid Limited.
 *
 * Written by Ilia Zarubin <ilia.zarubin@emlid.com>, 2020
 */

#include <stdio.h>
#include "sdkconfig.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_log.h"
#include "console.h"
#include "i2s_driver.h"

extern "C" {
void app_main();
}

constexpr uint32_t main_task_delay_ms = 10;

void app_main(void)
{
    cli_start();
    i2s_test_start();
    /* Sheduler started automatically on boot,
        so we don't need to call vTaskStartScheduler()*/
}
