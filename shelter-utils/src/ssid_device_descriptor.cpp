#include <shelter-utils/ssid_device_descriptor.hpp>

#include <algorithm>

#include <shelter-utils/buffer_reader.hpp>
#include <shelter-utils/private/base64.hpp>
#include <shelter-utils/private/crc8.hpp>
#include <shelter-utils/buffer_writer.hpp>

using namespace Shelter::Model;
using namespace Shelter;

namespace
{
    constexpr char MAGIC[4] = { 'S', 'H', 'L', '_' };
    constexpr size_t BASE64_STRING_SIZE = 28;
    constexpr size_t DESCRIPTOR_BUFFER_SIZE = Utils::GetBase64DecodedOutputSize(BASE64_STRING_SIZE);

    bool IsDescriptorChecksumValid(const uint8_t* buffer)
    {
        auto expectedChecksum = buffer[DESCRIPTOR_BUFFER_SIZE - 1];
        auto actualChecksum = Utils::Crc8().Update(buffer, DESCRIPTOR_BUFFER_SIZE - 1).Get();
        return expectedChecksum == actualChecksum;
    }

    DeviceDescriptor ReadDeviceDescriptor(Utils::BufferReader& buffer)
    {
        return {
            ShelterVersion {
                buffer.Read<uint8_t>(),
                buffer.Read<uint8_t>(),
                buffer.Read<uint8_t>()
            },
            Service { buffer.Read<uint8_t>() },
            DeviceClass { buffer.Read<uint32_t>() },
            DeviceVendor { buffer.Read<uint32_t>() },
            DeviceSerial { buffer.Read<uint64_t>() }
        };
    }

    void WriteDeviceDescriptor(const DeviceDescriptor& descriptor, Utils::BufferWriter& buffer)
    {
        buffer.Write<uint8_t>(descriptor.version.GetMajor());
        buffer.Write<uint8_t>(descriptor.version.GetMinor());
        buffer.Write<uint8_t>(descriptor.version.GetPatch());
        buffer.Write<uint8_t>(static_cast<uint8_t>(descriptor.service.GetId()));
        buffer.Write<uint32_t>(static_cast<uint32_t>(descriptor.deviceClass.GetId()));
        buffer.Write<uint32_t>(static_cast<uint32_t>(descriptor.vendor.GetId()));
        buffer.Write<uint64_t>(descriptor.serial.GetValue());
    }

}

DeviceDescriptor Utils::DecodeDeviceDescriptorFromSsid(const Ssid& ssid)
{
    if (!std::equal(ssid.data(), ssid.data() + sizeof(MAGIC), MAGIC))
    {
        throw WrongSsidMagicError();
    }

    uint8_t rawBuffer[DESCRIPTOR_BUFFER_SIZE] = {};
    DecodeBase64(ssid.data() + sizeof(MAGIC), BASE64_STRING_SIZE, rawBuffer);

    if (!IsDescriptorChecksumValid(rawBuffer))
    {
        throw WrongSsidPayloadChecksumError();
    }

    Utils::BufferReader buffer(rawBuffer, DESCRIPTOR_BUFFER_SIZE, Utils::ByteOrder::BE);
    return ReadDeviceDescriptor(buffer);
}

Utils::Ssid Utils::EncodeDeviceDescriptorToSsid(const Model::DeviceDescriptor& descriptor)
{
    Ssid ssid = {{}};
    std::copy(MAGIC, MAGIC + sizeof(MAGIC), ssid.data());

    uint8_t rawBuffer[DESCRIPTOR_BUFFER_SIZE] = {};

    // max write buffer size specified without checksum byte
    Utils::BufferWriter buffer(rawBuffer, DESCRIPTOR_BUFFER_SIZE - 1, ByteOrder::BE);

    WriteDeviceDescriptor(descriptor, buffer);

    // write resulting checksum to the last byte
    rawBuffer[DESCRIPTOR_BUFFER_SIZE - 1] =
        Crc8().Update(rawBuffer, DESCRIPTOR_BUFFER_SIZE - 1).Get();

    EncodeBase64(rawBuffer, DESCRIPTOR_BUFFER_SIZE, ssid.data() + sizeof(MAGIC));

    return ssid;
}
