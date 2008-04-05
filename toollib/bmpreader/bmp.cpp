#include "bmp.h"
#include <assert.h>
#include <stdlib.h>



#define DEBUG_HEADERS 0




typedef char BYTE;                /* 8 bits */
typedef unsigned short int WORD;  /* 16-bit unsigned integer. */
typedef unsigned int DWORD;       /* 32-bit unsigned integer */
typedef int LONG;                 /* 32-bit signed integer */



typedef struct tagBITMAPFILEHEADER {
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
    WORD bfReserved2;
    DWORD bfOffBits;
} BITMAPFILEHEADER;



typedef struct tagBITMAPINFOHEADER {
    DWORD biSize;
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG biXPelsPerMeter;
    LONG biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
} BITMAPINFOHEADER;



/* constants for the biCompression field */
#define BI_RGB        0L
#define BI_RLE8       1L
#define BI_RLE4       2L
#define BI_BITFIELDS  3L



typedef struct tagRGBTRIPLE {
    BYTE rgbtBlue;
    BYTE rgbtGreen;
    BYTE rgbtRed;
} RGBTRIPLE;



typedef struct tagRGBQUAD {
    BYTE rgbBlue;
    BYTE rgbGreen;
    BYTE rgbRed;
    BYTE rgbReserved;
} RGBQUAD;



/* Some magic numbers */

#define BMP_BF_TYPE 0x4D42
/* word BM */

#define BMP_BF_OFF_BITS 54
/* 14 for file header + 40 for info header (not sizeof(), but packed size) */

#define BMP_BI_SIZE 40
/* packed size of info header */



/* Reads a WORD from a file in little endian format */
static WORD WordReadLE(FILE *fp)
{
    WORD lsb, msb;

    lsb = getc(fp);
    msb = getc(fp);
    return (msb << 8) | lsb;
}



/* Writes a WORD to a file in little endian format */
static void WordWriteLE(WORD x, FILE *fp)
{
    BYTE lsb, msb;

    lsb = (BYTE) (x & 0x00FF);
    msb = (BYTE) (x >> 8);
    putc(lsb, fp);
    putc(msb, fp);
}



/* Reads a DWORD word from a file in little endian format */
static DWORD DWordReadLE(FILE *fp)
{
    DWORD b1, b2, b3, b4;

    b1 = getc(fp);
    b2 = getc(fp);
    b3 = getc(fp);
    b4 = getc(fp);
    return (b4 << 24) | (b3 << 16) | (b2 << 8) | b1;
}



/* Writes a DWORD to a file in little endian format */
static void DWordWriteLE(DWORD x, FILE *fp)
{
    unsigned char b1, b2, b3, b4;

    b1 = (x & 0x000000FF);
    b2 = ((x >> 8) & 0x000000FF);
    b3 = ((x >> 16) & 0x000000FF);
    b4 = ((x >> 24) & 0x000000FF);
    putc(b1, fp);
    putc(b2, fp);
    putc(b3, fp);
    putc(b4, fp);
}



/* Reads a LONG word from a file in little endian format */
static LONG LongReadLE(FILE *fp)
{
    LONG b1, b2, b3, b4;

    b1 = getc(fp);
    b2 = getc(fp);
    b3 = getc(fp);
    b4 = getc(fp);
    return (b4 << 24) | (b3 << 16) | (b2 << 8) | b1;
}



/* Writes a LONG to a file in little endian format */
static void LongWriteLE(LONG x, FILE *fp)
{
    char b1, b2, b3, b4;

    b1 = (x & 0x000000FF);
    b2 = ((x >> 8) & 0x000000FF);
    b3 = ((x >> 16) & 0x000000FF);
    b4 = ((x >> 24) & 0x000000FF);
    putc(b1, fp);
    putc(b2, fp);
    putc(b3, fp);
    putc(b4, fp);
}



