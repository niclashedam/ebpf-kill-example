#include "bpf_load.h"
#include "trace_helpers.h"

int main(int argc, char **argv) {
  if (load_bpf_file("src/bpf_program.o") != 0) {
    printf("The kernel didn't load the BPF program\n");
    return -1;
  }

  read_trace_pipe();

  return 0;
}
