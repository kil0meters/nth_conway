#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <png.h>

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
            printf("%s", game_board[i * width + j] ? "█" : "░");
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

// 1. Any live cell with fewer than two live neighbors dies, as if by underpopulation.
// 2. Any live cell with two or three live neighbors lives on to the next generation.
// 3. Any live cell with more than three live neighbors dies, as if by overpopulation.
// 4. Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.

static void game_board_step(
    bool *game_board,
    const int height,
    const int width
) {
    int neighbor_cells;
    int cell_pos;
    bool cell;

    // create a new board since we can't edit the existing one
    // while doing comparisons on it 
    bool new_board[width * height];

    for (int i = 0; i < width * height; i++) {
        cell_pos = i;
        cell = game_board[cell_pos];
        neighbor_cells = 0;

        if ((cell_pos + 1) % width != 0)                                          // east
            neighbor_cells += game_board[cell_pos + 1];
        
        if (cell_pos % width != 0)                                                // west 
            neighbor_cells += game_board[cell_pos - 1];
        
        if (cell_pos + width < width * height)                                    // south
            neighbor_cells += game_board[cell_pos + width];
        
        if (cell_pos > width)                                                     // north
           neighbor_cells += game_board[cell_pos - width];
        
        if ((cell_pos > width) && ((cell_pos + 1) % width != 0))                  // north east
            neighbor_cells += game_board[cell_pos - width + 1];
        
        if ((cell_pos > width) && (cell_pos % width != 0))                        // north west
            neighbor_cells += game_board[cell_pos - width - 1];
        
        if ((cell_pos + width < width * height) && ((cell_pos + 1) % width != 0)) // south east
            neighbor_cells += game_board[cell_pos + width + 1];
        
        if ((cell_pos + width < width * height) && (cell_pos % width != 0))       // south west
            neighbor_cells += game_board[cell_pos + width - 1];

        /*printf("debug: %d,%d,%d\n", neighbor_cells,
                                    cell_pos,
                                    game_board[cell_pos]);
        */
        if (cell && (neighbor_cells < 2 || neighbor_cells > 3)) {
            new_board[cell_pos] = false;
        }
        else if (cell && (neighbor_cells == 3 || neighbor_cells == 2)) {
            new_board[cell_pos] = true;
        }
        else if (!cell && neighbor_cells == 3) {
            new_board[cell_pos] = true;
        }
        else {
            new_board[cell_pos] = false;
        }
    }
    
    // game_board = new_board;
    for (int i = 0; i < height * width; i++) {
        game_board[i] = new_board[i];
    }
}

static void format_board_as_png(
    const bool *game_board,
    const int size,
    const int height,
    const int width,
    const char *filename
) {
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        fprintf(stderr, "error: Could not open \"%s\" for writing\n", filename);
        exit(1);
    }

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
                                      NULL, NULL, NULL);
    if (png_ptr == NULL) {
        fprintf(stderr, "error: Not enough memory\n");
        exit(1);
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        fprintf(stderr, "error: Not enough memory\n");
    }

    png_init_io(png_ptr, fp);

    // int size_adjusted_width = width * size;
    // int size_adjusted_height = height * size;

    // bit depth of 2 since it's a black and white image
    png_set_IHDR(png_ptr, info_ptr, width * size, height * size, 8,
                 PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    png_write_info(png_ptr, info_ptr);

    // 1 byte per pixel 
    png_bytep row = (png_bytep) malloc(1 * width * size * sizeof(png_byte));

    int index = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width * size; j += size) {
            bool cell = game_board[index];
            png_byte pixel_color;
            pixel_color = cell ? 0 : 255;
            for (int s = 0; s < size; s++) {
                row[j + s] = pixel_color;
            }
            index++;
        }
        for (int s = 0; s < size; s++) {
            png_write_row(png_ptr, row);
        }
    }

    png_write_end(png_ptr, NULL);

    if (fp != NULL) fclose(fp);
    if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
    if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
    if (row != NULL) free(row);
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

    // char frame[64];

    for (int i = 0; i < n; i++) {
        print_game_board(game_board, height, width);
        // sprintf(frame, "movie/conway%03d.png", i);
        // format_board_as_png(game_board, size, height, width, frame);
        game_board_step(game_board, height, width);
        printf("\n");
    }

    // print_game_board(game_board, height, width);
    format_board_as_png(game_board, size, height, width, "conway.png");

    printf("%d %d %d %d\n", n, width, height, size);
    return 0;
}
