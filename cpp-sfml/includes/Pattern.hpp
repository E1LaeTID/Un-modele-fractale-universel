#pragma once

#include <string>
#include <vector>

#include "Point.hpp"
#include "Segment.hpp"

class Pattern
{
public:
    bool loadFromJson(
        const std::string& filePath
    );

    const std::vector<Point>& getPoints() const;

    std::vector<Segment> getSegments() const;

private:
    std::vector<Point> points;

    static float parseFraction(
        const std::string& value
    );
};