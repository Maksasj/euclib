#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "stb_image_write.h"

#define EUCLIB_IMPLEMENTATION
#include "euclib.h"

int main() {
    euclib_plot_t plot = {
        .value = malloc(800 * 600 * sizeof(color_t)),
        .width = 800,
        .height = 600,
    };

    euclib_plot_line_params_t params = {
        .x_range = { -10.0f, 10.0f },
        .padding = 10.f,
        .color = EUCLIB_BLUE
    };

    euclib_plot_2d_line(&plot, &sinf, params);

    stbi_write_jpg("plot.png", 800, 600, 4, plot.value, 800);

    return 0;
}