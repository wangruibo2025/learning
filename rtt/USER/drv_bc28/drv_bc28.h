#ifndef __DRV_BC28_H
#define __DRV_BC28_H


#include <app.h>
#include "bc28.h"
/*��������*/
extern rt_device_t BC28_dev;
extern struct rt_timer bc28_10ms;

/*��������*/
void bc28_callback(void *parameter);
void BC28_SEND_DATA(char *fmt,...);


#endif

