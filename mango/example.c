#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <math.h>
#include <stdlib.h> 
#include "mango.h"



#define WIDTH 800
#define HEIGHT 600

#define COLS (8*2)
#define ROWS (6*2)
#define CELL_WIDTH (WIDTH/COLS)
#define CELL_HEIGHT (HEIGHT/ROWS)

#define BACKGROUND_COLOR 0xFF202020
#define FOREGROUND_COLOR 0xFF2020FF


//static uint32_t pixels[HEIGHT * WIDTH];



static uint32_t pixels[WIDTH * HEIGHT] = { 0 };

bool save_image(const char* file_path)
{
	char err_buf[256];
	Errno err = mangoc_save_to_ppm_file(pixels, WIDTH, HEIGHT, file_path);
	if (err) {
		strerror_s(err_buf, sizeof(err_buf), err);
		fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path, err_buf);
		return false;
	}
	return true;
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

bool triangle_example(void)
{
    mangoc_fill(pixels, WIDTH, HEIGHT, BACKGROUND_COLOR);

    mangoc_fill_triangle(pixels, WIDTH, HEIGHT,
        WIDTH / 2, HEIGHT / 4,  // Vertex 1
        WIDTH / 4, HEIGHT / 2,  // Vertex 2
        WIDTH / 2, HEIGHT / 2,  // Vertex 3
        FOREGROUND_COLOR);

    mangoc_fill_triangle(pixels, WIDTH, HEIGHT,
        WIDTH / 2, HEIGHT / 2,  // Vertex 1
        WIDTH / 4 * 3, HEIGHT / 2,  // Vertex 2
        WIDTH / 2, HEIGHT / 4 * 3,  // Vertex 3
        0xFF20FF20);

    const char* file_path = "triangle.ppm";

    char err_buf[256];
    Errno err = mangoc_save_to_ppm_file(pixels, WIDTH, HEIGHT, file_path);
    if (err) {
        strerror_s(err_buf, sizeof(err_buf), err);
        fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path, err_buf);
        return false;
    }
    return true;
}


bool polygon_example(void)
{
	mangoc_fill(pixels, WIDTH, HEIGHT, 0xFF202020); // Fill canvas with background color

	int sides;
	printf("Enter the number of sides for the polygon: ");
	if (scanf_s("%d", &sides) != 1 || sides < 3) {
		fprintf(stderr, "ERROR: Invalid number of sides\n");
		return false;
	}

	// Dynamically allocate arrays for vertices
	int* xs = (int*)malloc(sides * sizeof(int));
	int* ys = (int*)malloc(sides * sizeof(int));

	if (!xs || !ys) {
		fprintf(stderr, "ERROR: Memory allocation failed\n");
		if (xs) free(xs);
		if (ys) free(ys);
		return false;
	}

	int radius = HEIGHT / 3;
	int cx = WIDTH / 2;
	int cy = HEIGHT / 2;

	for (int i = 0; i < sides; ++i) {
		float angle = 2.0f * M_PI * i / sides;
		xs[i] = cx + radius * cosf(angle);
		ys[i] = cy + radius * sinf(angle);
	}

	mangoc_fill_polygon(pixels, WIDTH, HEIGHT, xs, ys, sides, 0xFF2020FF); // Fill polygon with foreground color

	free(xs);
	free(ys);

	return save_image("polygon.ppm");
}


int main(void)
{
	if (!checker_example()) return -1;
	if (!circle_example()) return -1;
	if (!lines_example()) return -1;
	if (!triangle_example()) return -1;
	if (!polygon_example()) return -1;

	return 0;

}