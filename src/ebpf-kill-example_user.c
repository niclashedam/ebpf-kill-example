#include <unistd.h>
#include <stdio.h>
#include <bpf/bpf.h>
#include <bpf/libbpf.h>

int main(int argc, char **argv)
{
   	// Load our newly compiled eBPF program
    int prog_fd;

    struct bpf_object * obj;
    if (bpf_prog_load("src/ebpf-kill-example_kern.o", BPF_PROG_TYPE_TRACEPOINT, &obj, &prog_fd) != 0)
    {
        printf("The kernel didn't load the BPF program\n");
        return -1;
    }

    if (prog_fd < 1)
    {
        printf("Error creating prog_fd\n");
        return -2;
    }

    printf("eBPF will listen to force kills for the next 30 seconds!\n");
    sleep(30);

    struct bpf_map *kill_map = bpf_object__find_map_by_name(obj, "kill_map");
    int kill_map_fd = bpf_map__fd(kill_map);
    long key = -1, prev_key;

   	// Iterate over all keys in the map
    while (bpf_map_get_next_key(kill_map_fd, &prev_key, &key) == 0)
    {
        printf("%ld was forcefully killed!\n", key);
        prev_key = key;
    }

    return 0;
}
