
file ./build/hypervisor.so
set disassembly-flavor intel
target remote localhost:1234

define hook-stop
x/4i $rip
end

hb _start
hb *0x7c00
c