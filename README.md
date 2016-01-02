# A QR Code encoding library

This library encodes data in a QR Code.
The output is a binary stream representing the QR Code.

The main function is EncodeData() and is declared as follows:

`int EncodeData(int, int, const char *, int, unsigned char *);`

The function takes the following arguments:
 - `(int) level`: One of QR_LEVEL_{L,M,Q,H}.
 - `(int) version`: One of QR_VERSION_{S,M,L}.
 - `(const char *) source`: A pointer to the data to encode.
 - `(int) len`: Size of data pointed to by source.
   If set to 0, the function will interpret source as a NUL terminated string.
 - `(unsigned char *) data`: Pointer to storage of at MAX_BITDATA bytes in
   which to store produced data.

The function returns an int representing the width of the produced QR code.

## Example use

```
unsigned char encoded[MAX_BITDATA];
memset(encoded, 0, sizeof(encoded));
int width = EncodeData(level, version, data, 0, encoded);
int size = ((width*width)/8) + (((width*width)%8)?1:0);

// Dump it out somewhere.
write(fd, encoded, size);

// Render it somehow.
for (x = 0; x < width; x++) {
	for (y = 0; y < width; y++) {
		int byte = (x * width + y) / 8;
		int bit = (x * width + y) % 8;
		int value = encoded[byte] & (0x80 >> bit);
		set_pixel(x, y, value ? BLACK : WHITE);
	}
}
```
