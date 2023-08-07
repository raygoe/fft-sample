#include "Data.hpp"

#include <cmath>

#define PI 3.14159265358979323846

static void generateFMWaveform(int32_t fftData[], int sampleSize, float peakAmplitude) {
    int carrierIndex = sampleSize / 2;  // center of the buffer
    int halfWidth = sampleSize / 4;     // 50% width of the FFT

    // FM parameters
    const int numSidebands = 5;
    const float sidebandDecay = 0.5f;  // How much each sideband is reduced in amplitude

    // Set carrier frequency peak
    fftData[carrierIndex] = peakAmplitude * 1000000.0f;

    // Generate sidebands
    for (int i = 1; i <= numSidebands; ++i) {
        // We want a normal distrubution with a mean of -100 and a standard deviation of 3.
        // Box-Muller transform to generate normal distribution from uniform distribution
        float u1 = (float)rand() / RAND_MAX;
        float u2 = (float)rand() / RAND_MAX;
        float z0 = sqrt(-2.0 * log(u1)) * cos(2.0 * PI * u2);
        float mean = 0.0;
        float stddev = 0.2;
        float signal_random = mean + z0 * stddev;

        float sidebandAmplitude = peakAmplitude * pow(sidebandDecay, i) + signal_random;

        // Using a simple sin modulation for this example
        int sidebandSpacing = (int)(i * (halfWidth / (numSidebands * 2.0f)));

        if (carrierIndex + sidebandSpacing < sampleSize) {
            fftData[carrierIndex + sidebandSpacing] = sidebandAmplitude * 1000000.0f;
        }
        if (carrierIndex - sidebandSpacing >= 0) {
            fftData[carrierIndex - sidebandSpacing] = sidebandAmplitude * 1000000.0f;
        }
    }
}

void Data::GenFFT()
{
    m_data.resize(4096);
    for (int i = 0; i < 4096; i++)
    {
        // We want a normal distrubution with a mean of -100 and a standard deviation of 3.
        // Box-Muller transform to generate normal distribution from uniform distribution
        float u1 = (float)rand() / RAND_MAX;
        float u2 = (float)rand() / RAND_MAX;
        float z0 = sqrt(-2.0 * log(u1)) * cos(2.0 * PI * u2);
        float mean = -110.0;
        float stddev = 3.0;

        float y = mean + z0 * stddev;

        m_data[i] = static_cast<int32_t>(y * 1000000.0f);

        // Generate FM waveform
        generateFMWaveform(m_data.data(), m_data.size(), 40.0f);
    }
}