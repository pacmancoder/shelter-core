#pragma once

#include <shelter-model/device_descriptor.hpp>
#include <shelter-model/error.hpp>

#include <shelter-utils/buffer_reader.hpp>

#include <cstdint>
#include <array>

namespace Shelter { namespace Utils
{
    class SsidParsingError : public Error
    {
    public:
        SsidParsingError()
            : Error(nullptr) {}
    };

    class WrongSsidMagicError : public SsidParsingError
    {
    private:
        void DescribeCause(std::ostream& out) const override
        {
            out << "SSID has wrong magic";
        }

        void DescribeName(std::ostream& out) const override
        {
            out << "WrongSsidMagicError";
        }
    };

    class WrongSsidPayloadChecksumError : public SsidParsingError
    {
    private:
        void DescribeCause(std::ostream& out) const override
        {
            out << "SSID payload has wrong checksum";
        }

        void DescribeName(std::ostream& out) const override
        {
            out << "WrongSsidPayloadChecksumError";
        }
    };

    using Ssid = std::array<char, 32>;

    Model::DeviceDescriptor DecodeDeviceDescriptorFromSsid(const Ssid& ssid);
    Ssid EncodeDeviceDescriptorToSsid(const Model::DeviceDescriptor& descriptor);
}}