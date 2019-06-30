#include <shelter-utils/ssid_device_descriptor.hpp>

#include <algorithm>

#include <shelter-utils/private/base64.hpp>

using namespace Shelter::Model;
using namespace Shelter;

namespace
{
    constexpr char MAGIC[4] = { 'S', 'H', 'L', 'D' };
    constexpr size_t BASE64_STRING_SIZE = 28;
    constexpr size_t DESCRIPTOR_BUFFER_SIZE = Utils::GetBase64DecodedOutputSize(BASE64_STRING_SIZE);

    bool IsDescriptorChecksumValid(const uint8_t* buffer)
    {
        auto expectedChecksum = buffer[DESCRIPTOR_BUFFER_SIZE - 1];

        uint8_t checksum = 0x00;
        for (int i = 0; i < DESCRIPTOR_BUFFER_SIZE - 1; ++i)
        {
            checksum ^= buffer[i];
        }

        return expectedChecksum == checksum;
    }

    uint32_t Uint32FromLittleEndian(uint8_t* v)
    {
        return static_cast<uint32_t>(v[0]) << 0
            | static_cast<uint32_t>(v[1]) << 8
            | static_cast<uint32_t>(v[2]) << 16
            | static_cast<uint32_t>(v[3]) << 24;
    }

    uint64_t Uint64FromLittleEndian(uint8_t* v)
    {
        return static_cast<uint64_t>(v[0]) << 0
            | static_cast<uint64_t>(v[1]) << 8
            | static_cast<uint64_t>(v[2]) << 16
            | static_cast<uint64_t>(v[3]) << 24
            | static_cast<uint64_t>(v[4]) << 32
            | static_cast<uint64_t>(v[5]) << 40
            | static_cast<uint64_t>(v[6]) << 48
            | static_cast<uint64_t>(v[7]) << 56;
    }
}

DeviceDescriptor Utils::DecodeDeviceDescriptorFromSsid(const Ssid& ssid)
{
    if (!std::equal(ssid.data(), ssid.data() + sizeof(MAGIC), MAGIC))
    {
        std::terminate();
    }

    uint8_t descriptorBuffer[DESCRIPTOR_BUFFER_SIZE] = {};
    DecodeBase64(ssid.data() + sizeof(MAGIC), BASE64_STRING_SIZE, descriptorBuffer);

    if (!IsDescriptorChecksumValid(descriptorBuffer))
    {
        std::terminate();
    }

    return DeviceDescriptor {
        Uint32FromLittleEndian(descriptorBuffer),
        Uint32FromLittleEndian(descriptorBuffer + sizeof(uint32_t)),
        Uint64FromLittleEndian(descriptorBuffer + sizeof(uint32_t) * 2)
    };
}
