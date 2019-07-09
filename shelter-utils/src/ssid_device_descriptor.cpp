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

    Utils::DecodeSsidResult ReadDeviceDescriptor(Utils::BufferReader& buffer)
    {
        auto majorVersion = buffer.Read<uint8_t>();
        auto minorVersion = buffer.Read<uint8_t>();
        auto patchVersion = buffer.Read<uint8_t>();

        auto service = buffer.Read<uint8_t>();

        auto deviceClass = buffer.Read<uint32_t>();
        auto deviceVendor = buffer.Read<uint32_t>();
        auto deviceSerial = buffer.Read<uint64_t>();

        if ((!minorVersion.is<uint8_t>())
            || (!minorVersion.is<uint8_t>())
            || (!patchVersion.is<uint8_t>())
            || (!service.is<uint8_t>())
            || (!deviceClass.is<uint32_t>())
            || (!deviceVendor.is<uint32_t>())
            || (!deviceSerial.is<uint64_t>()))
        {
            return Utils::SsidInvalidBufferError {};
        }

        return DeviceDescriptor {
            ShelterVersion {
                majorVersion.unwrap<uint8_t>(),
                minorVersion.unwrap<uint8_t>(),
                patchVersion.unwrap<uint8_t>()
            },
            Service { service.unwrap<uint8_t>() },
            DeviceClass { deviceClass.unwrap<uint32_t>() },
            DeviceVendor { deviceVendor.unwrap<uint32_t>() },
            DeviceSerial { deviceSerial.unwrap<uint64_t>() }
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

Utils::DecodeSsidResult Utils::DecodeDeviceDescriptorFromSsid(const Ssid& ssid)
{
    if (!std::equal(ssid.data(), ssid.data() + sizeof(MAGIC), MAGIC))
    {
        return Utils::SsidWrongMagicError {};
    }

    uint8_t rawBuffer[DESCRIPTOR_BUFFER_SIZE] = {};
    DecodeBase64(ssid.data() + sizeof(MAGIC), BASE64_STRING_SIZE, rawBuffer);

    if (!IsDescriptorChecksumValid(rawBuffer))
    {
        return Utils::SsidWrongPayloadChecksumError {};
    }

    auto buffer =
        Utils::BufferReader::Make(rawBuffer, DESCRIPTOR_BUFFER_SIZE, Utils::ByteOrder::BE);

    if (!buffer.is<BufferReader>())
    {
        return SsidInvalidBufferError {};
    }

    return ReadDeviceDescriptor(buffer.unwrap<BufferReader>());
}

Utils::Ssid Utils::EncodeDeviceDescriptorToSsid(const Model::DeviceDescriptor& descriptor)
{
    Ssid ssid = {{}};
    std::copy(MAGIC, MAGIC + sizeof(MAGIC), ssid.data());

    uint8_t rawBuffer[DESCRIPTOR_BUFFER_SIZE] = {};

    // max write buffer size specified without checksum byte
    auto buffer = Utils::BufferWriter::Make(
        rawBuffer,
        DESCRIPTOR_BUFFER_SIZE - 1,
        ByteOrder::BE
    );

    if (!buffer.is<BufferWriter>())
    {
        // Raw buffer length will be always sufficient for SSID encoding
        std::terminate();
    }

    WriteDeviceDescriptor(descriptor, buffer.unwrap<BufferWriter>());

    // write resulting checksum to the last byte
    rawBuffer[DESCRIPTOR_BUFFER_SIZE - 1] =
        Crc8().Update(rawBuffer, DESCRIPTOR_BUFFER_SIZE - 1).Get();

    EncodeBase64(rawBuffer, DESCRIPTOR_BUFFER_SIZE, ssid.data() + sizeof(MAGIC));

    return ssid;
}
