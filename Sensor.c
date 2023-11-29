//Simulation process is a "client"
#include "Simulation.h"
#include <ctype.h>

int main (int argc, char *argv[]) {
	int device_threshold;
	char device_name[256];
	if (argc == 3) {
			device_threshold = atoi(argv[2]);
			strcpy(device_name, argv[1]);
	} else {
		printf("Please supply a sensor name and threshold\n");
		exit(EXIT_FAILURE);
	}


	int controller_fifo_fd, device_fifo_fd;
	struct sensor_data_to_pass sensor_data;
	char device_fifo[256];
	srand(device_threshold);
	int sensor_value = rand() % 20;
	printf("%d\n", sensor_value);

	//opening pipe defined in "Controller as CONTROLLER_FIFO_NAME
	controller_fifo_fd = open(CONTROLLER_FIFO_NAME, O_WRONLY | O_NONBLOCK);

	//checking if successful connection
	if (controller_fifo_fd == -1) {
		fprintf(stderr, "Sorry, no server\n");
        	exit(EXIT_FAILURE);
	}

	//setting pid of sensor to object that will be passed to controller
	sensor_data.device_pid = getpid();
	sensor_data.sensor_data = sensor_value;
	sensor_data.threshold = device_threshold;
	strcpy(sensor_data.device_name, device_name);

	printf("here\n");
	printf("%d %d %d \n", sensor_data.device_pid, sensor_data.sensor_data, sensor_data.threshold);

	//if pipe wasn't successfully made
	sprintf(device_fifo, DEVICE_FIFO_NAME, sensor_data.device_pid);
	printf("%s\n", device_fifo);
	if(mkfifo(device_fifo, 0777) == -1) {
		fprintf(stderr, "Sorry, can't make %s\n", device_fifo);
        	exit(EXIT_FAILURE);
	}

	while (1) {
		write(controller_fifo_fd, &sensor_data, sizeof(sensor_data));
		sensor_data.sensor_data++;
		printf("Sending value: %d to Controller\n", sensor_data.sensor_data);
		sleep(1);
	}
}
