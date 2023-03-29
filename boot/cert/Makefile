

build:
	gcc cert.c -c
	ld cert.o -T cert.ld -o cert.bin
	objcopy -O binary --remove-section=.note.* cert.bin cert_obj.bin

clean:
	rm *.bin *.o

