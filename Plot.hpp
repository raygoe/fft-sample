#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <chrono>
#include "Data.hpp"

class Plot
{
public:
    Plot(sf::RenderWindow& window, sf::Font& boldFont, sf::Font& regFont)
        : m_window(window), m_boldFont(boldFont), m_regFont(regFont),
          m_data(Data::CreateData()), m_plotData(sf::LinesStrip, m_data->size()), m_lastUpdate{std::chrono::steady_clock::now()} {};
    virtual ~Plot() = default;

    void Init();
    void Update();
    void Draw();
    void Resize();

private:
    void BuildPlotLine();

    sf::RenderWindow& m_window;
    sf::Font& m_boldFont;
    sf::Font& m_regFont;
    sf::RectangleShape m_background;
    sf::RectangleShape m_plotArea;
    std::vector<sf::RectangleShape> m_gridLinesH;
    std::vector<sf::Text> m_gridLabelsH;
    std::vector<sf::RectangleShape> m_gridLinesV;
    std::vector<sf::Text> m_gridLabelsV;
    sf::Text m_titleText;
    sf::Text m_labelH;
    sf::Text m_labelV;
    std::shared_ptr<Data> m_data;
    sf::VertexArray m_plotData;
    std::chrono::time_point<std::chrono::steady_clock> m_lastUpdate;
    sf::CircleShape m_marker;
    sf::Text m_markerText;
};