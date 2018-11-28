# A QR Code encoding library

This library encodes data in a QR Code.
The output is a binary stream representing the QR Code.

The main function is EncodeData() and is declared as follows:

```c++
/**
 * @brief EncodeData encodes data into QR code
 * @param level One of QR_LEVEL_{L,M,Q,H}.
 * @param version 0 for auto detect or required output version.
 * @param maskPattern Choose concrete mask pattern or use autodetect
 * @param data_in A pointer to the input data to encode.
 * @param in_size Size of data pointed to by data_in. If set to 0, the function will interpret source as a NUL terminated string.
 * @param output Pointer to storage of at MAX_BITDATA bytes in which to store produced data.
 * @return -1 on error or width of the produced QR code
 */
int EncodeData(QR_Level level, int version, QR_MaskPattern maskPattern, const char* data_in, size_t in_size, unsigned char* output);
```
## Example use

```c++
uint8_t encoded[MAX_BITDATA];
int width = EncodeData(QR_LEVEL_L, 0, QR_MaskAuto, data, 0, encoded);
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
## in action
[![asciicast](https://asciinema.org/a/Mn3hwE4AzSZks7a67ra8ofqlh.png)](https://asciinema.org/a/3Nd3kf8cRvLjgLEdMHitDMrP5)

