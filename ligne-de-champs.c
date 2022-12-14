#include "ligne-de-champs.h"
#include "utils/gfx/gfx.h"
#include "utils/utils.h"
#include "utils/vec2/vec2.h"
// #include <cstdint>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * It draws a line between two points
 *
 * @param ctxt the graphics context
 * @param p0 the starting point of the line
 * @param p1 the coordinates of the first point
 * @param color the color of the line
 */
void gfx_draw_line(struct gfx_context_t *ctxt, coordinates_t p0,
                   coordinates_t p1, uint32_t color)
{
  int dy = p1.row - p0.row;
  int dx = p1.column - p0.column;
  int m_dx = p0.column - p1.column;

  // octants supérieurs
  if (dy >= 0)
  {
    if (dx >= dy)
    {
      double e = 0;
      double a = 2 * dy;
      int y = p0.row;

      for (uint32_t x = p0.column; x < p1.column; x++)
      {
        gfx_putpixel(ctxt, x, y, color);
        e += a;
        if (dx <= e)
        {
          e -= 2 * dx;
          y += 1;
        }
      }
    }
    else if ((dx >= 0) && (dx < dy))
    {
      int e = 0;
      int a = 2 * dx;
      int x = p0.column;

      for (uint32_t y = p0.row; y < p1.row; y++)
      {
        gfx_putpixel(ctxt, x, y, color);
        e += a;
        if (dy <= e)
        {
          e -= 2 * dy;
          x += 1;
        }
      }
    }
    else if (dy >= m_dx)
    {
      int e = 0;
      int a = 2 * dx;
      int m_dy = p0.row - p1.row;
      int x = p0.column;

      for (uint32_t y = p0.row; y < p1.row; y++)
      {
        gfx_putpixel(ctxt, x, y, color);
        e += a;
        if (e <= m_dy)
        {
          x -= 1;
          e += 2 * dy;
        }
      }
    }
    else
    {
      // echange des coordonnées des deux points
      coordinates_t c0 = (coordinates_t){
          .column = p1.column,
          .row = p1.row};
      coordinates_t c1 = (coordinates_t){
          .column = p0.column,
          .row = p0.row};

      gfx_draw_line(ctxt, c0, c1, color);
    }
  }
  else
  {
    if (m_dx <= dy)
    {
      int a = 2 * dy;
      int e = 0;
      int y = p0.row;

      for (uint32_t x = p0.column; x < p1.column; x++)
      {
        gfx_putpixel(ctxt, x, y, color);
        e += a;
        if (e <= m_dx)
        {
          y -= 1;
          e += 2 * dx;
        }
      }
    }
    else
    {
      coordinates_t c0 = (coordinates_t){
          .column = p1.column,
          .row = p1.row};
      coordinates_t c1 = (coordinates_t){
          .column = p0.column,
          .row = p0.row};

      gfx_draw_line(ctxt, c0, c1, color);
    }
  }
}

/**
 * "Draw a circle by
 * drawing a bunch of pixels."
 *
 * @param ctxt The graphics context.
 * @param c The center of the circle
 * @param r radius of the circle
 * @param color The color of the circle.
 */
