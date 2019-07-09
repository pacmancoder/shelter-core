#pragma once

#include <shelter-utils/buffer_reader.hpp>

#include <shelter-model/device_descriptor.hpp>
#include <shelter-model/error.hpp>

#include <exl/mixed.hpp>

#include <cstdint>
#include <array>

namespace Shelter { namespace Utils
{
    class SsidParsingError : public Error {};

    class SsidWrongMagicError : public SsidParsingError {};

    class SsidWrongPayloadChecksumError : public SsidParsingError {};

    class SsidInvalidBufferError : public SsidParsingError {};

    using Ssid = std::array<char, 32>;

    using DecodeSsidResult = exl::mixed<
        Model::DeviceDescriptor,
        SsidWrongMagicError,
        SsidWrongPayloadChecksumError,
        SsidInvalidBufferError
    >;

    DecodeSsidResult DecodeDeviceDescriptorFromSsid(const Ssid& ssid);
    Ssid EncodeDeviceDescriptorToSsid(const Model::DeviceDescriptor& descriptor);
}}