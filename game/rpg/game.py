import pygame
import sys
import random
import configparser

# Inicjalizacja Pygame
pygame.init()

# Odczytanie konfiguracji z pliku
config = configparser.ConfigParser()
config.read('config.ini')

# Ustawienia okna
screen_width = config.getint('Window', 'Width', fallback=600)
screen_height = config.getint('Window', 'Height', fallback=600)
fullscreen = config.getboolean('Window', 'Fullscreen', fallback=False)

if fullscreen:
    screen = pygame.display.set_mode((screen_width, screen_height), pygame.FULLSCREEN)
else:
    screen = pygame.display.set_mode((screen_width, screen_height))

# Definicja minimapy
minimap_size = config.getint('Minimap', 'Size', fallback=100)
minimap = pygame.Surface((minimap_size, minimap_size), pygame.SRCALPHA)  # Dodano flagę SRCALPHA dla powierzchni z przezroczystości

# Kolory
black = tuple(map(int, config.get('Colors', 'Black', fallback='0,0,0').split(',')))
white = tuple(map(int, config.get('Colors', 'White', fallback='255,255,255').split(',')))
green = tuple(map(int, config.get('Colors', 'Green', fallback='0,255,0').split(',')))
red = tuple(map(int, config.get('Colors', 'Red', fallback='255,0,0').split(',')))

# Wymiary postaci
player_size = config.getint('Player', 'Size', fallback=30)

# Wymiary mapy
map_width = config.getint('Map', 'Width', fallback=3000)
map_height = config.getint('Map', 'Height', fallback=3000)

# Wymiary przeszkód
obstacle_size = config.getint('Obstacle', 'Size', fallback=50)
obstacles_count = config.getint('Obstacle', 'Count', fallback=25)

# Inicjalizacja przeszkód z losowymi kolorami
obstacles = [(random.randint(0, map_width - obstacle_size),
              random.randint(0, map_height - obstacle_size),
              0) for _ in range(obstacles_count)]  # Indeks koloru: 0 (zielony)

# Dodanie widocznych bariery czerwonych na krawędziach mapy
barrier_size = 20
barriers = [
    (0, 0, map_width, barrier_size),  # Górna bariery
    (0, 0, barrier_size, map_height),  # Lewa bariery
    (map_width - barrier_size, 0, barrier_size, map_height),  # Prawa bariery
    (0, map_height - barrier_size, map_width, barrier_size)  # Dolna bariery
]

# Prędkość poruszania się postaci
player_velocity = config.getint('Player', 'Velocity', fallback=5)
velocity = player_velocity

# Flaga włączania/wyłączania minimapy
show_minimap = True

# Początkowa pozycja gracza
player_x, player_y = map_width // 2, map_height // 2

# Zegar do kontroli fps
clock = pygame.time.Clock()

# Funkcja sprawdzająca kolizję
def check_collision(player_x, player_y, obstacle_x, obstacle_y, obstacle_size):
    return (
        player_x < obstacle_x + obstacle_size and
        player_x + player_size > obstacle_x and
        player_y < obstacle_y + obstacle_size and
        player_y + player_size > obstacle_y
    )

# Funkcja sprawdzająca kolizję z bariarami
def check_barrier_collision(player_x, player_y, barrier_x, barrier_y, barrier_width, barrier_height):
    return (
        player_x < barrier_x + barrier_width and
        player_x + player_size > barrier_x and
        player_y < barrier_y + barrier_height and
        player_y + player_size > barrier_y
    )

