#include <shelter-utils/buffer_writer.hpp>

#include <shelter-model/error.hpp>

using namespace Shelter::Utils;

void BufferWriter::ThrowLENotSupported()
{
    throw NotSupportedError("BufferWriter does not support big LE byte order yet");
}

void BufferWriter::PerformStep(size_t step)
{
    if (pos_ + step > bufferSize_)
    {
        throw IndexOutOfBoundsError(pos_ + step);
    }

    pos_ += step;
}

template <>
void BufferWriter::Write<ByteOrder::BE, uint8_t>(uint8_t value)
{
    PerformStep(1);
    buffer_[pos_ - 1] = value;
}

template <>
void BufferWriter::Write<ByteOrder::BE, uint32_t>(uint32_t value)
{
    PerformStep(4);
    buffer_[pos_ - 1] = static_cast<uint8_t>((value & 0x000000FF) >> 0);
    buffer_[pos_ - 2] = static_cast<uint8_t>((value & 0x0000FF00) >> 8);
    buffer_[pos_ - 3] = static_cast<uint8_t>((value & 0x00FF0000) >> 16);
    buffer_[pos_ - 4] = static_cast<uint8_t>((value & 0xFF000000) >> 24);
}

template <>
void BufferWriter::Write<ByteOrder::BE, uint64_t>(uint64_t value)
{
    PerformStep(8);
    buffer_[pos_ - 1] = static_cast<uint8_t>((value & 0x00000000000000FF) >> 0);
    buffer_[pos_ - 2] = static_cast<uint8_t>((value & 0x000000000000FF00) >> 8);
    buffer_[pos_ - 3] = static_cast<uint8_t>((value & 0x0000000000FF0000) >> 16);
    buffer_[pos_ - 4] = static_cast<uint8_t>((value & 0x00000000FF000000) >> 24);
    buffer_[pos_ - 5] = static_cast<uint8_t>((value & 0x000000FF00000000) >> 32);
    buffer_[pos_ - 6] = static_cast<uint8_t>((value & 0x0000FF0000000000) >> 40);
    buffer_[pos_ - 7] = static_cast<uint8_t>((value & 0x00FF000000000000) >> 48);
    buffer_[pos_ - 8] = static_cast<uint8_t>((value & 0xFF00000000000000) >> 56);
}