#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "QR_Encode.h"

#define MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH) /* 0664 */

#define WHITE "\x1b[47m\x1b[30m"
#define BLACK "\x1b[40m\x1b[37m"
#define RESET "\x1b[0m"

int margin = 4;

/* Because getprogname(3) is not portable yet. */
const char *progname;

void usage(void);
void white(void);
void black(void);
void nl(void);
void top_bottom_margin(int);
void left_right_margin(void);
void ansi_qr(unsigned char *, int);

void
usage(void)
{
	fprintf(stderr, "%s [-l level] [-v version] [-o file] "
	    "[-m margin] <input string>\n", progname);
	fprintf(stderr, "level and version default to 3 and 0\n");
	exit(2);
}

int
main(int argc, char *argv[])
{
	progname = argv[0];
	int level = 3;
	int version = 0;
	char *fname = NULL;
	int fd = -1;
	int ch;

	while ((ch = getopt(argc, argv, "l:v:o:m:")) != -1) {
		switch (ch) {
		case 'l':
			level = atoi(optarg);
			break;
		case 'v':
			version = atoi(optarg);
			break;
		case 'o':
			fname = optarg;
			break;
		case 'm':
			margin = atoi(optarg);
			break;
		default:
			usage();
			/* NOTREACHED */
		}
	}
	argc -= optind;
	argv += optind;

	if (argc != 1) {
		usage();
		/* NOTREACHED */
	}

	unsigned char encoded[MAX_BITDATA];
	memset(encoded, 0, sizeof(encoded));
	int width = EncodeData(level, version, argv[0], 0, encoded);
	int size = ((width*width)/8) + (((width*width)%8)?1:0);

	printf("QR Code width: %d\n", width);

	if (fname != NULL && *fname != '\0') {
		fd = open(fname, O_WRONLY | O_CREAT | O_TRUNC, MODE);
		if (fd == -1)
			err(1, "open");

		printf("writing file (%d bytes)\n", size);
		if (write(fd, encoded, size) != size)
			err(1, "write");

		close(fd);
	}

	ansi_qr(encoded, width);

	return 0;
}

enum { was_reset, was_white, was_black } last_color = was_reset;

void
white(void)
{
	if (last_color != was_white) {
		printf(WHITE);
		last_color = was_white;
	}
	printf("  ");
}

void
black(void)
{
	if (last_color != was_black) {
		printf(BLACK);
		last_color = was_black;
	}
	printf("  ");
}

void
nl(void)
{
	if (last_color != was_reset) {
		printf(RESET);
		last_color = was_reset;
	}
	printf("\n");
}

void
top_bottom_margin(int width)
{
	int i, j;

	for (i = 0; i < margin; i++) {
		for (j = 0; j < width + (margin * 2); j++)
			white();
		nl();
	}
}

void
left_right_margin(void)
{
	int i;

	for (i = 0; i < margin; i++)
		white();
}

void
ansi_qr(unsigned char *data, int width)
{
	int i, j;

	top_bottom_margin(width);
	for (i = 0; i < width; i++) {
		left_right_margin();
		for (j = 0; j < width; j++) {
			long byte = (i * width + j) / 8;
			long bit = (i * width + j) % 8;
			if (data[byte] & (0x80 >> bit))
				black();
			else
				white();
		}
		left_right_margin();
		nl();
	}
	top_bottom_margin(width);
}
