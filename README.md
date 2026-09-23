# Modèle fractal universel — substitution géométrique multilangage

Le **modèle fractal universel** est une méthode générique de génération récursive : un motif ouvert, décrit par une chaîne ordonnée de points et de segments, est normalisé puis projeté sur chaque segment d'un chemin cible.

Le même dictionnaire géométrique peut être interprété par plusieurs technologies de rendu. Le dépôt fournit des démonstrations en **C++ avec SFML 3**, **Python avec Turtle** et des représentations JavaScript.

> English summary: a language-independent fractal substitution model that maps one normalized open pattern onto every segment of a target path.

## Objectif

Le projet sépare trois responsabilités :

1. **les données** décrivent le motif ;
2. **le moteur** applique les transformations et substitutions ;
3. **le renderer** affiche le résultat.

```text
motif JSON normalisé
        ↓
transformation affine
        ↓
substitution des segments
        ↓
itération récursive
        ↓
renderer choisi
```

Cette séparation permet de changer de motif, de langage ou de technologie d'affichage sans redéfinir le principe mathématique.

## Du motif à la fractale

### 1. Chaîne ouverte

Un motif valide est représenté par une chaîne continue :

```text
M0 → M1 → M2 → ... → Mn
```

Chaque paire de points consécutifs forme un segment. `M0` est l'origine et `Mn` l'extrémité.

### 2. Normalisation

Le motif est ramené dans un espace où :

```text
M0 = (0, 0)
Mn = (1, 0)
```

Pour une longueur `L`, un point normalisé `Mi = (αi, βi)` devient :

```text
xi = αi × L
yi = βi × L
```

### 3. Projection sur un segment

Pour un segment cible `[A, B]`, on définit :

```text
U = B - A
V = (-Uy, Ux)
```

Un point local `P(x, y)` est projeté par :

```text
P' = A + xU + yV
```

Cette transformation combine translation, rotation et changement d'échelle.

### 4. Substitution récursive

Chaque segment est remplacé par une copie transformée du motif. Si le motif possède `s` segments, l'ordre `n` peut produire jusqu'à :

```text
sⁿ segments
```

La profondeur doit donc rester limitée.

## Motif de référence

Le motif pédagogique du dépôt comporte **21 points et 20 segments**, de `M0` à `M20`. Son axe de symétrie est :

```text
x = L / 2
```

Dans l'espace normalisé :

```text
x = 1 / 2
```

Les couples symétriques vont de `M0 ↔ M20` jusqu'à `M9 ↔ M11`, tandis que `M10` appartient à l'axe.

![Critères de construction d'un motif source fractal](docs/images/fractale-motif-source-criteres-construction.png)

![Coordonnées et proportions du motif source fractal](docs/images/fractale-motif-source-coordonnees-proportions.png)

## Dictionnaire JSON

Exemple réduit :

```json
{
  "name": "reference-pattern",
  "origin": "M0",
  "endPoint": "M20",
  "points": [
    { "id": "M0", "normalized": ["0", "0"] },
    { "id": "M1", "normalized": ["1/32", "1/5"] }
  ],
  "segments": [
    ["M0", "M1"],
    ["M1", "M2"]
  ]
}
```

Les coordonnées restent indépendantes de la taille d'affichage et du langage d'implémentation.

## Huit motifs élémentaires

| Élément | Fichier |
|---|---|
| Eau | `water-pattern.json` |
| Feu | `fire-pattern.json` |
| Vent | `wind-pattern.json` |
| Bois | `wood-pattern.json` |
| Terre | `earth-pattern.json` |
| Glace | `ice-pattern.json` |
| Magmat | `magma-pattern.json` |
| Foudre | `lightning-pattern.json` |

Ces fichiers ne représentent pas huit algorithmes différents. Ils constituent huit dictionnaires interprétés par le même moteur.

## Implémentation C++ / SFML 3

Le code se trouve dans `cpp-sfml/`.

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
```

Compilation depuis `cpp-sfml/` :

```bash
cmake -S . -B build
cmake --build build --config Debug
```

Sous Windows :

```powershell
.\build\Debug\FractalDemo.exe
```

## Implémentation Python / Turtle

La version pédagogique se trouve dans `python-turtle/`.

```bash
python main.py
```

Elle boucle volontairement sur les ordres 1 à 3 afin de limiter les ralentissements.

## Ajouter un motif

1. Définissez une chaîne ouverte continue.
2. Placez son origine en `(0,0)`.
3. Placez son extrémité en `(1,0)`.
4. Normalisez les points intermédiaires.
5. Déclarez les segments dans leur ordre de parcours.
6. Enregistrez le dictionnaire JSON.
7. Chargez-le avec le moteur existant.

## Démonstrations

- [Animation C++ / SFML 3](docs/videos/fractal-animation-cpp-sfml3.mp4)
- [Animation Python / Turtle](docs/videos/fractal-animation-python-turtle.mp4)

## Relation avec le modèle étendu

Ce dépôt constitue la **référence fondamentale et pédagogique**. Il applique un motif ouvert à une chaîne de segments et répète la substitution.

Le [modèle fractal universel étendu](https://github.com/E1LaeTID/modele-fractal-universel-etendu) ajoute :

- une liste de motifs ouverts ;
- une liste de contours fermés ;
- une conversion fermé-vers-ouvert ;
- une rétrogradation des niveaux ;
- un résultat de cycle réutilisé comme source du cycle suivant.

## Questions fréquentes

### Pourquoi qualifier le modèle d'universel ?

Le terme indique ici que le même processus de normalisation, transformation affine et substitution peut interpréter différents motifs et plusieurs technologies de rendu. Il ne constitue pas une affirmation d'universalité mathématique démontrée pour toutes les fractales.

### Le modèle dépend-il de SFML ou de Turtle ?

Non. Ces bibliothèques servent au rendu. La géométrie normalisée et le principe de substitution restent indépendants.

### Peut-on ajouter son propre motif ?

Oui, à condition de fournir une chaîne ouverte continue, une origine, une extrémité et une liste ordonnée de segments compatibles.

### Pourquoi limiter le nombre d'itérations ?

Le nombre de segments augmente exponentiellement. Une profondeur excessive peut saturer la mémoire ou rendre l'affichage inutilisable.

### Quelle différence avec le modèle étendu ?

Le modèle initial répète un motif sur des segments. Le modèle étendu orchestre deux listes de géométries et transforme le résultat d'un cycle en entrée du suivant.

### Ce dépôt est-il une bibliothèque prête pour la production ?

Non. Il s'agit d'une référence pédagogique et expérimentale accompagnée de démonstrations.

## Statut et écosystème

Référence pédagogique fonctionnelle et base du modèle étendu.

- [Modèle fractal universel étendu](https://github.com/E1LaeTID/modele-fractal-universel-etendu)
- [time2d](https://github.com/E1LaeTID/time2d)
- [ElementChess](https://github.com/E1LaeTID/elementchess-fractal-poc)
- [Portail E1LaeTID](https://e1laetid.github.io/)

## Licence

Distribué sous licence **MIT**. Consultez [LICENSE](LICENSE).
