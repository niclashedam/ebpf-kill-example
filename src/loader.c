#include "bpf_load.h"
#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv) {
  if (load_bpf_file("src/bpf_program.o") != 0) {
    printf("The kernel didn't load the BPF program\n");
    return -1;
  }

  printf("eBPF will listen to force kills for the next 30 seconds!\n");
  sleep(30);

  int fd = map_fd[0];
  unsigned int key = -1, prev_key, val;

  while(bpf_map_get_next_key(fd, &prev_key, &key) == 0) {
    bpf_map_lookup_elem(fd, &key, &val);
    printf("%u was forcefully killed!\n", key);
    prev_key = key;
  }

  return 0;
}

