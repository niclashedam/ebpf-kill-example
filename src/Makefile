# SPDX-License-Identifier: (GPL-2.0 OR BSD-2-Clause)
# Baesd on https://raw.githubusercontent.com/Netronome/bpf-samples/master/programmable_rss/Makefile

TARGET := ebpf-kill-example

LLC ?= llc
CLANG ?= clang
CC ?= gcc

LIBBPF_DIR = ../libbpf/src/

BPF_C = kern.c
BPF_OBJ = ${BPF_C:.c=.o}
USER_C = user.c
USER_OBJ = ${USER_C:.c=.o}
OBJECT_LIBBPF = $(LIBBPF_DIR)/libbpf.a

CFLAGS ?= -I$(LIBBPF_DIR)/root/usr/include/
LDFLAGS ?= -L$(LIBBPF_DIR)

LIBS = -lbpf -lelf

all: llvm-check $(TARGET) $(BPF_OBJ)

.PHONY: clean $(CLANG) $(LLC)

clean:
	cd $(LIBBPF_DIR) && $(MAKE) clean;
	rm -f $(TARGET)
	rm -f $(BPF_OBJ)
	rm -f $(USER_OBJ)
	rm -f *.ll

llvm-check: $(CLANG) $(LLC)
	@for TOOL in $^ ; do \
		if [ ! $$(command -v $${TOOL} 2>/dev/null) ]; then \
			echo "*** ERROR: Cannot find tool $${TOOL}" ;\
			exit 1; \
		else true; fi; \
	done

$(OBJECT_LIBBPF):
	@if [ ! -d $(LIBBPF_DIR) ]; then \
		echo "Error: Need libbpf submodule"; \
		echo "May need to run git submodule update --init"; \
		exit 1; \
	else \
		cd $(LIBBPF_DIR) && $(MAKE) all; \
		mkdir -p root; DESTDIR=root $(MAKE) install_headers; \
	fi

$(TARGET): %: $(USER_C) $(OBJECT_LIBBPF) Makefile
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LIBS)

$(BPF_OBJ): %.o: %.c
	$(CLANG) -S \
	    -D __BPF_TRACING__ \
	    $(CFLAGS) \
	    -Wall \
	    -Wno-unused-value -Wno-pointer-sign \
	    -Wno-compare-distinct-pointer-types \
	    -Werror \
	    -O2 -emit-llvm -c -g $<
	$(LLC) -march=bpf -filetype=obj -o $@ ${@:.o=.ll}
