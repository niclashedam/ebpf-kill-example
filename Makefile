CLANG = clang

LINUX_VERSION = v5.8

EXECABLE = src/ebpf-kill-example
BPFCODE = src/bpf_program

BPFTOOLS = kernel-src/samples/bpf
BPFLOADER = $(BPFTOOLS)/bpf_load.c

CCINCLUDE += -Ikernel-src/tools/testing/selftests/bpf
CCINCLUDE += -Ikernel-src/tools/lib/bpf

LOADINCLUDE += -Ikernel-src/samples/bpf
LOADINCLUDE += -Ikernel-src/tools/lib
LOADINCLUDE += -Ikernel-src/tools/perf
LOADINCLUDE += -Ikernel-src/tools/include
LIBRARY_PATH = -Llib64
BPFSO = -lbpf

.PHONY: clean bpfload build deps

deps:
	sudo apt update
	sudo apt install -y build-essential git make gcc clang libelf-dev gcc-multilib

kernel-src:
	git clone --depth 1 --single-branch --branch ${LINUX_VERSION}  https://github.com/torvalds/linux.git kernel-src
	cd kernel-src/tools/lib/bpf && make && make install prefix=../../../../

clean:
	rm -f src/*.o src/*.so $(EXECABLE)

build: kernel-src ${BPFCODE.c} ${BPFLOADER}
	$(CLANG) -target bpf -c $(BPFCODE:=.c) $(CCINCLUDE) -o ${BPFCODE:=.o}

bpfload: build
	$(CLANG) -o $(EXECABLE) -lelf $(LOADINCLUDE) $(LIBRARY_PATH) $(BPFSO) \
        $(BPFLOADER) -DHAVE_ATTR_TEST=0 src/loader.c

$(EXECABLE): bpfload

.DEFAULT_GOAL := $(EXECABLE)
