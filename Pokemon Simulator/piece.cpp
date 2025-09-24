#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
using namespace std;

class Piece{
    private:
        const int name;
        const int movement [2];
    public:

        const int* getMovement() { return movement;}
};