# SomaOS
A Operating System made from scratch. This is a project me and my partner have imagined makeing for a very long time. Both me and my partner are really interested in Operating Systems and AI. So we decided why not try to combine both  things and make a OS that uses AI to optimize lower level processes. This is how Soma OS was born. The OS itself was made mostly using the c programming language. Below you can find the current features and the planned features in addition to how YOU can try this OS out!


## Current Features
### Drivers
- A Global Descriptor Table (GDT), sets up CPU privilege levels
- A Interrupt Descriptor Table (IDT), maps out the hardware interrupts
- A Video Graphics Array driver (VGA), allows for the display control
- A Programmable Interval Timer driver (PIT), allows to keep track of time and do time related fuctions
- A Keyboard driver, lets the user type on the vga
- A Physical Memory Manager driver (PMM), A way to manage memory by segmenting it into frames (chunks of 4kb)
### Kernel
- A basic kernel that initializes the drivers
### custom libraries (to help with coding)
- String lib, helps with string related fuction that c does not support natively
- kprintf lib, helps print strings similar to printf but on the vga
### Shell commands
- help, displays all the commands and what they do
- info, displays basic info about Soma OS
- clear, clears the vga
- test memory, tests the allocation of memory and memory fuctions
- uptime, uses the PIT driver to show how long the OS has been on for
- reboot, reboots the os 

## Planned Features
- custom ai (started in /Soir-MoE by Alex)
- upgrade keyboard driver to include special keys
- create a driver to manage storage
- use the storage driver and the memory driver to create, edit, and run files
- a better graphics screen

## How to run
You have 2 options to boot Soma OS

### 1. Boot the os in the web
I have hosted the OS on my domian [https://os.sarveshs.dev/](https://os.sarveshs.dev/). You can visit the website and it automatically boots the operating system in the web with no additional setup

### 2. Building and Running from Source
Install the target cross-compiler and tools:

#### Ubuntu / Debian:
```bash
sudo apt update
sudo apt install build-essential nasm qemu-system-x86 grub-pc-bin xorriso
```

#### Building and Running
```bash
git clone https://github.com/pyGuy152/SomaOS.git
cd SomaOS

make
make run
```

## AI Usage
AI tools like Claude were used to create the initial boilerplate code(parts of boot.s and parts of the /cpu folder), linker.ld, and Makefile. Also used AI to learn low level bits and major concepts. All other code is written by humans!
