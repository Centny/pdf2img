#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <list>
#include <set>
#include <map>
#include "pdf2img.h"
#include "jpeglib.h"

unsigned long FPDF_Pdf2Img(FPDF_STRING filepath, int multiple, FPDF_BitmapProcCallback callback)
{
    FPDF_DOCUMENT doc = FPDF_LoadDocument(filepath, NULL);
    if (!doc)
    {
        return FPDF_GetLastError();
    }
    unsigned long ec = 0;
    int pageCount = FPDF_GetPageCount(doc);
    for (int i = 0; i < pageCount; i++)
    {
        FPDF_PAGE page = FPDF_LoadPage(doc, i);
        int pageWidth = FPDF_GetPageWidth(page) * multiple;
        int pageHeight = FPDF_GetPageHeight(page) * multiple;
        FPDF_BITMAP bitmap = FPDFBitmap_CreateEx(pageWidth, pageHeight, FPDFBitmap_Gray, 0, 0);
        FPDFBitmap_FillRect(bitmap, 0, 0, pageWidth, pageHeight, 0xFFFFFFFF);
        FPDF_RenderPageBitmap(bitmap, page, 0, 0, pageWidth, pageHeight, 0, FPDF_ANNOT);
        ec = callback(doc, page, i, bitmap);
        FPDFBitmap_Destroy(bitmap);
        FPDF_ClosePage(page);
        if (ec)
        {
            break;
        }
    }
    FPDF_CloseDocument(doc);
    return ec;
}

int FPDF_WriteJpegFile(const char *filename, int quality, RawBitmap *bitmap)
{
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE *outfile;
    JSAMPROW row_pointer[1];
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    if ((outfile = fopen(filename, "wb")) == NULL)
    {
        fprintf(stderr, "can't open %s\n", filename);
        return -1;
    }
    jpeg_stdio_dest(&cinfo, outfile);
    cinfo.image_width = bitmap->width;
    cinfo.image_height = bitmap->height;
    cinfo.input_components = 1;
    cinfo.in_color_space = JCS_GRAYSCALE;
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, quality, TRUE);
    jpeg_start_compress(&cinfo, TRUE);
    size_t row_stride = bitmap->stride;
    JSAMPLE *buffer = (JSAMPLE *)bitmap->data;
    while (cinfo.next_scanline < cinfo.image_height)
    {
        row_pointer[0] = &buffer[cinfo.next_scanline * row_stride];
        (void)jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }
    jpeg_finish_compress(&cinfo);
    fclose(outfile);
    jpeg_destroy_compress(&cinfo);
    return 0;
}

void FPDF_RawBitmapFree(RawBitmap **raw)
{
    free((*raw)->data);
    free(*raw);
    *raw = 0;
}

typedef struct
{
    size_t x;
    size_t y;
    size_t w;
    size_t h;
    unsigned char avg;
} Rect;

unsigned char FPDF_StepAvg(RawBitmap *input, size_t x, size_t y, int step)
{
    unsigned long total = 0;
    for (int i = 0; i < step; i++)
    {
        for (int j = 0; j < step; j++)
        {
            total += input->data[(y + j) * input->width + x + i];
        }
    }
    return (unsigned char)total / (step * step);
}

Rect *FPDF_ExpandRect(RawBitmap *input, size_t x, size_t y, int step, int start)
{
    Rect *rect = (Rect *)malloc(sizeof(Rect));
    rect->x = x;
    rect->y = y;
    rect->avg = FPDF_StepAvg(input, x, y, step);
    switch (start)
    {
    case 1:
        int limit = 0;
        while (true)
        {
        }
    }
    return rect;
}

typedef struct
{
    size_t x0;
    size_t y0;
    size_t x1;
    size_t y1;
} FPDF_Line;

