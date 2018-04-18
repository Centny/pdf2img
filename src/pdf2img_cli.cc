#include <stdio.h>
#include <string>
#include "pdf2img.h"
#include "fpdf_annot.h"
#include "fpdf_attachment.h"
#include "fpdf_dataavail.h"
#include "fpdf_edit.h"
#include "fpdf_ext.h"
#include "fpdf_formfill.h"
#include "fpdf_progressive.h"
#include "fpdf_structtree.h"
#include "fpdf_text.h"

unsigned long FPDF_PrintBitmapStride(FPDF_DOCUMENT doc, FPDF_PAGE page, int pageIndex, FPDF_BITMAP bitmap)
{
    printf("page %d stride:%ld\n", pageIndex, FPDFBitmap_GetStride(bitmap));
}

void ExampleUnsupportedHandler(UNSUPPORT_INFO *info, int type)
{
    std::string feature = "Unknown";
    switch (type)
    {
    case FPDF_UNSP_DOC_XFAFORM:
        feature = "XFA";
        break;
    case FPDF_UNSP_DOC_PORTABLECOLLECTION:
        feature = "Portfolios_Packages";
        break;
    case FPDF_UNSP_DOC_ATTACHMENT:
    case FPDF_UNSP_ANNOT_ATTACHMENT:
        feature = "Attachment";
        break;
    case FPDF_UNSP_DOC_SECURITY:
        feature = "Rights_Management";
        break;
    case FPDF_UNSP_DOC_SHAREDREVIEW:
        feature = "Shared_Review";
        break;
    case FPDF_UNSP_DOC_SHAREDFORM_ACROBAT:
    case FPDF_UNSP_DOC_SHAREDFORM_FILESYSTEM:
    case FPDF_UNSP_DOC_SHAREDFORM_EMAIL:
        feature = "Shared_Form";
        break;
    case FPDF_UNSP_ANNOT_3DANNOT:
        feature = "3D";
        break;
    case FPDF_UNSP_ANNOT_MOVIE:
        feature = "Movie";
        break;
    case FPDF_UNSP_ANNOT_SOUND:
        feature = "Sound";
        break;
    case FPDF_UNSP_ANNOT_SCREEN_MEDIA:
    case FPDF_UNSP_ANNOT_SCREEN_RICHMEDIA:
        feature = "Screen";
        break;
    case FPDF_UNSP_ANNOT_SIG:
        feature = "Digital_Signature";
        break;
    }
    printf("Unsupported feature: %s.\n", feature.c_str());
}

int main()
{
    FPDF_LIBRARY_CONFIG config;
    config.version = 2;
    config.m_pUserFontPaths = nullptr;
    config.m_pIsolate = nullptr;
    config.m_v8EmbedderSlot = 0;
    FPDF_InitLibraryWithConfig(&config);

    // UNSUPPORT_INFO unsupported_info = {};
    // unsupported_info.version = 1;
    // unsupported_info.FSDK_UnSupport_Handler = ExampleUnsupportedHandler;
    // FSDK_SetUnSpObjProcessHandler(&unsupported_info);
    FPDF_DOCUMENT doc = FPDF_LoadDocument("./test-data/sample.pdf", NULL);
    //unsigned long ec = FPDF_Pdf2Img("../test-data/sample.pdf", FPDF_PrintBitmapStride);
    printf("ec->%p->%ld\n", doc, FPDF_GetLastError());
    return 0;
}