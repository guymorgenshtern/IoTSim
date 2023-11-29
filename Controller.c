//Controller process is a "Server"

#include "Simulation.h"
#include <signal.h>
#include <unistd.h>
#include <ctype.h>

static int actuator_triggered = 0;
void set_threshold_reach_flag(int sig) {
	actuator_triggered = 1;
}


int main () {

	int controller_read_fifo_fd, controller_actuator_write_fifo_fd, controller_cloud_write_fifo_fd;
	struct sensor_data_to_pass sensor_read_data;
	struct actuator_data actuator_data;
	struct cloud_data cloud_data;
	int read_res;
	char device_fifo[256];
	pid_t pid_list[5];
	int device_count = -1;
	int device_actuator_list[5];

	struct sigaction act;
	act.sa_handler = set_threshold_reach_flag;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	mkfifo(CONTROLLER_FIFO_NAME, 0777);
	controller_read_fifo_fd = open(CONTROLLER_FIFO_NAME, O_RDONLY);
	if (controller_read_fifo_fd == -1) {
		fprintf(stderr, "Server fifo failure\n");
		exit(EXIT_FAILURE);
	}

	sleep(10);

	controller_actuator_write_fifo_fd = open(ACTUATOR_FIFO_NAME, O_WRONLY | O_NONBLOCK);

	if (controller_actuator_write_fifo_fd == -1) {
		fprintf(stderr, "Sorry, no Actuator\n");
		exit(EXIT_FAILURE);
	}

	controller_cloud_write_fifo_fd = open(CLOUD_FIFO_NAME, O_WRONLY | O_NONBLOCK);

	if (controller_cloud_write_fifo_fd == -1) {
		fprintf(stderr, "Sorry, no Cloud\n");
		exit(EXIT_FAILURE);
	}



	printf("PID: %d\n", getpid());
	pid_t pid;

	printf("Process start\n");

	pid = fork();

	switch(pid) {
	case -1:

	case 0:

		//initialize all device_actuator_list values to 0
		for (int i =0; i < sizeof(device_actuator_list); i++) {
			device_actuator_list[i] = 0;
		}
		printf("in child\n");
		do {


			read_res = read(controller_read_fifo_fd, &sensor_read_data, sizeof(sensor_read_data));
			int device_found = 0;
			int curr_device;
			if (read_res > 0) {

				//tmp_char_ptr = sensor_read_data.sensor_data;
				sprintf(device_fifo, DEVICE_FIFO_NAME, sensor_read_data.device_pid);

				for (int i = 0; i < sizeof(pid_list); i++) {
					if (pid_list[i] == sensor_read_data.device_pid) {
						device_found = 1;
						curr_device = i;
					}
				}

				if (!device_found) {
					device_count++;
					pid_list[device_count] = sensor_read_data.device_pid;
					curr_device = device_count;
					printf("New device '%s' connected, PID: %d\n", sensor_read_data.device_name, sensor_read_data.device_pid);
				}


				if (sensor_read_data.sensor_data > sensor_read_data.threshold) {

					if (device_actuator_list[curr_device] == 0) {
						strcpy(actuator_data.device_name, sensor_read_data.device_name);
						actuator_data.device_pid =  sensor_read_data.device_pid;
						actuator_data.sensor_value = sensor_read_data.sensor_data;
						write(controller_actuator_write_fifo_fd, &actuator_data, sizeof(actuator_data));

						device_actuator_list[curr_device] = 1;

						kill(getppid(), SIGUSR1);

					}
				}
			}
    } while (read_res > 0);
	default:

		do {
			actuator_triggered = 0;

			sigaction(SIGUSR1, &act, 0);
			pause();
			if (actuator_triggered == 1) {
				strcpy(cloud_data.message, "Actuator has been triggered\n");
				cloud_data.controller_pid = getpid();
				write(controller_cloud_write_fifo_fd, &cloud_data, sizeof(cloud_data));
			}
		} while(actuator_triggered == 1);

	}
}
