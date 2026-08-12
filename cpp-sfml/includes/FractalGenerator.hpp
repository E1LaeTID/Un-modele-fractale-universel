#pragma once

#include <vector>

#include "Point.hpp"
#include "Segment.hpp"

class FractalGenerator
{
public:
    static std::vector<Segment> generate(
        const std::vector<Point>& pattern,
        unsigned int iteration
    );

private:
    static std::vector<Segment> substitute(
        const std::vector<Segment>& source,
        const std::vector<Point>& pattern
    );
};