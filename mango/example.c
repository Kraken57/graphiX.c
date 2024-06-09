#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include "mango.h"



#define WIDTH 800
#define HEIGHT 600

#define COLS (8*2)
#define ROWS (6*2)
#define CELL_WIDTH (WIDTH/COLS)
#define CELL_HEIGHT (HEIGHT/ROWS)

#define BACKGROUND_COLOR 0xFF202020
#define FOREGROUND_COLOR 0xFF2020FF

static uint32_t pixels[HEIGHT*WIDTH];

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


bool checker_example(void)
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



	const char* file_path = "checker.ppm";
	char err_buf[256];
	Errno err = mangoc_save_to_ppm_file(pixels, WIDTH, HEIGHT, file_path);
	if (err) {
		strerror_s(err_buf, sizeof(err_buf), err);
		fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path, err_buf);
		return false;
	}
	return true;
}

float lerpf(float a, float b, float t)
{
	return a + (b - a) * t;
}

bool circle_example(void)
{
	mangoc_fill(pixels, WIDTH, HEIGHT, BACKGROUND_COLOR);

	for (int y = 0; y < ROWS; ++y) {
		for (int x = 0; x < COLS; ++x) {
			float u = (float)x / COLS;
			float v = (float)y / ROWS;
			float t = (u + v) / 2;

			size_t radius = CELL_WIDTH;
			if (CELL_HEIGHT < radius) radius = CELL_HEIGHT;

			mangoc_fill_circle(pixels, WIDTH, HEIGHT,
				x * CELL_WIDTH + CELL_WIDTH / 2, y * CELL_HEIGHT + CELL_HEIGHT / 2,
				(size_t)lerpf((float)radius / 8, (float)radius / 2, t),
				FOREGROUND_COLOR);
		}
	}

	const char* file_path = "circle.ppm";
	char err_buf[256];
	Errno err = mangoc_save_to_ppm_file(pixels, WIDTH, HEIGHT, file_path);
	if (err) {
		strerror_s(err_buf, sizeof(err_buf), err);
		fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path, err_buf);
		return false;
	}
	return true;
	return 0;
}

bool lines_example(void)
{
	mangoc_fill(pixels, WIDTH, HEIGHT, BACKGROUND_COLOR);

	mangoc_draw_line(pixels, WIDTH, HEIGHT,
		0, 0, WIDTH, HEIGHT,
		FOREGROUND_COLOR);

	mangoc_draw_line(pixels, WIDTH, HEIGHT,
		WIDTH, 0, 0, HEIGHT,
		FOREGROUND_COLOR);

	mangoc_draw_line(pixels, WIDTH, HEIGHT,
		0, 0, WIDTH / 4, HEIGHT,
		0xFF20FF20);

	mangoc_draw_line(pixels, WIDTH, HEIGHT,
		WIDTH / 4, 0, 0, HEIGHT,
		0xFF20FF20);

	mangoc_draw_line(pixels, WIDTH, HEIGHT,
		WIDTH / 2, 0, WIDTH, HEIGHT,
		0xFF20FF20);

	mangoc_draw_line(pixels, WIDTH, HEIGHT,
		WIDTH, 0, WIDTH / 4 * 3, HEIGHT,
		0xFF20FF20);

	mangoc_draw_line(pixels, WIDTH, HEIGHT,
		WIDTH / 4 * 3, 0, WIDTH, HEIGHT,
		0xFF20FF20);

	mangoc_draw_line(pixels, WIDTH, HEIGHT,
		0, HEIGHT / 2, WIDTH, HEIGHT / 2,
		0xFFFF3030);

	mangoc_draw_line(pixels, WIDTH, HEIGHT,
		WIDTH / 2, 0, WIDTH / 2, HEIGHT,
		0xFFFF3030);


	//New pattern
	//mangoc_draw_line(pixels, WIDTH, HEIGHT,
	//	0, 0, WIDTH, HEIGHT,
	//	0xFFFFFFFF);  // Diagonal solid line

	//mangoc_draw_line(pixels, WIDTH, HEIGHT,
	//	WIDTH, 0, 0, HEIGHT,
	//	0xFF00FFFF);  // Another diagonal solid line

	//mangoc_draw_line(pixels, WIDTH, HEIGHT,
	//	0, HEIGHT / 4, WIDTH, HEIGHT / 4,
	//	0xFF20FF20);  // Horizontal dashed line

	//mangoc_draw_line(pixels, WIDTH, HEIGHT,
	//	WIDTH / 4, 0, WIDTH / 4, HEIGHT,
	//	0xFFFF20FF);  // Vertical dashed line

	//mangoc_draw_line(pixels, WIDTH, HEIGHT,
	//	WIDTH / 2, 0, WIDTH, HEIGHT / 2,
	//	0xFFFFA500);  // Diagonal dotted line

	//mangoc_draw_line(pixels, WIDTH, HEIGHT,
	//	0, HEIGHT / 2, WIDTH, HEIGHT,
	//	0xFF20FFFF);  // Diagonal zigzag line

	//mangoc_draw_line(pixels, WIDTH, HEIGHT,
	//	WIDTH / 4, HEIGHT / 2, WIDTH / 4 * 3, HEIGHT / 2,
	//	0xFFFF3030);  // Horizontal solid line

	//mangoc_draw_line(pixels, WIDTH, HEIGHT,
	//	WIDTH / 2, HEIGHT / 4, WIDTH / 2, HEIGHT / 4 * 3,
	//	0xFF3030FF);  // Vertical solid line


	const char* file_path = "lines.ppm";
	char err_buf[256];
	Errno err = mangoc_save_to_ppm_file(pixels, WIDTH, HEIGHT, file_path);
	if (err) {
		strerror_s(err_buf, sizeof(err_buf), err);
		fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path, err_buf);
		return false;
	}

	return true;

}

int main(void)
{
	if (!checker_example()) return -1;
	if (!circle_example()) return -1;
	if (!lines_example()) return -1;

	return 0;

}