#include "Simulation.h"
#include <ctype.h>


int main() {
  int cloud_read_fifo;

  struct cloud_data cloud_data;
  int read_res;

  mkfifo(CLOUD_FIFO_NAME, 0777);
  cloud_read_fifo = open(CLOUD_FIFO_NAME, O_RDONLY);

  if (cloud_read_fifo == -1) {
    fprintf(stderr, "Server fifo failure\n");
    exit(EXIT_FAILURE);
  }

  sleep(12);

  while (1) {
    read_res = read(cloud_read_fifo, &cloud_data, sizeof(cloud_data));
    if (read_res > 0) {
      printf("Message received from %d\n%s", cloud_data.controller_pid, cloud_data.message);
    }
  }
}
