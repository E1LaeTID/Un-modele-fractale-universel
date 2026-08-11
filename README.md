# Un modèle fractale universel

Ce repository présente une méthode générique de construction de fractales à partir d'un motif source défini comme une chaîne ordonnée de segments.

L'objectif est de proposer une représentation géométrique indépendante du langage de programmation, puis d'en montrer plusieurs implémentations graphiques.

Les premières implémentations prévues sont :

- C++ avec SFML 3 ;
- Python avec Turtle.

Le même motif source est utilisé dans chaque langage afin de montrer que la logique de construction fractale peut être séparée du moteur de rendu.

---

## 1. Principe général

La construction repose sur cinq étapes principales.

1. Définir un motif source.
2. Décomposer le motif en une chaîne ordonnée de points et de segments.
3. Normaliser le motif relativement à une longueur de référence `length`.
4. Remplacer chaque segment par une copie transformée du motif source.
5. Répéter récursivement cette transformation.

Le processus général peut être résumé ainsi :

```text
Motif source
    ↓
Décomposition en points et segments
    ↓
Normalisation
    ↓
Transformation
    ↓
Substitution segment par segment
    ↓
Itération
    ↓
Fractale
