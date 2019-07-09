#include <shelter-utils/buffer_writer.hpp>

#include <shelter-model/error.hpp>

using namespace Shelter::Utils;

BufferWriter::StepResult BufferWriter::PerformStep(size_t step)
{
    if (pos_ + step > bufferSize_)
    {
        return BufferOutOfBoundsError {};
    }

    pos_ += step;
    return NoError {};
}

template <>
BufferWriter::WriteResult BufferWriter::Write<ByteOrder::BE, uint8_t>(uint8_t value)
{
    auto stepResult = PerformStep(1);

    if (stepResult.is<NoError>())
    {
        buffer_[pos_ - 1] = value;
    }

    return  stepResult;
}

template <>
BufferWriter::WriteResult BufferWriter::Write<ByteOrder::BE, uint32_t>(uint32_t value)
{
    auto stepResult = PerformStep(4);

    if (stepResult.is<NoError>())
    {
        buffer_[pos_ - 1] = static_cast<uint8_t>((value & 0x000000FF) >> 0);
        buffer_[pos_ - 2] = static_cast<uint8_t>((value & 0x0000FF00) >> 8);
        buffer_[pos_ - 3] = static_cast<uint8_t>((value & 0x00FF0000) >> 16);
        buffer_[pos_ - 4] = static_cast<uint8_t>((value & 0xFF000000) >> 24);
    }

    return stepResult;
}

template <>
BufferWriter::WriteResult BufferWriter::Write<ByteOrder::BE, uint64_t>(uint64_t value)
{
    auto stepResult = PerformStep(8);

    if (stepResult.is<NoError>())
    {
        buffer_[pos_ - 1] = static_cast<uint8_t>((value & 0x00000000000000FF) >> 0);
        buffer_[pos_ - 2] = static_cast<uint8_t>((value & 0x000000000000FF00) >> 8);
        buffer_[pos_ - 3] = static_cast<uint8_t>((value & 0x0000000000FF0000) >> 16);
        buffer_[pos_ - 4] = static_cast<uint8_t>((value & 0x00000000FF000000) >> 24);
        buffer_[pos_ - 5] = static_cast<uint8_t>((value & 0x000000FF00000000) >> 32);
        buffer_[pos_ - 6] = static_cast<uint8_t>((value & 0x0000FF0000000000) >> 40);
        buffer_[pos_ - 7] = static_cast<uint8_t>((value & 0x00FF000000000000) >> 48);
        buffer_[pos_ - 8] = static_cast<uint8_t>((value & 0xFF00000000000000) >> 56);
    }

    return stepResult;
}