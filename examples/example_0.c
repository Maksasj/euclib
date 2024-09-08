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

    euclib_fill_rect(&plot, (vec2i_t){10, 20}, (vec2i_t){50, 50}, EUCLIB_GREEN);

    euclib_draw_circle(&plot, (vec2i_t){50, 50}, 20, EUCLIB_ORANGE);

    stbi_write_jpg("plot.png", 800, 600, 4, plot.value, 800);

    return 0;
}