#include "Data.hpp"

#include <iostream>
#include <cmath>
#include <cstdint>

const char * dataPath = "assets/data.bin";

#define MAX_SPECAN_SAMPLE  ((1ULL << 50) - 1)

double linearToDb(uint64_t linear) {
    return 10.0 * log10((double)linear / (double)MAX_SPECAN_SAMPLE);
}

uint64_t dbToLinear(double db) {
    return (uint64_t)(pow(10.0, db / 10.0) * (double)MAX_SPECAN_SAMPLE);
}

float Data::Get(size_t index)
{
    uint64_t sample = m_data.at(index);
    //std::cout << index << "," << linearToDb(sample) << std::endl;
    return static_cast<float>(linearToDb(sample));
}

#define PI 3.14159265358979323846

static void generateFMWaveform(uint64_t fftData[], int sampleSize, float peakAmplitude) {
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
    // We want to open assets/data.bin and then read the data into m_data.
    // If we can't open the file, we should just return.

    FILE *fp = fopen(dataPath, "rb");
    if (fp == NULL)
        return;

    // Read the data into m_data
    fread(m_data.data(), sizeof(uint64_t), m_data.size(), fp);

    // Close the file
    fclose(fp);

    return;
    /*
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

        // Generate FM waveform
        generateFMWaveform(m_data.data(), m_data.size(), -40.0f);
    }
    */
}