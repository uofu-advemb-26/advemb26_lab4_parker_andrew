/**
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>

#include "FreeRTOS.h"
#include "portmacro.h"
#include "task.h"

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/cyw43_arch.h"
#include "semphr.h"
#include "signaling.h"

void signal_handle_calculation(SemaphoreHandle_t request,
                               SemaphoreHandle_t response,
                               struct signal_data *data)
{
    while (xSemaphoreTake(request, portMAX_DELAY) == pdTRUE)
    {
        data->output = data->input + 5;
        printf("Data.output = data.input + 5");
        xSemaphoreGive(response);
        return;
    }

    printf("Worker timed out.");
    return;
}

BaseType_t signal_request_calculate(SemaphoreHandle_t request,
                                    SemaphoreHandle_t response,
                                    struct signal_data *data)
{
    if(xSemaphoreGive(request) == pdFALSE)
    {
        printf("Maximum amount of work prescribed");
    }

    while (xSemaphoreTake(response, portMAX_DELAY) == pdTRUE)
    {
        printf("Output: %d - Input: %d", data->output, data->input);
    }

    printf("No workers available");
    return;
}


