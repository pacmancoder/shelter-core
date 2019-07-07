#include <shelter-model/device_descriptor.hpp>

bool Shelter::Model::Service::IsUnknown() const
{
    switch (id_)
    {
        case Id::Unspecified:
        case Id::ConfigureSta:
        {
            return false;
        }
    }

    return true;
}

const char* Shelter::Model::Service::GetDescription() const
{
    switch (id_)
    {
        case Id::Unspecified:  return "Unspecified service";
        case Id::ConfigureSta: return "WiFi Access point configuration endpoint";
    }

    return "Unknown service";
}

bool Shelter::Model::DeviceVendor::IsUnknown() const
{
    switch (id_)
    {
        case Id::Unspecified:
        case Id::IndividualPacmancoder:
        {
            return false;
        }
    }

    return true;
}

const char* Shelter::Model::DeviceVendor::GetDescription() const
{
    switch (id_)
    {
        case Id::Unspecified:           return "Unspecified vendor";
        case Id::IndividualPacmancoder: return "Vladislav Nikonov <mail@pacmancoder.xyz>";
    }

    return "Unknown vendor";
}

bool Shelter::Model::DeviceClass::IsUnknown() const
{
    switch (id_)
    {
        case Id::Unspecified:
        case Id::RgbLedStrip:
        {
            return false;
        }
    }

    return true;
}

const char* Shelter::Model::DeviceClass::GetDescription() const
{
    switch (id_)
    {
        case Id::Unspecified: return "Unspecified device class";
        case Id::RgbLedStrip: return "RGB LED strip";
    }

    return "Unknown device class";
}
