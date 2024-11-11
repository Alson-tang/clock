#include "main.h"

void application_task(void *pvParameters)
{
    event_e event = EVENT_INIT;

    for(;;) {
        xQueueReceive(g_queue_state_handle, (void*)&event, portMAX_DELAY);

        fsm_state_trans(event);
    }
}
