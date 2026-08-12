#include "Transform.hpp"

#include <algorithm>
#include <limits>


// =========================================================
// TRANSFORMATION D'UN POINT SUR UN SEGMENT
// =========================================================

Point Transform::mapPointToSegment(
    const Point& point,
    const Segment& target
)
{
    const float dx =
        target.end.x - target.start.x;

    const float dy =
        target.end.y - target.start.y;


    return {

        target.start.x
            + point.x * dx
            - point.y * dy,

        target.start.y
            + point.x * dy
            + point.y * dx

    };
}


// =========================================================
// CENTRAGE ET AJUSTEMENT AUTOMATIQUE DANS LA FENETRE
// =========================================================

std::vector<Segment>
Transform::fitToScreen(
    const std::vector<Segment>& segments,
    float windowWidth,
    float windowHeight,
    float margin
)
{
    // Aucun segment : rien à transformer.
    if (segments.empty())
    {
        return {};
    }


    // -----------------------------------------------------
    // 1. RECHERCHE DES LIMITES DE LA FRACTALE
    // -----------------------------------------------------

    float minX =
        std::numeric_limits<float>::max();

    float maxX =
        std::numeric_limits<float>::lowest();

    float minY =
        std::numeric_limits<float>::max();

    float maxY =
        std::numeric_limits<float>::lowest();


    for (const Segment& segment : segments)
    {
        minX = std::min(
            minX,
            std::min(
                segment.start.x,
                segment.end.x
            )
        );

        maxX = std::max(
            maxX,
            std::max(
                segment.start.x,
                segment.end.x
            )
        );

        minY = std::min(
            minY,
            std::min(
                segment.start.y,
                segment.end.y
            )
        );

        maxY = std::max(
            maxY,
            std::max(
                segment.start.y,
                segment.end.y
            )
        );
    }


    // -----------------------------------------------------
    // 2. DIMENSIONS REELLES
    // -----------------------------------------------------

    const float fractalWidth =
        maxX - minX;

    const float fractalHeight =
        maxY - minY;


    const float availableWidth =
        windowWidth - 2.f * margin;

    const float availableHeight =
        windowHeight - 2.f * margin;


    // -----------------------------------------------------
    // 3. FACTEURS D'ECHELLE
    // -----------------------------------------------------

    const float scaleX =
        fractalWidth > 0.f
        ? availableWidth / fractalWidth
        : 1.f;


    const float scaleY =
        fractalHeight > 0.f
        ? availableHeight / fractalHeight
        : scaleX;


    // Même facteur sur X et Y afin de conserver
    // exactement les proportions du motif.
    const float scale =
        std::min(
            scaleX,
            scaleY
        );


    // -----------------------------------------------------
    // 4. CENTRE GEOMETRIQUE
    // -----------------------------------------------------

    const float fractalCenterX =
        (minX + maxX) / 2.f;

    const float fractalCenterY =
        (minY + maxY) / 2.f;


    const float screenCenterX =
        windowWidth / 2.f;

    const float screenCenterY =
        windowHeight / 2.f;


    // -----------------------------------------------------
    // 5. PROJECTION VERS LE REPERE SFML
    // -----------------------------------------------------

    std::vector<Segment> result;

    result.reserve(
        segments.size()
    );


    for (const Segment& segment : segments)
    {
        const Point start = {

            screenCenterX
                + (segment.start.x - fractalCenterX)
                * scale,

            // Inversion de Y :
            // le repère mathématique monte,
            // le repère écran descend.
            screenCenterY
                - (segment.start.y - fractalCenterY)
                * scale

        };


        const Point end = {

            screenCenterX
                + (segment.end.x - fractalCenterX)
                * scale,

            screenCenterY
                - (segment.end.y - fractalCenterY)
                * scale

        };


        result.push_back(
            {
                start,
                end
            }
        );
    }


    return result;
}
