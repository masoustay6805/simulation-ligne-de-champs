#ifndef _LIGNE_DE_CHAMPS_H_
#define _LIGNE_DE_CHAMPS_H_
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000
#define LIMIT_X 1
#define LIMIT_Y 1

#include "utils/gfx/gfx.h"
#include "utils/utils.h"

void gfx_draw_line(struct gfx_context_t *ctxt, coordinates_t p0,
                   coordinates_t p1, uint32_t color);

void gfx_draw_circle(struct gfx_context_t *ctxt, coordinates_t c,
uint32_t r, uint32_t color);

// Compute E*qP/norm(qP)
// Return false if norm(qP) < eps
bool compute_e(charge_t c, vec2 p, double eps, vec2 *e);
// Compute the normalized sum of Ei*qiP/norm(qiP)
// Return false if for some qiP, norm(qiP) < eps
bool compute_total_normalized_e(charge_t *charges, int num_charges, vec2 p,
                                double eps, vec2 *e);
// Compute and then draw all the points belonging to a field line,
// starting from pos0.
// Returns false if pos0 is not a valid position
// (for example if pos0 is too close to a charge).
void draw_field_line(struct gfx_context_t *ctxt, charge_t *charges,
                            int num_charges, double dx, vec2 pos0, int color);
// Draw all the charges
// A circle with minus sign for negative charges
// A circle with a plus sign for positive charges
void draw_charges(struct gfx_context_t *context, charge_t *charges,
                         int num_charges, int color);

#endif