# Główna pętla gry
running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_g:
                show_minimap = not show_minimap  # Zmiana stanu flagi minimapy po naciśnięciu "g"
            elif event.key == pygame.K_f:
                fullscreen = not fullscreen
                if fullscreen:
                    screen = pygame.display.set_mode((screen_width, screen_height), pygame.FULLSCREEN)
                else:
                    screen = pygame.display.set_mode((screen_width, screen_height))

    keys = pygame.key.get_pressed()
    new_x, new_y = player_x, player_y
    if keys[pygame.K_a]:
        new_x -= velocity
    if keys[pygame.K_d]:
        new_x += velocity
    if keys[pygame.K_w]:
        new_y -= velocity
    if keys[pygame.K_s]:
        new_y += velocity

    # Sprawdzenie kolizji z przeszkodami
    for obstacle in obstacles:
        if check_collision(new_x, new_y, obstacle[0], obstacle[1], obstacle_size):
            # Kolizja, zmniejsz prędkość o połowę
            velocity = player_velocity / 2
            # Tworzenie nowej krotki z nowym indeksem koloru
            obstacle = (obstacle[0], obstacle[1], 1)
            break
    else:
        # Brak kolizji, przywróć pełną prędkość
        velocity = player_velocity
        # Przywrócenie indeksu koloru przeszkód na 0 (zielony)
        obstacles = [(obstacle[0], obstacle[1], 0) for obstacle in obstacles]

    # Sprawdzenie kolizji z bariarami
    for barrier in barriers:
        if check_barrier_collision(new_x, new_y, *barrier):
            # Jeśli kolizja z bariarami, nie pozwól na ruch
            new_x, new_y = player_x, player_y
            break

    player_x, player_y = max(0, min(map_width - player_size, new_x)), max(0, min(map_height - player_size, new_y))

    map_offset_x = max(0, min(map_width - screen_width, player_x - screen_width // 2))
    map_offset_y = max(0, min(map_height - screen_height, player_y - screen_height // 2))

    # Czyszczenie ekranu i rysowanie mapy
    screen.fill(white)

    # Rysowanie widocznych bariery czerwonych na krawędziach mapy
    for barrier in barriers:
        pygame.draw.rect(screen, red, (barrier[0] - map_offset_x, barrier[1] - map_offset_y, barrier[2], barrier[3]))

    # Rysowanie postaci
    pygame.draw.rect(screen, black, (player_x - map_offset_x, player_y - map_offset_y, player_size, player_size))
    for obstacle in obstacles:
        obstacle_color = red if obstacle[2] == 1 else green
        pygame.draw.rect(screen, obstacle_color, (obstacle[0] - map_offset_x, obstacle[1] - map_offset_y, obstacle_size, obstacle_size))

    if show_minimap:
        # Aktualizacja minimapy tylko jeśli jest włączona
        minimap.fill((black))  # Wypełnienie przezroczystym kolorem
        minimap_player_x = (player_x / map_width) * minimap_size
        minimap_player_y = (player_y / map_height) * minimap_size
        pygame.draw.rect(minimap, red, (minimap_player_x, minimap_player_y, player_size / map_width * minimap_size, player_size / map_height * minimap_size))

        # Rysowanie przeszkód na minimapie
        for obstacle in obstacles:
            minimap_obstacle_x = (obstacle[0] / map_width) * minimap_size
            minimap_obstacle_y = (obstacle[1] / map_height) * minimap_size
            minimap_obstacle_size = (obstacle_size / map_width) * minimap_size
            minimap_obstacle_color = red if obstacle[2] == 1 else green
            pygame.draw.rect(minimap, minimap_obstacle_color, (minimap_obstacle_x, minimap_obstacle_y, minimap_obstacle_size, minimap_obstacle_size))

        # Rysowanie widocznych bariery czerwonych na krawędziach minimapy
        for barrier in barriers:
            pygame.draw.rect(minimap, red, (barrier[0] / map_width * minimap_size, barrier[1] / map_height * minimap_size,
                                           barrier[2] / map_width * minimap_size, barrier[3] / map_height * minimap_size))

        # Rysowanie ramki minimapy
        pygame.draw.rect(minimap, white, (0, 0, minimap_size, minimap_size), 2)

        # Rysowanie minimapy na głównym ekranie
        screen.blit(minimap, (screen_width - minimap_size, 0))

    pygame.display.flip()
    clock.tick(60)

pygame.quit()
sys.exit()

