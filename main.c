#include "ligne-de-champs.h"
#include "utils/utils.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
  struct gfx_context_t *ctxt = gfx_create("Planetary system", SCREEN_WIDTH, SCREEN_HEIGHT);
  if (!ctxt) {
    fprintf(stderr, "Graphics initialization failed!\n");
    return EXIT_FAILURE;
  }
  coordinates_t p0 = coordinates_create(50, 50);
  coordinates_t p1 = coordinates_create(28, 38);
  coordinates_t c = coordinates_create(500, 500);

  while (true) {

    gfx_present(ctxt);
    gfx_clear(ctxt, COLOR_BLACK);
    if (gfx_keypressed() == SDLK_ESCAPE) {
      break;
    }
    // gfx_draw_circle(ctxt, c, 300, COLOR_BLUE);
    gfx_draw_line(ctxt, p0, p1, COLOR_BLUE);
    // show_system(ctxt, &system);
  }

  //   free_system(&system);
  gfx_destroy(ctxt);
  return EXIT_SUCCESS;
}