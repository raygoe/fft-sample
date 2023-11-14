#include "Plot.hpp"

#include <chrono>

using namespace std::chrono;

static const sf::Color BackgroundColor(0x35, 0x37, 0x40);  // #353740
static const sf::Color PlotBackgroundColor(0x44, 0x46, 0x55); // #444655
static const sf::Color GridLineColor(0x81, 0x85, 0xa1); // #8185a1
static const sf::Color PlotLineColor(0x00, 0x7a, 0xcc); // #007acc
static const sf::Color TextColor(0xff, 0xff, 0xfa) ; // #fffffa
static const sf::Color MarkerColor(0xff, 0xcb, 0x6b); // #ffcb6b

static const int numGridLinesH = 44;
static const int numGridLinesV = 40;

static const float minPlotY = -110.0f;
static const float maxPlotY =  0.0f;

static const float plotWidth_MHz = 400.0f;
static const float plotCF_MHz = 400.0f;

static const int tracesPerSecond = 10;

void Plot::Init()
{
    m_data->GenFFT();
    Resize();
}

void Plot::Update()
{
    auto now = steady_clock::now();
    if (now < m_lastUpdate + milliseconds(1000 / tracesPerSecond))
    {
        return;
    }

    // Force an FFT update.
    m_data->GenFFT();

    // Build the plot line.
    BuildPlotLine();

    m_lastUpdate = now;
}

void Plot::Resize()
{
    // First, layout the background and set its color.
    m_background.setSize(sf::Vector2f(m_window.getSize().x, m_window.getSize().y));
    m_background.setFillColor(BackgroundColor);

    // Next, we want to create a 40 px border around the plot area and set its color.
    m_plotArea.setSize(sf::Vector2f(m_window.getSize().x - 120, m_window.getSize().y - 120));
    m_plotArea.setPosition(60, 60);
    m_plotArea.setFillColor(PlotBackgroundColor);

    // Next we want to create the grid lines. We want to create horizontal lines and vertical lines.
    // We want to space them evenly across the plot area.
    // We want to make sure that the lines are evenly spaced, so we need to calculate the spacing.
    float spacingH = m_plotArea.getSize().y / numGridLinesH;
    float spacingV = m_plotArea.getSize().x / numGridLinesV;

    // Now we can create the lines.
    m_gridLinesH.clear();
    m_gridLinesV.clear();
    m_gridLabelsH.clear();
    m_gridLabelsV.clear();

    for (int i = 0; i <= numGridLinesH; i++)
    {
        sf::RectangleShape line(sf::Vector2f(m_plotArea.getSize().x, 1));
        line.setPosition(m_plotArea.getPosition().x, m_plotArea.getPosition().y + (i * spacingH));
        line.setFillColor(GridLineColor);
        m_gridLinesH.push_back(line);

        // Now let's create the labels.
        // For the unit, we'll make sure to refer to the min and max plot Y values.
        // Note that we generate the labels from the top down so when i == 0, the label is the max value.
        float labelValue = maxPlotY - ((maxPlotY - minPlotY) * ((float)i / (float)numGridLinesH));

        sf::Text label;
        label.setFont(m_regFont);
        label.setString(std::to_string((int)labelValue));
        label.setCharacterSize(8);
        label.setFillColor(TextColor);
        label.setOrigin(label.getLocalBounds().width + 5, label.getLocalBounds().height / 2);
        label.setPosition(m_plotArea.getPosition().x, m_plotArea.getPosition().y + (i * spacingH));
        m_gridLabelsH.push_back(label);
    }

    for (int i = 0; i <= numGridLinesV; i++)
    {
        sf::RectangleShape line(sf::Vector2f(1, m_plotArea.getSize().y));
        line.setPosition(m_plotArea.getPosition().x + (i * spacingV), m_plotArea.getPosition().y);
        line.setFillColor(GridLineColor);
        m_gridLinesV.push_back(line);

        // Now let's create the labels.
        // For the unit, we'll make sure to refer to the CF and width values.
        // Note that we generate the labels from the right to left so when i == 0, the label is the max value.
        // when i == numGridLinesV/2, the label is the CF value.
        // when i == numGridLinesV, the label is the min value.
        float labelValue = plotCF_MHz + (i - (numGridLinesV / 2)) * (plotWidth_MHz / numGridLinesV);

        sf::Text label;
        label.setFont(m_regFont);
        label.setString(std::to_string((int)labelValue));
        label.setCharacterSize(8);
        label.setFillColor(TextColor);
        label.setOrigin(label.getLocalBounds().width / 2, label.getLocalBounds().height - 10);
        label.setPosition(m_plotArea.getPosition().x + (i * spacingV), m_plotArea.getPosition().y + m_plotArea.getSize().y);
        m_gridLabelsV.push_back(label);
    }

    // Build the plot line.
    BuildPlotLine();

    // Let's create the title text.
    m_titleText.setFont(m_boldFont);
    m_titleText.setString("FFT Test");
    m_titleText.setCharacterSize(24);
    m_titleText.setFillColor(TextColor);
    m_titleText.setOrigin(m_titleText.getLocalBounds().width / 2, m_titleText.getLocalBounds().height / 2);
    m_titleText.setPosition(m_window.getSize().x / 2, 20);

    // Let's create the horizontal label.
    m_labelH.setFont(m_regFont);
    m_labelH.setString("Frequency (MHz)");
    m_labelH.setCharacterSize(16);
    m_labelH.setFillColor(TextColor);
    m_labelH.setOrigin(m_labelH.getLocalBounds().width / 2, m_labelH.getLocalBounds().height / 2);
    m_labelH.setPosition(m_window.getSize().x / 2, m_window.getSize().y - 20);

    // Let's create the vertical label.
    m_labelV.setFont(m_regFont);
    m_labelV.setString("Amplitude (dB)");
    m_labelV.setCharacterSize(16);
    m_labelV.setFillColor(TextColor);
    m_labelV.setOrigin(m_labelV.getLocalBounds().width / 2, m_labelV.getLocalBounds().height / 2);
    m_labelV.setPosition(20, m_window.getSize().y / 2);
    m_labelV.setRotation(-90);
}

