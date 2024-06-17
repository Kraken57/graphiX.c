#pragma once

#ifndef MANGO_H_
#define MANGO_H_

#include <stdint.h>
#include <stddef.h>

void mangoc_fill(uint32_t* pixels, size_t width, size_t height, uint32_t color);
void mangoc_fill_rect(uint32_t* pixels, size_t pixels_width, size_t pixels_height,
    int x0, int y0, size_t w, size_t h, uint32_t color);
void mangoc_fill_circle(uint32_t* pixels, size_t pixels_width, size_t pixels_height,
    int cx, int cy, int r, uint32_t color);

//for triangle
void mangoc_fill_triangle(uint32_t* pixels, size_t pixels_width, size_t pixels_height,
    int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);

//for polygon
void mangoc_fill_polygon(uint32_t* pixels, size_t pixels_width, size_t pixels_height,
    const int* xs, const int* ys, size_t n, uint32_t color);

// For sprite rendering
void mangoc_draw_sprite(uint32_t* pixels, size_t pixels_width, size_t pixels_height,
    const uint32_t* sprite, size_t sprite_width, size_t sprite_height, int x, int y, float scale);


typedef int Errno;
Errno mangoc_save_to_ppm_file(uint32_t* pixels, size_t width, size_t height, const char* file_path);



#endif // MANGO_H_
