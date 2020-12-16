#include <linux/bpf.h>
#include "bpf_helpers.h"

struct bpf_map_def SEC("maps") kill_map = {
      .type        = BPF_MAP_TYPE_HASH,
      .key_size    = sizeof(unsigned int),
      .value_size  = sizeof(unsigned int),
      .max_entries = 64,
};

struct syscalls_enter_kill_args {
    long long pad;

    long syscall_nr;
    long pid;
    long sig;
};

SEC("tracepoint/syscalls/sys_enter_kill")
int bpf_prog(struct syscalls_enter_kill_args *ctx) {
  if(ctx->sig != 9) return 0;

  unsigned int key = ctx->pid;
  unsigned int val = 1;

  bpf_map_update_elem(&kill_map, &key, &val, BPF_NOEXIST);

  return 0;
}

char _license[] SEC("license") = "GPL";
