#include "ligne-de-champs.h"
#include "utils/utils.h"
#include "utils/vec2/vec2.h"
#include <stdio.h>
#include <stdlib.h>

vec2 randomP() { return (vec2){.x = rand_one(), .y = rand_one()}; }

int main()
{
  struct gfx_context_t *ctxt =
      gfx_create("Lignes de champs éléctriques", SCREEN_WIDTH, SCREEN_HEIGHT);
  if (!ctxt)
  {
    fprintf(stderr, "Graphics initialization failed!\n");
    return EXIT_FAILURE;
  }
  //create all charges
  vec2 qPos;
  charge_t charges[3];
  qPos = vec2_create(0.25, 0.3);
  charges[0] = charge_create(+30, qPos);

  qPos = vec2_create(0.25, 0.6);
  charges[1] = charge_create(+30, qPos);

  qPos = vec2_create(0.75, 0.6);
  charges[2] = charge_create(-30, qPos);

  vec2 P;

  //draw all charges
  draw_charges(ctxt, charges, 3, COLOR_RED, COLOR_BLUE);
  for (int i = 0; i < LINENB; i++)
  {
    //draw field lines for each random P
    P = randomP();
    draw_field_line(ctxt, charges, 3, CHARGE_RADIUS / SCREEN_HEIGHT, P, COLOR_YELLOW);
  }
  //
  gfx_present(ctxt);
  while (gfx_keypressed() != SDLK_ESCAPE)
  {
  }

  gfx_destroy(ctxt);
  return EXIT_SUCCESS;
}