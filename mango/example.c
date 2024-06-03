#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include "mango.c"



#define WIDTH 800
#define HEIGHT 600

void mangoc_fill(uint32_t* pixels, size_t width, size_t height, uint32_t color)
{
	for (size_t i = 0; i < width * height; ++i)
	{
		pixels[i] = color;
	}
}

typedef int Errno;

#define return_defer(value) do{ result = (value); goto defer;} while(0)

Errno mangoc_save_to_ppm_file(uint32_t *pixels,size_t width, size_t height, const char *file_path)
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

static uint32_t pixels[HEIGHT*WIDTH];

int main(void)
{
	mangoc_fill(pixels, WIDTH, HEIGHT, 0xFF00FFFF);
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