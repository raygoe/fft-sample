#include "Data.hpp"

#include <cmath>
#include <cstdint>

double linearToDb(uint32_t linear) {
    return 10.0 * log10((double)linear / (double)0xffffffff);
}

uint32_t dbToLinear(double db) {
    return (uint32_t)(pow(10.0, db / 10.0) * (double)0xffffffff);
}

float Data::Get(size_t index)
{
    uint32_t sample = m_data.at(index);
    return static_cast<float>(linearToDb(sample));
}

#define PI 3.14159265358979323846

static void generateFMWaveform(uint32_t fftData[], int sampleSize, float peakAmplitude) {
    int carrierIndex = sampleSize / 2;  // center of the buffer
    int halfWidth = sampleSize / 4;     // 50% width of the FFT

    // FM parameters
    const int numSidebands = 5;
    const float sidebandDecay = 0.5f;  // How much each sideband is reduced in amplitude

    // Set carrier frequency peak
    fftData[carrierIndex] = dbToLinear(peakAmplitude);

    // Generate sidebands
    for (int i = 1; i <= numSidebands; ++i) {
        // Box-Muller transform to generate normal distribution from uniform distribution
        float u1 = (float)rand() / RAND_MAX;
        float u2 = (float)rand() / RAND_MAX;
        float z0 = sqrt(-2.0 * log(u1)) * cos(2.0 * PI * u2);
        float mean = 0.0;
        float stddev = 0.2;
        float signal_random = mean + z0 * stddev;

        float sidebandAmplitude;

        if (peakAmplitude > 0) {
            sidebandAmplitude = peakAmplitude * pow(sidebandDecay, i) + signal_random;
        } else {
            sidebandAmplitude = peakAmplitude * pow(sidebandDecay, numSidebands - i + 1) + signal_random;
        }

        // Using a simple sin modulation for this example
        int sidebandSpacing = (int)(i * (halfWidth / (numSidebands * 2.0f)));

        if (carrierIndex + sidebandSpacing < sampleSize) {
            fftData[carrierIndex + sidebandSpacing] = dbToLinear(sidebandAmplitude);
        }
        if (carrierIndex - sidebandSpacing >= 0) {
            fftData[carrierIndex - sidebandSpacing] = dbToLinear(sidebandAmplitude);
        }
    }
}

void Data::GenFFT()
{
    m_data.resize(4096);
    for (int i = 0; i < 4096; i++)
    {
        // Box-Muller transform to generate normal distribution from uniform distribution
        float u1 = (float)rand() / RAND_MAX;
        float u2 = (float)rand() / RAND_MAX;
        float z0 = sqrt(-2.0 * log(u1)) * cos(2.0 * PI * u2);
        float mean = -85.0;
        float stddev = 3.0;

        float y = mean + z0 * stddev;

        m_data[i] = dbToLinear(y);
    }

    // Generate FM waveform
    generateFMWaveform(m_data.data(), m_data.size(), -40.0f);
}