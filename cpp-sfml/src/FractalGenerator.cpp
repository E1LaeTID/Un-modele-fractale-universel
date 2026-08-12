#include "FractalGenerator.hpp"
#include "Transform.hpp"

std::vector<Segment>
FractalGenerator::generate(
    const std::vector<Point>& pattern,
    unsigned int iteration
)
{
    std::vector<Segment> current = {
        {
            {0.f, 0.f},
            {1.f, 0.f}
        }
    };

    for (
        unsigned int level = 0;
        level < iteration;
        ++level
    )
    {
        current =
            substitute(
                current,
                pattern
            );
    }

    return current;
}

std::vector<Segment>
FractalGenerator::substitute(
    const std::vector<Segment>& source,
    const std::vector<Point>& pattern
)
{
    std::vector<Segment> result;

    if (pattern.size() < 2)
    {
        return result;
    }

    result.reserve(
        source.size()
        *
        (pattern.size() - 1)
    );

    for (
        const Segment& target :
        source
    )
    {
        for (
            std::size_t i = 0;
            i + 1 < pattern.size();
            ++i
        )
        {
            const Point start =
                Transform::mapPointToSegment(
                    pattern[i],
                    target
                );

            const Point end =
                Transform::mapPointToSegment(
                    pattern[i + 1],
                    target
                );

            result.push_back(
                {
                    start,
                    end
                }
            );
        }
    }

    return result;
}