Image *BMPReadImage(FILE *fp)
{
    Image *img = NULL;
    BITMAPFILEHEADER bmfh;
    BITMAPINFOHEADER bmih;
    int x, y;
    int lineLength;

    assert(fp != NULL);

    /* Read file header */

    /* fread(&bmfh, sizeof(bmfh), 1, fp); */
    /* fread won't work on different platforms because of endian
     * issues.  Sigh... */

    bmfh.bfType = WordReadLE(fp);
    bmfh.bfSize = DWordReadLE(fp);
    bmfh.bfReserved1 = WordReadLE(fp);
    bmfh.bfReserved2 = WordReadLE(fp);
    bmfh.bfOffBits = DWordReadLE(fp);

    /* Debug file header */
#if DEBUG_HEADERS
    fprintf(stderr, "file header:\n");
    fprintf(stderr, "\tbfType: %x\n", (int) bmfh.bfType);
    fprintf(stderr, "\tbfSize: %d\n", (int) bmfh.bfSize);
    fprintf(stderr, "\tbfReserved1: %d\n", (int) bmfh.bfReserved1);
    fprintf(stderr, "\tbfReserved2: %d\n", (int) bmfh.bfReserved2);
    fprintf(stderr, "\tbfOffBits: %d\n", (int) bmfh.bfOffBits);
#endif

    /* Check file header */
    assert(bmfh.bfType == BMP_BF_TYPE);
    /* ignore bmfh.bfSize */
    /* ignore bmfh.bfReserved1 */
    /* ignore bmfh.bfReserved2 */
    assert(bmfh.bfOffBits == BMP_BF_OFF_BITS);

    /* Read info header */

    /* fread(&bmih, sizeof(bmih), 1, fp); */
    /* same problem as above... */

    bmih.biSize = DWordReadLE(fp);
    bmih.biWidth = LongReadLE(fp);
    bmih.biHeight = LongReadLE(fp);
    bmih.biPlanes = WordReadLE(fp);
    bmih.biBitCount = WordReadLE(fp);
    bmih.biCompression = DWordReadLE(fp);
    bmih.biSizeImage = DWordReadLE(fp);
    bmih.biXPelsPerMeter = LongReadLE(fp);
    bmih.biYPelsPerMeter = LongReadLE(fp);
    bmih.biClrUsed = DWordReadLE(fp);
    bmih.biClrImportant = DWordReadLE(fp);

    /* Debug info header */
#if DEBUG_HEADERS
    fprintf(stderr, "info header:\n");
    fprintf(stderr, "\tbiSize: %d\n", (int) bmih.biSize);
    fprintf(stderr, "\tbiWidth: %d\n", (int) bmih.biWidth);
    fprintf(stderr, "\tbiHeight: %d\n", (int) bmih.biHeight);
    fprintf(stderr, "\tbiPlanes: %d\n", (int) bmih.biPlanes);
    fprintf(stderr, "\tbiBitCount: %d\n", (int) bmih.biBitCount);
    fprintf(stderr, "\tbiCompression: %d\n", (int) bmih.biCompression);
    fprintf(stderr, "\tbiSizeImage: %d\n", (int) bmih.biSizeImage);
    fprintf(stderr, "\tbiXPelsPerMeter: %d\n", (int) bmih.biXPelsPerMeter);
    fprintf(stderr, "\tbiYPelsPerMeter: %d\n", (int) bmih.biYPelsPerMeter);
    fprintf(stderr, "\tbiClrUsed: %d\n", (int) bmih.biClrUsed);
    fprintf(stderr, "\tbiClrImportant: %d\n", (int) bmih.biClrImportant);
#endif

    /* Check info header */
    assert(bmih.biSize == BMP_BI_SIZE);
    assert(bmih.biWidth > 0);
    assert(bmih.biHeight > 0);
    assert(bmih.biPlanes == 1);
    assert(bmih.biBitCount == 24);  /* RGB */
    assert(bmih.biCompression == BI_RGB);   /* RGB */
    lineLength = bmih.biWidth * 3;  /* RGB */
    if ((lineLength % 4) != 0) {
    lineLength = (lineLength / 4 + 1) * 4;
    }
    //Fake
    bmih.biSizeImage = (DWORD) lineLength * (DWORD) bmih.biHeight;
    //End Fake
	
    assert(bmih.biSizeImage == (DWORD) lineLength * (DWORD) bmih.biHeight);


    
    /* ignore bmih.biXPelsPerMeter */
    /* ignore bmih.biYPelsPerMeter */
    /* ignore bmih.biClrUsed - we assume a true color display, and
     * won't use palettes */
    /* ignore bmih.biClrImportant - same reason */

    /* Creates the image */
    img = new Image(bmih.biWidth, bmih.biHeight);

    /* Read triples */
    /* RGB */
    {
    RGBTRIPLE *triples;
    triples = (RGBTRIPLE *)malloc(lineLength);
    assert(triples != NULL);
    fseek(fp, (long) bmfh.bfOffBits, SEEK_SET);

    for (y = 0; y < img->height; y++) {
        fread(triples, 1, lineLength, fp);
        assert(ferror(fp) == 0);

        /* Copy triples */
        for (x = 0; x < img->width; x++)
            img->GetPixel(x, img->height-y-1).Set(
                triples[x].rgbtRed,
                triples[x].rgbtGreen,
                triples[x].rgbtBlue,
                255);
    }
    free(triples);
    }
    return img;
}



