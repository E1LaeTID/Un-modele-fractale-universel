#include <SFML/Graphics.hpp>

#include <cstdint>
#include <filesystem>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include "Pattern.hpp"
#include "Segment.hpp"
#include "Transform.hpp"
#include "FractalGenerator.hpp"
#include "Renderer.hpp"


// =========================================================
// CONFIGURATION DU PROGRAMME
// =========================================================
//
// Pour changer de motif fractal :
//
// 1. supprimer le JSON actuellement présent dans patterns/
// 2. placer un nouveau fichier .json dans patterns/
// 3. relancer FractalDemo.exe
//
// AUCUNE RECOMPILATION N'EST NECESSAIRE.
//
// Le motif est automatiquement :
//
// - chargé
// - généré
// - redimensionné
// - centré
//
// =========================================================


// ---------------------------------------------------------
// Fenêtre
// ---------------------------------------------------------

constexpr unsigned int WINDOW_WIDTH = 1200;
constexpr unsigned int WINDOW_HEIGHT = 900;


// Marge minimale conservée autour de la fractale.
constexpr float DISPLAY_MARGIN = 70.f;


// ---------------------------------------------------------
// Animation
// ---------------------------------------------------------

constexpr unsigned int MIN_ITERATION = 1;


// Limite théorique.
//
// La limite réellement affichable dépend du nombre de
// segments présents dans le motif source.
constexpr unsigned int MAX_ITERATION = 6;


// Temps entre deux niveaux.
constexpr float ITERATION_DURATION_SECONDS = 2.f;


// Protection mémoire / performances.
constexpr std::uint64_t SAFE_SEGMENT_LIMIT =
    4'000'000;


// =========================================================
// RECHERCHE AUTOMATIQUE DU JSON
// =========================================================

std::filesystem::path findPatternFile()
{
    const std::filesystem::path patternsDirectory =
        std::filesystem::path(
            PROJECT_SOURCE_DIR_PATH
        )
        / "patterns";


    std::vector<std::filesystem::path> jsonFiles;


    if (
        !std::filesystem::exists(
            patternsDirectory
        )
    )
    {
        throw std::runtime_error(
            "Le dossier patterns est introuvable."
        );
    }


    for (
        const auto& entry :
        std::filesystem::directory_iterator(
            patternsDirectory
        )
    )
    {
        if (
            entry.is_regular_file()
            &&
            entry.path().extension() == ".json"
        )
        {
            jsonFiles.push_back(
                entry.path()
            );
        }
    }


    if (jsonFiles.empty())
    {
        throw std::runtime_error(
            "Aucun fichier JSON trouve dans patterns/."
        );
    }


    if (jsonFiles.size() > 1)
    {
        throw std::runtime_error(
            "Plusieurs fichiers JSON sont presents dans patterns/. "
            "Conservez un seul motif source."
        );
    }


    return jsonFiles.front();
}


// =========================================================
// ESTIMATION DU NOMBRE DE SEGMENTS
// =========================================================

std::uint64_t estimateSegmentCount(
    std::size_t segmentsPerPattern,
    unsigned int iteration
)
{
    std::uint64_t result = 1;


    for (
        unsigned int i = 0;
        i < iteration;
        ++i
    )
    {
        result *= segmentsPerPattern;
    }


    return result;
}


// =========================================================
// PROGRAMME PRINCIPAL
// =========================================================

