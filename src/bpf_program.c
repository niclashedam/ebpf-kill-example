#include <linux/bpf.h>
#include "bpf_helpers.h"

struct syscalls_enter_kill_args {
    long long pad;

    long syscall_nr;
    long pid;
    long sig;
};

SEC("tracepoint/syscalls/sys_enter_kill")
int bpf_prog(struct syscalls_enter_kill_args *ctx) {
  
  if(ctx->sig != 9) return 0;

  char fmt[] = "PID %u is being killed!";
  bpf_trace_printk(fmt, sizeof(fmt), ctx->pid, sizeof(ctx->pid));

  return 0;
}

char _license[] SEC("license") = "GPL";
