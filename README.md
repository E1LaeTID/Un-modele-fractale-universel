# Un modèle fractale universel

Méthode générique de construction de fractales à partir d'un **motif source composé d'une chaîne ordonnée de segments**.

Ce repository présente une représentation géométrique indépendante du langage de programmation. Le même modèle pourra ensuite être exploité pour construire et animer une fractale avec différentes technologies, notamment :

- **C++ / SFML**
- **Python / Turtle**
- **JavaScript**

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

![Exemples de motifs sources pour une construction fractale](docs/images/fractale-motif-source-criteres-construction.png)

*Figure 1 — Exemples de propriétés géométriques à prendre en compte lors de la définition d'un motif source.*

Dans ce modèle, on cherche notamment à disposer :

- d'un contour ouvert ;
- d'une chaîne continue de segments ;
- d'un espace suffisant autour du motif ;
- d'une géométrie limitant les intersections indésirables lors des substitutions ;
- d'un premier et d'un dernier point clairement identifiés.

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

Le motif d'exemple de ce repository comporte **21 points et 20 segments** :

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

Par exemple, si :

```text
M7 = (1/8, 7/12)
```

alors ses coordonnées géométriques sont :

```text
x7 = length / 8
y7 = 7 × length / 12
```

Le motif peut donc être agrandi ou réduit sans modifier son dictionnaire de proportions.

---

# 4. Translation vers une surface graphique

Une surface graphique utilise généralement son coin supérieur gauche comme origine.

On introduit donc deux paramètres :

```text
offsetX
offsetY
```

Pour un point normalisé :

```text
Mi = (αi, βi)
```

ses coordonnées d'affichage deviennent :

```text
Xi = offsetX + αi × length
Yi = offsetY - βi × length
```

soit :

\[
X_i = offsetX + \alpha_i L
\]

\[
Y_i = offsetY - \beta_i L
\]

Le signe négatif sur l'axe `Y` permet de conserver une définition géométrique où les valeurs positives montent, alors que les coordonnées écran augmentent généralement vers le bas.

Le même dictionnaire peut ainsi être utilisé avec différentes tailles de fenêtre et différents moteurs graphiques.

---

# 5. Dictionnaire JSON du motif

La définition du motif d'exemple est stockée dans :

```text
patterns/reference-pattern.json
```

Le dictionnaire contient notamment :

- les paramètres généraux ;
- les coordonnées normalisées ;
- les formules proportionnelles à `length` ;
- l'origine et l'extrémité du motif ;
- les segments ;
- les relations de symétrie.

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

Cette séparation est importante :

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

L'axe de symétrie est défini par :

\[
x = \frac{L}{2}
\]

Dans l'espace normalisé :

\[
x = \frac{1}{2}
\]

Pour un point :

\[
P=(x,y)
\]

son symétrique est :

\[
P'=(1-x,y)
\]

Le motif de référence utilise notamment les correspondances :

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

le motif de référence doit être transformé de façon à faire correspondre :

```text
M0  → A
M20 → B
```

Cette opération nécessite principalement :

```text
translation
+ rotation
+ changement d'échelle
```

Le motif obtenu remplace alors le segment initial.

---

# 8. Itération

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

Le nombre de segments augmente ainsi à chaque itération.

---

# 9. Implémentations

Le même modèle géométrique sera utilisé pour plusieurs démonstrations.

```text
patterns/reference-pattern.json
              │
              ├── C++ / SFML
              │
              ├── Python / Turtle
              │
              └── JavaScript
```

L'objectif est de montrer que la définition du motif et le processus de construction restent indépendants du langage de programmation.

---

# Structure du repository

```text
Un-modele-fractale-universel/
│
├── README.md
├── LICENSE
│
├── docs/
│   └── images/
│       ├── fractale-motif-source-criteres-construction.png
│       └── fractale-motif-source-coordonnees-proportions.png
│
├── patterns/
│   └── reference-pattern.json
│
├── cpp/
│   ├── CMakeLists.txt
│   └── src/
│       └── main.cpp
│
└── python/
    └── main.py
```

---

# Licence

Ce projet est distribué sous **licence MIT**.

Les exemples et modèles peuvent être utilisés, modifiés et redistribués conformément aux conditions de la licence.
