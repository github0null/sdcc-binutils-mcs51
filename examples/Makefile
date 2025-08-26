
CC = ../build/_install/bin/sdcc

all: main.hex
	@echo "ALL DONE"

clean:
	@rm -f *.elf *.map *.asm *.hex *.bin *.o *.o.lst

main.hex: main.o delay.o foo.o
	@echo "link $@"
	$(CC) -mmcs51 --model-small --stack-auto --nooverlay --out-fmt-ihx -Wl,--print-memory-usage -o $@ $^

%.o: %.c | Makefile
	@echo "CC $<"
	@$(CC) -c -mmcs51 --model-small --stack-auto --nooverlay -o $@ $<

.PHONY: all clean