void BMPWriteImage(Image *img, FILE *fp)
{
    assert(img);

    BITMAPFILEHEADER bmfh;
    BITMAPINFOHEADER bmih;
    int x, y;
    int lineLength;

    lineLength = img->width * 3;    /* RGB */
    if ((lineLength % 4) != 0) {
    lineLength = (lineLength / 4 + 1) * 4;
    }
    /* Write file header */

    bmfh.bfType = BMP_BF_TYPE;
    bmfh.bfSize = BMP_BF_OFF_BITS + lineLength * img->height;
    bmfh.bfReserved1 = 0;
    bmfh.bfReserved2 = 0;
    bmfh.bfOffBits = BMP_BF_OFF_BITS;

    /* Debug file header */
#if DEBUG_HEADERS
    fprintf(stderr, "file header:\n");
    fprintf(stderr, "\tbfType: %x\n", bmfh.bfType);
    fprintf(stderr, "\tbfSize: %d\n", (int) bmfh.bfSize);
    fprintf(stderr, "\tbfReserved1: %d\n", (int) bmfh.bfReserved1);
    fprintf(stderr, "\tbfReserved2: %d\n", (int) bmfh.bfReserved2);
    fprintf(stderr, "\tbfOffBits: %d\n", (int) bmfh.bfOffBits);
#endif

    WordWriteLE(bmfh.bfType, fp);
    DWordWriteLE(bmfh.bfSize, fp);
    WordWriteLE(bmfh.bfReserved1, fp);
    WordWriteLE(bmfh.bfReserved2, fp);
    DWordWriteLE(bmfh.bfOffBits, fp);

    /* Write info header */

    bmih.biSize = BMP_BI_SIZE;
    bmih.biWidth = img->width;
    bmih.biHeight = img->height;
    bmih.biPlanes = 1;
    bmih.biBitCount = 24;       /* RGB */
    bmih.biCompression = BI_RGB;    /* RGB */
    bmih.biSizeImage = lineLength * (DWORD) bmih.biHeight;  /* RGB */
    bmih.biXPelsPerMeter = 2925;
    bmih.biYPelsPerMeter = 2925;
    bmih.biClrUsed = 0;
    bmih.biClrImportant = 0;

    /* Debug info header */
#if DEBUG_HEADERS
    fprintf(stderr, "info header:\n");
    fprintf(stderr, "\tbiSize: %d\n", (int) bmih.biSize);
    fprintf(stderr, "\tbiWidth: %d\n", (int) bmih.biWidth);
    fprintf(stderr, "\tbiHeight: %d\n", (int) bmih.biHeight);
    fprintf(stderr, "\tbiPlanes: %d\n", (int) bmih.biPlanes);
    fprintf(stderr, "\tbiBitCount: %d\n", (int) bmih.biBitCount);
    fprintf(stderr, "\tbiCompression: %d\n", (int) bmih.biCompression);
    fprintf(stderr, "\tbiSizeImage: %d\n", (int) bmih.biSizeImage);
    fprintf(stderr, "\tbiXPelsPerMeter: %d\n", (int) bmih.biXPelsPerMeter);
    fprintf(stderr, "\tbiYPelsPerMeter: %d\n", (int) bmih.biYPelsPerMeter);
    fprintf(stderr, "\tbiClrUsed: %d\n", (int) bmih.biClrUsed);
    fprintf(stderr, "\tbiClrImportant: %d\n", (int) bmih.biClrImportant);
#endif

    DWordWriteLE(bmih.biSize, fp);
    LongWriteLE(bmih.biWidth, fp);
    LongWriteLE(bmih.biHeight, fp);
    WordWriteLE(bmih.biPlanes, fp);
    WordWriteLE(bmih.biBitCount, fp);
    DWordWriteLE(bmih.biCompression, fp);
    DWordWriteLE(bmih.biSizeImage, fp);
    LongWriteLE(bmih.biXPelsPerMeter, fp);
    LongWriteLE(bmih.biYPelsPerMeter, fp);
    DWordWriteLE(bmih.biClrUsed, fp);
    DWordWriteLE(bmih.biClrImportant, fp);

    /* Write pixels */
    for (y = 0; y < img->height; y++) {
    int nbytes = 0;
    for (x = 0; x < img->width; x++) {
        Pixel p = img->GetPixel(x, img->height-y-1);
        putc(p.b, fp), nbytes++;
        putc(p.g, fp), nbytes++;
        putc(p.r, fp), nbytes++;
        /* putc(p.a, fp), nbytes++; */
        /* RGB */
    }
    /* Padding for 32-bit boundary */
    while ((nbytes % 4) != 0) {
        putc(0, fp);
        nbytes++;
    }
    }
}

