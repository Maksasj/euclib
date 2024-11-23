#ifndef EUCLIB_H
#define EUCLIB_H

#ifndef EUCLIB_STRLEN
    #include <string.h>

    #define EUCLIB_STRLEN strlen
#endif

#ifndef EUCLIB_SQRT
    #include <math.h>

    #define EUCLIB_SQRT sqrt
#endif

#ifndef EUCLIB_RAND
    #include <stdlib.h>

    #define EUCLIB_RAND rand
#endif

#ifndef EUCLIB_SPRINTF
    #include <stdio.h>

    #define EUCLIB_SPRINTF sprintf
#endif

#define EUCLIB_INLINE static inline

#include "ibm_bios_font.h"

typedef unsigned int color_t;

#define EUCLIB_WHITE    ((color_t)(0xffffffff))
#define EUCLIB_BLACK    ((color_t)(0xff000000))
#define EUCLIB_RED      ((color_t)(0xff3440eb))
#define EUCLIB_GREEN    ((color_t)(0xff34eb40))
#define EUCLIB_BLUE     ((color_t)(0xffeb3440))
#define EUCLIB_PURPLE   ((color_t)(0xffeb34eb))
#define EUCLIB_YELLOW   ((color_t)(0xff34ebeb))
#define EUCLIB_ORANGE   ((color_t)(0xff3486eb))

#define EUCLIB_ACCENT   ((color_t)(0xffaaaaaa))

#define EUCLIB_PI       ((float)(3.14159265359f))

#define EUCLIB_PLOT_AT(PLOT, X, Y) ((PLOT)->value[X + (((PLOT)->height - 1) - Y) * (PLOT)->width])

typedef float (*graph_value_callback_t)(const float x);

typedef struct {
    float x;
    float y;
} vec2f_t;

typedef struct {
    float x;
    float y;
    float z;
} vec3f_t;

typedef struct {
    int x;
    int y;
} vec2i_t;

typedef struct
{
    color_t *value;
    unsigned long long width;
    unsigned long long height;
} euclib_plot_t;

typedef unsigned char euclib_bool_t;
#define EUCLIB_TRUE     ((euclib_bool_t) 1)
#define EUCLIB_FALSE    ((euclib_bool_t) 0)

EUCLIB_INLINE color_t euclib_random_color();

EUCLIB_INLINE euclib_bool_t euclib_in_bounds(euclib_plot_t *plot, vec2i_t point);
EUCLIB_INLINE void euclib_swap(int *a, int *b);
EUCLIB_INLINE int euclib_clip(int min, int max, int value);
EUCLIB_INLINE float euclib_distance(vec2f_t first, vec2f_t second);
EUCLIB_INLINE float euclib_length_v2(vec2f_t vec);
EUCLIB_INLINE vec2f_t euclib_normalize(vec2f_t vec);
EUCLIB_INLINE vec2i_t euclib_to_plot_cord(euclib_plot_t* plot, vec2f_t cord, vec2f_t x_range, vec2f_t y_range);

EUCLIB_INLINE void euclib_fill_rect(euclib_plot_t *plot, vec2i_t left_bottom, vec2i_t right_top, color_t color);
EUCLIB_INLINE void euclib_fill_plot(euclib_plot_t *plot, color_t color);
 
// Code from https://github.com/tsoding/olive.c/blob/master/olive.c
EUCLIB_INLINE void euclib_draw_line(euclib_plot_t *plot, vec2i_t first, vec2i_t second, color_t color);
EUCLIB_INLINE void euclib_draw_line_width(euclib_plot_t *plot, vec2i_t first, vec2i_t second, float line_width, color_t color);

EUCLIB_INLINE void euclib_draw_circle(euclib_plot_t *plot, vec2i_t pos, float radius, color_t color);
EUCLIB_INLINE void euclib_draw_text(euclib_plot_t *plot, vec2i_t pos, const char* text, color_t color, unsigned int font_size);

typedef struct
{
    vec2f_t x_range;
    vec2f_t y_range;
    color_t line_color;
    int line_width;
    float dashed_spacing;
} euclib_plot_line_params_t;

