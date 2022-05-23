#ifndef _LIGNE_DE_CHAMPS_H_
#define _LIGNE_DE_CHAMPS_H_
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000
#define CHARGE_RADIUS 10.0
#define LIMIT_X 1
#define LIMIT_Y 1
#define LINENB 300
#define NBCHARGES 4
#define MIN_Q -100
#define MAX_Q 100



#include "utils/gfx/gfx.h"
#include "utils/utils.h"

void gfx_draw_line(struct gfx_context_t *ctxt, coordinates_t p0,
                   coordinates_t p1, uint32_t color);

void gfx_draw_circle(struct gfx_context_t *ctxt, coordinates_t c,
uint32_t r, uint32_t color);

// Compute and then draw all the points belonging to a field line,
// starting from pos0.
// Returns false if pos0 is not a valid position
// (for example if pos0 is too close to a charge).
void draw_field_line(struct gfx_context_t *ctxt, charge_t *charges,
                            int num_charges, double eps, vec2 pos0, int color);
// Draw all the charges
// A circle with minus sign for negative charges
// A circle with a plus sign for positive charges
void draw_charges(struct gfx_context_t *context, charge_t *charges,
                         int num_charges, int colorPositive, int colorNegative);

#endif