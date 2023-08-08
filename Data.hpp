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

    float Get(size_t index);

    size_t size()
    {
        return m_data.size();
    }

    void GenFFT();

private:
    std::vector<uint32_t> m_data;
};