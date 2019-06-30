/// Inspired by the following library:
/// https://github.com/zhicheng/base64
#pragma once

#include <cstddef>
#include <cstdint>

namespace Shelter { namespace Utils {

    /// Calculates encoded Base64 size from source buffer size
    /// @warning result size does not include null terminator
    constexpr size_t GetBase64EncodedOutputSize(size_t sourceSize)
    {
        return (((sourceSize) + 2) / 3) * 4;
    }

    /// Calculates decoded Base64 buffer size from source text
    /// @warning source should not include null terminator
    constexpr size_t GetBase64DecodedOutputSize(size_t sourceSize)
    {
        return ((sourceSize) / 4) * 3;
    }

    /// Encodes buffer in to output buffer out.
    /// @return Size of encoded data string (without terminating null character)
    size_t EncodeBase64(const uint8_t* source, size_t sourceSize, char* out);

    /// Decodes text, pointed by in to output buffer out
    /// @return Size of decoded buffer or 0 in case of failure
    size_t DecodeBase64(const char* source, size_t sourceSize, uint8_t* out);
}}