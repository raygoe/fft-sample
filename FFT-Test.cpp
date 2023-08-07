#include <SFML/Graphics.hpp>
#include "Plot.hpp"

int main()
{
    // Load Fonts.
    sf::Font light, regular, bold;
    if (!light.loadFromFile("assets/REM-Light.ttf") || !regular.loadFromFile("assets/REM-Regular.ttf") || !bold.loadFromFile("assets/REM-SemiBold.ttf"))
    {
        return 1;
    }

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(800, 600), "FFT Test", sf::Style::Default, settings);

    // Create the plot.
    Plot plot(window, bold, regular);
    plot.Init();

    // Main loop to keep the window running
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // Check for close event
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::Resized)
            {
                // Get the aspect ratio
                float aspectRatio = static_cast<float>(event.size.width) / static_cast<float>(event.size.height);

                // Create a new view with the new size while keeping the aspect ratio
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                sf::View view(visibleArea);

                // Reset the window view
                window.setView(view);

                // Resize the plot
                plot.Resize();
            }
        }

        // Clear the window with black color
        window.clear(sf::Color(0x35, 0x37, 0x40));

        // Update the plot
        plot.Update();

        // Draw the plot
        plot.Draw();

        // Finally, display the rendered frame on screen
        window.display();
    }

    return 0;
}