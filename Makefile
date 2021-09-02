all:
	nasm -f bin ./src/boot/boot.asm -o ./bin/boot.bin


emulate:
	qemu-system-x86_64 -hda ./boot.bin

clean:
	rm -rf ./bin/boot.bin