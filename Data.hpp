#pragma once

#include <cstdint>
#include <vector>
#include <memory>

class Data
{
public:
    Data() {
        m_data.resize(4096);
        GenFFT();
    };
    virtual ~Data() = default;

    static std::shared_ptr<Data> CreateData()
    {
        return std::make_shared<Data>();
    }

    float Get(size_t index)
    {
        int32_t sample = m_data.at(index);
        return static_cast<float>(sample) / 1000000.0f;
    }

    size_t size()
    {
        return m_data.size();
    }

    void GenFFT();

private:
    std::vector<int32_t> m_data;
};