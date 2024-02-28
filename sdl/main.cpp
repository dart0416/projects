#include <SDL2/SDL.h>

enum Color { RED, GREEN, BLUE, YELLOW };
Color currentColor = RED;

void handleKeyPress(SDL_Event& event) {
    switch (event.key.keysym.sym) {
        case SDLK_UP:
            currentColor = static_cast<Color>((currentColor + 1) % 4);
            break;
        case SDLK_DOWN:
            currentColor = static_cast<Color>((currentColor - 1 + 4) % 4);
            break;
        default:
            break;
    }
}

void setRendererColor(SDL_Renderer* renderer) {
    switch (currentColor) {
        case RED:
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Czerwony
            break;
        case GREEN:
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Zielony
            break;
        case BLUE:
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);  // Niebieski
            break;
        case YELLOW:
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);  // Żółty
            break;
        default:
            break;
    }
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Nie można zainicjować SDL: %s", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("OKNOOOOOOOOOOOOOOOOO", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Log("Nie można utworzyć okna: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_Log("Nie można utworzyć renderera: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Event e;
    bool quit = false;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                handleKeyPress(e);
            }
        }

        setRendererColor(renderer);

        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

