# Un modèle fractale universel

Méthode générique de construction de fractales à partir d'un **motif source composé d'une chaîne ordonnée de segments**.

Ce repository présente une représentation géométrique indépendante du langage de programmation. Le même modèle peut être exploité pour construire et animer une fractale avec différentes technologies, notamment :

* **C++ / SFML 3**
* **Python / Turtle**
* **JavaScript**

L'objectif est de séparer la **définition mathématique du motif** de son **implémentation graphique**.

---

## Principe général

La construction repose sur cinq étapes :

1. définir un motif source valide ;
2. représenter ce motif sous la forme d'un dictionnaire de points et de segments ;
3. normaliser sa géométrie relativement à une longueur de référence `length` ;
4. substituer à chaque segment une transformation du motif source ;
5. répéter récursivement cette substitution.

```text
Motif source
      ↓
Dictionnaire géométrique
      ↓
Normalisation
      ↓
Transformation
      ↓
Substitution des segments
      ↓
Itération récursive
      ↓
Fractale
```

---

# 1. Définir un motif source

Le point de départ est une **chaîne ouverte et ordonnée de segments**.

Tous les motifs ne sont cependant pas adaptés à une substitution récursive.

![Critères de construction d'un motif source fractal](docs/images/fractale-motif-source-criteres-construction.png)

*Figure 1 — Exemples de propriétés géométriques à prendre en compte lors de la définition d'un motif source.*

Dans ce modèle, on cherche notamment à disposer :

* d'un contour ouvert ;
* d'une chaîne continue de segments ;
* d'un espace suffisant autour du motif ;
* d'une géométrie limitant les intersections indésirables lors des substitutions ;
* d'un premier et d'un dernier point clairement identifiés.

Le motif retenu peut ensuite être décrit mathématiquement.

---

# 2. Décomposer le motif en dictionnaire

Le motif est représenté par une suite ordonnée de points :

```text
M0 → M1 → M2 → ... → Mn
```

Chaque paire de points consécutifs définit un segment :

```text
S0 = [M0, M1]
S1 = [M1, M2]
S2 = [M2, M3]
...
Sn-1 = [Mn-1, Mn]
```

Cette représentation permet de dissocier la géométrie du motif du langage utilisé pour l'afficher.

Le motif de référence utilisé pour illustrer la méthode comporte **21 points et 20 segments** :

```text
M0 → M1 → ... → M20
```

`M0` constitue l'origine du motif et `M20` son extrémité.

---

# 3. Motif de référence

Le repository fournit un motif d'exemple permettant d'illustrer la méthode.

![Coordonnées et proportions du motif source fractal](docs/images/fractale-motif-source-coordonnees-proportions.png)

*Figure 2 — Motif de référence et principales proportions utilisées pour construire son dictionnaire géométrique.*

La distance horizontale entre `M0` et `M20` définit la longueur de référence :

```text
length = L
```

Ainsi :

```text
M0  = (0, 0)
M20 = (L, 0)
```

La figure possède un axe vertical de symétrie situé à :

```text
x = L / 2
```

La seconde moitié du motif peut donc être obtenue à partir de la première par symétrie.

---

## Coordonnées normalisées

Pour rendre le modèle indépendant de sa taille réelle, chaque point `Mi` est défini par deux coefficients :

```text
Mi = (αi, βi)
```

avec :

```text
xi = αi × length
yi = βi × length
```

Par exemple :

```text
M7 = (1/8, 7/12)
```

correspond à :

```text
x7 = length / 8
y7 = 7 × length / 12
```

Le motif peut ainsi être agrandi, réduit, translaté ou projeté dans différentes surfaces graphiques sans modifier son dictionnaire géométrique.

---

# 4. Projection vers une surface graphique

Dans une représentation graphique classique dont l'origine se trouve dans le coin supérieur gauche :

```text
Xi = offsetX + αi × length
Yi = offsetY - βi × length
```

soit :

[
X_i = offsetX + \alpha_i L
]

[
Y_i = offsetY - \beta_i L
]

Le signe négatif sur `Y` permet de conserver une définition mathématique où les coordonnées positives montent, alors que les coordonnées écran augmentent généralement vers le bas.

Les implémentations actuelles effectuent également un **centrage et une mise à l'échelle automatiques** de la fractale finale afin que les motifs de proportions différentes restent visibles dans la fenêtre.

---

# 5. Dictionnaire JSON du motif

La géométrie d'un motif est stockée dans un fichier JSON.

Exemple simplifié :

```json
{
  "name": "reference-pattern",

  "parameters": {
    "length": "L",
    "offsetX": "X0",
    "offsetY": "Y0"
  },

  "points": [
    {
      "id": "M0",
      "normalized": ["0", "0"]
    },
    {
      "id": "M1",
      "normalized": ["1/32", "1/5"]
    }
  ],

  "segments": [
    ["M0", "M1"],
    ["M1", "M2"]
  ]
}
```

Le dictionnaire peut notamment contenir :

* les paramètres généraux ;
* les coordonnées normalisées ;
* les formules proportionnelles à `length` ;
* l'origine et l'extrémité ;
* la chaîne de segments ;
* les éventuelles relations de symétrie.

La séparation entre données et moteur est volontaire :

```text
JSON
 ↓
décrit la géométrie

C++ / Python / JavaScript
 ↓
interprètent la géométrie

SFML / Turtle / Canvas
 ↓
effectuent le rendu
```

---

# 6. Symétrie du motif de référence

L'axe de symétrie du motif de référence est :

[
x = \frac{L}{2}
]

Dans l'espace normalisé :

[
x = \frac{1}{2}
]

Pour un point :

[
P=(x,y)
]

son symétrique est :

[
P'=(1-x,y)
]

Les correspondances du motif de référence sont notamment :

```text
M9  ↔ M11
M8  ↔ M12
M7  ↔ M13
M6  ↔ M14
M5  ↔ M15
M4  ↔ M16
M3  ↔ M17
M2  ↔ M18
M1  ↔ M19
M0  ↔ M20
```

`M10` appartient à l'axe de symétrie.

---

# 7. Substitution des segments

Une fois le motif défini, chaque segment peut servir de support à une nouvelle occurrence transformée du motif.

Pour un segment :

```text
A → B
```

le motif normalisé est transformé de façon à faire correspondre :

```text
M0 → A
Mn → B
```

Cette opération combine :

```text
translation
+ rotation
+ changement d'échelle
```

Le motif transformé remplace alors le segment initial.

---

# 8. Itération fractale

La substitution peut être appliquée récursivement.

```text
Niveau 0
segment initial

        ↓

Niveau 1
motif source

        ↓

Niveau 2
chaque segment devient un motif

        ↓

Niveau 3
nouvelle substitution

        ↓

...

        ↓

Niveau n
```

Si un motif comporte `s` segments, l'ordre `n` peut produire jusqu'à :

```text
s^n segments
```

Le coût augmente donc très rapidement avec l'ordre d'itération.

La version **Python / Turtle** est volontairement limitée aux ordres `1 à 3` pour conserver une exécution stable.

La version **C++ / SFML 3** peut gérer des ordres supérieurs selon la complexité du motif et les ressources disponibles.

---

# 9. Les huit motifs élémentaires

Le repository contient huit motifs sources permettant de tester le même moteur fractal avec différentes géométries.

| Élément | Fichier JSON             |
| ------- | ------------------------ |
| Eau     | `water-pattern.json`     |
| Feu     | `fire-pattern.json`      |
| Vent    | `wind-pattern.json`      |
| Bois    | `wood-pattern.json`      |
| Terre   | `earth-pattern.json`     |
| Glace   | `ice-pattern.json`       |
| Magmat  | `magma-pattern.json`     |
| Foudre  | `lightning-pattern.json` |

Ces fichiers suivent tous la même convention :

```text
M0 = origine du motif
Mn = extrémité du motif
Mn normalisé en (1, 0)
```

Les coordonnées intermédiaires sont exprimées relativement à `length`.

---

## Tester un motif

Les programmes sont conçus pour utiliser **un seul fichier JSON actif dans le dossier `patterns/`**.

Pour tester un autre motif :

1. choisir l'un des huit fichiers JSON ;
2. conserver uniquement le fichier à tester dans le dossier `patterns/` utilisé par le programme ;
3. retirer ou déplacer le motif précédent ;
4. relancer le programme.

Aucune modification de l'algorithme fractal n'est nécessaire.

```text
water-pattern.json
        ↓
même moteur

fire-pattern.json
        ↓
même moteur

wind-pattern.json
        ↓
même moteur

...
```

Le changement de JSON modifie uniquement la géométrie utilisée pour les substitutions.

---

# 10. Implémentation C++ / SFML 3

Le code C++ se trouve dans :

```text
cpp-sfml/
```

Architecture principale :

```text
Point
 ↓
Segment
 ↓
Pattern
 ↓
Transform
 ↓
FractalGenerator
 ↓
Renderer
 ↓
SFML 3
```

Le programme :

* charge le JSON ;
* génère les différents ordres de fractale ;
* calcule automatiquement les limites géométriques ;
* adapte l'échelle ;
* centre la fractale dans la fenêtre ;
* joue les différents niveaux sous forme d'animation.

## Première compilation

Depuis `cpp-sfml/` :

```powershell
cmake -S . -B build
cmake --build build --config Debug
```

Puis :

```powershell
.\build\Debug\FractalDemo.exe
```

## Tester un autre JSON

Après la compilation initiale, il n'est pas nécessaire de reconstruire le programme si seul le fichier JSON change.

Remplacez le fichier actif dans `patterns/`, puis relancez :

```powershell
.\build\Debug\FractalDemo.exe
```

Le nouveau motif est lu au démarrage du programme.

---

# 11. Implémentation Python / Turtle

La version Python se trouve dans :

```text
python-turtle/
```

Elle reprend volontairement le même processus avec une implémentation plus compacte :

```text
JSON
 ↓
points normalisés
 ↓
substitution
 ↓
génération fractale
 ↓
centrage automatique
 ↓
Turtle
```

Depuis `python-turtle/` :

```powershell
python main.py
```

La démonstration boucle automatiquement sur :

```text
ordre 1
↓
ordre 2
↓
ordre 3
↓
ordre 1
↓
...
```

Turtle est volontairement limité à trois ordres afin d'éviter les ralentissements importants liés au nombre de segments.

## Tester un autre JSON

Remplacez simplement le fichier actif dans `patterns/`, puis relancez :

```powershell
python main.py
```

Aucune compilation n'est nécessaire.

---

# 12. Ajouter son propre motif

Le repository n'est pas limité aux huit éléments fournis.

Pour créer un nouveau motif :

1. définir une chaîne ouverte de points ;
2. choisir `M0` comme origine ;
3. définir le dernier point sur `(1, 0)` dans l'espace normalisé ;
4. exprimer les autres points relativement à `length` ;
5. créer la liste ordonnée des segments ;
6. enregistrer le résultat dans un fichier JSON compatible ;
7. remplacer le fichier actif dans `patterns/`.

Le moteur fractal reste inchangé.

```text
Nouveau JSON
     ↓
Pattern
     ↓
Transform
     ↓
FractalGenerator
     ↓
Renderer
```

---

# 13. Structure du repository

```text
Un-modele-fractale-universel/
│
├── README.md
├── LICENSE
│
├── cpp-sfml/
│   ├── CMakeLists.txt
│   │
│   ├── includes/
│   │   ├── Point.hpp
│   │   ├── Segment.hpp
│   │   ├── Pattern.hpp
│   │   ├── Transform.hpp
│   │   ├── FractalGenerator.hpp
│   │   └── Renderer.hpp
│   │
│   └── src/
│       ├── main.cpp
│       ├── Pattern.cpp
│       ├── Transform.cpp
│       ├── FractalGenerator.cpp
│       └── Renderer.cpp
│
├── python-turtle/
│   └── main.py
│
├── patterns/
│   ├── reference-pattern.json
│   ├── water-pattern.json
│   ├── fire-pattern.json
│   ├── wind-pattern.json
│   ├── wood-pattern.json
│   ├── earth-pattern.json
│   ├── ice-pattern.json
│   ├── magma-pattern.json
│   └── lightning-pattern.json
│
└── docs/
    ├── images/
    │   ├── fractale-motif-source-criteres-construction.png
    │   └── fractale-motif-source-coordonnees-proportions.png
    │
    └── videos/
        ├── fractal-animation-cpp-sfml3.mp4
        └── fractal-animation-python-turtle.mp4
```

---

# 14. Démonstrations vidéo

## C++ / SFML 3

[▶ Voir l'animation C++ / SFML 3](docs/videos/fractal-animation-cpp-sfml3.mp4)

La démonstration montre l'évolution automatique du nombre d'itérations avec un rendu graphique accéléré par SFML.

## Python / Turtle

[▶ Voir l'animation Python / Turtle](docs/videos/fractal-animation-python-turtle.mp4)

La démonstration reproduit le même principe avec Turtle et boucle volontairement sur les ordres `1 à 3`.

---

# 15. Principe d'indépendance du modèle

L'objectif principal du repository est de montrer que les motifs, les transformations et les technologies de rendu peuvent rester découplés.

```text
              Motif JSON
                   │
                   ▼
          Géométrie normalisée
                   │
                   ▼
        Transformation des segments
                   │
                   ▼
          Substitution récursive
             ┌─────┴─────┐
             ▼           ▼
        C++ / SFML    Python / Turtle
             │           │
             ▼           ▼
          animation   animation
```

Les huit éléments ne constituent donc pas huit programmes différents.

Ils représentent **huit dictionnaires géométriques interprétés par le même processus fractal**.

---

# Licence

Ce projet est distribué sous **licence MIT**.

Les exemples, fichiers JSON et implémentations peuvent être utilisés, modifiés et redistribués conformément aux conditions de la licence.
