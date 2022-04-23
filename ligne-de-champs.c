#include "ligne-de-champs.h"
#include "utils/gfx/gfx.h"
#include "utils/utils.h"
#include "utils/vec2/vec2.h"
// #include <cstdint>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void gfx_draw_line(struct gfx_context_t *ctxt, coordinates_t p0,
                   coordinates_t p1, uint32_t color) {
  int dy = p1.row - p0.row;
  int dx = p1.column - p0.column;
  int m_dx = p0.column - p1.column;

  if (dy >= 0) // octants supérieurs
  {
    if (dx >= dy) // premier octant
    {
      double a = 2 * dy;
      double e = 0;

      int y = p0.row;
      for (int x = p0.column; x < p1.column; x++) {
        gfx_putpixel(ctxt, x, y, color);

        e += a;

        if (e >= dx) {
          y += 1;
          e -= 2 * dx;
        }
      }
    } else if ((dx >= 0) && (dx < dy)) { // deuxième octant
      int a = 2 * dx;
      int e = 0;

      int x = p0.column;

      for (int y = p0.row; y < p1.row; y++) {
        gfx_putpixel(ctxt, x, y, color);

        e += a;

        if (e >= dy) {
          x += 1;
          e -= 2 * dy;
        }
      }
    } else if (dy >= m_dx) { // troisième octant
      int a = 2 * dx;
      int e = 0;

      int m_dy = p0.row - p1.row;

      int x = p0.column;

      for (int y = p0.row; y < p1.row; y++) {
        gfx_putpixel(ctxt, x, y, color);

        e += a;

        if (e <= m_dy) {
          x -= 1;
          e += 2 * dy;
        }
      }
    } else { // quatrième octant
      coordinates_t n_p0;
      coordinates_t n_p1;

      // echange des coordonnées des deux points
      n_p0.column = p1.column;
      n_p0.row = p1.row;

      n_p1.column = p0.column;
      n_p1.row = p0.row;

      gfx_draw_line(ctxt, n_p0, n_p1, color);
    }
  } else {
    if (m_dx <= dy) // huitième octant
    {
      int a = 2 * dy;
      int e = 0;

      int y = p0.row;

      for (int x = p0.column; x < p1.column; x++) {
        gfx_putpixel(ctxt, x, y, color);
        e += a;

        if (e <= m_dx) {
          y -= 1;
          e += 2 * dx;
        }
      }
    } else {
      coordinates_t n_p0;
      coordinates_t n_p1;

      n_p0.column = p1.column;
      n_p0.row = p1.row;

      n_p1.column = p0.column;
      n_p1.row = p0.row;

      gfx_draw_line(ctxt, n_p0, n_p1, color);
    }
  }
}

void gfx_draw_circle(struct gfx_context_t *ctxt, coordinates_t c, uint32_t r,
                     uint32_t color) {
  uint32_t x = 0;
  uint32_t y = r;
  uint32_t d = r - 1;
  while (y >= x) {
    gfx_putpixel(ctxt, c.column + y, c.row + x, color);
    gfx_putpixel(ctxt, c.column + x, c.row + y, color);
    gfx_putpixel(ctxt, c.column + y, c.row - x, color);
    gfx_putpixel(ctxt, c.column + x, c.row - y, color);
    gfx_putpixel(ctxt, c.column - y, c.row + x, color);
    gfx_putpixel(ctxt, c.column - x, c.row + y, color);
    gfx_putpixel(ctxt, c.column - y, c.row - x, color);
    gfx_putpixel(ctxt, c.column - x, c.row - y, color);

    if (d >= 2 * x) {
      d -= 2 * x - 1;
      x = x + 1;
    } else if (d < (2 * x - 2 * y)) {
      d += 2 * y - 1;
      y--;
    } else {
      d += (2 * y - 2 * x - 2);
      y--;
      x++;
    }
  }
}

// calcule le vecteur entre p et la charge
vec2 qp_distance(vec2 p, charge_t c) { return vec2_sub(p, c.pos); }

// calcule la charge electrique par rapport à son emplacement
// E = K * Q / ||qP²||
double e_calculate(charge_t c, vec2 qP) {
  return K * c.q / pow(vec2_norm(qP), 2.0);
}

double tetaX_calculate(int WIDTH, int HEIGHT)
{
    return 1 / sqrt((WIDTH * WIDTH) + (HEIGHT * HEIGHT));
}

// Psuivant = P + tetaX * totalE / ||totalE||
vec2 P_next_calculate(vec2 previous_P, double tetaX, vec2 totalE)
{
    vec2 P_next;
    P_next = vec2_mul(tetaX, totalE);
    P_next = vec2_div(vec2_norm(totalE), P_next);
    P_next = vec2_add(previous_P, P_next);
    return P_next;
}

// Pprécédant = P - tetaX * totalE / ||totalE||
vec2 P_previous_calculate(vec2 previous_P, double tetaX, vec2 totalE)
{
    vec2 P_previous;
    P_previous = vec2_mul(tetaX, totalE);
    P_previous = vec2_div(vec2_norm(totalE), P_previous);
    P_previous = vec2_sub(previous_P, P_previous);
    return P_previous;
}

// Compute E*qP/norm(qP)
// Return false if norm(qP) < eps
bool compute_e(charge_t c, vec2 p, double eps, vec2 *e) {
  vec2 qP = qp_distance(p, c);
  if (vec2_norm(qP) < eps) {
    return false;
  } else {
  }
}

// Compute the normalized sum of Ei*qiP/norm(qiP)
// Return false if for some qiP, norm(qiP) < eps
bool compute_total_normalized_e(charge_t *charges, int num_charges, vec2 p,
                                double eps, vec2 *e) {
  for (int i = 0; i < num_charges; i++) {
    if (vec2_norm(qp_distance(p, charges[i])) < eps) {
      return false;
    }
  }
}

// Compute and then draw all the points belonging to a field line,
// starting from pos0.
// Returns false if pos0 is not a valid position
// (for example if pos0 is too close to a charge).
static bool draw_field_line(struct gfx_context_t *ctxt, charge_t *charges,
                            int num_charges, double dx, vec2 pos0, double x0,
                            double x1, double y0, double y1) {}
// Draw all the charges
// A circle with minus sign for negative charges
// A circle with a plus sign for positive charges
static void draw_charges(struct gfx_context_t *context, charge_t *charges,
                         int num_charges, double x0, double x1, double y0,
                         double y1) {}