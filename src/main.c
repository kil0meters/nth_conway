#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// #include <png.h>

void print_help(void) { 
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

long int get_int(const char *string, const char *name) {
    long int n = strtol(string, NULL, 10);
    if (n == 0) {
        print_help();
        fprintf(stderr, "\nerror: expected integer greater than 0 for '%s'\n", name);
        exit(1);
    }
    return n;
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

        printf("%d %d %d %d\n", n, width, height, size);
        return 0;
}
