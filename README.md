## How to Load and Render Tiled Maps in Your SDL2 Game

![Tiled Map Example](screenshot/example.png?raw=true)

**This repository contains sample code for [my article on loading and rendering Tiled maps in SDL2](#).**

This repository's *only* focus is on demonstrating how to load and render Tiled maps in SDL2. The rest of the code is a quick and dirty set-up for the Tiled level loading functionality to stand on and should not be used or regarded as good game design with SDL2.

## Credit

The tileset I used in the example Tiled map was the result of Hyptosis's hard work at [opengameart.org](https://opengameart.org/). Check out his [website](http://www.lorestrome.com/) or his [opengameart profile](https://opengameart.org/users/hyptosis).

## Pre-requisites

* A C++ compiler

* cmake

* SDL2, SDL2 Image, and TMXLite libraries installed

## Building

Whatever you do, make sure that you run your built binary from the project root (where the `res/` folder is), so that the executable can find the images and map.

```
$ git clone https://github.com/connorkuehl/sdl2-tiled-example.git

$ cd sdl2-tiled-example

$ mkdir build

$ cd build

$ cmake ..

$ make

$ cd ..

$ ./build/sdl2_example
```