int main()
{
    std::cout
        << "=====================================\n"
        << " Universal Fractal Model - SFML 3\n"
        << "=====================================\n\n";


    // -----------------------------------------------------
    // 1. RECHERCHE DU JSON
    // -----------------------------------------------------

    std::filesystem::path patternPath;


    try
    {
        patternPath =
            findPatternFile();
    }
    catch (
        const std::exception& error
    )
    {
        std::cerr
            << "Erreur : "
            << error.what()
            << '\n';

        std::cin.get();

        return 1;
    }


    std::cout
        << "Motif source : "
        << patternPath.filename().string()
        << '\n';


    // -----------------------------------------------------
    // 2. CHARGEMENT DU MOTIF
    // -----------------------------------------------------

    Pattern pattern;


    if (
        !pattern.loadFromJson(
            patternPath.string()
        )
    )
    {
        std::cerr
            << "Impossible de charger le motif source.\n";

        std::cin.get();

        return 1;
    }


    const std::size_t pointCount =
        pattern.getPoints().size();


    if (pointCount < 2)
    {
        std::cerr
            << "Le motif doit contenir au moins deux points.\n";

        return 1;
    }


    const std::size_t segmentsPerPattern =
        pointCount - 1;


    std::cout
        << "Points : "
        << pointCount
        << '\n';


    std::cout
        << "Segments du motif : "
        << segmentsPerPattern
        << "\n\n";


    // -----------------------------------------------------
    // 3. FENETRE SFML
    // -----------------------------------------------------

    sf::RenderWindow window(
        sf::VideoMode(
            {
                WINDOW_WIDTH,
                WINDOW_HEIGHT
            }
        ),
        "Universal Fractal Model"
    );


    window.setFramerateLimit(60);


    // -----------------------------------------------------
    // 4. ANIMATION
    // -----------------------------------------------------

    unsigned int iteration =
        MIN_ITERATION;


    const sf::Time iterationDuration =
        sf::seconds(
            ITERATION_DURATION_SECONDS
        );


    sf::Clock iterationClock;


    // -----------------------------------------------------
    // Fonction locale de génération
    // -----------------------------------------------------

    auto generateIteration =
        [&](unsigned int level)
        -> std::vector<Segment>
    {
        const std::uint64_t estimated =
            estimateSegmentCount(
                segmentsPerPattern,
                level
            );


        std::cout
            << "Iteration "
            << level
            << " : "
            << estimated
            << " segments\n";


        return FractalGenerator::generate(
            pattern.getPoints(),
            level
        );
    };


    // -----------------------------------------------------
    // 5. PREMIER NIVEAU
    // -----------------------------------------------------

    std::vector<Segment> fractal =
        generateIteration(
            iteration
        );


    std::vector<Segment> screenSegments =
        Transform::fitToScreen(
            fractal,
            static_cast<float>(
                WINDOW_WIDTH
            ),
            static_cast<float>(
                WINDOW_HEIGHT
            ),
            DISPLAY_MARGIN
        );


    // -----------------------------------------------------
    // 6. BOUCLE PRINCIPALE
    // -----------------------------------------------------

    while (window.isOpen())
    {
        // -------------------------------------------------
        // EVENEMENTS
        // -------------------------------------------------

        while (
            const std::optional event =
                window.pollEvent()
        )
        {
            if (
                event->is<
                    sf::Event::Closed
                >()
            )
            {
                window.close();
            }


            if (
                const auto* keyPressed =
                    event->getIf<
                        sf::Event::KeyPressed
                    >()
            )
            {
                if (
                    keyPressed->scancode ==
                    sf::Keyboard::Scancode::Escape
                )
                {
                    window.close();
                }
            }
        }


        // -------------------------------------------------
        // CHANGEMENT AUTOMATIQUE DE NIVEAU
        // -------------------------------------------------

        if (
            iterationClock.getElapsedTime()
            >= iterationDuration
        )
        {
            iterationClock.restart();


            ++iteration;


            if (
                iteration > MAX_ITERATION
            )
            {
                iteration =
                    MIN_ITERATION;
            }


            const std::uint64_t estimated =
                estimateSegmentCount(
                    segmentsPerPattern,
                    iteration
                );


            // ---------------------------------------------
            // Limite de sécurité
            // ---------------------------------------------

            if (
                estimated >
                SAFE_SEGMENT_LIMIT
            )
            {
                std::cout
                    << "Iteration "
                    << iteration
                    << " ignoree : "
                    << estimated
                    << " segments.\n";

                continue;
            }


            // ---------------------------------------------
            // Nouvelle fractale
            // ---------------------------------------------

            fractal =
                generateIteration(
                    iteration
                );


            // ---------------------------------------------
            // Ajustement automatique
            // ---------------------------------------------

            screenSegments =
                Transform::fitToScreen(
                    fractal,
                    static_cast<float>(
                        WINDOW_WIDTH
                    ),
                    static_cast<float>(
                        WINDOW_HEIGHT
                    ),
                    DISPLAY_MARGIN
                );
        }


        // -------------------------------------------------
        // RENDU
        // -------------------------------------------------

        window.clear();


        Renderer::drawSegments(
            window,
            screenSegments
        );


        window.display();
    }


    return 0;
}