int FPDF_TrimRawBitmap(FPDF_BITMAP bitmap, RawBitmap *input, RawBitmap **output)
{
    int setp = 3;
    RawBitmap *out = (RawBitmap *)malloc(sizeof(RawBitmap));
    //
    size_t cs[26];
    memset(cs, 0, sizeof(size_t) * 26);
    for (size_t x = 0; x < input->width; x++)
    {
        for (size_t y = 0; y < input->height; y++)
        {
            cs[input->data[y * input->width + x] / 26]++;
        }
    }
    int max = -1, second = -1;
    for (int i = 0; i < 26; i++)
    {
        if (max < 0 || cs[i] > cs[max])
        {
            if (max > 0)
            {
                second = max;
            }
            max = i;
        }
        else if (second < 0 || cs[i] > cs[second])
        {
            second = i;
        }
    }
    float rateThreshold = ((float)cs[second]) / ((float)input->width * input->height);
    unsigned char valThreshold = 50;
    int scanMax = 2;
    std::list<FPDF_Line *> lines;
    for (size_t y = scanMax; y < input->height; y++)
    {
        double hited = 0;
        for (size_t x = 0; x < input->width; x++)
        {
            for (int i = 1; i <= scanMax; i++)
            {
                int val = input->data[y * input->width + x] - input->data[(y - i) * input->width + x];
                if (abs(val) > valThreshold)
                {
                    hited++;
                    break;
                }
            }
        }
        double xval = hited / ((double)input->width);
        if (xval < rateThreshold)
        {
            continue;
        }
        FPDF_Line *line = (FPDF_Line *)malloc(sizeof(FPDF_Line));
        line->y0 = y;
        line->y1 = y;
        for (size_t x = 1; x < input->width; x++)
        {
            hited = 0;
            for (int i = 1; i <= scanMax; i++)
            {
                int val = input->data[y * input->width + x] - input->data[y * input->width + x - 1];
                if (abs(val) > valThreshold)
                {
                    hited++;
                }
            }
            if (hited > 0)
            {
                line->x0 = x;
                break;
            }
        }
        for (size_t x = input->width - 1; x > 0; x--)
        {
            hited = 0;
            for (int i = 1; i <= scanMax; i++)
            {
                int val = input->data[y * input->width + x] - input->data[y * input->width + x - 1];
                if (abs(val) > valThreshold)
                {
                    hited++;
                }
            }
            if (hited > 0)
            {
                line->x1 = x;
                break;
            }
        }
        lines.push_back(line);
    }
    for (std::list<FPDF_Line *>::iterator it = lines.begin(); it != lines.end(); it++)
    {
        FPDF_Line *line = *it;
        FPDFBitmap_FillRect(bitmap, line->x0, line->y0, line->x1 - line->x0, 1, 0x0000FFFF);
    }
}

class ImgGroup
{
  public:
    FPDF_BITMAP bitmap;
    std::set<int> pages;
};

long FPDF_BitmapDiff(FPDF_BITMAP a, FPDF_BITMAP b)
{
    int width = FPDFBitmap_GetWidth(a);
    int height = FPDFBitmap_GetHeight(a);
    int stride = FPDFBitmap_GetStride(b);
    if (width != FPDFBitmap_GetWidth(b))
    {
        return -1;
    }
    if (height != FPDFBitmap_GetHeight(b))
    {
        return -1;
    }
    if (stride != FPDFBitmap_GetStride(b))
    {
        return -1;
    }
    unsigned char *abuf = (unsigned char *)FPDFBitmap_GetBuffer(a);
    unsigned char *bbuf = (unsigned char *)FPDFBitmap_GetBuffer(b);
    long total = 0;
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            total += abs(abuf[y * stride + x] - bbuf[y * stride + x]);
        }
    }
    return total;
}

long FPDF_BitmapMerge(FPDF_BITMAP a, FPDF_BITMAP b)
{
    int width = FPDFBitmap_GetWidth(a);
    int height = FPDFBitmap_GetHeight(a);
    int stride = FPDFBitmap_GetStride(b);
    if (width != FPDFBitmap_GetWidth(b))
    {
        return -1;
    }
    if (height != FPDFBitmap_GetHeight(b))
    {
        return -1;
    }
    if (stride != FPDFBitmap_GetStride(b))
    {
        return -1;
    }
    unsigned char *abuf = (unsigned char *)FPDFBitmap_GetBuffer(a);
    unsigned char *bbuf = (unsigned char *)FPDFBitmap_GetBuffer(b);
    long total = 0;
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            if (abuf[y * stride + x] > bbuf[y * stride + x])
            {
                abuf[y * stride + x] = bbuf[y * stride + x];
            }
        }
    }
    return total;
}