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


//triangle logic 
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

//polygon logic
void mangoc_fill_polygon(uint32_t* pixels, size_t pixels_width, size_t pixels_height,
	const int* xs, const int* ys, size_t n, uint32_t color)
{
	if (n < 3) return; // A polygon must have at least 3 vertices

	// Find the bounding box of the polygon
	int min_y = ys[0];
	int max_y = ys[0];
	for (size_t i = 1; i < n; ++i) {
		if (ys[i] < min_y) min_y = ys[i];
		if (ys[i] > max_y) max_y = ys[i];
	}

	// Scanline fill algorithm
	for (int y = min_y; y <= max_y; ++y) {
		// Find intersections with the scanline
		int intersections[256]; // Maximum number of intersections, increase if necessary
		int num_intersections = 0;

		for (size_t i = 0; i < n; ++i) {
			size_t j = (i + 1) % n;

			int y1 = ys[i], y2 = ys[j];
			int x1 = xs[i], x2 = xs[j];

			if (y1 == y2) continue; // Skip horizontal edges

			if (y < y1 && y < y2) continue; // The scanline is above the edge
			if (y > y1 && y > y2) continue; // The scanline is below the edge

			if (y1 > y2) { MANGOC_SWAP(int, y1, y2); MANGOC_SWAP(int, x1, x2); }

			// Compute the intersection of the scanline with the edge
			int x = x1 + (y - y1) * (x2 - x1) / (y2 - y1);
			intersections[num_intersections++] = x;
		}

		// Sort intersections
		for (int i = 0; i < num_intersections - 1; ++i) {
			for (int j = i + 1; j < num_intersections; ++j) {
				if (intersections[i] > intersections[j]) {
					MANGOC_SWAP(int, intersections[i], intersections[j]);
				}
			}
		}

		// Fill the pixels between pairs of intersections
		for (int i = 0; i < num_intersections; i += 2) {
			if (i + 1 >= num_intersections) break;
			int x1 = intersections[i];
			int x2 = intersections[i + 1];

			if (x1 > x2) MANGOC_SWAP(int, x1, x2);

			for (int x = x1; x <= x2; ++x) {
				if (0 <= x && x < (int)pixels_width && 0 <= y && y < (int)pixels_height) {
					pixels[y * pixels_width + x] = color;
				}
			}
		}
	}
}


//for sprites rendering
void mangoc_draw_sprite(uint32_t* pixels, size_t pixels_width, size_t pixels_height,
	const uint32_t* sprite, size_t sprite_width, size_t sprite_height, int x, int y, float scale)
{
	for (size_t sy = 0; sy < sprite_height; ++sy) {
		for (size_t sx = 0; sx < sprite_width; ++sx) {
			uint32_t color = sprite[sy * sprite_width + sx];
			if (color >> 24) // If alpha is not zero
			{
				int start_x = x + sx * scale;
				int start_y = y + sy * scale;
				int end_x = start_x + scale;
				int end_y = start_y + scale;

				for (int py = start_y; py < end_y; ++py) {
					for (int px = start_x; px < end_x; ++px) {
						if (px >= 0 && px < (int)pixels_width && py >= 0 && py < (int)pixels_height) {
							pixels[py * pixels_width + px] = color;
						}
					}
				}
			}
		}
	}
}


#endif // MANGO_c_