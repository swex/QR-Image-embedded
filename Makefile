PROG = qr_encode
OBJS = main.o QR_Encode.o
CFLAGS = -std=c99 -D_XOPEN_SOURCE

$(PROG): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

main.o: CFLAGS += -Wall

clean:
	rm -f $(PROG) $(OBJS)
