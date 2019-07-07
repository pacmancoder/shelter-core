#pragma once

#include <cstdint>
#include <array>

namespace Shelter { namespace Model
{
    class ShelterVersion
    {
    public:
        constexpr ShelterVersion(uint8_t major, uint8_t minor, uint8_t patch)
            : major_(major), minor_(minor), patch_(patch) {}

        uint8_t GetMajor() const { return major_; };
        uint8_t GetMinor() const { return minor_; };
        uint8_t GetPatch() const { return patch_; };

    private:
        uint8_t major_;
        uint8_t minor_;
        uint8_t patch_;
    };

    class Service
    {
    public:
        enum class Id : uint8_t
        {
            Unspecified  = 0x00,
            ConfigureSta = 0x01
        };

    public:
        explicit constexpr Service(Id id)
            : id_(id) {}
        explicit constexpr Service(uint8_t id)
            : id_(static_cast<Id>(id)) {}

        Id GetId() const { return id_; }
        bool IsUnknown() const;
        const char* GetDescription() const;

    private:
        Id id_;
    };

    class DeviceClass
    {
    public:
        enum class Id : uint32_t
        {
            Unspecified = 0x00000000,
            RgbLedStrip = 0x00000001
        };

    public:
        explicit constexpr DeviceClass(Id id)
            : id_(id) {}
        explicit constexpr DeviceClass(uint32_t id)
            : id_(static_cast<Id>(id)) {}

        Id GetId() const { return id_; }
        bool IsUnknown() const;
        const char* GetDescription() const;

    private:
        Id id_;
    };

    class DeviceVendor
    {
    public:
        enum class Id : uint32_t
        {
            Unspecified           = 0x00000000,
            IndividualPacmancoder = 0x00000001,
        };

    public:
        explicit constexpr DeviceVendor(Id id)
            : id_(id) {}
        explicit constexpr DeviceVendor(uint32_t id)
            : id_(static_cast<Id>(id)) {}

        Id GetId() const { return id_; }
        bool IsUnknown() const;
        const char* GetDescription() const;

    private:
        Id id_;
    };

    class DeviceSerial
    {
    public:
        explicit constexpr DeviceSerial(uint64_t serial)
            : serial_(serial) {}

        uint64_t GetValue() const { return serial_; }

    private:
        uint64_t serial_;
    };


    struct DeviceDescriptor
    {
        ShelterVersion version;
        Service service;
        DeviceClass deviceClass;
        DeviceVendor vendor;
        DeviceSerial serial;
    };
}}