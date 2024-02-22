
import pygame
from pygame.locals import *
import random
import configparser

pygame.init()

config = configparser.ConfigParser()

config.read('config')

clock = pygame.time.Clock()
fps = int(config.get('Settings', 'fps'))

screen_width = int(config.get('Settings', 'screen_width'))
screen_height = int(config.get('Settings', 'screen_height'))

name = config.get('Settings', 'name')
screen = pygame.display.set_mode((screen_width, screen_height))
pygame.display.set_caption(name)

#define font
font = pygame.font.SysFont('Bauhaus 93', 60)

#define colors
white = (255, 255, 255)

ground_scroll = int(config.get('Settings', 'ground_scroll'))
scroll_speed = int(config.get('Settings', 'scroll_speed'))
pipe_gap = int(config.get('Settings', 'pipe_gap'))
pipe_frequency = int(config.get('Settings', 'pipe_freq'))
last_pipe = pygame.time.get_ticks() - int(config.get('Settings', 'pipe_freq'))
flying = False
game_over = False
score = 0
pass_pipe = False

#load images
bg = pygame.image.load('img/wtc.jpg')
ground_img = pygame.image.load('img/ground.png')
button_img = pygame.image.load('img/restart.png')

pygame.mixer.music.load('img/music.mp3')
pygame.mixer.music.set_volume(0.5)
#pygame.mixer.music.play(loops=-1)

def draw_text(text, font, text_col, x, y):
	img = font.render(text, True, text_col)
	screen.blit(img, (x, y))


def reset_game():
	pipe_group.empty()
	flappy.rect.x = 100
	flappy.rect.y = int(screen_height / 2)
	score = 0
	return score



class Bird(pygame.sprite.Sprite):
	def __init__(self, x, y):
		pygame.sprite.Sprite.__init__(self)
		self.images = []
		self.index = 0
		self.counter = 0
		#img = pygame.image.load('img/plane1.png')
		for num in range(1, 4):
		    img = pygame.image.load(f'img/plane{num}.png')
		    self.images.append(img)
		self.image = self.images[self.index]
		self.rect = self.image.get_rect()
		self.rect.center = [x, y]
		self.vel = 0
		self.clicked = False

	def update(self):

		if flying == True:
			#gravity
			self.vel += 0.5
			if self.vel > 8:
				self.vel = 8
			if self.rect.bottom < 768:
				self.rect.y += int(self.vel)

		if game_over == False:
			#jump
			if pygame.mouse.get_pressed()[0] == 1 and self.clicked == False:
				self.clicked = True
				self.vel = -10
			if pygame.mouse.get_pressed()[0] == 0:
				self.clicked = False

			#handle the animation
			self.counter += 1
			flap_cooldown = 5

			if self.counter > flap_cooldown:
				self.counter = 0
				self.index += 1
				if self.index >= len(self.images):
					self.index = 0
			self.image = self.images[self.index]

			#rotate the bird
			self.image = pygame.transform.rotate(self.images[self.index], self.vel * -2)
		else:
			self.image = pygame.transform.rotate(self.images[self.index], -90)



class Pipe(pygame.sprite.Sprite):
	def __init__(self, x, y, position):
		pygame.sprite.Sprite.__init__(self)
		self.image = pygame.image.load('img/build.jpg')
		self.rect = self.image.get_rect()
		#position 1 is from the top, -1 is from the bottom
		if position == 1:
			self.image = pygame.transform.flip(self.image, False, True)
			self.rect.bottomleft = [x, y - int(pipe_gap / 2)]
		if position == -1:
			self.rect.topleft = [x, y + int(pipe_gap / 2)]

	def update(self):
		self.rect.x -= scroll_speed
		if self.rect.right < 0:
			self.kill()


class Button():
	def __init__(self, x, y, image):
		self.image = image
		self.rect = self.image.get_rect()
		self.rect.topleft = (x, y)

	def draw(self):

		action = False

		#get mouse position
		pos = pygame.mouse.get_pos()

		#check if mouse is over the button
		if self.rect.collidepoint(pos):
			if pygame.mouse.get_pressed()[0] == 1:
				action = True

		#draw button
		screen.blit(self.image, (self.rect.x, self.rect.y))

		return action

bird_group = pygame.sprite.Group()
pipe_group = pygame.sprite.Group()

flappy = Bird(100, int(screen_height / 2))

bird_group.add(flappy)

#create restart button instance
button = Button(screen_width // 2 - 50, screen_height // 2 - 100, button_img)

pygame.mixer.music.play(loops=-1)

#sound = pygame.mixer.Sound('img/music.mp3')

run = True
while run:

	#pygame.mixer.music.play(loops=-1)  # Play in an infinite loop
	#pygame.mixer.music.play(loops=-1)
	clock.tick(fps)

	#draw background
	screen.blit(bg, (0,0))

	bird_group.draw(screen)
	bird_group.update()
	pipe_group.draw(screen)

	#draw the ground
	screen.blit(ground_img, (ground_scroll, 768))

	#check the score
	if len(pipe_group) > 0:
		if bird_group.sprites()[0].rect.left > pipe_group.sprites()[0].rect.left\
			and bird_group.sprites()[0].rect.right < pipe_group.sprites()[0].rect.right\
			and pass_pipe == False:
			pass_pipe = True
		if pass_pipe == True:
			if bird_group.sprites()[0].rect.left > pipe_group.sprites()[0].rect.right:
				score += 1
				pass_pipe = False


	draw_text(str(score), font, white, int(screen_width / 2), 20)

	#look for collision
	if pygame.sprite.groupcollide(bird_group, pipe_group, False, False) or flappy.rect.top < 0:
		game_over = True

	#check if bird has hit the ground
	if flappy.rect.bottom >= 768:
		game_over = True
		flying = False


	if game_over == False and flying == True:
		#pygame.mixer.music.play(loops=-1)
		#generate new pipes
		time_now = pygame.time.get_ticks()
		if time_now - last_pipe > pipe_frequency:
			pipe_height = random.randint(-100, 100)
			btm_pipe = Pipe(screen_width, int(screen_height / 2) + pipe_height, -1)
			top_pipe = Pipe(screen_width, int(screen_height / 2) + pipe_height, 1)
			pipe_group.add(btm_pipe)
			pipe_group.add(top_pipe)
			last_pipe = time_now


		#draw and scroll the ground
		ground_scroll -= scroll_speed
		if abs(ground_scroll) > 35:
			ground_scroll = 0

		pipe_group.update()


	#check for game over and reset
	if game_over == True:
		pygame.mixer.music.pause()
		if button.draw() == True:
			pygame.mixer.music.rewind()
			pygame.mixer.music.unpause()
			game_over = False
			score = reset_game()



	for event in pygame.event.get():
		if event.type == pygame.QUIT:
			run = False
		if event.type == pygame.MOUSEBUTTONDOWN and flying == False and game_over == False:
			flying = True

	pygame.display.update()

pygame.quit()
