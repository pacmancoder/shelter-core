#include <catch2/catch.hpp>

#include <algorithm>

#include <shelter-utils/ssid_device_descriptor.hpp>

using namespace Shelter::Utils;

TEST_CASE("SSID encoding is reversible", "[shelter-utils]")
{
    // type     vendor   serial           reserved checksum
    // 54322211 08070605 8887868584838281 00000000 51
    const char* ssidString = "SHLDVDIiEQgHBgWIh4aFhIOCgQAAAABR";
    Ssid ssid = {{}};
    std::copy(ssidString, ssidString + ssid.size(), ssid.data());

    auto descriptor = DecodeDeviceDescriptorFromSsid(ssid);

    REQUIRE(descriptor.type == 0x11223254);
    REQUIRE(descriptor.vendor == 0x05060708);
    REQUIRE(descriptor.serial == 0x8182838485868788);
}