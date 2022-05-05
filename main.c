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
  coordinates_t p0 = coordinates_create(50, 50);
  coordinates_t p1 = coordinates_create(28, 38);
  coordinates_t c = coordinates_create(500, 500);
  vec2 pos;
  charge_t charges[2], charges_test[2];
  pos = vec2_create(0.25, 0.5);
  charges[0] = charge_create(+30, pos);

  pos = vec2_create(0.75, 0.5);
  charges[1] = charge_create(-30, pos);

  vec2 P;

  draw_charges(ctxt, charges, 2, COLOR_BLUE);
  for (int i = 0; i < LINENB; i++)
  {
    P = randomP();
    // P=(vec2){.x=0.73, .y=0.5};
    // printf("P.x=%lf // P.y=%lf\n", P.x, P.y);
    draw_field_line(ctxt, charges, 2, CHARGE_RADIUS / SCREEN_HEIGHT, P, COLOR_YELLOW);
  }

  gfx_present(ctxt);
  while (gfx_keypressed() != SDLK_ESCAPE)
  {
  }

  gfx_destroy(ctxt);
  return EXIT_SUCCESS;
}