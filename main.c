#include "QR_Encode.h"
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH) /* 0664 */

#define WHITE "\x1b[47m\x1b[30m"
#define BLACK "\x1b[40m\x1b[37m"
#define RESET "\x1b[0m"

static int margin = 4;

/* Because getprogname(3) is not portable yet. */
const char* progname;

static void usage(void);
static void white(void);
static void black(void);
static void nl(void);
static void top_bottom_margin(int width);
static void left_right_margin(void);
static void ansi_qr(const unsigned char* data, int width);

static void usage(void)
{
    fprintf(stderr, "%s [-l level] [-v version] [-p mask] [-o file] "
                    "[-m margin] <input string>\n",
        progname);
    fprintf(stderr, "level, version and mask defaults to 3, 0 and auto respectively\n");
    exit(2);
}

static void printError(int error)
{
    switch (error) {
    case QR_E_EmptyInput:
        dprintf(STDOUT_FILENO, "empty input\n");
        break;
    case QR_E_VersionNotFit:
        dprintf(STDOUT_FILENO, "Required version does not fit into output buffer\n");
        break;
    case QR_E_InvalidVersion:
        dprintf(STDOUT_FILENO, "Input data can't fit in QR code\n");
        break;
    default:
        dprintf(STDOUT_FILENO, "Unknown error\n");
        break;
    }
}

int main(int argc, char* argv[])
{
    progname = argv[0];
    QR_Level level = QR_LEVEL_H;
    int version = 0;
    QR_MaskPattern mask = QR_MaskAuto;
    char* fname = NULL;
    int fd = -1;
    int ch;

    while ((ch = getopt(argc, argv, "l:v:o:m:p:")) != -1) {
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
        case 'p':
            mask = atoi(optarg);
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
    errno = EINVAL;
    if (level < QR_LEVEL_L || level > QR_LEVEL_H) {
        err(1, "level");
    }
    if (version < 0 || version > 40) {
        err(1, "version");
    }
    if (mask < QR_MaskAuto || mask > QR_Mask8) {
        err(1, "mask");
    }
    errno = 0;
    unsigned char encoded[MAX_BITDATA];

    int width = EncodeData(level, version, mask, argv[0], 0, encoded);
    if (width <= 0) {
        printError(width);
        exit(EXIT_FAILURE);
    }
    int size = ((width * width) / 8) + (((width * width) % 8) ? 1 : 0);

    printf("QR Code width: %d\n", width);

    if (fname != NULL && *fname != '\0') {
        fd = open(fname, O_WRONLY | O_CREAT | O_TRUNC, MODE);
        if (fd == -1) {
            err(1, "open");
        }

        printf("writing file (%d bytes)\n", size);
        if (write(fd, encoded, size) != size) {
            err(1, "write");
        }

        close(fd);
    }

    ansi_qr(encoded, width);

    return 0;
}

enum { was_reset,
    was_white,
    was_black } static last_color
    = was_reset;

static void white(void)
{
    if (last_color != was_white) {
        printf(WHITE);
        last_color = was_white;
    }
    printf("  ");
}

static void black(void)
{
    if (last_color != was_black) {
        printf(BLACK);
        last_color = was_black;
    }
    printf("  ");
}

static void nl(void)
{
    if (last_color != was_reset) {
        printf(RESET);
        last_color = was_reset;
    }
    printf("\n");
}

static void top_bottom_margin(int width)
{
    int i, j;

    for (i = 0; i < margin; i++) {
        for (j = 0; j < width + (margin * 2); j++) {
            white();
        }
        nl();
    }
}

static void left_right_margin(void)
{
    int i;

    for (i = 0; i < margin; i++) {
        white();
    }
}

static void ansi_qr(const unsigned char* data, int width)
{
    int i, j;

    top_bottom_margin(width);
    for (i = 0; i < width; i++) {
        left_right_margin();
        for (j = 0; j < width; j++) {
            long byte = (i * width + j) / 8;
            long bit = (i * width + j) % 8;
            if (data[byte] & (0x80 >> bit)) {
                black();
            } else {
                white();
            }
        }
        left_right_margin();
        nl();
    }
    top_bottom_margin(width);
}