void gfx_draw_circle(struct gfx_context_t *ctxt, coordinates_t c, uint32_t r,
                     uint32_t color)
{
  uint32_t x = 0;
  uint32_t y = r;
  uint32_t d = r - 1;
  while (y >= x)
  {
    // draw full circle
    // put pixel in the differents sides
    gfx_putpixel(ctxt, c.column + y, c.row + x, color);
    gfx_putpixel(ctxt, c.column + x, c.row + y, color);
    gfx_putpixel(ctxt, c.column + y, c.row - x, color);
    gfx_putpixel(ctxt, c.column + x, c.row - y, color);
    gfx_putpixel(ctxt, c.column - y, c.row + x, color);
    gfx_putpixel(ctxt, c.column - x, c.row + y, color);
    gfx_putpixel(ctxt, c.column - y, c.row - x, color);
    gfx_putpixel(ctxt, c.column - x, c.row - y, color);

    if (d >= 2 * x)
    {
      d -= 2 * x - 1;
      x = x + 1;
    }
    else if (d < (2 * x - 2 * y))
    {
      d += 2 * y - 1;
      y--;
    }
    else
    {
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

double e_calculate(charge_t c, vec2 qP)
{
  return K * c.q / pow(vec2_norm(qP), 2.0);
}

// calcul de la constante tetaX
double tetaX_calculate(int WIDTH, int HEIGHT)
{
  return 1 / sqrt((WIDTH * WIDTH) + (HEIGHT * HEIGHT));
}

// calcul du vecteur total E
// E⃗ =∑Ni Ei⋅qiP/||qiP||.
vec2 totalE(charge_t *charges, int num_charges, vec2 P)
{
  vec2 tmp, qP;
  vec2 totalE = vec2_create_zero();
  for (int i = 0; i < num_charges; i++)
  {
    qP = qp_distance(P, charges[i]);
    tmp = vec2_mul(e_calculate(charges[i], qP), qP);
    tmp = vec2_div(vec2_norm(qP), tmp);
    totalE = vec2_add(totalE, tmp);
  }
  return totalE;
}

// Psuivant = P + tetaX * totalE / ||totalE||
vec2 P_next_calculate(vec2 previous_P, double tetaX, vec2 totalE)
{
  vec2 P_next = vec2_mul(tetaX, totalE);
  P_next = vec2_div(vec2_norm(totalE), P_next);
  P_next = vec2_add(previous_P, P_next);
  return P_next;
}

// Pprécédant = P - tetaX * totalE / ||totalE||
vec2 P_previous_calculate(vec2 previous_P, double tetaX, vec2 totalE)
{
  vec2 P_previous = vec2_mul(tetaX, totalE);
  P_previous = vec2_div(vec2_norm(totalE), P_previous);
  P_previous = vec2_sub(previous_P, P_previous);
  return P_previous;
}

// Returns false if P is not a valid position
bool in_univers(vec2 P_actual, double limitX, double limitY)
{
  // check if P_actual is in the window
  return P_actual.x <= limitX && P_actual.y <= limitY && P_actual.x >= 0.0 && P_actual.y >= 0.0;
}

// (for example if P is too close to a charge).
bool in_charge(vec2 P_actual, charge_t *charges, int num_charges, double eps)
{
  // check if P_actual is in the charges(pos(charges)-eps or pos(charges)+eps)
  // if it in the charge don't draw it
  for (int i = 0; i < num_charges; i++)
  {
    if (P_actual.x > (charges[i].pos.x - eps) && P_actual.x < (charges[i].pos.x + eps) &&
        P_actual.y > (charges[i].pos.y - eps) && P_actual.y < charges[i].pos.y + eps)
    {
      return true;
    }
  }
  return false;
}

// Compute and then draw all the points belonging to a field line,
// starting from pos0.
void draw_field_line(struct gfx_context_t *ctxt, charge_t *charges,
                     int num_charges, double eps, vec2 pos0, int color)
{
  vec2 total_E, P;
  double tetaX = tetaX_calculate(SCREEN_WIDTH, SCREEN_HEIGHT);
  P = pos0;
  // draw all pNext until the charges
  while (in_univers(P, LIMIT_X, LIMIT_Y) && !in_charge(P, charges, num_charges, eps))
  {
    coordinates_t Pnext = position_to_coordinates(SCREEN_HEIGHT, SCREEN_WIDTH, 0, LIMIT_X, 0, LIMIT_Y, P);
    gfx_putpixel(ctxt, Pnext.column, Pnext.row, color);
    total_E = totalE(charges, num_charges, P);
    P = P_next_calculate(P, tetaX, total_E);
  }
  P = pos0;
  // draw all pPrevious until the charges
  while (in_univers(P, LIMIT_X, LIMIT_Y) && !in_charge(P, charges, num_charges, eps))
  {
    coordinates_t Pprev = position_to_coordinates(SCREEN_HEIGHT, SCREEN_WIDTH, 0, LIMIT_X, 0, LIMIT_Y, P);
    gfx_putpixel(ctxt, Pprev.column, Pprev.row, color);
    total_E = totalE(charges, num_charges, P);
    P = P_previous_calculate(P, tetaX, total_E);
  }
}
// Draw all the charges
// A circle with minus sign for negative charges
// A circle with a plus sign for positive charges
void draw_charges(struct gfx_context_t *context, charge_t *charges,
                  int num_charges, int colorPositive, int colorNegative)
{
  for (int i = 0; i < num_charges; i++)
  {

    // remise à l'échelle de la fenêtre les coordonnées de chaque charge
    coordinates_t c = position_to_coordinates(SCREEN_WIDTH, SCREEN_HEIGHT, 0, LIMIT_X, 0, LIMIT_Y, charges[i].pos);
    // dessin de la charge

    if (charges[i].q > 0)
    { // plus sign
      gfx_draw_circle(context, c, CHARGE_RADIUS, colorPositive);

      // calcul des deux points pour la ligne verticale du plus
      coordinates_t p1 = (coordinates_t){
          .column = c.column,
          .row = c.row - CHARGE_RADIUS / 2};
      coordinates_t p2 = (coordinates_t){
          .column = c.column,
          .row = c.row + CHARGE_RADIUS / 2};

      // dessin
      gfx_draw_line(context, p1, p2, colorPositive);

      // calcul des deux points pour la ligne horizontale du plus
      coordinates_t p3 = (coordinates_t){
          .column = c.column - CHARGE_RADIUS / 2,
          .row = c.row};
      coordinates_t p4 = (coordinates_t){
          .column = c.column + CHARGE_RADIUS / 2,
          .row = c.row};

      // dessin
      gfx_draw_line(context, p3, p4, colorPositive);
    }
    else
    { // minus sign
      gfx_draw_circle(context, c, CHARGE_RADIUS, colorNegative);

      // calcul des deux points pour dessiner le signe -
      coordinates_t m1 = (coordinates_t){
          .column = c.column - CHARGE_RADIUS / 2,
          .row = c.row};
      coordinates_t m2 = (coordinates_t){
          .column = c.column + CHARGE_RADIUS / 2,
          .row = c.row};

      // dessiner le moins à l'écran
      gfx_draw_line(context, m1, m2, colorNegative);
    }
  }
}