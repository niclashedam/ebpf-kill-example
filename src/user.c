/*
 * This file is part of the ebpf-kill-example distribution
 * (https://github.com/niclashedam/ebpf-kill-example). Copyright (c) 2021 Niclas
 * Hedam.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <bpf/bpf.h>
#include <bpf/libbpf.h>
#include <libgen.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
  char path[PATH_MAX];
  sprintf(path, "%s/kern.o", dirname(argv[0]));

  struct bpf_object *obj;
  struct bpf_link *link = NULL;

  int err = -1;

  // Open the eBPF object file with the path above
  obj = bpf_object__open_file(path, NULL);

  // Check for errors in opening the object file
  if (libbpf_get_error(obj)) {
    fprintf(stderr, "ERROR: opening BPF object file failed\n");
    return err;
  }

  // Find the program with the name "ebpf_kill_example" in the object file
  // This is the name of the function in src/kern.c
  struct bpf_program *prog =
      bpf_object__find_program_by_name(obj, "ebpf_kill_example");

  // Check for errors in finding the program
  if (!prog) {
    fprintf(stderr, "ERROR: program not found in object\n");
    goto cleanup;
  }

  // Load the eBPF object file into the kernel
  if (bpf_object__load(obj)) {
    fprintf(stderr, "ERROR: loading BPF object file failed\n");
    goto cleanup;
  }

  // Attach the program to the tracepoint
  link = bpf_program__attach(prog);
  if (libbpf_get_error(link)) {
    fprintf(stderr, "ERROR: bpf_program__attach failed\n");
    link = NULL;
    goto cleanup;
  }

  // Wait for 30 seconds to allow the eBPF program to catch some signals
  printf("eBPF will listen to force kills for the next 30 seconds!\n");
  sleep(30);

  // Find the map with the name "kill_map" in the object file
  struct bpf_map *kill_map = bpf_object__find_map_by_name(obj, "kill_map");
  int kill_map_fd = bpf_map__fd(kill_map);
  long key = -1, prev_key;

  // Iterate over all keys in the map
  while (bpf_map_get_next_key(kill_map_fd, &prev_key, &key) == 0) {
    printf("%ld was forcefully killed!\n", key);
    prev_key = key;
  }

  err = 0;

cleanup:
  bpf_link__destroy(link);
  bpf_object__close(obj);

  return err;
}
