src = $(wildcard *.c)
obj = $(src:.c=.o)

CFLAGS = -std=c99 -pedantic -Wall -Wextra -Werror -g

myprog: $(obj)
	$(CC) $(CFLAGS) -o $@ $^ 
PHONY: clean
clean:
	rm -f $(obj) myprog
