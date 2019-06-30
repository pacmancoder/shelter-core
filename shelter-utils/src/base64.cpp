#include <shelter-utils/private/base64.hpp>

using namespace Shelter;

namespace
{
    constexpr char PADDING_CHAR = '=';

    constexpr char ENCODE_LOOKUP_TABLE[] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
        'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
        'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
        'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
        'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
        'w', 'x', 'y', 'z', '0', '1', '2', '3',
        '4', '5', '6', '7', '8', '9', '+', '/',
    };

    // @formatter:off

    /// 255 => Unused character
    constexpr uint8_t DECODE_LOOKUP_TABLE[] = {
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255,  62, 255, 255, 255,  63,
        52,   53,  54,  55,  56,  57,  58,  59,
        60,   61, 255, 255, 255, 255, 255, 255,
        255,   0,   1,  2,   3,   4,   5,    6,
        7,     8,   9,  10,  11,  12,  13,  14,
        15,   16,  17,  18,  19,  20,  21,  22,
        23,   24,  25, 255, 255, 255, 255, 255,
        255,  26,  27,  28,  29,  30,  31,  32,
        33,   34,  35,  36,  37,  38,  39,  40,
        41,   42,  43,  44,  45,  46,  47,  48,
        49,   50,  51, 255, 255, 255, 255, 255
    };

    // @formatter:on
}

size_t Utils::EncodeBase64(const uint8_t* source, size_t sourceSize, char* out)
{
    size_t subCharIndex = 0, sourcePos = 0, outputPos = 0;
    uint8_t previousByte = 0;

    while (sourcePos < sourceSize)
    {
        uint8_t currentByte = source[sourcePos];

        switch (subCharIndex)
        {
            case 0:
            {
                out[outputPos++] = ENCODE_LOOKUP_TABLE[(currentByte >> 2) & 0x3F];
                break;
            }
            case 1:
            {
                out[outputPos++] = ENCODE_LOOKUP_TABLE[((previousByte & 0x3) << 4)
                    | ((currentByte >> 4) & 0xF)];
                break;
            }
            default:
                out[outputPos++] = ENCODE_LOOKUP_TABLE[((previousByte & 0xF) << 2)
                    | ((currentByte >> 6) & 0x3)];
                out[outputPos++] = ENCODE_LOOKUP_TABLE[currentByte & 0x3F];
                break;
        }

        subCharIndex = (subCharIndex + 1) % 3;
        previousByte = currentByte;
        ++sourcePos;
    }

    switch (subCharIndex)
    {
        case 1:
        {
            out[outputPos++] = ENCODE_LOOKUP_TABLE[(previousByte & 0x3) << 4];
            out[outputPos++] = PADDING_CHAR;
            out[outputPos++] = PADDING_CHAR;
            break;
        }
        case 2:
        {
            out[outputPos++] = ENCODE_LOOKUP_TABLE[(previousByte & 0xF) << 2];
            out[outputPos++] = PADDING_CHAR;
            break;
        }
        default:
        {
            break;
        }
    }

    return outputPos;
}

size_t Utils::DecodeBase64(const char* source, size_t sourceSize, uint8_t* out)
{
    if (sourceSize & 0x3)
    {
        return 0;
    }

    size_t sourcePos = 0, outputPos = 0;

    while (sourcePos < sourceSize)
    {
        if (source[sourcePos] == PADDING_CHAR)
        {
            break;
        }

        if (source[sourcePos] < 0)
        {
            return 0;
        }

        uint8_t currentNumberPart = DECODE_LOOKUP_TABLE[static_cast<size_t>(source[sourcePos])];

        if (currentNumberPart == 255)
        {
            return 0;
        }

        switch (sourcePos & 0x3)
        {
            case 0:
            {
                out[outputPos] = (currentNumberPart << 2) & 0xFF;
                break;
            }
            case 1:
            {
                out[outputPos++] |= (currentNumberPart >> 4) & 0x3;
                out[outputPos] = (currentNumberPart & 0xF) << 4;
                break;
            }
            case 2:
            {
                out[outputPos++] |= (currentNumberPart >> 2) & 0xF;
                out[outputPos] = (currentNumberPart & 0x3) << 6;
                break;
            }
            default:
            {
                out[outputPos++] |= currentNumberPart;
                break;
            }
        }
        ++sourcePos;
    }

    return outputPos;
}
