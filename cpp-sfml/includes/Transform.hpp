#pragma once

#include <vector>

#include "Point.hpp"
#include "Segment.hpp"


class Transform
{
public:

    // -----------------------------------------------------
    // TRANSFORMATION FRACTALE
    // -----------------------------------------------------
    //
    // Adapte un point du motif normalisé à un segment cible.
    //
    // Le motif de référence est défini entre :
    //
    // M0 = (0, 0)
    // Mn = (1, 0)
    //
    // Cette transformation réalise simultanément :
    //
    // - changement d'échelle
    // - rotation
    // - translation
    //
    // -----------------------------------------------------

    static Point mapPointToSegment(
        const Point& point,
        const Segment& target
    );


    // -----------------------------------------------------
    // AJUSTEMENT AUTOMATIQUE DANS LA FENETRE
    // -----------------------------------------------------
    //
    // Calcule la bounding box de la fractale complète,
    // détermine automatiquement l'échelle maximale
    // disponible et centre le résultat dans la fenêtre.
    //
    // Le JSON reste donc totalement indépendant :
    //
    // - de la résolution
    // - de la taille de la fenêtre
    // - des offsets d'affichage
    //
    // -----------------------------------------------------

    static std::vector<Segment> fitToScreen(
        const std::vector<Segment>& segments,
        float windowWidth,
        float windowHeight,
        float margin = 70.f
    );
};
