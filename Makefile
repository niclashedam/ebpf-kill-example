.PHONY: clean build deps

deps:
	sudo apt update
	sudo apt install -y build-essential git make gcc clang llvm libelf-dev
	git submodule update --init

build:
	$(MAKE) --directory=src

clean:
	$(MAKE) --directory=src clean

load:
	sudo LD_LIBRARY_PATH=./libbpf/src:$LD_LIBRARY_PATH src/ebpf-kill-example

.DEFAULT_GOAL := build
