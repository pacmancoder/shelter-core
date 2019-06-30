#pragma once

#include <cstdint>
#include <array>

#include <shelter-model/device_descriptor.hpp>

namespace Shelter { namespace Utils
{
    using Ssid = std::array<char, 32>;

    Ssid EncodeDeviceDescriptorToSsid(const Model::DeviceDescriptor& descriptor);
    Model::DeviceDescriptor DecodeDeviceDescriptorFromSsid(const Ssid& ssid);
}}