# (n)th conway

![example image](conway.png)

Create PNG files of a random game of
[Conway's Game of Life](https://en.wikipedia.org/wiki/Conway's_Game_of_Life)
after N iterations.

## Building

Dependencies on Fedora:

```
sudo dnf install gcc make libpng libpng-devel
```

Clone and build:

```
git clone https://github.com/kil0meters/nth_conway
cd nth_conway
make
```

## Usage

```
nth_conway {n} {width}x{height} {size}
```

This outputs a file called conway.png according to the set parameters.

```
n      -- The number of iterations to run Conway's Game of Life
width  -- The width of the visible game board
height -- The height of the visible game board
size   -- The size of pixels on the game board
```
