#include "Simulation.h"
#include <ctype.h>


int main(int argc, char *argv[]) {
  char device_name[256];

  if (argc == 2) {
      strcpy(device_name, argv[1]);
  } else {
    printf("Please supply a device name\n");
    exit(EXIT_FAILURE);
  }


  int actuator_read_fifo;

  struct actuator_data actuator_data;
  int read_res;

  mkfifo(ACTUATOR_FIFO_NAME, 0777);
  actuator_read_fifo = open(ACTUATOR_FIFO_NAME, O_RDONLY);

  if (actuator_read_fifo == -1) {
    fprintf(stderr, "Server fifo failure\n");
    exit(EXIT_FAILURE);
  }

  sleep(11);

  printf("Initialized actuator %s {PID: %d}\n", device_name, getpid());
  while (1) {
    read_res = read(actuator_read_fifo, &actuator_data, sizeof(actuator_data));
    if (read_res > 0) {
      printf("Sensor %s {PID: %d} triggered actuator at value %d\n", actuator_data.device_name, actuator_data.device_pid, actuator_data.sensor_value);
    }
  }
}
