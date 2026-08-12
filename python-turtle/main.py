import json
import turtle
from pathlib import Path


# =========================================================
# CONFIGURATION GENERALE
# =========================================================

WINDOW_WIDTH = 1200
WINDOW_HEIGHT = 900

DISPLAY_LENGTH = 700

MIN_ITERATION = 1
MAX_ITERATION = 3

# Durée d'affichage de chaque niveau en millisecondes
ITERATION_DELAY_MS = 2000


# =========================================================
# RECHERCHE AUTOMATIQUE DU MOTIF
# =========================================================

def find_pattern_file():
    pattern_dir = Path(__file__).parent / "patterns"

    json_files = list(pattern_dir.glob("*.json"))

    if len(json_files) == 0:
        raise FileNotFoundError(
            "Aucun fichier JSON dans le dossier patterns/"
        )

    if len(json_files) > 1:
        raise RuntimeError(
            "Conservez un seul fichier JSON dans patterns/"
        )

    return json_files[0]


# =========================================================
# CONVERSION DES FRACTIONS
# =========================================================

def parse_fraction(value):
    if "/" in value:
        numerator, denominator = value.split("/")

        return (
            float(numerator)
            / float(denominator)
        )

    return float(value)


# =========================================================
# CHARGEMENT DU DICTIONNAIRE
# =========================================================

def load_pattern():
    file_path = find_pattern_file()

    print(
        f"Motif source : {file_path.name}"
    )

    with open(
        file_path,
        "r",
        encoding="utf-8"
    ) as file:
        data = json.load(file)

    points = []

    for point_data in data["points"]:

        x = parse_fraction(
            point_data["normalized"][0]
        )

        y = parse_fraction(
            point_data["normalized"][1]
        )

        points.append((x, y))

    print(
        f"{len(points)} points charges"
    )

    return points


# =========================================================
# TRANSFORMATION D'UN POINT SUR UN SEGMENT
# =========================================================

def map_point_to_segment(
    point,
    start,
    end
):
    x, y = point

    ax, ay = start
    bx, by = end

    dx = bx - ax
    dy = by - ay

    return (
        ax + x * dx - y * dy,
        ay + x * dy + y * dx
    )


# =========================================================
# SUBSTITUTION
# =========================================================

def substitute(
    segments,
    pattern
):
    result = []

    for start, end in segments:

        transformed = [
            map_point_to_segment(
                point,
                start,
                end
            )
            for point in pattern
        ]

        for i in range(
            len(transformed) - 1
        ):
            result.append(
                (
                    transformed[i],
                    transformed[i + 1]
                )
            )

    return result


# =========================================================
# GENERATION D'UN ORDRE FRACTAL
# =========================================================

def generate_fractal(
    pattern,
    iteration
):
    segments = [
        (
            (0.0, 0.0),
            (1.0, 0.0)
        )
    ]

    for _ in range(iteration):

        segments = substitute(
            segments,
            pattern
        )

    return segments


# =========================================================
# CONVERSION VERS LE REPERE TURTLE
# =========================================================

def to_screen(point):
    x, y = point

    screen_x = (
        x * DISPLAY_LENGTH
        - DISPLAY_LENGTH / 2
    )

    screen_y = (
        y * DISPLAY_LENGTH
        - DISPLAY_LENGTH / 4
    )

    return (
        screen_x,
        screen_y
    )


# =========================================================
# TRACAGE
# =========================================================

def draw_segments(
    pen,
    screen,
    segments
):
    pen.clear()
    pen.penup()

    for start, end in segments:

        x1, y1 = to_screen(start)
        x2, y2 = to_screen(end)

        pen.goto(x1, y1)

        pen.pendown()
        pen.goto(x2, y2)

        pen.penup()

    screen.update()


# =========================================================
# PROGRAMME PRINCIPAL
# =========================================================

def main():

    pattern = load_pattern()

    # -----------------------------------------------------
    # FENETRE
    # -----------------------------------------------------

    screen = turtle.Screen()

    screen.setup(
        width=WINDOW_WIDTH,
        height=WINDOW_HEIGHT
    )

    screen.title(
        "Universal Fractal Model - Python Turtle"
    )

    # Désactivation de l'animation native de Turtle.
    # On contrôle nous-mêmes chaque rafraîchissement.
    screen.tracer(0, 0)

    pen = turtle.Turtle()

    pen.hideturtle()
    pen.speed(0)
    pen.pensize(1)

    # -----------------------------------------------------
    # ETAT DE L'ANIMATION
    # -----------------------------------------------------

    iteration = MIN_ITERATION

    # -----------------------------------------------------
    # FONCTION D'ANIMATION
    # -----------------------------------------------------

    def animate():

        nonlocal iteration

        print(
            f"Generation iteration {iteration}..."
        )

        segments = generate_fractal(
            pattern,
            iteration
        )

        print(
            f"{len(segments)} segments"
        )

        draw_segments(
            pen,
            screen,
            segments
        )

        # Niveau suivant
        iteration += 1

        # Boucle infinie
        if iteration > MAX_ITERATION:
            iteration = MIN_ITERATION

        # Planifie le prochain niveau.
        #
        # Contrairement à time.sleep(),
        # ontimer laisse la boucle Tkinter active.
        screen.ontimer(
            animate,
            ITERATION_DELAY_MS
        )

    # -----------------------------------------------------
    # DEMARRAGE
    # -----------------------------------------------------

    animate()

    turtle.done()


if __name__ == "__main__":
    main()
