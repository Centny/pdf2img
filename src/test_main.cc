#include "gtest/gtest.h"
#include <fpdfview.h>
int main(int argc, char **argv)
{
    FPDF_LIBRARY_CONFIG config;
    config.version = 2;
    config.m_pUserFontPaths = NULL;
    config.m_pIsolate = NULL;
    config.m_v8EmbedderSlot = 0;
    FPDF_InitLibraryWithConfig(&config);
    //
    ::testing::InitGoogleTest(&argc, argv);
    int ec = RUN_ALL_TESTS();
    //
    FPDF_DestroyLibrary();
    return ec;
}