import pygame
import pickle
from os import path

pygame.init()

clock = pygame.time.Clock()
fps = 60

# Game window
tile_size = 50
cols = 24  # Number of columns
rows = 16  # Number of rows
margin = 100
screen_width = tile_size * cols
screen_height = (tile_size * rows) + margin

screen = pygame.display.set_mode((screen_width, screen_height))
pygame.display.set_caption('Level Editor')

# Load images
air_img = pygame.image.load('img/air.png')
sun_img = pygame.image.load('img/sun.png')
sun_img = pygame.transform.scale(sun_img, (tile_size, tile_size))
bg_img = pygame.image.load('img/sky.png')
bg_img = pygame.transform.scale(bg_img, (screen_width, screen_height - margin))
dirt_img = pygame.image.load('img/dirt.png')
grass_img = pygame.image.load('img/grass.png')
blob_img_1 = pygame.image.load('img/blob_1.png')
blob_img_2 = pygame.image.load('img/blob_2.png')
platform_x_img = pygame.image.load('img/platform_x.png')
platform_y_img = pygame.image.load('img/platform_y.png')
platform_img = pygame.image.load('img/platform.png')
lava_img = pygame.image.load('img/lava.png')
coin_img = pygame.image.load('img/coin.png')
exit_img = pygame.image.load('img/exit.png')
save_img = pygame.image.load('img/save_btn.png')
load_img = pygame.image.load('img/load_btn.png')
reset_img = pygame.image.load('img/restart_btn.png')
hearth_img = pygame.image.load('img/hearth.png')

# Define game variables
clicked = False
level = 1

# Define colours
white = (255, 255, 255)
green = (144, 201, 120)

font = pygame.font.SysFont('Futura', 24)

# Create empty tile list
world_data = []
for row in range(rows):
    r = [0] * cols
    world_data.append(r)

# Create border
for tile in range(cols):
    world_data[rows - 1][tile] = 2
    world_data[0][tile] = 1
for tile in range(rows):
    world_data[tile][0] = 1
    world_data[tile][cols - 1] = 1

def draw_text(text, font, text_col, x, y):
    img = font.render(text, True, text_col)
    screen.blit(img, (x, y))

def draw_grid():
    for c in range(cols + 1):  
        pygame.draw.line(screen, white, (c * tile_size, 0), (c * tile_size, screen_height - margin))
    for r in range(rows + 1):  
        pygame.draw.line(screen, white, (0, r * tile_size), (screen_width, r * tile_size))

