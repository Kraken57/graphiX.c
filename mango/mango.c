#ifndef MANGO_C_
#define MANGO_C_

#include "mango.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

typedef int Errno;

#define return_defer(value) do { result = (value); goto defer; } while (0)
#define MANGOC_SWAP(T, a, b) do { T t = a; a = b; b = t; } while (0)
#define MANGOC_SIGN(T, x) ((T)((x) > 0) - (T)((x) < 0))
#define MANGOC_ABS(T, x) (MANGOC_SIGN(T, x)*(x))

void mangoc_fill(uint32_t* pixels, size_t width, size_t height, uint32_t color)
{
	for (size_t i = 0; i < width * height; ++i)
	{
		pixels[i] = color;
	}
}


Errno mangoc_save_to_ppm_file(uint32_t* pixels, size_t width, size_t height, const char* file_path)
{
	int result = 0;
	FILE* f = NULL;

	{
		errno_t err;
		err = fopen_s(&f, file_path, "wb");
		if (err != 0) return_defer(err);


		fprintf(f, "P6\n%zu %zu 255\n", width, height);
		if (ferror(f)) return_defer(errno);

		for (size_t i = 0; i < width * height; ++i)
		{
			// 0XAABBGGRR
			uint32_t pixel = pixels[i];
			uint8_t bytes[3] = {
				(pixel >> (8 * 0)) & 0xFF,
				(pixel >> (8 * 1)) & 0xFF,
				(pixel >> (8 * 2)) & 0xFF,
			};
			fwrite(bytes, sizeof(bytes), 1, f);
			if (ferror(f)) return_defer(errno);
		}
	}

defer:
	if (f) fclose(f);
	return result;
}

void mangoc_fill_rect(uint32_t* pixels, size_t pixels_width, size_t pixels_height,
	int x1, int y1, int w, int h, uint32_t color)
{
	int x2 = x1 + MANGOC_SIGN(int, w) * (MANGOC_ABS(int, w) - 1);
	if (x1 > x2) MANGOC_SWAP(int, x1, x2);
	int y2 = y1 + MANGOC_SIGN(int, h) * (MANGOC_ABS(int, h) - 1);
	if (y1 > y2) MANGOC_SWAP(int, y1, y2);

	for (int y = y1; y <= y2; ++y) {
		if (0 <= y && y < (int)pixels_height) {
			for (int x = x1; x <= x2; ++x) {
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
	if (r == 0) return;

	int x1 = cx - r;
	int x2 = cx + r;

	if (x1 > x2) MANGOC_SWAP(int, x1, x2);

	int y1 = cy - r;
	int y2 = cy + r;

	if (y1 > y2) MANGOC_SWAP(int, y1, y2);

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

void swap_int(int* a, int* b)
{
	int t = *a;
	*a = *b;
	*b = t;
}

void mangoc_draw_line(uint32_t* pixels, size_t pixels_width, size_t pixels_height,
	int x1, int y1, int x2, int y2,
	uint32_t color)
{
	// y = k*x + c
	//let y1= k*x1 + c and y2 = k*x2 + c
	//Rearranging we get 
	// y1 - k*x1 = c and (y2 - y1)/(x2 - x1) = k

	int dx = x2 - x1;
	int dy = y2 - y1;

	if (dx != 0) {
		int c = y1 - dy * x1 / dx;

		if (x1 > x2) swap_int(&x1, &x2);
		for (int x = x1; x <= x2; ++x) {
			if (0 <= x && x < (int)pixels_width) {
				int sy1 = dy * x / dx + c;
				int sy2 = dy * (x + 1) / dx + c;
				if (sy1 > sy2) swap_int(&sy1, &sy2);
				for (int y = sy1; y <= sy2; ++y) {
					if (0 <= y && y < (int)pixels_height) {
						pixels[y * pixels_width + x] = color;
					}
				}
			}
		}


	}
	else {
		int x = x1;
		if (0 <= x && x < (int)pixels_width) {
			if (y1 > y2) swap_int(&y1, &y2);
			for (int y = y1; y <= y2; ++y) {
				if (0 <= y && y < (int)pixels_height) {
					pixels[y * pixels_width + x] = color;
				}
			}
		}
	}
}

void mangoc_fill_triangle(uint32_t* pixels, size_t pixels_width, size_t pixels_height,
	int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
	if (y0 > y1) { MANGOC_SWAP(int, y0, y1); MANGOC_SWAP(int, x0, x1); }
	if (y0 > y2) { MANGOC_SWAP(int, y0, y2); MANGOC_SWAP(int, x0, x2); }
	if (y1 > y2) { MANGOC_SWAP(int, y1, y2); MANGOC_SWAP(int, x1, x2); }

	int total_height = y2 - y0;
	for (int i = 0; i < total_height; i++) {
		bool second_half = i > y1 - y0 || y1 == y0;
		int segment_height = second_half ? y2 - y1 : y1 - y0;
		float alpha = (float)i / total_height;
		float beta = (float)(i - (second_half ? y1 - y0 : 0)) / segment_height;
		int ax = x0 + (x2 - x0) * alpha;
		int ay = y0 + i;
		int bx = second_half ? x1 + (x2 - x1) * beta : x0 + (x1 - x0) * beta;
		int by = y0 + i;
		if (ax > bx) { MANGOC_SWAP(int, ax, bx); MANGOC_SWAP(int, ay, by); }
		for (int j = ax; j <= bx; j++) {
			if (0 <= j && j < (int)pixels_width && 0 <= ay && ay < (int)pixels_height) {
				pixels[ay * pixels_width + j] = color;
			}
		}
	}
}




#endif // MANGO_c_