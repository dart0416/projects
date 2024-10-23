import pygame 
from pygame import *
from pygame import mixer
import pickle
from os import path

# import pygame.music

pygame.mixer.pre_init(44100, -16, 2, 512)
mixer.init()
pygame.init()

clock = pygame.time.Clock()
fps = 60

screen_width = 1200
screen_height = 800

screen = pygame.display.set_mode((screen_width, screen_height))
pygame.display.set_caption('Platformer')

# Define font
font = pygame.font.SysFont('Bauhaus 93', 70)
font_score = pygame.font.SysFont('Bauhaus 93', 35)


# Define variables
main_menu = True
tile_size = 50
game_over = 0
level = 1
max_levels = 7
global_score = 0
score = 0
start_lives = 2
lives = 2
speaker_on = True
is_rocket_used = False

# Define colors
white = (255, 255, 255)
black = (0, 0, 0)
red = (255, 0, 0)
green = (0, 255, 0)
blue = (0, 0, 255)


# Load images
bg_img = pygame.image.load('img/sky.png')
restart_img = pygame.image.load('img/restart_btn.png')
start_img = pygame.image.load('img/start_btn.png')
exit_img = pygame.image.load('img/exit_btn.png')
exit_img_2 = pygame.image.load('img/exit_btn_2.png')
speaker_on_img = pygame.image.load('img/speaker_on.png')
speaker_off_img = pygame.image.load('img/speaker_off.png')

new_icon = pygame.image.load('img/icon.png')
pygame.display.set_icon(new_icon)

# Load sounds
pygame.mixer.music.load('sound/music.mp3')
pygame.mixer.music.play(-1, 0.0, 5000)
coin_fx = pygame.mixer.Sound('sound/coin.wav')
coin_fx.set_volume(0.5)
jump_fx = pygame.mixer.Sound('sound/jump.wav')
jump_fx.set_volume(0.5)
game_over_fx = pygame.mixer.Sound('sound/game_over.wav')
game_over_fx.set_volume(0.5)

sounds = [coin_fx, jump_fx, game_over_fx]

def draw_text(text, font, text_color, x, y):
    img = font.render(text, True, text_color)
    screen.blit(img, (x, y))


def draw_grid():
    for line in range(0, 24):
        pygame.draw.line(screen, (0, 0, 0), (0, line * tile_size), (screen_width, line * tile_size))
        pygame.draw.line(screen, (0, 0, 0), (line * tile_size, 0), (line * tile_size, screen_height))



# Level reset functions
def reset_level(level):
    player.reset(100, screen_height - 130)
    enemy_group.empty()
    lava_group.empty()
    exit_group.empty()
    hearth_group.empty()
    coin_group.empty()
    platform_group.empty()

    # Load level data and create it
    if path.exists(f'level{level}_data'):
        with open(f'level{level}_data', 'rb') as pickle_in:
            world_data = pickle.load(pickle_in)
    world = World(world_data)
    return world


class Button():
    def __init__(self, x, y, image):
        self.image = image
        self.rect = self.image.get_rect()
        self.rect.x = x
        self.rect.y = y
        self.clicked = False

    def draw(self):
        action = False

        # Get mouse position
        pos = pygame.mouse.get_pos()

        # Check mouse
        if self.rect.collidepoint(pos):
            if pygame.mouse.get_pressed()[0] == 1 and self.clicked == False:
                action = True
                self.clicked = True
        if pygame.mouse.get_pressed()[0] == 0:
            self.clicked = False

        # Draw button
        screen.blit(self.image, self.rect)

        return action


