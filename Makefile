CC=gcc
CFLAGS=-Wall -Wextra -g -pedantic -fsanitize=address -fsanitize=leak
LDFLAGS=-fsanitize=address -fsanitize=leak


ligne-de-champs: ligne-de-champs.o ./utils/vec2/vec2.o ./utils/utils.o ./utils/gfx/gfx.o main.o
	$(CC) $(LDFLAGS) $^ -o $@ -lm -lSDL2

ligne-de-champs.o: ligne-de-champs.c ligne-de-champs.h

./utils/vec2/vec2.o: ./utils/vec2/vec2.c ./utils/vec2/vec2.h 

./utils/utils.o: ./utils/utils.c ./utils/utils.h 

./utils/gfx/gfx.o: ./utils/gfx/gfx.c ./utils/gfx/gfx.h

clean: 
	rm -f ligne-de-champs
	find . -type f -name '*.o' -delete

rebuild: clean ligne-de-champs