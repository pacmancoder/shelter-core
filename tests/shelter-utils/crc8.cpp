#include <catch2/catch.hpp>

#include <vector>

#include <shelter-utils/private/crc8.hpp>

using namespace Shelter::Utils;

TEST_CASE("Calculated CRC-8 is correct", "[shelter-utils][crc8]")
{
    std::vector<uint8_t> data = { 0x12, 0x34, 0x56, 0x78, 0x90 };
    REQUIRE(Crc8().Update(data.data(), data.size()).Get() == 0xAD);
}

TEST_CASE("Multipart CRC-8 is correct", "[shelter-utils][crc8]")
{
    std::vector<uint8_t> data1 = { 0x12, 0x34 };
    std::vector<uint8_t> data2 = { 0x56, 0x78, 0x90 };

    Crc8 crc;

    crc.Update(data1.data(), data1.size());
    crc.Update(data2.data(), data2.size());

    REQUIRE(crc.Get() == 0xAD);
}