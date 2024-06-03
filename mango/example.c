#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include "mango.h"



#define WIDTH 800
#define HEIGHT 600

#define COLS 8 
#define ROWS 6
#define CELL_WIDTH (WIDTH/COLS)
#define CELL_HEIGHT (HEIGHT/ROWS)

#define BACKGROUND_COLOR 0xFF202020

static uint32_t pixels[HEIGHT*WIDTH];



int main(void)
{
	mangoc_fill(pixels, WIDTH, HEIGHT, BACKGROUND_COLOR);

	for (int y = 0; y < ROWS; ++y) {
		for (int x = 0; x < COLS; ++x) {
			uint32_t color = BACKGROUND_COLOR;
			if ((x + y) % 2 == 0) {
				color = 0xFF2020FF;
			}
			mangoc_fill_rect(pixels, WIDTH, HEIGHT, x * CELL_WIDTH, y * CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT, color);
		}
	}



	const char* file_path = "checker_example.ppm";
	char err_buf[256];
	Errno err = mangoc_save_to_ppm_file(pixels, WIDTH, HEIGHT, file_path);
	if (err) {
		strerror_s(err_buf, sizeof(err_buf), err);
		fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path, err_buf);
		return 1;
	}
	return 0;

}