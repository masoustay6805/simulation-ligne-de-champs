#include "ligne-de-champs.h"
#include "utils/utils.h"
#include "utils/vec2/vec2.h"
#include <stdio.h>
#include <stdlib.h>

vec2 randomPos() { return (vec2){.x = rand_one(), .y = rand_one()}; }

double randomQ(){
  return (double)(rand()%(int)(MAX_Q-MIN_Q+1)+MIN_Q);
}

int main()
{
  struct gfx_context_t *ctxt =
      gfx_create("Lignes de champs éléctriques", SCREEN_WIDTH, SCREEN_HEIGHT);
  if (!ctxt)
  {
    fprintf(stderr, "Graphics initialization failed!\n");
    return EXIT_FAILURE;
  }
  // create all charges
  vec2 qPos;
  charge_t charges[NBCHARGES];
  qPos = vec2_create(0.25, 0.5);
  charges[0] = charge_create(+30, qPos);

  qPos = vec2_create(0.5, 0.25);
  charges[1] = charge_create(-30, qPos);

  qPos = vec2_create(0.5, 0.75);
  charges[2] = charge_create(-30, qPos);

  qPos = vec2_create(0.75, 0.5);
  charges[3] = charge_create(+30, qPos);
  // for(int i=0; i<NBCHARGES; i++){
  //   charges[i].pos=randomPos();
  //   charges[i].q=randomQ();
  // }

  vec2 P;

  // draw all charges
  draw_charges(ctxt, charges, NBCHARGES, COLOR_RED, COLOR_BLUE);
  for (int i = 0; i < LINENB; i++)
  {
    printf("draw field line ...\n");
    // draw field lines for each random P
    P = randomPos();
    draw_field_line(ctxt, charges, NBCHARGES, CHARGE_RADIUS / SCREEN_HEIGHT, P, COLOR_YELLOW);
  }
  gfx_present(ctxt);
  while (gfx_keypressed() != SDLK_ESCAPE)
  {
  }

  gfx_destroy(ctxt);
  return EXIT_SUCCESS;
}