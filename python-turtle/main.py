import json
import turtle

from pathlib import Path


# =========================================================
# CONFIGURATION GENERALE
# =========================================================
#
# Pour changer de motif :
#
# 1. supprimer le JSON actuellement présent dans patterns/
# 2. placer un nouveau fichier .json
# 3. relancer :
#
#       python main.py
#
# Aucun changement dans le code n'est nécessaire.
#
# =========================================================


# ---------------------------------------------------------
# Fenêtre
# ---------------------------------------------------------

WINDOW_WIDTH = 1100
WINDOW_HEIGHT = 760

# Position de la fenêtre sur l'écran Windows.
#
# Cela évite que la barre de titre sorte de l'écran.
WINDOW_START_X = 50
WINDOW_START_Y = 50

# Marge conservée autour de la fractale.
DISPLAY_MARGIN = 70


# ---------------------------------------------------------
# Animation Turtle
# ---------------------------------------------------------

MIN_ITERATION = 1

# Turtle est volontairement limité à 3 niveaux.
MAX_ITERATION = 3

# Durée d'affichage d'un niveau en millisecondes.
ITERATION_DELAY_MS = 2000


# =========================================================
# RECHERCHE AUTOMATIQUE DU JSON
# =========================================================

def find_pattern_file():

    pattern_dir = (
        Path(__file__).parent
        / "patterns"
    )


    json_files = list(
        pattern_dir.glob("*.json")
    )


    if len(json_files) == 0:

        raise FileNotFoundError(
            "Aucun fichier JSON dans patterns/"
        )


    if len(json_files) > 1:

        raise RuntimeError(
            "Conservez un seul fichier JSON "
            "dans patterns/"
        )


    return json_files[0]


# =========================================================
# CONVERSION DES FRACTIONS
# =========================================================

def parse_fraction(value):

    if "/" in value:

        numerator, denominator = (
            value.split("/")
        )


        return (
            float(numerator)
            / float(denominator)
        )


    return float(value)


# =========================================================
# CHARGEMENT DU MOTIF
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


        points.append(
            (x, y)
        )


    print(
        f"{len(points)} points charges"
    )


    return points


# =========================================================
# TRANSFORMATION FRACTALE
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

        ax
        + x * dx
        - y * dy,

        ay
        + x * dy
        + y * dx

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
# GENERATION FRACTALE
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
# AJUSTEMENT AUTOMATIQUE
# =========================================================

def fit_to_screen(
    segments,
    width,
    height,
    margin
):

    if not segments:
        return []


    xs = []
    ys = []


    # -----------------------------------------------------
    # Bounding box
    # -----------------------------------------------------

    for start, end in segments:

        xs.extend(
            [
                start[0],
                end[0]
            ]
        )


        ys.extend(
            [
                start[1],
                end[1]
            ]
        )


    min_x = min(xs)
    max_x = max(xs)

    min_y = min(ys)
    max_y = max(ys)


    fractal_width = (
        max_x - min_x
    )


    fractal_height = (
        max_y - min_y
    )


    available_width = (
        width
        - 2 * margin
    )


    available_height = (
        height
        - 2 * margin
    )


    # -----------------------------------------------------
    # Echelle
    # -----------------------------------------------------

    if fractal_width > 0:

        scale_x = (
            available_width
            / fractal_width
        )

    else:

        scale_x = 1


    if fractal_height > 0:

        scale_y = (
            available_height
            / fractal_height
        )

    else:

        scale_y = scale_x


    scale = min(
        scale_x,
        scale_y
    )


    # -----------------------------------------------------
    # Centre
    # -----------------------------------------------------

    center_x = (
        min_x + max_x
    ) / 2


    center_y = (
        min_y + max_y
    ) / 2


    fitted = []


    # -----------------------------------------------------
    # Projection Turtle
    # -----------------------------------------------------
    #
    # Turtle utilise déjà :
    #
    # x positif vers la droite
    # y positif vers le haut
    #
    # Il n'est donc pas nécessaire d'inverser Y.
    #
    # -----------------------------------------------------

    for start, end in segments:

        start_screen = (

            (
                start[0]
                - center_x
            )
            * scale,

            (
                start[1]
                - center_y
            )
            * scale

        )


        end_screen = (

            (
                end[0]
                - center_x
            )
            * scale,

            (
                end[1]
                - center_y
            )
            * scale

        )


        fitted.append(
            (
                start_screen,
                end_screen
            )
        )


    return fitted


# =========================================================
# TRACAGE TURTLE
# =========================================================

def draw_segments(
    pen,
    screen,
    segments
):

    fitted_segments = fit_to_screen(
        segments,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        DISPLAY_MARGIN
    )


    pen.clear()

    pen.penup()


    for start, end in fitted_segments:

        pen.goto(
            start[0],
            start[1]
        )


        pen.pendown()


        pen.goto(
            end[0],
            end[1]
        )


        pen.penup()


    screen.update()


# =========================================================
# PROGRAMME PRINCIPAL
# =========================================================

def main():

    # -----------------------------------------------------
    # Motif
    # -----------------------------------------------------

    pattern = load_pattern()


    # -----------------------------------------------------
    # Fenêtre Turtle
    # -----------------------------------------------------

    screen = turtle.Screen()


    screen.setup(

        width=WINDOW_WIDTH,
        height=WINDOW_HEIGHT,

        startx=WINDOW_START_X,
        starty=WINDOW_START_Y

    )


    screen.title(
        "Universal Fractal Model - Python Turtle"
    )


    # Le dessin natif de Turtle est désactivé
    # pour accélérer les itérations.
    screen.tracer(
        0,
        0
    )


    # -----------------------------------------------------
    # Stylo
    # -----------------------------------------------------

    pen = turtle.Turtle()


    pen.hideturtle()

    pen.speed(0)

    pen.pensize(1)


    # -----------------------------------------------------
    # Animation
    # -----------------------------------------------------

    iteration = MIN_ITERATION


    def animate():

        nonlocal iteration


        print(
            f"Generation iteration "
            f"{iteration}..."
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


        # Boucle infinie :
        #
        # 1 -> 2 -> 3 -> 1 -> ...
        if (
            iteration
            > MAX_ITERATION
        ):

            iteration = (
                MIN_ITERATION
            )


        # Lance le prochain niveau sans
        # bloquer la boucle Tkinter.
        screen.ontimer(
            animate,
            ITERATION_DELAY_MS
        )


    # -----------------------------------------------------
    # Démarrage
    # -----------------------------------------------------

    animate()


    turtle.done()


# =========================================================
# POINT D'ENTREE
# =========================================================

if __name__ == "__main__":

    main()
