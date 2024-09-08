#ifndef EUCLIB_H
#define EUCLIB_H

#define EUCLIB_INLINE static inline

typedef unsigned int color_t;

#define EUCLIB_WHITE    ((color_t)(0xffffffff))
#define EUCLIB_BLACK    ((color_t)(0xff000000))
#define EUCLIB_RED      ((color_t)(0xff3440eb))
#define EUCLIB_GREEN    ((color_t)(0xff34eb40))
#define EUCLIB_BLUE     ((color_t)(0xffeb3440))
#define EUCLIB_PURPLE   ((color_t)(0xffeb34eb))
#define EUCLIB_YELLOW   ((color_t)(0xff34ebeb))
#define EUCLIB_ORANGE   ((color_t)(0xff3486eb))

#define EUCLIB_ACCENT   ((color_t)(0xffeeeeee))

#define EUCLIB_PLOT_AT(PLOT, X, Y) ((PLOT)->value[X + (((PLOT)->height - 1) - Y) * (PLOT)->width])

typedef float (*graph_value_callback_t)(const float x);

typedef struct {
    float x;
    float y;
} vec2f_t;

typedef struct {
    int x;
    int y;
} vec2i_t;

typedef struct {
    float x;
    float y;
    float z;
} vec3f_t;

typedef struct
{
    color_t *value;
    unsigned long long width;
    unsigned long long height;
} euclib_plot_t;

EUCLIB_INLINE void euclib_fill_rect(euclib_plot_t *plot, vec2i_t left_bottom, vec2i_t right_top, color_t color);
EUCLIB_INLINE void euclib_fill_plot(euclib_plot_t *plot, color_t color);

typedef struct
{

} euclib_plot_generic_params_t;

typedef struct
{

} euclib_plot_bar_params_t;

EUCLIB_INLINE void euclib_plot_2d_bar(
    euclib_plot_t *plot, 
    float values[], 
    unsigned int count, 
    euclib_plot_bar_params_t params);

typedef struct
{

} euclib_plot_dots_params_t;

EUCLIB_INLINE void euclib_plot_2d_dots(
    euclib_plot_t *plot, 
    float values[], 
    unsigned int count, 
    euclib_plot_dots_params_t params);

typedef struct
{
    vec2f_t x_range;
    unsigned int padding;
    color_t color;
} euclib_plot_line_params_t;

EUCLIB_INLINE void euclib_plot_2d_line(
    euclib_plot_t *plot, 
    graph_value_callback_t callback, 
    euclib_plot_line_params_t params);

#ifdef EUCLIB_IMPLEMENTATION

EUCLIB_INLINE void euclib_fill_rect(
    euclib_plot_t *plot, 
    vec2i_t left_bottom, 
    vec2i_t right_top, 
    color_t color
) {
    int width = right_top.x - left_bottom.x;
    int height = right_top.y - left_bottom.y;

    for(int i = 0; i < width; ++i) {
        for(int j = 0; j < height; ++j) {
            int x = i + left_bottom.x;
            int y = j + left_bottom.y; // Y flipping

            EUCLIB_PLOT_AT(plot, x, y) = color;
        }
    }
}

EUCLIB_INLINE void euclib_fill_plot(euclib_plot_t *plot, color_t color) {
    for(int x = 0; x < plot->width; ++x) {
        for(int y = 0; y < plot->height; ++y) {
            EUCLIB_PLOT_AT(plot, x, y) = color;
        }
    }
}

EUCLIB_INLINE void euclib_plot_2d_bar(
    euclib_plot_t *plot, 
    float values[], 
    unsigned int count, 
    euclib_plot_bar_params_t params
) {
    // Todo
}

EUCLIB_INLINE void euclib_plot_2d_dots(
    euclib_plot_t *plot, 
    float values[], 
    unsigned int count, 
    euclib_plot_dots_params_t params
) {
    // Todo
}

EUCLIB_INLINE void euclib_draw_circle(
    euclib_plot_t *plot, 
    vec2i_t pos, 
    int radius, 
    color_t color
) {
    int diameter = radius + radius;
    int r2 = radius * radius;

    for(int x = -radius; x < radius; ++x) {
        for(int y = -radius; y < radius; ++y) {
            int plot_x = x + pos.x;
            int plot_y = y + pos.y;

            if(x * x + y * y > r2)
                continue;

            EUCLIB_PLOT_AT(plot, plot_x, plot_y) = color;
        }
    }
}

EUCLIB_INLINE void euclib_plot_2d_line(
    euclib_plot_t *plot, 
    graph_value_callback_t callback, 
    euclib_plot_line_params_t params
) {
    euclib_fill_plot(plot, EUCLIB_ACCENT);

    int width = plot->width;
    int height = plot->height;

    euclib_fill_rect(
        plot,
        (vec2i_t){params.padding, params.padding},
        (vec2i_t){width - params.padding, height - params.padding},
        EUCLIB_WHITE);

 
    int area_width = width - params.padding;
    int area_height = height - params.padding;

    for(int i = params.padding; i < area_width; ++i) {
        float d = (float) i / (float) area_width;

        float x = (d * (params.x_range.y - params.x_range.x));

        float value = callback(x);

        euclib_draw_circle(
            plot, 
            (vec2i_t){i, ((int) ((height / 2) - params.padding + value * 100) + params.padding)}, 
            3, 
            params.color);
    }
}

#endif

#endif