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

// calcul de la constante tetaX
double tetaX_calculate(int WIDTH, int HEIGHT) {
  return 1 / sqrt((WIDTH * WIDTH) + (HEIGHT * HEIGHT));
}

// calcul du vecteur total E
vec2 totalE(charge_t *charges, int num_charges, vec2 P) {
  vec2 tmp, qP;
  vec2 totalE = vec2_create_zero();
  for (int i = 0; i < num_charges; i++) {
    qP = qp_distance(P, charges[i]);
    tmp = vec2_mul(e_calculate(charges[i], qP), charges[i].pos);
    tmp = vec2_div(vec2_norm(charges[i].pos), tmp);
    totalE = vec2_add(totalE, tmp);
  }
  return totalE;
}

// Psuivant = P + tetaX * totalE / ||totalE||
vec2 P_next_calculate(vec2 previous_P, double tetaX, vec2 totalE) {
  vec2 P_next;
  P_next = vec2_mul(tetaX, totalE);
  P_next = vec2_div(vec2_norm(totalE), P_next);
  P_next = vec2_add(previous_P, P_next);
  return P_next;
}

// Pprécédant = P - tetaX * totalE / ||totalE||
vec2 P_previous_calculate(vec2 previous_P, double tetaX, vec2 totalE) {
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

bool in_univers(vec2 p, charge_t *charges, double limitX, double limitY,
                int num_charges, double limitCharge) {
  bool in_univers = false;
  if (p.x <= limitX && p.y <= limitY && p.x >= 0.0 && p.y >= 0.0) {
    in_univers = true;
    for (int i = 0; i < num_charges; i++) {
      if (p.x > (charges[i].pos.x - limitCharge) &&
          p.x < (charges[i].pos.x + limitCharge) &&
          p.y > (charges[i].pos.y - limitCharge) &&
          p.y < charges[i].pos.y + limitCharge) {
        in_univers = false;
      }
    }
  }
  return in_univers;
}
// Compute and then draw all the points belonging to a field line,
// starting from pos0.
// Returns false if pos0 is not a valid position
// (for example if pos0 is too close to a charge).
void draw_field_line(struct gfx_context_t *ctxt, charge_t *charges,
                            int num_charges, double dx, vec2 pos0, int color) {
  vec2 total_E, qP, P;
  P = pos0;
  double tetaX = tetaX_calculate(SCREEN_WIDTH, SCREEN_HEIGHT);
  while (in_univers(P, charges, LIMIT_X, LIMIT_Y, num_charges, dx)) {
    gfx_putpixel(ctxt, P.x * SCREEN_WIDTH, P.y * SCREEN_HEIGHT, color);
    total_E = totalE(charges, num_charges, P);
    P = P_next_calculate(P, tetaX, total_E);
  }
  P = pos0;
  while (in_univers(P, charges, LIMIT_X, LIMIT_Y, num_charges, dx)) {
    gfx_putpixel(ctxt, P.x * SCREEN_WIDTH, P.y * SCREEN_HEIGHT, color);
    total_E = totalE(charges, num_charges, P);
    P = P_previous_calculate(P, tetaX, total_E);
  }
  // gfx_present(ctxt);
}
// Draw all the charges
// A circle with minus sign for negative charges
// A circle with a plus sign for positive charges
void draw_charges(struct gfx_context_t *context, charge_t *charges,
                         int num_charges, int color) {
    int circleRadius=10;
  for (int i = 0; i < num_charges; i++) {
    coordinates_t c;
    c.column = charges[i].pos.x * SCREEN_WIDTH;
    c.row = charges[i].pos.y * SCREEN_HEIGHT;

    if (charges[i].q > 0) { // charge +
      coordinates_t p1, p2, p3, p4;

      //calcul des deux points pour la ligne vericale du plus
      p1.column = c.column;
      p1.row = c.row - circleRadius / 2;
      p2.column = c.column;
      p2.row = c.row + circleRadius / 2;

      //calcul des deux points pour la ligne horizontale du plus
      p3.column = c.column - circleRadius / 2;
      p3.row = c.row;
      p4.column = c.column + circleRadius / 2;
      p4.row = c.row;

      

      //dessiner le plus a l'ecran 
      gfx_draw_line(context, p1, p2, color);
      gfx_draw_line(context, p3, p4, color);
    } else { // charge -
      coordinates_t m1, m2;
      //calcul des deux points pour dessiner le signe -
      m1.column = c.column - circleRadius / 2;
      m1.row = c.row;
      m2.column = c.column + circleRadius / 2;
      m2.row = c.row;
      //dessiner le moins à l'écran
      gfx_draw_line(context, m1, m2, color);
    }
    //dessin de la charge
    gfx_draw_circle(context, c, circleRadius, color);
  }

  // gfx_present(context);
}