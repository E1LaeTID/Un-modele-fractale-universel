#include "Renderer.hpp"

#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>

void Renderer::drawSegments(
    sf::RenderWindow& window,
    const std::vector<Segment>& segments
)
{
    sf::VertexArray lines(sf::PrimitiveType::Lines);

    for (const Segment& segment : segments)
    {
        lines.append(
            sf::Vertex{
                {segment.start.x, segment.start.y},
                sf::Color::White
            }
        );

        lines.append(
            sf::Vertex{
                {segment.end.x, segment.end.y},
                sf::Color::White
            }
        );
    }

    window.draw(lines);
}