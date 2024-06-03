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

#define COLS 8 
#define ROWS 6
#define CELL_WIDTH (WIDTH/COLS)
#define CELL_HEIGHT (HEIGHT/ROWS)

int main(void)
{
	mangoc_fill(pixels, WIDTH, HEIGHT, 0xFF202020);

	for (int y = 0; y < ROWS; ++y) {
		for (int x = 0; x < COLS; ++x) {
			uint32_t color;
			if ((x + y) % 2 == 0) {
				color = 0xFF0000FF;
			}
			else {
				color = 0xFF00FF00;
			}
			mangoc_fill_rect(pixels, WIDTH, HEIGHT, x * CELL_WIDTH, y * CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT, color);
		}
	}



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