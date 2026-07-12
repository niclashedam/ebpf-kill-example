TARGET := ebpf-kill-example

SRC_DIR = $(shell realpath .)
LIBBPF_DIR = $(SRC_DIR)/../libbpf/src/
CFLAGS ?= -I$(LIBBPF_DIR)/root/usr/include/
LDFLAGS ?= -L$(LIBBPF_DIR)

LIBS = -lbpf -lelf

# clang's `bpf` target does not search the host's multiarch include directory,
# so kernel/libc headers such as <asm/types.h> are not found. Point it there
# explicitly. Using $(uname -m) keeps this portable across architectures
# (e.g. x86_64-linux-gnu, aarch64-linux-gnu).
ARCH_INCLUDES = -I/usr/include/$(shell uname -m)-linux-gnu

all: $(TARGET) kern.o

.PHONY: clean

clean:
	rm -f $(TARGET)
	rm -f kern.o
	rm -f kern.ll

$(TARGET): %: user.c Makefile
	gcc $(CFLAGS) $(LDFLAGS) -o $(TARGET) user.c -Wl,-rpath=$(LIBBPF_DIR) $(LIBS)

kern.o: kern.c
	clang -S \
	    -target bpf \
	    -D __BPF_TRACING__ \
	    $(CFLAGS) \
	    $(ARCH_INCLUDES) \
	    -Wall \
	    -Werror \
	    -O2 -emit-llvm -c -g kern.c
	llc -march=bpf -filetype=obj -o kern.o kern.ll
