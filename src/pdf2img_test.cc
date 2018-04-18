
#include "pdf2img.h"
#include <stdlib.h>
#include "gtest/gtest.h"
#include <fpdfview.h>

unsigned long FPDF_PrintBitmapStride(FPDF_DOCUMENT doc, FPDF_PAGE page, int pageIndex, FPDF_BITMAP bitmap)
{
    printf("page %d stride:%ld\n", pageIndex, FPDFBitmap_GetStride(bitmap));
    char filepath[100];
    RawBitmap raw;
    raw.width = FPDFBitmap_GetWidth(bitmap);
    raw.height = FPDFBitmap_GetHeight(bitmap);
    raw.stride = FPDFBitmap_GetStride(bitmap);
    raw.data = (unsigned char *)FPDFBitmap_GetBuffer(bitmap);
    filepath[sprintf(filepath, "/tmp/pdf1-%d.jpg", pageIndex)] = 0;
    FPDF_WriteJpegFile(filepath, 30, &raw);
    if (pageIndex == 30)
    {
        FPDF_TrimRawBitmap(bitmap, &raw, 0);
        raw.data = (unsigned char *)FPDFBitmap_GetBuffer(bitmap);
        filepath[sprintf(filepath, "/tmp/pdf0-%d.jpg", pageIndex)] = 0;
        FPDF_WriteJpegFile(filepath, 30, &raw);
    }
    if (pageIndex > 30)
    {
        return 1;
    }
    return 0;
}

TEST(Pdf2ImgTest, FPDF_Pdf2Img)
{
    //unsigned long ec = FPDF_Pdf2Img("./test-data/sample.pdf", FPDF_PrintBitmapStride);
    //printf("ec->%ld\n", ec);
    // FPDF_DOCUMENT doc = FPDF_LoadDocument("./test-data/sample.pdf", NULL);
    // unsigned long ec =
    FPDF_Pdf2Img("./test-data/sample3.pdf", 3, FPDF_PrintBitmapStride);
    // FPDF_Pdf2TrimedImg("./test-data/sample3.pdf", 3);
    // printf("ec->%p->%ld\n", doc, FPDF_GetLastError());
}
