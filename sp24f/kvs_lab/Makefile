# Source files for Baseline and Custom
SRCS_BASELINE = baseline_main.c do_snapshot_baseline.c do_recovery_baseline.c open.c close.c put.c get.c
SRCS_CUSTOM = custom_main.c do_snapshot_custom.c do_recovery_custom.c open.c close.c put.c get.c

OBJS_BASELINE = $(SRCS_BASELINE:.c=.o)
OBJS_CUSTOM = $(SRCS_CUSTOM:.c=.o)

TARG_BASELINE = kvs_baseline
TARG_CUSTOM = kvs_custom

# Compiler and flags
CC = gcc
CFLAGS = -Wall -g

# Build targets
all: baseline custom

baseline: $(TARG_BASELINE)

$(TARG_BASELINE): $(OBJS_BASELINE)
	$(CC) -o $(TARG_BASELINE) $(OBJS_BASELINE)

custom: $(TARG_CUSTOM)

$(TARG_CUSTOM): $(OBJS_CUSTOM)
	$(CC) -o $(TARG_CUSTOM) $(OBJS_CUSTOM)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS_BASELINE) $(OBJS_CUSTOM) $(TARG_BASELINE) $(TARG_CUSTOM)
