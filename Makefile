qr_encode: main.c QR_Encode.c
	rm -f qr_encode
	gcc -std=c99 main.c QR_Encode.c -o qr_encode
