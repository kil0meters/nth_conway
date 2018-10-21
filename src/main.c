#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
// #include <png.h>

static void print_help(void) { 
    fprintf(stderr,
            "nth_conway (0.1)\n"
            "kilometers <kilo@meters.sh>\n\n"

            "USAGE:\n"
            "    nth_conway {n} {width} {height} {size}\n\n"

            "    n      -- The number of iterations to run Conway's Game of Life\n"
            "    width  -- The width of the visible game board\n"
            "    height -- The height of the visible game board\n"
            "    size   -- The size of pixels on the game board\n");
}

static long int get_int(const char *string, const char *name) {
    long int n = strtol(string, NULL, 10);
    if (n == 0) {
        print_help();
        fprintf(stderr, "\nerror: expected integer greater than 0 for '%s'\n", name);
        exit(1);
    }
    return n;
}

static void print_game_board(
    const bool *game_board,
    const int height,
    const int width
) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            printf("%s", game_board[i * width + j] ? "█" : " ");
        }
        printf("\n");
    }
}

static bool *game_board_new(
    bool *game_board,
    const int size
) {
    // generate large random number, then use bitwise operators
    // to get a sigular random bit;
    srand(time(NULL));
    uint32_t r = rand();
    for (int i = 0; i < size; i++) {
        game_board[i] = (r & 0x00000001);
        r = r >> 1;
        if (r == 0) {
            r = rand();
        }
    }

    return game_board;
}

int main(int argc, char *argv[]) {
    if (argc != 5
        || !strcmp(argv[1], "--help")
        || !strcmp(argv[1], "-h")) {
        print_help();
        exit(0);
    }

    int n      = get_int(argv[1], "n");
    int width  = get_int(argv[2], "width");
    int height = get_int(argv[3], "height");
    int size   = get_int(argv[4], "size"); 

    bool game_board[height * width];

    game_board_new(game_board, height * width);

    print_game_board(game_board, height, width);

    printf("%d %d %d %d\n", n, width, height, size);
    return 0;
}
