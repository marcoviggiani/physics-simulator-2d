import math
import random
import sys
import os

# Percorso relativo: funziona su qualsiasi PC dopo aver compilato in x64-release
PYD_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                       "..", "out", "build", "x64-release", "Simulatore_Fisica_2D")
sys.path.insert(0, PYD_DIR)

import pygame
import fisica

# ---------- Parametri ----------
WORLD_W, WORLD_H = 20.0, 20.0      # dimensioni del mondo in metri (come World in C++)
SCALE = 30                         # pixel per metro
WIDTH, HEIGHT = int(WORLD_W * SCALE), int(WORLD_H * SCALE)
FPS = 60
SUBSTEPS = 4                       # passi di fisica per ogni frame (più stabile)
DT = 1.0 / FPS
MAX_BODIES = 120                   # il motore è O(n^2): oltre diventa lento


def make_world():
    w = fisica.World()
    w.width, w.height = WORLD_W, WORLD_H
    w.gravity = fisica.Vec2(0.0, -9.8)    # y verso l'alto, come nel motore
    return w


def spawn_ball(world, colors, x, y):
    """Aggiunge una pallina casuale in (x, y) espressa in metri."""
    if len(world.bodies) >= MAX_BODIES:
        return
    b = fisica.Body()
    b.radius = random.uniform(0.3, 0.7)
    b.mass = math.pi * b.radius ** 2          # massa proporzionale all'area
    b.restitution = 0.9
    b.position = fisica.Vec2(x, y)
    b.velocity = fisica.Vec2(random.uniform(-5, 5), random.uniform(-5, 5))
    try:
        world.add_body(b)                     # in C++ lancia invalid_argument -> ValueError
    except ValueError as e:
        print("Pallina rifiutata:", e)
        return
    colors.append((random.randint(80, 255), random.randint(80, 255), random.randint(80, 255)))


def to_screen(x, y):
    """Metri (y verso l'alto) -> pixel (y verso il basso)."""
    return int(x * SCALE), int(HEIGHT - y * SCALE)


def to_world(px, py):
    return px / SCALE, (HEIGHT - py) / SCALE


def main():
    pygame.init()
    screen = pygame.display.set_mode((WIDTH, HEIGHT))
    pygame.display.set_caption("Simulatore fisico 2D (C++ + Python)")
    font = pygame.font.SysFont("consolas", 16)
    clock = pygame.time.Clock()

    world = make_world()
    colors = []
    for _ in range(10):
        spawn_ball(world, colors, random.uniform(2, 18), random.uniform(10, 18))

    paused = False
    running = True
    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE:
                    running = False
                elif event.key == pygame.K_SPACE:
                    paused = not paused
                elif event.key == pygame.K_r:
                    world = make_world()
                    colors.clear()
            elif event.type == pygame.MOUSEBUTTONDOWN and event.button == 1:
                wx, wy = to_world(*event.pos)
                spawn_ball(world, colors, wx, wy)

        # ---- fisica: tutto il calcolo pesante è in C++ ----
        if not paused:
            for _ in range(SUBSTEPS):
                world.step(DT / SUBSTEPS)

        # ---- disegno ----
        screen.fill((20, 22, 30))
        for i, b in enumerate(world.bodies):      # b è un riferimento reale al corpo C++
            cx, cy = to_screen(b.position.x, b.position.y)
            r = max(1, int(b.radius * SCALE))
            pygame.draw.circle(screen, colors[i], (cx, cy), r)
            pygame.draw.circle(screen, (255, 255, 255), (cx, cy), r, 1)

        hud = f"palline: {len(world.bodies)}  FPS: {clock.get_fps():.0f}  " \
              f"[click] aggiungi  [spazio] pausa  [R] reset"
        screen.blit(font.render(hud, True, (200, 200, 200)), (8, 8))
        pygame.display.flip()
        clock.tick(FPS)

    pygame.quit()


if __name__ == "__main__":
    main()