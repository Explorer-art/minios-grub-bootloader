# Minios (GRUB)

### Install tools
```
apt install gcc nasm make xorriso mtools qemu-system
```

### Build
```
make
```

### Run
```
./run.sh
```
or
```
qemu-system-i386 -kernel build/minios.bin
```