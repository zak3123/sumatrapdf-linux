/* Copyright 2026 the SumatraPDF project authors (see AUTHORS file).
   License: GPLv3 */

#include <string.h>

#include "mupdf/fitz.h"
#include "tessocr.h"

#include "ocr-text.h"

typedef struct {
    fz_buffer* text;
    int line_bbox[4];
    int word_bbox[4];
    int has_line;
    int has_word;
} ocr_text_state;

static void append_ocr_character(fz_context* ctx, void* arg, int unicode, const char* font_name, const int* line_bbox,
                                 const int* word_bbox, const int* char_bbox, int pointsize) {
    ocr_text_state* state = (ocr_text_state*)arg;
    int new_line = state->has_line && memcmp(state->line_bbox, line_bbox, sizeof(state->line_bbox)) != 0;
    int new_word = state->has_word && memcmp(state->word_bbox, word_bbox, sizeof(state->word_bbox)) != 0;

    if (new_line) {
        fz_append_byte(ctx, state->text, '\n');
    } else if (new_word) {
        fz_append_byte(ctx, state->text, ' ');
    }
    memcpy(state->line_bbox, line_bbox, sizeof(state->line_bbox));
    memcpy(state->word_bbox, word_bbox, sizeof(state->word_bbox));
    state->has_line = 1;
    state->has_word = 1;
    fz_append_rune(ctx, state->text, unicode);
}

static fz_buffer* recognise_gray_pixmap(fz_context* ctx, fz_pixmap* gray, const char* language, const char* datadir) {
    void* api = NULL;
    fz_buffer* text = NULL;
    ocr_text_state state = {0};
    fz_var(api);
    fz_var(text);

    fz_try(ctx) {
        text = fz_new_buffer(ctx, 256);
        state.text = text;
        api = ocr_init(ctx, language, datadir, NULL);
        ocr_recognise(ctx, api, gray, append_ocr_character, NULL, &state);
    }
    fz_always(ctx) {
        ocr_fin(ctx, api);
    }
    fz_catch(ctx) {
        fz_drop_buffer(ctx, text);
        fz_rethrow(ctx);
    }
    return text;
}

fz_buffer* sumatra_ocr_pixmap_text(fz_context* ctx, fz_pixmap* pixmap, const char* language, const char* datadir) {
    fz_pixmap* gray = NULL;
    fz_buffer* text = NULL;
    fz_var(gray);
    fz_var(text);

    fz_try(ctx) {
        gray = fz_convert_pixmap(ctx, pixmap, fz_device_gray(ctx), NULL, NULL, fz_default_color_params, 0);
        if (gray->xres <= 0 || gray->yres <= 0) {
            fz_set_pixmap_resolution(ctx, gray, 300, 300);
        }
        text = recognise_gray_pixmap(ctx, gray, language, datadir);
    }
    fz_always(ctx) {
        fz_drop_pixmap(ctx, gray);
    }
    fz_catch(ctx) {
        fz_rethrow(ctx);
    }
    return text;
}

fz_buffer* sumatra_ocr_bgra_text(fz_context* ctx, const unsigned char* samples, int width, int height, int stride,
                                 int xres, int yres, const char* language, const char* datadir) {
    fz_pixmap* gray = NULL;
    fz_buffer* text = NULL;
    int x;
    int y;
    fz_var(gray);
    fz_var(text);

    if (!samples || width <= 0 || height <= 0 || stride < width * 4) {
        fz_throw(ctx, FZ_ERROR_ARGUMENT, "invalid BGRA pixmap for OCR");
    }
    fz_try(ctx) {
        gray = fz_new_pixmap(ctx, fz_device_gray(ctx), width, height, NULL, 0);
        for (y = 0; y < height; y++) {
            const unsigned char* src = samples + (size_t)y * (size_t)stride;
            unsigned char* dst = gray->samples + (size_t)y * (size_t)gray->stride;
            for (x = 0; x < width; x++) {
                unsigned int b = src[x * 4 + 0];
                unsigned int g = src[x * 4 + 1];
                unsigned int r = src[x * 4 + 2];
                unsigned int a = src[x * 4 + 3];
                unsigned int value = (77 * r + 150 * g + 29 * b) >> 8;
                dst[x] = (unsigned char)((value * a + 255 * (255 - a) + 127) / 255);
            }
        }
        fz_set_pixmap_resolution(ctx, gray, xres > 0 ? xres : 300, yres > 0 ? yres : 300);
        text = recognise_gray_pixmap(ctx, gray, language, datadir);
    }
    fz_always(ctx) {
        fz_drop_pixmap(ctx, gray);
    }
    fz_catch(ctx) {
        fz_rethrow(ctx);
    }
    return text;
}
