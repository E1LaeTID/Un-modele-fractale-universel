## Modèle géométrique

Le motif est défini relativement à une longueur de référence `length = L`.

Chaque point `Mi` est représenté par deux coefficients normalisés :

\[
M_i = (\alpha_i,\beta_i)
\]

Les coordonnées réelles sont obtenues par changement d'échelle :

\[
x_i = \alpha_i L
\]

\[
y_i = \beta_i L
\]

Le motif peut ensuite être placé dans une surface graphique à l'aide d'une
translation `(offsetX, offsetY)`.

Pour un système graphique dont l'origine est située dans le coin supérieur
gauche :

\[
X_i = offsetX + \alpha_i L
\]

\[
Y_i = offsetY - \beta_i L
\]

Ainsi, le motif est indépendant de sa résolution d'affichage.

Modifier `length` change son échelle, tandis que modifier `offsetX` et
`offsetY` change sa position sans modifier sa géométrie.

### Exemple

Pour :

```text
length  = 600
offsetX = 100
offsetY = 500
