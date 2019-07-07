#include <catch2/catch.hpp>

#include <algorithm>

#include <shelter-utils/ssid_device_descriptor.hpp>

using namespace Shelter::Utils;
using namespace Shelter::Model;

TEST_CASE("Shelter SSID is parsed correctly", "[shelter-utils]")
{
    // major|minor|patch|service  |class    |vendor  |serial          |crc-8
    // 01   |00   |00   |01       |00000001 |00000001|1122334455667788|18
    const char* ssidString = "SHL_AQAAAQAAAAEAAAABESIzRFVmd4gY";
    Ssid ssid = {{}};
    std::copy(ssidString, ssidString + ssid.size(), ssid.data());

    auto descriptor = DecodeDeviceDescriptorFromSsid(ssid);

    REQUIRE(descriptor.version.GetMajor() == 1);
    REQUIRE(descriptor.version.GetMinor() == 0);
    REQUIRE(descriptor.version.GetPatch() == 0);

    REQUIRE(descriptor.service.GetId() == Service::Id::ConfigureSta);
    REQUIRE(descriptor.deviceClass.GetId() == DeviceClass::Id::RgbLedStrip);
    REQUIRE(descriptor.vendor.GetId() == DeviceVendor::Id::IndividualPacmancoder);
    REQUIRE(descriptor.serial.GetValue() == 0x1122334455667788LL);
}

TEST_CASE("SSID parsing throws on wrong magic", "[shelter-utils]")
{
    const char* ssidString = "ABS_AQAAAQAAAAEAAAABESIzRFVmd4gY";
    Ssid ssid = {{}};
    std::copy(ssidString, ssidString + ssid.size(), ssid.data());
    REQUIRE_THROWS_AS(DecodeDeviceDescriptorFromSsid(ssid), WrongSsidMagicError);
}

TEST_CASE("SSID parsing throws on wrong checksum", "[shelter-utils]")
{
    const char* ssidString = "SHL_AQAAAQAAAAEAAAABESIzRFVmd4gK";
    Ssid ssid = {{}};
    std::copy(ssidString, ssidString + ssid.size(), ssid.data());
    REQUIRE_THROWS_AS(DecodeDeviceDescriptorFromSsid(ssid), WrongSsidPayloadChecksumError);
}

TEST_CASE("Shelter SSID is constructed correctly", "[shelter-utils]")
{
    DeviceDescriptor descriptor {
        ShelterVersion { 1, 0, 0 },
        Service { Service::Id::ConfigureSta },
        DeviceClass { DeviceClass::Id::RgbLedStrip },
        DeviceVendor { DeviceVendor::Id::IndividualPacmancoder },
        DeviceSerial { 0x1122334455667788LL }
    };

    Ssid actualSsidBuffer = EncodeDeviceDescriptorToSsid(descriptor);
    std::string actualSsid;
    for (char c : actualSsidBuffer)
    {
        actualSsid.push_back(c);
    }

    REQUIRE(actualSsid == "SHL_AQAAAQAAAAEAAAABESIzRFVmd4gY");
}