typedef struct
{
    vec2f_t x_range;
    vec2f_t y_range;
    color_t figure_color;
} euclib_plot_figure_params_t;

typedef struct
{
    vec2f_t x_range;
    vec2f_t y_range;
    color_t font_color;
    unsigned int font_size;
} euclib_plot_text_params_t;

EUCLIB_INLINE void euclib_plot_2d_line(
    euclib_plot_t *plot, 
    graph_value_callback_t callback, 
    euclib_plot_line_params_t params);

EUCLIB_INLINE void euclib_plot_2d_line_smooth(
    euclib_plot_t *plot, 
    graph_value_callback_t callback, 
    euclib_plot_line_params_t params);

EUCLIB_INLINE void euclib_plot_circle(
    euclib_plot_t *plot, 
    vec2f_t center,
    float radius,
    euclib_plot_figure_params_t params);

EUCLIB_INLINE void euclib_plot_line(euclib_plot_t *plot, euclib_plot_line_params_t params, vec2f_t start, vec2f_t end);
EUCLIB_INLINE void euclib_plot_line_dashed(euclib_plot_t *plot, euclib_plot_line_params_t params, vec2f_t start, vec2f_t end);

EUCLIB_INLINE void euclib_plot_x_axis(euclib_plot_t *plot, euclib_plot_line_params_t params);
EUCLIB_INLINE void euclib_plot_y_axis(euclib_plot_t *plot, euclib_plot_line_params_t params);

EUCLIB_INLINE void euclib_plot_axis(euclib_plot_t *plot, euclib_plot_line_params_t params);

EUCLIB_INLINE void euclib_plot_grid_horizontal(euclib_plot_t *plot, vec2f_t center, float step, euclib_plot_line_params_t params);
EUCLIB_INLINE void euclib_plot_grid_vertical(euclib_plot_t *plot,  vec2f_t center, float step, euclib_plot_line_params_t params);
EUCLIB_INLINE void euclib_plot_grid(euclib_plot_t *plot, float step, euclib_plot_line_params_t params);

EUCLIB_INLINE void euclib_plot_text(euclib_plot_t *plot, vec2f_t pos, const char* text, euclib_plot_text_params_t params);
EUCLIB_INLINE void euclib_plot_axis_text(euclib_plot_t *plot, float step, euclib_plot_text_params_t params);

#ifdef EUCLIB_IMPLEMENTATION

EUCLIB_INLINE color_t euclib_random_color() {
    return EUCLIB_RAND() | 0xff000000;
}

EUCLIB_INLINE euclib_bool_t euclib_in_bounds(euclib_plot_t *plot, vec2i_t point) {
    if(point.x < 0 || point.y < 0)
        return EUCLIB_FALSE;

    if(point.x >= plot->width || point.y >= plot->height)
        return EUCLIB_FALSE;

    return EUCLIB_TRUE;
}

EUCLIB_INLINE void euclib_swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

EUCLIB_INLINE void euclib_swap_vec2f(vec2f_t *a, vec2f_t *b) {
    vec2f_t temp = *a;
    *a = *b;
    *b = temp;
}

EUCLIB_INLINE void euclib_swap_float(float *a, float *b) {
    float temp = *a;
    *a = *b;
    *b = temp;
}

EUCLIB_INLINE int euclib_clip(int min, int max, int value) {
    if(value < min) value = min;
    if(value > max) value = max;

    return value;
}

EUCLIB_INLINE float euclib_distance(vec2f_t first, vec2f_t second) {
    const float a = second.x - first.x;
    const float b = second.y - first.y;

    return EUCLIB_SQRT((a*a)+(b*b));
}

EUCLIB_INLINE float euclib_length_v2(vec2f_t vec) {
    return EUCLIB_SQRT(vec.x * vec.x + vec.y * vec.y);
}

