import pygame
import os

try:
    # Initialize Pygame
    pygame.init()

    # Set the path to your music file
    music_file = "/home/user/projects/game/img/music.mp3"

    # Check if the file exists
    if not os.path.isfile(music_file):
        raise FileNotFoundError(f"Music file not found: {music_file}")

    # Load the music file
    pygame.mixer.music.load(music_file)

    # Set the volume (optional)
    pygame.mixer.music.set_volume(0.5)  # Adjust the volume as needed (0.0 to 1.0)

    # Play the music in an infinite loop
    pygame.mixer.music.play(loops=-1)

    # Run an infinite loop to keep the program running
    running = True
    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False

finally:
    # Quit Pygame in the end, even if an exception occurs
    pygame.quit()

