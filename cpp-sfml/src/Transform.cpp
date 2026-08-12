#include "Transform.hpp"

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

std::vector<Segment>
Transform::toScreen(
    const std::vector<Segment>& segments,
    float length,
    float offsetX,
    float offsetY
)
{
    std::vector<Segment> result;

    result.reserve(
        segments.size()
    );

    for (
        const Segment& segment :
        segments
    )
    {
        result.push_back(
            {
                {
                    offsetX
                        + segment.start.x * length,

                    offsetY
                        - segment.start.y * length
                },

                {
                    offsetX
                        + segment.end.x * length,

                    offsetY
                        - segment.end.y * length
                }
            }
        );
    }

    return result;
}