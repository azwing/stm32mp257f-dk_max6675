PROG = max6675
SRCS = max6675.c

CLEANFILES = $(PROG)

all: $(PROG)

$(PROG): $(SRCS)
        $(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
        rm -f $(CLEANFILES) $(patsubst %.c,%.o, $(SRCS))
