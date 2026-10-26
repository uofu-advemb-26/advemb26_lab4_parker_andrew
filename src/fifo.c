#include <stdio.h>

#include "FreeRTOS.h"
#include "portmacro.h"
#include "projdefs.h"
#include "task.h"

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/cyw43_arch.h"
#include "semphr.h"
#include "fifo.h"

void fifo_worker_handler(QueueHandle_t requests, QueueHandle_t results, int id)
{
    // wait in while loop for work to become available from requests
    while (1)
    {
        struct request_msg data = {};

        if (xQueueReceive(requests, &data, 1000))
        {
            // when work is found, perform work
            data.output = data.input + 5;
            data.handled_by = id;
            xQueueSendToBack(results, &data, portMAX_DELAY);
        }
        
        printf("Waiting for work.\n");
    }

    return;
}