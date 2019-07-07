#include <shelter-utils/buffer_reader.hpp>

#include <shelter-model/error.hpp>

using namespace Shelter::Utils;

void BufferReader::ThrowLENotSupported()
{
    throw NotSupportedError("BufferReader does not support big LE byte order yet");
}

void BufferReader::PerformStep(size_t step)
{
    if (pos_ + step > bufferSize_)
    {
        throw IndexOutOfBoundsError(pos_ + step);
    }

    pos_ += step;
}

template <>
uint8_t BufferReader::Read<ByteOrder::BE, uint8_t>()
{
    PerformStep(1);
    return buffer_[pos_ - 1];
}

template <>
uint32_t BufferReader::Read<ByteOrder::BE, uint32_t>()
{
    PerformStep(4);

    return static_cast<uint32_t>(buffer_[pos_ - 1]) << 0
        | static_cast<uint32_t>(buffer_[pos_ - 2]) << 8
        | static_cast<uint32_t>(buffer_[pos_ - 3]) << 16
        | static_cast<uint32_t>(buffer_[pos_ - 4]) << 24;
}

template <>
uint64_t BufferReader::Read<ByteOrder::BE, uint64_t>()
{
    PerformStep(8);

    return static_cast<uint64_t>(buffer_[pos_ - 1]) << 0
        | static_cast<uint64_t>(buffer_[pos_ - 2]) << 8
        | static_cast<uint64_t>(buffer_[pos_ - 3]) << 16
        | static_cast<uint64_t>(buffer_[pos_ - 4]) << 24
        | static_cast<uint64_t>(buffer_[pos_ - 5]) << 32
        | static_cast<uint64_t>(buffer_[pos_ - 6]) << 40
        | static_cast<uint64_t>(buffer_[pos_ - 7]) << 48
        | static_cast<uint64_t>(buffer_[pos_ - 8]) << 56;
}