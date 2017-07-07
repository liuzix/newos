CC = gcc
LD = ld
CFLAGS = -ffreestanding
targets = test
objects = $(addsuffix .o,$(targets))
all: $(targets)

$(targets): %: %.o
	$(LD) -e 0x10000000000 -T linker.ld -gc-sections $< -o $@


$(objects): %.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f ./*.o
	
