#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include <vector>

#include "Segment.hpp"

class Renderer
{
public:
    static void drawSegments(
        sf::RenderWindow& window,
        const std::vector<Segment>& segments
    );
};