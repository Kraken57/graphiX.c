#include "mango.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

void mangoc_fill(uint32_t* pixels, size_t width, size_t height, uint32_t color)
{
    for (size_t i = 0; i < width * height; ++i) {
        pixels[i] = color;
    }
}

Errno mangoc_save_to_ppm_file(uint32_t* pixels, size_t width, size_t height, const char* file_path)
{
    int result = 0;
    FILE* f = NULL;
    char err_buf[256];

    {
        errno_t err = fopen_s(&f, file_path, "wb");
        if (err != 0) {
            strerror_s(err_buf, sizeof(err_buf), err);
            fprintf(stderr, "ERROR: Failed to open file %s for writing: %s\n", file_path, err_buf);
            result = err;
            goto defer;
        }

        fprintf(f, "P6\n%zu %zu 255\n", width, height);
        if (ferror(f)) {
            strerror_s(err_buf, sizeof(err_buf), errno);
            fprintf(stderr, "ERROR: Failed to write PPM header to file %s: %s\n", file_path, err_buf);
            result = errno;
            goto defer;
        }

        for (size_t i = 0; i < width * height; ++i) {
            uint32_t pixel = pixels[i];
            uint8_t bytes[3] = {
                (pixel >> (8 * 0)) & 0xFF,
                (pixel >> (8 * 1)) & 0xFF,
                (pixel >> (8 * 2)) & 0xFF,
            };
            if (fwrite(bytes, sizeof(bytes), 1, f) != 1) {
                strerror_s(err_buf, sizeof(err_buf), errno);
                fprintf(stderr, "ERROR: Failed to write pixel data to file %s: %s\n", file_path, err_buf);
                result = errno;
                goto defer;
            }
        }
    }

defer:
    if (f) fclose(f);
    return result;
}

void mangoc_fill_rect(uint32_t* pixels, size_t pixels_width, size_t pixels_height,
    int x0, int y0, size_t w, size_t h, uint32_t color)
{
    for (int dy = 0; dy < (int)h; ++dy) {
        int y = y0 + dy;
        if (0 <= y && y < (int)pixels_height) {
            for (int dx = 0; dx < (int)w; ++dx) {
                int x = x0 + dx;
                if (0 <= x && x < (int)pixels_width) {
                    pixels[y * pixels_width + x] = color;
                }
            }
        }
    }
}

void mangoc_fill_circle(uint32_t* pixels, size_t pixels_width, size_t pixels_height,
    int cx, int cy, int r, uint32_t color)
{
    int x1 = cx - r;
    int y1 = cy - r;
    int x2 = cx + r;
    int y2 = cy + r;
    for (int y = y1; y <= y2; ++y) {
        if (0 <= y && y < (int)pixels_height) {
            for (int x = x1; x <= x2; ++x) {
                if (0 <= x && x < (int)pixels_width) {
                    int dx = x - cx;
                    int dy = y - cy;
                    if (dx * dx + dy * dy <= r * r) {
                        pixels[y * pixels_width + x] = color;
                    }
                }
            }
        }
    }
}
