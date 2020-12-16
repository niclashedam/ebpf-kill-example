#include <linux/bpf.h>
#include <stdlib.h>
#include "bpf_helpers.h"

// Data in this map is accessible in user-space
struct bpf_map_def SEC("maps") kill_map = {
      .type        = BPF_MAP_TYPE_HASH,
      .key_size    = sizeof(long),
      .value_size  = sizeof(char),
      .max_entries = 64,
};

// This is the tracepoint arguments of the kill functions
// /sys/kernel/debug/tracing/events/syscalls/sys_enter_kill/format
struct syscalls_enter_kill_args {
    long long pad;

    long syscall_nr;
    long pid;
    long sig;
};

SEC("tracepoint/syscalls/sys_enter_kill")
int bpf_prog(struct syscalls_enter_kill_args *ctx) {
  // Ignore normal program terminations
  if(ctx->sig != 9) return 0;

  // We can call glibc functions in eBPF
  long key = labs(ctx->pid);
  int val = 1;

  // Mark the PID as killed in the map
  bpf_map_update_elem(&kill_map, &key, &val, BPF_NOEXIST);

  return 0;
}

// All eBPF programs must be GPL licensed
char _license[] SEC("license") = "GPL";
