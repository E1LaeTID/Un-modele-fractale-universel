#include "Pattern.hpp"

#include <fstream>
#include <iostream>
#include <string>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

float Pattern::parseFraction(
    const std::string& value
)
{
    const std::size_t slash =
        value.find('/');

    if (slash == std::string::npos)
    {
        return std::stof(value);
    }

    const float numerator =
        std::stof(
            value.substr(0, slash)
        );

    const float denominator =
        std::stof(
            value.substr(slash + 1)
        );

    return numerator / denominator;
}

bool Pattern::loadFromJson(
    const std::string& filePath
)
{
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        std::cerr
            << "Impossible d'ouvrir le dictionnaire : "
            << filePath
            << '\n';

        return false;
    }

    json data;

    try
    {
        file >> data;
    }
    catch (const json::parse_error& error)
    {
        std::cerr
            << "Erreur JSON : "
            << error.what()
            << '\n';

        return false;
    }

    points.clear();

    for (
        const auto& pointData :
        data.at("points")
    )
    {
        const std::string x =
            pointData
                .at("normalized")
                .at(0)
                .get<std::string>();

        const std::string y =
            pointData
                .at("normalized")
                .at(1)
                .get<std::string>();

        points.push_back(
            {
                parseFraction(x),
                parseFraction(y)
            }
        );
    }

    return true;
}

const std::vector<Point>&
Pattern::getPoints() const
{
    return points;
}

std::vector<Segment>
Pattern::getSegments() const
{
    std::vector<Segment> segments;

    if (points.size() < 2)
    {
        return segments;
    }

    segments.reserve(
        points.size() - 1
    );

    for (
        std::size_t i = 0;
        i + 1 < points.size();
        ++i
    )
    {
        segments.push_back(
            {
                points[i],
                points[i + 1]
            }
        );
    }

    return segments;
}