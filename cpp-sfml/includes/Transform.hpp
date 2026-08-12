#pragma once

#include <vector>

#include "Point.hpp"
#include "Segment.hpp"

class Transform
{
public:
    static Point mapPointToSegment(
        const Point& point,
        const Segment& target
    );

    static std::vector<Segment> toScreen(
        const std::vector<Segment>& segments,
        float length,
        float offsetX,
        float offsetY
    );
};