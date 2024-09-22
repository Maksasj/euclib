#ifndef EUCLIB_H
#define EUCLIB_H

#ifndef EUCLIB_STRLEN
    #include <string.h>

    #define EUCLIB_STRLEN(STR) (strlen(STR))
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

#define EUCLIB_ACCENT   ((color_t)(0xffeeeeee))

#define EUCLIB_PI       ((float)(3.14159265359f))

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

typedef struct
{
    color_t *value;
    unsigned long long width;
    unsigned long long height;
} euclib_plot_t;

typedef unsigned char euclib_bool_t;
#define EUCLIB_TRUE     ((euclib_bool_t) 1)
#define EUCLIB_FALSE    ((euclib_bool_t) 0)

EUCLIB_INLINE euclib_bool_t euclib_in_bounds(euclib_plot_t *plot, vec2i_t point);
EUCLIB_INLINE void euclib_swap(int *a, int *b);

EUCLIB_INLINE void euclib_fill_rect(euclib_plot_t *plot, vec2i_t left_bottom, vec2i_t right_top, color_t color);
EUCLIB_INLINE void euclib_fill_plot(euclib_plot_t *plot, color_t color);
 
// Code from https://github.com/tsoding/olive.c/blob/master/olive.c
EUCLIB_INLINE void euclib_draw_line(euclib_plot_t *plot, vec2i_t first, vec2i_t second, color_t color);
EUCLIB_INLINE void euclib_draw_line_width(euclib_plot_t *plot, vec2i_t first, vec2i_t second, float line_width, color_t color);

EUCLIB_INLINE void euclib_draw_circle(euclib_plot_t *plot, vec2i_t pos, int radius, color_t color);
EUCLIB_INLINE void euclib_draw_text(euclib_plot_t *plot, vec2i_t pos, const char* text,color_t color);

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
    vec2f_t y_range;
    color_t line_color;
    int line_width;
} euclib_plot_line_params_t;

EUCLIB_INLINE void euclib_plot_2d_line(
    euclib_plot_t *plot, 
    graph_value_callback_t callback, 
    euclib_plot_line_params_t params);

EUCLIB_INLINE void euclib_plot_2d_line_smooth(
    euclib_plot_t *plot, 
    graph_value_callback_t callback, 
    euclib_plot_line_params_t params);

#ifdef EUCLIB_IMPLEMENTATION

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
    int radius, 
    color_t color
) {
    int diameter = radius + radius;
    int r2 = radius * radius;

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
    color_t color
) {
    unsigned long text_length = EUCLIB_STRLEN(text);

    for(int c = 0; c < text_length; ++c) {
        unsigned char character = text[c];
        FontgGlyph glyph = ibm_bios_font[character];

        for(int i = 0; i < 8; ++i) {
            for(int j = 0; j < 8; ++j) {
                int x = pos.y + i + (c * (8 + 1));
                int y = pos.y + j;

                if(!euclib_in_bounds(plot, (vec2i_t){ x, y })) 
                    continue;

                static unsigned long long start = 1u;
            
                if((glyph & (start << ((8 - i) + j*8))) != 0) {
                    EUCLIB_PLOT_AT(plot, x, y) = color;
                }
            }
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

        if(!euclib_in_bounds(plot, point)) {
            continue;
        }

        euclib_draw_line_width(plot, last_point, point, params.line_width, params.line_color);
        last_point = point;
    }
}

#endif

#endif