def draw_world():
    for row in range(rows):
        for col in range(cols):
            if world_data[row][col] == 0: # AIR
                img = pygame.transform.scale(air_img, (tile_size, tile_size))
            elif world_data[row][col] == 1: # DIRT
                img = pygame.transform.scale(dirt_img, (tile_size, tile_size))
                screen.blit(img, (col * tile_size, row * tile_size)) 
            elif world_data[row][col] == 2: # GRASS
                img = pygame.transform.scale(grass_img, (tile_size, tile_size))
                screen.blit(img, (col * tile_size, row * tile_size))
            elif world_data[row][col] == 3: # ENEMY X
                img = pygame.transform.scale(blob_img_1, (tile_size, int(tile_size * 0.75)))
                screen.blit(img, (col * tile_size, row * tile_size + (tile_size * 0.25)))
            elif world_data[row][col] == 4: # ENEMY Y
                img = pygame.transform.scale(blob_img_2, (tile_size, int(tile_size * 0.75)))
                screen.blit(img, (col * tile_size, row * tile_size + (tile_size * 0.25)))
            elif world_data[row][col] == 5: # PLATFORM-X
                img = pygame.transform.scale(platform_x_img, (tile_size, tile_size // 2))
                screen.blit(img, (col * tile_size, row * tile_size))
            elif world_data[row][col] == 6: # PLATFORM-Y
                img = pygame.transform.scale(platform_y_img, (tile_size, tile_size // 2))
                screen.blit(img, (col * tile_size, row * tile_size))
            elif world_data[row][col] == 7: # LAVA
                img = pygame.transform.scale(lava_img, (tile_size, tile_size // 2))
                screen.blit(img, (col * tile_size, row * tile_size + (tile_size // 2)))
            elif world_data[row][col] == 8: # COIN
                img = pygame.transform.scale(coin_img, (tile_size // 2, tile_size // 2))
                screen.blit(img, (col * tile_size + (tile_size // 4), row * tile_size + (tile_size // 4)))
            elif world_data[row][col] == 9: # EXIT
                img = pygame.transform.scale(exit_img, (tile_size, int(tile_size * 1.5)))
                screen.blit(img, (col * tile_size, row * tile_size - (tile_size // 2)))
            elif world_data[row][col] == 10: # HEARTH
                img = pygame.transform.scale(hearth_img, (tile_size // 2, tile_size // 2))
                screen.blit(img, (col * tile_size + (tile_size // 4), row * tile_size + (tile_size // 4)))
            elif world_data[row][col] == 11: # PLATFORM
                img = pygame.transform.scale(platform_img, (tile_size, tile_size // 2))
                screen.blit(img, (col * tile_size, row * tile_size))


class Button():
    def __init__(self, x, y, image):
        self.image = image
        self.rect = self.image.get_rect()
        self.rect.topleft = (x, y)
        self.clicked = False

    def draw(self):
        action = False
        pos = pygame.mouse.get_pos()
        if self.rect.collidepoint(pos):
            if pygame.mouse.get_pressed()[0] == 1 and not self.clicked:
                action = True
                self.clicked = True
        if pygame.mouse.get_pressed()[0] == 0:
            self.clicked = False
        screen.blit(self.image, (self.rect.x, self.rect.y))
        return action

class ResetButton():
    def __init__(self, x, y, image):
        self.image = image
        self.rect = self.image.get_rect()
        self.rect.topleft = (x, y)
        self.clicked = False

    def draw(self):
        action = False
        pos = pygame.mouse.get_pos()
        if self.rect.collidepoint(pos):
            if pygame.mouse.get_pressed()[0] == 1 and not self.clicked:
                action = True
                self.clicked = True
        if pygame.mouse.get_pressed()[0] == 0:
            self.clicked = False
        screen.blit(self.image, (self.rect.x, self.rect.y))
        return action

# Create load, save, and reset buttons
save_button = Button(screen_width // 2 - 200, screen_height - 80, save_img)
load_button = Button(screen_width // 2 + 100, screen_height - 80, load_img)
reset_button = ResetButton(screen_width // 2 - 65, screen_height - 80, reset_img)

# Main game loop
run = True
while run:
    clock.tick(fps)

    screen.fill(green)
    screen.blit(bg_img, (0, 0))
    screen.blit(sun_img, (tile_size * 2, tile_size * 2))

    if save_button.draw():
        pickle_out = open(f'level{level}_data', 'wb')
        pickle.dump(world_data, pickle_out)
        pickle_out.close()
    if load_button.draw():
        if path.exists(f'level{level}_data'):
            pickle_in = open(f'level{level}_data', 'rb')
            world_data = pickle.load(pickle_in)
    if reset_button.draw():
        world_data = []
        for row in range(rows):
            r = [0] * cols
            world_data.append(r)
        for tile in range(cols):
            world_data[rows - 1][tile] = 2
            world_data[0][tile] = 1
        for tile in range(rows):
            world_data[tile][0] = 1
            world_data[tile][cols - 1] = 1

        for tile in range(rows - 2):
            for t in range(cols - 2):
                world_data[tile + 1][t + 1] = 0

    draw_grid()
    draw_world()

    draw_text(f'Level: {level}', font, white, tile_size, screen_height - 60)
    draw_text('Press UP or DOWN to change level', font, white, tile_size, screen_height - 40)

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            run = False
        if event.type == pygame.MOUSEBUTTONDOWN and not clicked:
            clicked = True
            pos = pygame.mouse.get_pos()
            x = pos[0] // tile_size
            y = pos[1] // tile_size
            if x < cols and y < rows:
                if pygame.mouse.get_pressed()[0] == 1:
                    world_data[y][x] += 1
                    if world_data[y][x] > 11:
                        world_data[y][x] = 0
                elif pygame.mouse.get_pressed()[2] == 1:
                    world_data[y][x] -= 1
                    if world_data[y][x] < 0:
                        world_data[y][x] = 11
                elif pygame.mouse.get_pressed()[1] == 1:
                    world_data[y][x] = 0
        if event.type == pygame.MOUSEBUTTONUP:
            clicked = False
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_UP:
                level += 1
            elif event.key == pygame.K_DOWN and level > 1:
                level -= 1

    pygame.display.update()

pygame.quit()
