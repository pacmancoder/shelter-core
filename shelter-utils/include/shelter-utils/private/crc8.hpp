/// Inspired by the following article:
/// http://oshgarage.com/the-crc8-checksum/
#pragma once

#include <cstddef>
#include <cstdint>

namespace Shelter { namespace Utils
{
    class Crc8 final
    {
    public:
        explicit Crc8();
        Crc8& Update(const uint8_t* buffer, size_t size);

        uint8_t Get() const;

    private:
        uint8_t crc_;
    };
}}