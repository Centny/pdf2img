
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fpdfview.h>

#define FPDF_ERR_OPEN_FILE 100

typedef unsigned long (*FPDF_BitmapProcCallback)(FPDF_DOCUMENT doc, FPDF_PAGE page, int pageIndex, FPDF_BITMAP bitmap);
unsigned long FPDF_Pdf2Img(FPDF_STRING filepath, int multiple, FPDF_BitmapProcCallback callback);

typedef struct
{
    size_t width;
    size_t height;
    size_t stride;
    unsigned char *data;
} RawBitmap;

int FPDF_WriteJpegFile(const char *filename, int quality, RawBitmap *bitmap);

// int FPDF_RawBitmapCreate();

void FPDF_RawBitmapFree(RawBitmap *raw);

int FPDF_TrimRawBitmap(FPDF_BITMAP bitmap, RawBitmap *input, RawBitmap **output);

// unsigned long FPDF_Pdf2TrimedImg(FPDF_STRING filepath, int multiple);