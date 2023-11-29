#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>

#define CONTROLLER_FIFO_NAME "/tmp/serv_fifo"
#define DEVICE_FIFO_NAME "/tmp/cli_%d_fifo"
#define ACTUATOR_FIFO_NAME "/tmp/acc_fifo"
#define CLOUD_FIFO_NAME "/tmp/cloud_fifo"

#define BUFFER_SIZE 20

struct sensor_data_to_pass {
	char device_name[256];
	pid_t device_pid;
  int sensor_data;
	int threshold;
};

struct actuator_data {
	pid_t device_pid;
	char device_name[256];
	int sensor_value;
};

struct cloud_data {
	char message[256];
	pid_t controller_pid;
};
