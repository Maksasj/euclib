#ifndef EUCLIB_H
#define EUCLIB_H

typedef unsigned int color_t;

#define EUCLIB_WHITE    ((color_t)(0xffffffff))
#define EUCLIB_BLACK    ((color_t)(0xff000000))
#define EUCLIB_RED      ((color_t)(0xff3440eb))
#define EUCLIB_GREEN    ((color_t)(0xff34eb40))
#define EUCLIB_BLUE     ((color_t)(0xffeb3440))
#define EUCLIB_PURPLE   ((color_t)(0xffeb34eb))
#define EUCLIB_YELLOW   ((color_t)(0xff34ebeb))
#define EUCLIB_ORANGE   ((color_t)(0xff3486eb))

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

void euclib_plot_2d_bar(
    euclib_plot_t *plot, 
    float values[], 
    unsigned int count, 
    euclib_plot_bar_params_t params);

typedef struct
{

} euclib_plot_dots_params_t;

void euclib_plot_2d_dots(
    euclib_plot_t *plot, 
    float values[], 
    unsigned int count, 
    euclib_plot_dots_params_t params);

typedef struct
{
    vec2f_t range;
} euclib_plot_line_params_t;

void euclib_plot_2d_line(
    euclib_plot_t *plot, 
    graph_value_callback_t callback, 
    euclib_plot_line_params_t params);

#ifdef EUCLIB_IMPLEMENTATION

void euclib_plot_2d_bar(
    euclib_plot_t *plot, 
    float values[], 
    unsigned int count, 
    euclib_plot_bar_params_t params
) {

}

void euclib_plot_2d_dots(
    euclib_plot_t *plot, 
    float values[], 
    unsigned int count, 
    euclib_plot_dots_params_t params
) {

}

void euclib_plot_2d_line(
    euclib_plot_t *plot, 
    graph_value_callback_t callback, 
    euclib_plot_line_params_t params
) {
    for(int i = 0; i < plot->width; ++i) {
        for(int j = 0; j < plot->height; ++j) {
            plot->value[i + j*plot->width] = EUCLIB_WHITE;
        }
    }
}

#endif

#endif