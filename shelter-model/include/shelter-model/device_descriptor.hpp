#pragma once

#include <cstdint>
#include <array>

namespace Shelter { namespace Model
{
    using DeviceType = uint32_t;
    using DeviceVendor = uint32_t;
    using DeviceSerial = uint64_t;

    struct DeviceDescriptor
    {
        DeviceType type;
        DeviceVendor vendor;
        DeviceSerial serial;
    };
}}