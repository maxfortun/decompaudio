CC = gcc
CFLAGS = -Wall

LD = gcc
LDFLAGS =

RM = /bin/rm -f

OBJS = factor refactor

all: $(OBJS) 

$(OBJS): %: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	$(RM) $(OBJS)
