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
        .x_range = { -3 * EUCLIB_PI, 3 * EUCLIB_PI },
        .y_range = { -4.0 , 4.0 },
        .line_color = EUCLIB_BLUE,
        .line_width = 2
    };

    euclib_fill_plot(&plot, EUCLIB_WHITE);

    euclib_plot_2d_line(&plot, &sinf, params);
    
    params.line_color = EUCLIB_RED;
    euclib_plot_2d_line(&plot, &cosf, params);

    params.line_color = EUCLIB_PURPLE;
    euclib_plot_2d_line(&plot, &tanf, params);

    euclib_draw_text(&plot, (vec2i_t){ 300, 300 }, "Hello world!", EUCLIB_BLACK);

    euclib_draw_line_width(&plot, (vec2i_t){ 0, 0 }, (vec2i_t){ 800, 600 }, 10, EUCLIB_BLACK);

    stbi_write_jpg("plot.png", 800, 600, 4, plot.value, 800);

    free(plot.value);

    return 0;
}