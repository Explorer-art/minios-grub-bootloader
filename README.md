# Minios (GRUB)

### Install tools
```
sudo apt install gcc nasm make mtools qemu-system grub-pc
```

### Build
```
sudo make
```

### Run
```
make run
```
or
```
qemu-system-x86_64 -drive format=raw,file=minios.img,if=ide,index=0
```