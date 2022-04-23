#include "ligne-de-champs.h"
#include "utils/utils.h"
#include "utils/vec2/vec2.h"
#include <stdio.h>
#include <stdlib.h>

vec2 randomP() { return (vec2){.x = rand_one(), .y = rand_one()}; }

int main() {
  struct gfx_context_t *ctxt =
      gfx_create("Planetary system", SCREEN_WIDTH, SCREEN_HEIGHT);
  if (!ctxt) {
    fprintf(stderr, "Graphics initialization failed!\n");
    return EXIT_FAILURE;
  }
  coordinates_t p0 = coordinates_create(50, 50);
  coordinates_t p1 = coordinates_create(28, 38);
  coordinates_t c = coordinates_create(500, 500);
  vec2 pos;
  charge_t charges[2];
  pos = vec2_create(0.2, 0.5);
  charges[0] = charge_create(10, pos);

  pos = vec2_create(0.8, 0.5);
  charges[1] = charge_create(-10, pos);

  vec2 P;
  printf("P.x=%lf // P.y=%lf\n", P.x, P.y);

  draw_charges(ctxt, charges, 2, COLOR_BLUE);
  for (int i = 0; i < 50; i++) {
    P=randomP();
    draw_field_line(ctxt, charges, 2, 0.001, P, COLOR_YELLOW);
  }

  gfx_present(ctxt);
  while (gfx_keypressed() != SDLK_ESCAPE) {
  }

  // while (true) {

  //   gfx_present(ctxt);
  //   gfx_clear(ctxt, COLOR_BLACK);
  //   if (gfx_keypressed() == SDLK_ESCAPE) {
  //     break;
  //   }

  //   // gfx_draw_circle(ctxt, c, 300, COLOR_BLUE);
  //   // gfx_draw_line(ctxt, p0, p1, COLOR_BLUE);
  //   // show_system(ctxt, &system);
  // }

  //   free_system(&system);
  gfx_destroy(ctxt);
  return EXIT_SUCCESS;
}