class Player():
    def __init__(self, x, y):
        self.reset(x, y)


    def update(self, game_over, lives):

        dx = 0
        dy = 0
        walk_cooldown = 15
        col_thresh = 50


        if game_over == 0:
            # Get keypresses
            key = pygame.key.get_pressed()
            if key[pygame.K_SPACE] and self.jumped == False and self.in_air == False:
                jump_fx.play()
                self.vel_y = -15
                self.jumped = True

            if key[pygame.K_SPACE] == False:
                self.jumped = False

            if key[pygame.K_LEFT]:
                dx -= 5
                self.counter +=1
                self.direction = -1

            if key[pygame.K_RIGHT]:
                dx += 5
                self.counter +=1
                self.direction = 1

            if key[pygame.K_RIGHT] == False and key[pygame.K_LEFT] == False:
                self.counter = 0
                self.index = 0
                if self.direction == 1:
                    self.image = self.images_right[self.index]
                if self.direction == -1:
                    self.image = self.images_left[self.index]

            # if key[pygame.K_RETURN] and is_rocket_used == False and self.jumped == False and self.in_air == False:
            #     self.jumped = True 
            #     self.vel_y = -35
            #     self.counter +=1
            #     self.direction = 1
            #     dx = 0
            #     dy = 0

            # Handle animation
            self.counter += 1
            if self.counter > walk_cooldown:
                self.counter = 0
                self.index += 1
                if self.index >= len(self.images_right):
                    self.index = 0
                if self.direction == 1:
                    self.image = self.images_right[self.index]
                if self.direction == -1:
                    self.image = self.images_left[self.index]


            # Add gravity
            self.vel_y += 1
            if self.vel_y > 8:
                self.vel_y = 8
            dy += self.vel_y


            # Check for collisions
            self.in_air = True
            for tile in world.tile_list:
                # Check for collisions in x
                if tile[1].colliderect(self.rect.x + dx, self.rect.y, self.width, self.height):
                    dx = 0

                # Check for collision in y
                if tile[1].colliderect(self.rect.x, self.rect.y + dy, self.width, self.height):
                    # Check if below the ground
                    if self.vel_y < 0:
                        dy = tile[1].bottom - self.rect.top
                        self.vel_y = 0
                    
                    # Check if above the ground
                    elif self.vel_y >= 0:
                        dy = tile[1].top - self.rect.bottom
                        self.vel_y = 0
                        self.in_air = False

            # Check for collisioon with enemies
            if pygame.sprite.spritecollide(self, enemy_group, False):
                game_over = -1

            # Check for collisioon with lava
            if pygame.sprite.spritecollide(self, lava_group, False):
                game_over = -1

            # Check for collisioon with hearth
            if pygame.sprite.spritecollide(self, exit_group, False):
                if lives < 5:
                    lives += 1

            # Check for collisioon with exit
            if pygame.sprite.spritecollide(self, exit_group, False):
                game_over = 1

            # MY
            # Check for collision with platforms
            for platform in platform_group:
                #collision in x
                if platform.rect.colliderect(self.rect.x + dx, self.rect.y, self.width, self.height):
                    dx = 0
                #collision in y
                if platform.rect.colliderect(self.rect.x, self.rect.y + dy, self.width, self.height):
                    #check if below platform
                    if abs((self.rect.top + dy) - platform.rect.bottom) < col_thresh:
                        self.vel_y = 0
                        dy = platform.rect.bottom - self.rect.top
                    #chech if above
                    elif abs((self.rect.bottom + dy) - platform.rect.top) < col_thresh:
                        self.rect.bottom = platform.rect.top -1
                        self.in_air = False
                        dy = 0
                    #move L & R with platform
                    if platform.move_x != 0:
                        self.rect.x += platform.move_direction


            #  #check for collision with platforms
            #     for platform in platform_group:
			# 	#collision in the x direction
            #         if platform.rect.colliderect(self.rect.x + dx, self.rect.y, self.width, self.height):
            #             dx = 0
			# 	#collision in the y direction
            #     if platform.rect.colliderect(self.rect.x, self.rect.y + dy, self.width, self.height):
			# 		#check if below platform
            #         if abs((self.rect.top + dy) - platform.rect.bottom) < col_thresh:
            #             self.vel_y = 0
            #             dy = platform.rect.bottom - self.rect.top
			# 		#check if above platform
            #         elif abs((self.rect.bottom + dy) - platform.rect.top) < col_thresh:
            #             self.rect.bottom = platform.rect.top - 1
            #             self.in_air = False
            #             dy = 0
                




            # Update player coords
            self.rect.x += dx
            self.rect.y += dy

            if self.rect.bottom > screen_height:
                self.rect.bottom = screen_height


        elif game_over == -1:
            self.image = self.dead_image
            draw_text('Game over', font, red, screen_width // 2 - 120, screen_height // 2)
            self.rect.y -= 10



        # Draw player
        screen.blit(self.image, self.rect)

        # player hitbox
        #pygame.draw.rect(screen, (255, 0, 0), self.rect, 2)

        return game_over

    def reset(self, x, y):
        self.images_right = []
        self.images_left = []
        self.index = 0
        self.health = lives
        self.counter = 0
        for num in range(1, 5):
            img_right = pygame.image.load(f'img/guy{num}.png')
            img_right = pygame.transform.scale(img_right, (40, 50))
            img_left = pygame.transform.flip(img_right, True, False)
            self.images_right.append(img_right)
            self.images_left.append(img_left)
        self.dead_image = pygame.image.load('img/ghost.png')
        self.image = self.images_right[self.index]
        self.rect = self.image.get_rect()
        self.rect.x = x
        self.rect.y = y
        self.width = self.image.get_width()
        self.height = self.image.get_height()
        self.vel_y = 0
        self.jumped = False
        self.direction = 0
        self.in_air = True



class World():
    def __init__(self, data):
        self.tile_list = []

        # Load images
        dirt_img = pygame.image.load('img/dirt.png')
        grass_img = pygame.image.load('img/grass.png')
        air_img = pygame.image.load('img/air.png')

        row_count = 0
        for row in data:
            col_count = 0
            for tile in row:
                # if tile == 0:  # For air
                #     pygame.draw.rect(screen, (0, 100, 255), (50, 50, 162, 100), 3)  # width = 3
                #     img = pygame.transform.scale(air_img, (tile_size, tile_size))
                #     img_rect = img.get_rect()
                #     img_rect.x = col_count * tile_size
                #     img_rect.y = row_count * tile_size
                #     tile = (img, img_rect)
                #     self.tile_list.append(tile)

                if tile == 1:  # For dirt
                    img = pygame.transform.scale(dirt_img, (tile_size, tile_size))
                    img_rect = img.get_rect()
                    img_rect.x = col_count * tile_size
                    img_rect.y = row_count * tile_size
                    tile = (img, img_rect)
                    self.tile_list.append(tile)

                if tile == 2:  # For grass
                    img = pygame.transform.scale(grass_img, (tile_size, tile_size))
                    img_rect = img.get_rect()
                    img_rect.x = col_count * tile_size
                    img_rect.y = row_count * tile_size
                    tile = (img, img_rect)
                    self.tile_list.append(tile)

                if tile == 3:
                    blob = Enemy_1(col_count * tile_size, row_count * tile_size + 15)
                    enemy_group.add(blob)

                if tile == 4:
                    blob = Enemy_2(col_count * tile_size, row_count * tile_size + 15)
                    enemy_group.add(blob)

                if tile == 5:
                    platform = Platform(col_count * tile_size, row_count * tile_size, 1, 0, 50)
                    platform_group.add(platform)

                if tile == 6:
                    platform = Platform(col_count * tile_size, row_count * tile_size, 0, 1, 50)
                    platform_group.add(platform)

                if tile == 7:
                    lava = Lava(col_count * tile_size, row_count * tile_size + (tile_size // 2))
                    lava_group.add(lava)

                if tile == 8:
                    coin = Coin(col_count * tile_size + (tile_size // 2), row_count * tile_size + (tile_size // 2))
                    coin_group.add(coin)

                if tile == 9:
                    exit = Exit(col_count * tile_size, row_count * tile_size - (tile_size // 2))
                    exit_group.add(exit)

                if tile == 10:
                    hearth = Hearth(col_count * tile_size + (tile_size // 2), row_count * tile_size + (tile_size // 2))
                    hearth_group.add(hearth)
                
                if tile == 11:
                    platform = Platform(col_count * tile_size, row_count * tile_size, 0, 0, 0)
                    platform_group.add(platform)

                col_count += 1
            row_count += 1

        
    def draw(self):
        for tile in self.tile_list:
            screen.blit(tile[0], tile[1])

            # game grid
            # pygame.draw.rect(screen, (255, 255, 255), tile[1], 2)

class Enemy_1(pygame.sprite.Sprite):
    def __init__(self, x, y):
        pygame.sprite.Sprite.__init__(self)
        self.image = pygame.image.load('img/blob_1.png')
        self.rect = self.image.get_rect()
        self.rect.x = x
        self.rect.y = y
        self.move_direction = 1
        self.move_counter = 0

    def update(self):
        self.rect.x += self.move_direction
        self.move_counter += 1
        if abs(self.move_counter) > 50:
            self.move_direction *= -1
            self.move_counter *= -1

class Enemy_2(pygame.sprite.Sprite):
    def __init__(self, x, y):
        pygame.sprite.Sprite.__init__(self)
        self.image = pygame.image.load('img/blob_2.png')
        self.rect = self.image.get_rect()
        self.rect.x = x
        self.rect.y = y
        self.move_direction = 1
        self.move_counter = 0

    def update(self):
        self.rect.y += self.move_direction
        self.move_counter += 1
        if abs(self.move_counter) > 50:
            self.move_direction *= -1
            self.move_counter *= -1

class Platform(pygame.sprite.Sprite):
    def __init__(self, x, y, move_x, move_y, distance):
        pygame.sprite.Sprite.__init__(self)
        img = pygame.image.load('img/platform.png')
        self.image = pygame.transform.scale(img, (tile_size, tile_size // 2))
        self.rect = self.image.get_rect()
        self.rect.x = x
        self.rect.y = y
        self.move_counter = 0
        self.move_direction = 1
        self.move_x = move_x
        self.move_y = move_y
        self.distance = distance
    
    def update(self):
        self.rect.x += self.move_direction * self.move_x
        self.rect.y += self.move_direction * self.move_y
        self.move_counter += 1
        if abs(self.move_counter) > self.distance:
            self.move_direction *= -1
            self.move_counter *= -1

class Lava(pygame.sprite.Sprite):
    def __init__(self, x, y):
        pygame.sprite.Sprite.__init__(self)
        img = pygame.image.load('img/lava.png')
        self.image = pygame.transform.scale(img, (tile_size, tile_size // 2))
        self.rect = self.image.get_rect()
        self.rect.x = x
        self.rect.y = y

class Coin(pygame.sprite.Sprite):
    def __init__(self, x, y):
        pygame.sprite.Sprite.__init__(self)
        img = pygame.image.load('img/coin.png')
        self.image = pygame.transform.scale(img, (tile_size // 2, tile_size // 2))
        self.rect = self.image.get_rect()
        self.rect.center = (x, y)

class Hearth(pygame.sprite.Sprite):
    def __init__(self, x, y):
        pygame.sprite.Sprite.__init__(self)
        img = pygame.image.load('img/hearth.png')
        self.image = pygame.transform.scale(img, (tile_size // 2, tile_size // 2))
        self.rect = self.image.get_rect()
        self.rect.center = (x, y)

class Exit(pygame.sprite.Sprite):
    def __init__(self, x, y):
        pygame.sprite.Sprite.__init__(self)
        img = pygame.image.load('img/exit.png')
        self.image = pygame.transform.scale(img, (tile_size, int(tile_size * 1.5)))
        self.rect = self.image.get_rect()
        self.rect.x = x
        self.rect.y = y



player = Player(105, screen_height - 100)

enemy_group = pygame.sprite.Group()
platform_group = pygame.sprite.Group()
lava_group = pygame.sprite.Group()
coin_group = pygame.sprite.Group()
hearth_group = pygame.sprite.Group()
exit_group = pygame.sprite.Group()
display_group = pygame.sprite.Group()

# Create coin and hearth for show score
score_coin = Coin(tile_size // 2 - 5 , tile_size // 2 - 5 )
display_group.add(score_coin)

hearth_display = Hearth(int(tile_size + 75), tile_size // 2 )
display_group.add(hearth_display)

# Load level data and create it
if path.exists(f'level{level}_data'):
    pickle_in = open(f'level{level}_data', 'rb')
    world_data = pickle.load(pickle_in)
world = World(world_data)

# Create buttons
restart_button = Button(screen_width // 2 + 50, screen_height // 2 + 100, restart_img)
start_button = Button(screen_width // 2 - 350, screen_height // 2, start_img)
exit_button = Button(screen_width // 2 + 50, screen_height // 2 ,exit_img)
exit_button_2 = Button(screen_width // 2 - 150, screen_height // 2 + 100 , exit_img_2)
speaker_on_button = Button(screen_width -1185, screen_height -750, speaker_on_img)
speaker_off_button = Button(screen_width -1185, screen_height -750, speaker_off_img)

# Initial level setup
# Initial level setup
world = reset_level(level)

run = True
while run:
    clock.tick(fps)
    screen.blit(bg_img, (0, 0))

    if main_menu:
        if exit_button.draw():
            run = False
        if start_button.draw():
            main_menu = False
            world = reset_level(level)
    
        if speaker_on:
            if speaker_on_button.draw():
                pygame.mixer.music.pause()
                speaker_on = False
        else:
            if speaker_off_button.draw():
                pygame.mixer.music.unpause()
                speaker_on = True

    else:
        world.draw()

        

        if game_over == 0:
            enemy_group.update()
            platform_group.update()

            # Check if coin has been collected
            if pygame.sprite.spritecollide(player, coin_group, True):
                coin_fx.play()
                score += 1
            if pygame.sprite.spritecollide(player, hearth_group, True) and lives < 5:
                lives += 1

            draw_text('x ' + str(global_score + score), font_score, white, tile_size - 10, 10)
            draw_text('x ' + str(lives), font_score, white, tile_size + 100, 10)

            coin_group.draw(screen)
            hearth_group.draw(screen)
            enemy_group.draw(screen)
            platform_group.draw(screen)
            lava_group.draw(screen)
            exit_group.draw(screen)
            display_group.draw(screen)

            coin_group.update()

            game_over = player.update(game_over, lives)

            
            if speaker_on:
                if speaker_on_button.draw():
                    pygame.mixer.music.pause()
                    speaker_on = False
            else:
                if speaker_off_button.draw():
                    pygame.mixer.music.unpause()
                    speaker_on = True



        elif game_over == -1: # fail
            if lives <= 1: # fail permanent
                player.image = player.dead_image
                draw_text('Game over', font, red, screen_width // 2 - 120, screen_height // 2 - 80)
                draw_text('FOR REAL', font, black, screen_width // 2 - 120, screen_height // 2)
                pygame.mixer.music.stop()
                if restart_button.draw():
                    level = 1
                    world = reset_level(level)
                    game_over = 0
                    score = 0
                    global_score = 0
                    lives = start_lives  # Reset lives if needed
                if exit_button_2.draw():
                        run = False
            else:
                player.image = player.dead_image
                draw_text('Game over', font, red, screen_width // 2 - 120, screen_height // 2)
                if restart_button.draw():
                    world = reset_level(level)
                    game_over = 0
                    score = 0
                    lives -= 1  # Reset lives if needed
                if exit_button_2.draw():
                        run = False
            pygame.mixer.music.stop()
            game_over_fx.play()

        elif game_over == 1: # not fail
            level += 1
            if level <= max_levels:
                world = reset_level(level)
                game_over = 0
                global_score += score
                score = 0
            else:
                draw_text('You Won!', font, blue, screen_width // 2 - 120, screen_height // 2) # won
                pygame.mixer.music.stop()
                if restart_button.draw():
                    level = 1
                    world = reset_level(level)
                    game_over = 0
                    score = 0
                    global_score = 0
                    lives = start_lives
                if exit_button_2.draw():
                    run =  False

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            run = False

    pygame.display.update()

pygame.quit()