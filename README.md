# ebpf-kill-example

ebpf-kill-example is an example of an eBPF program hooking into the kill tracepoint.
It will print to the debug trace pipe when a program is about to be forcefully killed.

## Installation

To install ebpf-kill-example, first clone this repository.

```
git clone https://github.com/niclashedam/ebpf-kill-example
```

Install dependencies needed to compile *ebpf-kill-example*.

```
make deps
```

Download the newest instance of the kernel and compile necessary modules. This will take 5-10 minutes depending on your connection.

```
make kernel-src
```

Compile *ebpf-kill-example*.

```
make
```

## Usage

To start ebpf-kill-example, go to the root of the repository and run.

```
sudo LD_LIBRARY_PATH=lib64/:$LD_LIBRARY_PATH ./src/ebpf-kill-example
```

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License
[GPLv3](https://choosealicense.com/licenses/gpl-3.0/)