EUCLIB_INLINE float euclib_length_v3(vec3f_t vec) {
    return EUCLIB_SQRT(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

EUCLIB_INLINE vec2f_t euclib_normalize(vec2f_t vec) {
    const float length = euclib_length_v2(vec);

    return (vec2f_t) {
        vec.x / length,
        vec.y / length
    };
}

EUCLIB_INLINE vec2i_t euclib_to_plot_cord(euclib_plot_t* plot, vec2f_t cord, vec2f_t x_range, vec2f_t y_range) {
    float tmpx = (cord.x - x_range.x) / (x_range.y - x_range.x);
    float i = tmpx * plot->width;

    float tmpy = (cord.y - y_range.x) / (y_range.y - y_range.x);
    float j = tmpy * plot->height;

    return (vec2i_t) { i, j };
}
    
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
            int y = j + left_bottom.y;

            if(!euclib_in_bounds(plot, (vec2i_t){x, y})) 
                    continue;

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

EUCLIB_INLINE void euclib_draw_line(euclib_plot_t *plot, vec2i_t first, vec2i_t second, color_t color) {
    vec2i_t d = (vec2i_t) { 
        second.x - first.x, 
        second.y - first.y
    };

    if (d.x == 0 && d.y == 0) {
        if (euclib_in_bounds(plot, (vec2i_t){ first.x, first.y })) {
            EUCLIB_PLOT_AT(plot, first.x, first.y) = color;
        }

        return;
    }

    if (abs(d.x) > abs(d.y)) {
        if (first.x > second.x) {
            euclib_swap(&first.x, &second.x);
            euclib_swap(&first.y, &second.y);
        }

        for (int x = first.x; x <= second.x; ++x) {
            int y = d.y*(x - first.x)/d.x + first.y;

            if (euclib_in_bounds(plot, (vec2i_t){ x, y })) {
                EUCLIB_PLOT_AT(plot, x, y) = color;
            }
        }
    } else {
        if (first.y > second.y) {
            euclib_swap(&first.x, &second.x);
            euclib_swap(&first.y, &second.y);
        }

        for (int y = first.y; y <= second.y; ++y) {
            int x = d.x*(y - first.y)/d.y + first.x;

            if (euclib_in_bounds(plot, (vec2i_t){ x, y })) {
                EUCLIB_PLOT_AT(plot, x, y) = color;
            }
        }
    }
}

EUCLIB_INLINE void euclib_draw_line_width(euclib_plot_t *plot, vec2i_t first, vec2i_t second, float line_width, color_t color) {
    first.x = euclib_clip(0, plot->width - 1, first.x);
    first.y = euclib_clip(0, plot->height - 1, first.y);
    
    second.x = euclib_clip(0, plot->width - 1, second.x);
    second.y = euclib_clip(0, plot->height - 1, second.y);

    vec2i_t d = (vec2i_t) { 
        second.x - first.x, 
        second.y - first.y
    };

    if (d.x == 0 && d.y == 0) {
        euclib_draw_circle(plot, (vec2i_t){ first.x, first.y }, line_width, color);
        return;
    }

    if (abs(d.x) > abs(d.y)) {
        if (first.x > second.x) {
            euclib_swap(&first.x, &second.x);
            euclib_swap(&first.y, &second.y);
        }

        for (int x = first.x; x <= second.x; ++x) {
            int y = d.y*(x - first.x)/d.x + first.y;

            euclib_draw_circle(plot, (vec2i_t){ x, y }, line_width, color);
        }
    } else {
        if (first.y > second.y) {
            euclib_swap(&first.x, &second.x);
            euclib_swap(&first.y, &second.y);
        }

        for (int y = first.y; y <= second.y; ++y) {
            int x = d.x*(y - first.y)/d.y + first.x;

            euclib_draw_circle(plot, (vec2i_t){ x, y }, line_width, color);
        }
    }
}

EUCLIB_INLINE void euclib_draw_circle(
    euclib_plot_t *plot, 
    vec2i_t pos, 
    float radius, 
    color_t color
) {
    float r2 = radius * radius;

    // Todo
    if(radius < 1) {
        EUCLIB_PLOT_AT(plot, pos.x, pos.y) = color;
        return;
    }

    for(int x = -radius; x < radius; ++x) {
        for(int y = -radius; y < radius; ++y) {
            int plot_x = x + pos.x;
            int plot_y = y + pos.y;

            if(!euclib_in_bounds(plot, (vec2i_t){plot_x, plot_y})) 
                continue;

            if(x * x + y * y > r2)
                continue;

            EUCLIB_PLOT_AT(plot, plot_x, plot_y) = color;
        }
    }
}

EUCLIB_INLINE void euclib_draw_text(
    euclib_plot_t *plot, 
    vec2i_t pos, 
    const char* text,
    color_t color,
    unsigned int font_size
) {
    unsigned long text_length = EUCLIB_STRLEN(text);

    for(int c = 0; c < text_length; ++c) {
        unsigned char character = text[c];
        FontgGlyph glyph = ibm_bios_font[character];

        for(int i = 0; i < 8; ++i) {
            for(int j = 0; j < 8; ++j) {
                int x = pos.x + i * font_size + (c  * font_size * (8 + 1));
                int y = pos.y + j * font_size;

                if(!euclib_in_bounds(plot, (vec2i_t){ x, y })) 
                    continue;

                static unsigned long long start = 1u;


                if((glyph & (start << ((8 - i) + j*8))) != 0) {
                    EUCLIB_PLOT_AT(plot, x, y) = color;
                    euclib_draw_circle(plot, (vec2i_t){ x, y, }, font_size, color);
                }
            }
        }
    }
}

EUCLIB_INLINE void euclib_plot_2d_line(
    euclib_plot_t *plot, 
    graph_value_callback_t callback, 
    euclib_plot_line_params_t params
) {
    const int width = plot->width;
    const int height = plot->height;

    for(int i = 0; i < width; ++i) {
        float d = (float) i / (float) width;

        // point coordinates
        float x = (d * (params.x_range.y - params.x_range.x)) + params.x_range.x;
        float y = callback(x);
  
        float tmp = (y - params.y_range.x) / (params.y_range.y - params.y_range.x);
        float j = tmp * plot->height;

        vec2i_t point = { i, j };

        if(!euclib_in_bounds(plot, point)) 
            continue;

        euclib_draw_circle(plot, point, params.line_width, params.line_color);
    }
}

EUCLIB_INLINE void euclib_plot_2d_line_smooth(
    euclib_plot_t *plot, 
    graph_value_callback_t callback, 
    euclib_plot_line_params_t params
) {
    const int width = plot->width;
    const int height = plot->height;

    vec2i_t last_point = { -100, -100 }; 

    for(int i = 0; i < width; ++i) {
        float d = (float) i / (float) width;

        // point coordinates
        float x = (d * (params.x_range.y - params.x_range.x)) + params.x_range.x;
        float y = callback(x);
  
        float tmp = (y - params.y_range.x) / (params.y_range.y - params.y_range.x);
        float j = tmp * plot->height;

        vec2i_t point = { i, j };

        if(!euclib_in_bounds(plot, point))
            continue;

        if(last_point.x < 0 || last_point.y < 0) {
            last_point = point;
            continue;
        }

        euclib_draw_line_width(plot, last_point, point, params.line_width, params.line_color);
        last_point = point;
    }
}

EUCLIB_INLINE void euclib_plot_circle(
    euclib_plot_t *plot, 
    vec2f_t center,
    float radius,
    euclib_plot_figure_params_t params
) {
    vec2i_t point = euclib_to_plot_cord(plot, center, params.x_range, params.y_range);

    euclib_draw_circle(plot, point, radius, params.figure_color);
}

EUCLIB_INLINE void euclib_plot_line(euclib_plot_t *plot, euclib_plot_line_params_t params, vec2f_t start, vec2f_t end) {
    vec2i_t first_point = euclib_to_plot_cord(plot, start, params.x_range, params.y_range);
    vec2i_t second_point = euclib_to_plot_cord(plot, end, params.x_range, params.y_range);

    euclib_draw_line_width(plot, first_point, second_point, params.line_width, params.line_color);
}

EUCLIB_INLINE void euclib_plot_line_dashed(euclib_plot_t *plot, euclib_plot_line_params_t params, vec2f_t start, vec2f_t end) {
    float distance = euclib_distance(start, end);

    vec2f_t direction = euclib_normalize((vec2f_t) { end.x - start.x, end.y - start.y });
    vec2f_t point = start;

    for(float d = 0; d < (distance / 2.0f); d += (params.dashed_spacing)) {
        vec2f_t next = (vec2f_t) {
            point.x + direction.x * params.dashed_spacing,
            point.y + direction.y * params.dashed_spacing
        };

        euclib_plot_line(plot, params, point, next);

        next.x += direction.x * params.dashed_spacing;
        next.y += direction.y * params.dashed_spacing;

        point = next;
    }
}

EUCLIB_INLINE void euclib_plot_x_axis(
    euclib_plot_t *plot, 
    euclib_plot_line_params_t params
) {
    euclib_plot_line(plot, params, (vec2f_t){ params.x_range.x, 0.0f }, (vec2f_t){ params.x_range.y, 0.0f });
}

EUCLIB_INLINE void euclib_plot_y_axis(
    euclib_plot_t *plot, 
    euclib_plot_line_params_t params
) {
    euclib_plot_line(plot, params, (vec2f_t){ 0.0f, params.y_range.x }, (vec2f_t){ 0.0f, params.y_range.y });
}

EUCLIB_INLINE void euclib_plot_axis(
    euclib_plot_t *plot, 
    euclib_plot_line_params_t params
) {
    euclib_plot_x_axis(plot, params);
    euclib_plot_y_axis(plot, params);
}

EUCLIB_INLINE void euclib_plot_grid_horizontal(
    euclib_plot_t *plot, 
    vec2f_t center,
    float step,
    euclib_plot_line_params_t params
) {
    for(float y = center.y; y <= params.y_range.y; y += step)
        euclib_plot_line(plot, params, (vec2f_t){ params.x_range.x, y }, (vec2f_t){ params.x_range.y, y });
}

EUCLIB_INLINE void euclib_plot_grid_vertical(
    euclib_plot_t *plot, 
    vec2f_t center,
    float step,
    euclib_plot_line_params_t params
) {
    for(float x = center.x; x <= params.x_range.y; x += step)
        euclib_plot_line(plot, params, (vec2f_t){ x, params.y_range.x }, (vec2f_t){ x, params.y_range.y });
}

EUCLIB_INLINE void euclib_plot_grid(
    euclib_plot_t *plot, 
    float step,
    euclib_plot_line_params_t params
) {
    vec2f_t center = { 
        ((int) (params.x_range.x / step))*step, 
        ((int) (params.y_range.x / step))*step,
    };

    euclib_plot_grid_horizontal(plot, center, step, params);
    euclib_plot_grid_vertical(plot, center, step, params);
}

EUCLIB_INLINE void euclib_plot_text(euclib_plot_t *plot, vec2f_t pos, const char* text, euclib_plot_text_params_t params) {
    vec2i_t point = euclib_to_plot_cord(plot, pos, params.x_range, params.y_range);

    euclib_draw_text(plot, point, text, params.font_color, params.font_size);
}

EUCLIB_INLINE void euclib_plot_axis_text(euclib_plot_t *plot, float step, euclib_plot_text_params_t params) {
    vec2f_t center = { 
        ((int) (params.x_range.x / step))*step, 
        ((int) (params.y_range.x / step))*step,
    };

    for(float y = center.y; y <= params.y_range.y; y += step) {
        char buffer[100] = { '\0' };
        sprintf(buffer, "%.1f", y);
        euclib_plot_text(plot, (vec2f_t){ 0.0f  + 0.05f, y  + 0.1f}, buffer, params);
    }

    for(float x = center.x; x <= params.x_range.y; x += step) {
        char buffer[100] = { '\0' };
        sprintf(buffer, "%.1f", x);
        euclib_plot_text(plot, (vec2f_t){ x + 0.05f, 0.1f }, buffer, params);
    }
}

#endif

#endif