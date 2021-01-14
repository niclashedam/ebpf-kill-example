# ebpf-kill-example

*ebpf-kill-example* is an example of an eBPF program hooking into the kill tracepoint.
This project is a Proof-of-Concept (PoC) showing the feasibility and viability of eBPF.
Furthermore, the project shows how to create and run a simple eBPF program.

## Installation

To install ebpf-kill-example, first clone this repository.

```
git clone https://github.com/niclashedam/ebpf-kill-example
```

Install dependencies needed to compile *ebpf-kill-example*.

```
make deps
```

Download an instance of the kernel and compile necessary modules. This will take 5-10 minutes depending on your connection.
By default, the Makefile is configured to download a LTS version of the kernel.

```
make kernel-src
```

Compile *ebpf-kill-example*.

```
make
```

## Usage

To start *ebpf-kill-example*, go to the root of the repository and run.

```
sudo LD_LIBRARY_PATH=lib64/:$LD_LIBRARY_PATH ./src/ebpf-kill-example
```

## Test

To test *ebpf-kill-example*, run `./test/test.sh`.
This will load the eBPF program, start a looping process and kill it. It will
verify that the eBPF program was invoked when kill was called.

```
nhed@nhed-1:~/Development/ebpf-kill-example$ ./test/test.sh
-- Loading eBPF program.
-- Starting test process to kill.
-- PID of test process is 332996.
-- Killed. Waiting for eBPF program to terminate ..
[ OK ] -- eBPF program ran as expected.
```

## Example
![Malicious user](/img/example-user.png?raw=true "Malicious user")
![eBPF Program](/img/example-ebpf.png?raw=true "eBPF Program")

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License
[GPLv3](https://choosealicense.com/licenses/gpl-3.0/)
