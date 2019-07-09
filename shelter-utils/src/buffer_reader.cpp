#include <shelter-utils/buffer_reader.hpp>

#include <shelter-model/error.hpp>

using namespace Shelter::Utils;

BufferReader::StepResult BufferReader::PerformStep(size_t step)
{
    if (pos_ + step > bufferSize_)
    {
        return BufferOutOfBoundsError {};
    }

    pos_ += step;
    return NoError {};
}

template <>
BufferReader::ReadResult<uint8_t> BufferReader::Read<ByteOrder::BE, uint8_t>()
{
    return PerformStep(1).map < ReadResult < uint8_t >> (
        exl::when_exact<NoError>(
            [this](const NoError&)
            {
                return buffer_[pos_ - 1];
            }
        ),
        exl::when_exact<BufferOutOfBoundsError>(
            [](const BufferOutOfBoundsError& e)
            {
                return e;
            }
        )
    );
}

template <>
BufferReader::ReadResult<uint32_t> BufferReader::Read<ByteOrder::BE, uint32_t>()
{
    return PerformStep(4).map < ReadResult < uint32_t >> (
        exl::when_exact<NoError>(
            [this](const NoError&)
            {
                return static_cast<uint32_t>(buffer_[pos_ - 1]) << 0
                    | static_cast<uint32_t>(buffer_[pos_ - 2]) << 8
                    | static_cast<uint32_t>(buffer_[pos_ - 3]) << 16
                    | static_cast<uint32_t>(buffer_[pos_ - 4]) << 24;
            }
        ),
        exl::when_exact<BufferOutOfBoundsError>(
            [](const BufferOutOfBoundsError& e)
            {
                return e;
            }
        )
    );
}

template <>
BufferReader::ReadResult<uint64_t> BufferReader::Read<ByteOrder::BE, uint64_t>()
{
    return PerformStep(8).map < ReadResult < uint64_t >> (
        exl::when_exact<NoError>(
            [this](const NoError&)
            {
                return static_cast<uint64_t>(buffer_[pos_ - 1]) << 0
                    | static_cast<uint64_t>(buffer_[pos_ - 2]) << 8
                    | static_cast<uint64_t>(buffer_[pos_ - 3]) << 16
                    | static_cast<uint64_t>(buffer_[pos_ - 4]) << 24
                    | static_cast<uint64_t>(buffer_[pos_ - 5]) << 32
                    | static_cast<uint64_t>(buffer_[pos_ - 6]) << 40
                    | static_cast<uint64_t>(buffer_[pos_ - 7]) << 48
                    | static_cast<uint64_t>(buffer_[pos_ - 8]) << 56;
            }
        ),
        exl::when_exact<BufferOutOfBoundsError>(
            [](const BufferOutOfBoundsError& e)
            {
                return e;
            }
        )
    );
}