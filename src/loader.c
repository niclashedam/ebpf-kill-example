#include "bpf_load.h"
#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv) {
  // Load our newly compiled eBPF program
  if (load_bpf_file("src/bpf_program.o") != 0) {
    printf("The kernel didn't load the BPF program\n");
    return -1;
  }

  printf("eBPF will listen to force kills for the next 30 seconds!\n");
  sleep(30);

  // map_fd is a global variable containing all eBPF map file descriptors
  int fd = map_fd[0], val;
  long key = -1, prev_key;

  // Iterate over all keys in the map
  while(bpf_map_get_next_key(fd, &prev_key, &key) == 0) {
    printf("%ld was forcefully killed!\n", key);
    prev_key = key;
  }

  return 0;
}

