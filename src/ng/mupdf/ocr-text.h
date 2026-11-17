/* Copyright 2026 the SumatraPDF project authors (see AUTHORS file).
   License: GPLv3 */

#ifndef SUMATRA_OCR_TEXT_H
#define SUMATRA_OCR_TEXT_H

#include "mupdf/fitz.h"

#ifdef __cplusplus
extern "C" {
#endif

fz_buffer* sumatra_ocr_pixmap_text(fz_context* ctx, fz_pixmap* pixmap, const char* language, const char* datadir);
fz_buffer* sumatra_ocr_bgra_text(fz_context* ctx, const unsigned char* samples, int width, int height, int stride,
                                 int xres, int yres, const char* language, const char* datadir);

#ifdef __cplusplus
}
#endif

#endif
