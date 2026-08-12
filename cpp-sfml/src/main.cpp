#include <SFML/Graphics.hpp>

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
// PARAMETRES PERSONNALISABLES
// =========================================================
//
// Ces valeurs peuvent être modifiées sans toucher
// au moteur fractal.
//
// Le motif lui-même NE DOIT PAS être défini ici.
// Pour changer de motif, remplacer simplement le fichier
// JSON contenu dans le dossier :
//
//      patterns/
//
// =========================================================

// Taille du motif affiché dans la fenêtre.
constexpr float DISPLAY_LENGTH = 700.f;

// Position du point d'origine M0 dans la fenêtre.
constexpr float OFFSET_X = 250.f;
constexpr float OFFSET_Y = 750.f;

// Première itération affichée.
constexpr unsigned int MIN_ITERATION = 1;

// Limite théorique disponible dans cette démonstration.
//
// ATTENTION :
// avec un motif de 20 segments :
//
// ordre 4 =      160 000 segments
// ordre 5 =    3 200 000 segments
// ordre 6 =   64 000 000 segments
//
// L'ordre 6 nécessite un moteur de rendu plus optimisé.
constexpr unsigned int MAX_ITERATION = 6;

// Durée d'affichage de chaque niveau.
constexpr float ITERATION_DURATION_SECONDS = 2.f;


// =========================================================
// RECHERCHE AUTOMATIQUE DU FICHIER JSON
// =========================================================
//
// L'utilisateur doit placer UN SEUL fichier .json dans
// le dossier patterns.
//
// Cela permet de remplacer le motif sans modifier main.cpp.
//
// =========================================================

std::filesystem::path findPatternFile()
{
    const std::filesystem::path patternsDirectory =
        std::filesystem::path(PROJECT_SOURCE_DIR_PATH)
        / "patterns";

    std::vector<std::filesystem::path> jsonFiles;

    if (!std::filesystem::exists(patternsDirectory))
    {
        throw std::runtime_error(
            "Le dossier patterns est introuvable."
        );
    }

    for (
        const auto& entry :
        std::filesystem::directory_iterator(patternsDirectory)
    )
    {
        if (
            entry.is_regular_file()
            &&
            entry.path().extension() == ".json"
        )
        {
            jsonFiles.push_back(entry.path());
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
// CALCUL DU NOMBRE THEORIQUE DE SEGMENTS
// =========================================================
//
// Permet d'avertir l'utilisateur avant de demander
// une itération extrêmement coûteuse.
//
// =========================================================

std::uint64_t estimateSegmentCount(
    std::size_t segmentsPerPattern,
    unsigned int iteration
)
{
    std::uint64_t result = 1;

    for (unsigned int i = 0; i < iteration; ++i)
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
        << " Universal Fractal Model - SFML\n"
        << "=====================================\n\n";

    // -----------------------------------------------------
    // 1. DETECTION DU MOTIF JSON
    // -----------------------------------------------------

    std::filesystem::path patternPath;

    try
    {
        patternPath = findPatternFile();
    }
    catch (const std::exception& error)
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
        << patternPath.string()
        << '\n';


    // -----------------------------------------------------
    // 2. CHARGEMENT DU DICTIONNAIRE
    // -----------------------------------------------------

    Pattern pattern;

    if (!pattern.loadFromJson(patternPath.string()))
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
    // 3. CREATION DE LA FENETRE SFML
    // -----------------------------------------------------

    sf::RenderWindow window(
        sf::VideoMode({1200, 900}),
        "Universal Fractal Model"
    );

    window.setFramerateLimit(60);


    // -----------------------------------------------------
    // 4. INITIALISATION DE L'ANIMATION
    // -----------------------------------------------------

    unsigned int iteration =
        MIN_ITERATION;

    const sf::Time iterationDuration =
        sf::seconds(
            ITERATION_DURATION_SECONDS
        );

    sf::Clock iterationClock;


    // -----------------------------------------------------
    // Fonction locale permettant de générer un niveau.
    // -----------------------------------------------------

    auto generateIteration =
        [&](unsigned int level)
        -> std::vector<Segment>
    {
        const std::uint64_t estimatedSegments =
            estimateSegmentCount(
                segmentsPerPattern,
                level
            );

        std::cout
            << "Iteration "
            << level
            << " : "
            << estimatedSegments
            << " segments"
            << '\n';

        return FractalGenerator::generate(
            pattern.getPoints(),
            level
        );
    };


    // -----------------------------------------------------
    // 5. GENERATION DU PREMIER NIVEAU
    // -----------------------------------------------------

    std::vector<Segment> fractal =
        generateIteration(iteration);

    std::vector<Segment> screenSegments =
        Transform::toScreen(
            fractal,
            DISPLAY_LENGTH,
            OFFSET_X,
            OFFSET_Y
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
                // Echap ferme la démonstration.
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
        // 7. PASSAGE AUTOMATIQUE AU NIVEAU SUIVANT
        // -------------------------------------------------

        if (
            iterationClock.getElapsedTime()
            >= iterationDuration
        )
        {
            iterationClock.restart();

            ++iteration;

            // Boucle infinie :
            //
            // 1 → 2 → 3 → ... → MAX → 1 → ...
            //
            if (iteration > MAX_ITERATION)
            {
                iteration =
                    MIN_ITERATION;
            }

            const std::uint64_t estimatedSegments =
                estimateSegmentCount(
                    segmentsPerPattern,
                    iteration
                );

            // -------------------------------------------------
            // SECURITE
            // -------------------------------------------------
            //
            // Cette architecture stocke tous les segments
            // en mémoire.
            //
            // Au-delà de quelques millions de segments,
            // le rendu devient très coûteux.
            //
            // On autorise le modèle jusqu'à 6, mais on évite
            // ici d'essayer de matérialiser 64 millions de
            // segments avec le renderer pédagogique actuel.
            //
            // -------------------------------------------------

            constexpr std::uint64_t SAFE_SEGMENT_LIMIT =
                4'000'000;

            if (
                estimatedSegments
                > SAFE_SEGMENT_LIMIT
            )
            {
                std::cout
                    << "Iteration "
                    << iteration
                    << " ignoree pour ce renderer : "
                    << estimatedSegments
                    << " segments.\n";

                continue;
            }


            // -------------------------------------------------
            // GENERATION DU NOUVEAU NIVEAU
            // -------------------------------------------------

            fractal =
                generateIteration(
                    iteration
                );

            screenSegments =
                Transform::toScreen(
                    fractal,
                    DISPLAY_LENGTH,
                    OFFSET_X,
                    OFFSET_Y
                );
        }


        // -------------------------------------------------
        // 8. AFFICHAGE
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