void Plot::Draw()
{
    m_window.draw(m_background);
    m_window.draw(m_plotArea);

    for (size_t i = 0; i < m_gridLinesH.size(); i++)
    {
        m_window.draw(m_gridLinesH[i]);
        m_window.draw(m_gridLabelsH[i]);
    }

    for (size_t i = 0; i < m_gridLinesV.size(); i++)
    {
        m_window.draw(m_gridLinesV[i]);
        m_window.draw(m_gridLabelsV[i]);
    }

    m_window.draw(m_titleText);
    m_window.draw(m_labelH);
    m_window.draw(m_labelV);

    m_window.draw(m_marker);
    m_window.draw(m_markerText);

    // Let's clip the plot line to the plot area.
    sf::View oldView = m_window.getView();
    float x = m_plotArea.getPosition().x;
    float y = m_plotArea.getPosition().y;
    float width = m_plotArea.getSize().x;
    float height = m_plotArea.getSize().y;
    sf::View plotClipView(sf::FloatRect(x, y, width, height));
    sf::Vector2u windowSize = m_window.getSize();
    plotClipView.setViewport(sf::FloatRect(x / windowSize.x, y / windowSize.y, width / windowSize.x, height / windowSize.y));
    m_window.setView(plotClipView);
    m_window.draw(m_plotData);
    m_window.setView(oldView);
}

void Plot::BuildPlotLine()
{
    auto mapToPixel = [](float d, float d_min, float d_max, float p_min, float p_max) {
        return p_min + (d - d_min) * (p_max - p_min) / (d_max - d_min);
    };

    float peak_y = -200;
    float peak_x = -200;

    for (size_t i = 0; i < m_data->size(); i++)
    {
        if (m_data->Get(i) > peak_y)
        {
            peak_y = m_data->Get(i);
            peak_x = i;
        }

        float x = (i / static_cast<float>(m_data->size())) * m_plotArea.getSize().x;
        float y = mapToPixel(m_data->Get(i), minPlotY, maxPlotY, m_plotArea.getSize().y, 0.0f);

        m_plotData[i].position = sf::Vector2f(x + m_plotArea.getPosition().x, y + m_plotArea.getPosition().y);
        m_plotData[i].color = PlotLineColor;
    }

    // Let's use the peak data to draw a peak marker.
    if (peak_x >= 0)
    {
        sf::CircleShape peakMarker(2.0f);
        float x = (peak_x / static_cast<float>(m_data->size())) * m_plotArea.getSize().x;
        float y = mapToPixel(peak_y, minPlotY, maxPlotY, m_plotArea.getSize().y, 0.0f);
        peakMarker.setOrigin(peakMarker.getRadius(), peakMarker.getRadius());
        peakMarker.setPosition(x + m_plotArea.getPosition().x, y + m_plotArea.getPosition().y);
        peakMarker.setOutlineColor(MarkerColor);
        peakMarker.setOutlineThickness(1);
        peakMarker.setFillColor(sf::Color::Transparent);
        m_marker = peakMarker;

        // Let's create the peak label just to the right of the marker.
        float x_mhz = plotCF_MHz + (peak_x - (m_data->size() / 2)) * (plotWidth_MHz / m_data->size());
        std::string peakLabelStr = "Peak: " + std::to_string(x_mhz) + " MHz, " + std::to_string(peak_y) + " dB";
        sf::Text peakLabel;
        peakLabel.setFont(m_regFont);
        peakLabel.setString(peakLabelStr);
        peakLabel.setCharacterSize(10);
        peakLabel.setFillColor(MarkerColor);
        peakLabel.setOrigin(0, peakLabel.getLocalBounds().height / 2);
        peakLabel.setPosition(peakMarker.getPosition().x + 5, peakMarker.getPosition().y);
        m_markerText = peakLabel;
    }
}