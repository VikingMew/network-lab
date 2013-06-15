TARGET 	= sircd
TARGET2 = srouted

CC		= gcc
LD		= gcc

CFLAGS	= -O2 -Wall
LFLAGS	= 
LIBS	= -lpthread

OBJECTS = sircd.o rtlib.o rtgrading.o csapp.o network.o ircservice.o 
OBJECTS2 = srouted.o rtlib.o rtgrading.o csapp.o irc.o rt_network.o routing.o

$(TARGET): $(OBJECTS)
	$(LD) $(LFLAGS)  -o $@ $^ $(LIBS)

$(TARGET2): $(OBJECTS2)
	$(LD) $(LFLAGS)  -o $@ $^ $(LIBS)

srouted.o: srouted.c srouted.h rtlib.h rtgrading.h csapp.h 
	$(CC) $(CFLAGS) -c -o $@ $<

irc.o: irc.c irc.h csapp.h rtlib.h srouted.h routing.h
	$(CC) $(CFLAGS) -c -o $@ $<

rt_network.o: rt_network.c rt_network.h irc.h routing.h srouted.h
	$(CC) $(CFLAGS) -c -o $@ $<

routing.o: routing.c routing.h csapp.h rtgrading.h rtlib.h
	$(CC) $(CFLAGS) -c -o $@ $<

sircd.o: sircd.c rtlib.h rtgrading.h csapp.h
	$(CC) $(CFLAGS) -c -o $@ $<

rtlib.o: rtlib.c rtlib.h csapp.h
	$(CC) $(CFLAGS) -c -o $@ $<

rtgrading.o: rtgrading.c rtgrading.h
	$(CC) $(CFLAGS) -c -o $@ $<

csapp.o: csapp.c csapp.h
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o
	rm -f $(TARGET)
	rm -f $(TARGET2)

.PHONY: clean
