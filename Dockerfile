# Reproducible build/test environment for ebpf-kill-example.
#
#   Build:  docker build -t ebpf-kill-example .
#   Test:   docker run --rm --privileged ebpf-kill-example
#
# The eBPF program is loaded into the kernel and attached to a tracepoint at
# test time, so running the tests requires --privileged (or CAP_BPF +
# CAP_PERFMON + CAP_SYS_ADMIN) and a kernel that exposes the kill tracepoint.
FROM ubuntu:24.04

# Toolchain: clang/llvm emit the BPF bytecode, libelf + zlib are libbpf's
# link-time dependencies, and build-essential/make build libbpf and the loader.
RUN apt-get update && apt-get install -y --no-install-recommends \
        build-essential \
        clang \
        llvm \
        libelf-dev \
        zlib1g-dev \
        make \
        ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

# Build the vendored libbpf submodule and the example.
RUN make

# The entrypoint mounts tracefs so the tracepoint is reachable at test time.
RUN chmod +x docker-entrypoint.sh
ENTRYPOINT ["./docker-entrypoint.sh"]
CMD ["make", "test"]
