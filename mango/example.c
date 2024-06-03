#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include "mango.h"



#define WIDTH 800
#define HEIGHT 600

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

static uint32_t pixels[HEIGHT*WIDTH];

int main(void)
{
	mangoc_fill(pixels, WIDTH, HEIGHT, 0xFF202020);
	size_t rw = 50 * 4;
	size_t rh = 30 * 4;

	mangoc_fill_rect(pixels, WIDTH, HEIGHT, WIDTH / 2 - rw / 2, HEIGHT / 2 - rh / 2, rw, rh, 0xFF2020FF);

	const char* file_path = "output.ppm";
	char err_buf[256];
	Errno err = mangoc_save_to_ppm_file(pixels, WIDTH, HEIGHT, file_path);
	if (err) {
		strerror_s(err_buf, sizeof(err_buf), err);
		fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path, err_buf);
		return 1;
	}
	return 0;

}