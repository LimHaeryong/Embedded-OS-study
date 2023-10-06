## Ch 3

- compile Entry.S
```bash
arm-none-eabi-as -march=armv7-a -mcpu=cortex-a8 -o Entry.o ./Entry.S
```

- get binary file
```
arm-none-eabi-objcopy -O binary Entry.o Entry.bin
hexdump Entry.bin
```

- use linker
```
arm-none-eabi-ld -n -T ./navilos.ld -nostdlib -o navilos.axf boot/Entry.o
arm-none-eabi-objdump -D navilos.axf
```

- run with QEMU 
```
qemu-system-arm -M realview-pb-a8 -kernel navilos.axf -S -gdb tcp::1234,ipv4
```

- run gdb
```
gdb-multiarch

(gdb) target remote:1234

```

- makefile
```

make all
make debug

---

make gdb
```

- gdb debuging
```
(gdb) file build/navilos.axf
(gdb) list
... codes

(gdb) info register
(gdb) step
(gdb) i r
(gdb) s
...
```

## CH4

