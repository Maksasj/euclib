#ifndef EUCLIB_H
#define EUCLIB_H

typedef void (*graph_value_callback_t)(const float x);

typedef unsigned int color_t;

typedef struct
{
    color_t *value;
    unsigned long long width;
    unsigned long long height;
} euclib_plot_t;

typedef struct
{

} euclib_plot_generic_params_t;

typedef struct
{

} euclib_plot_bar_params_t;

void euclib_plot_2d_bar(euclib_plot_t *plot, float values[], unsigned int count);

typedef struct
{

} euclib_plot_dots_params_t;

void euclib_plot_2d_dots(euclib_plot_t *plot, float values[], unsigned int count);

typedef struct
{

} euclib_plot_line_params_t;

void euclib_plot_2d_line(euclib_plot_t *plot, graph_value_callback_t callback);

#ifdef EUCLIB_IMPLEMENTATION

#endif

#endif