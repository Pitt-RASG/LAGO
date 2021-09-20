PRED_DIR=../prediction
DEC_DIR=../decision
SENSE_DIR=../sensing
POWER_DIR=../power
UTILS_DIR=../utils
LDFLAGS=-pthread -lpthread

CC=gcc
CFLAGS= -Wall -g

INC= -I ../power -I ../sensing -I ../prediction -I ../utils
OBJS=lago.o \
	$(UTILS_DIR)/utils.o \
	$(UTILS_DIR)/outsource.o \
	$(SENSE_DIR)/sensing.o \
	$(SENSE_DIR)/events.o \
	$(POWER_DIR)/power.o \
	$(DEC_DIR)/decision.o \
	$(PRED_DIR)/prediction.o \

all: lago

%.o: %.c
	$(CC) $(INC) -c -o $*.o $*.c

lago.o: lago.c $(UTILS_DIR)/utils.h $(UTILS_DIR)/outsource.h \
		$(UTILS_DIR)/def.h $(SENSE_DIR)/sensing.h \
		$(POWER_DIR)/power.h $(SENSE_DIR)/events.h \
		$(DEC_DIR)/decision.h $(PRED_DIR)/prediction.h
	$(CC) -c $(CFLAGS) $(INC) -o lago.o lago.c

lago: $(OBJS)
	$(CC) -o lago $(OBJS) $(LDFLAGS)

clean:
	rm -f $(OBJS)

.PHONY: all clean
