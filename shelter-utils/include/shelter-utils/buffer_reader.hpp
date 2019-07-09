#pragma once

#include <shelter-utils/byte_order.hpp>

#include <shelter-model/error.hpp>

#include <exl/mixed.hpp>

#include <cstddef>
#include <cstdint>

namespace Shelter { namespace Utils
{
    class BufferReader
    {
    public:
        class NotSupportedByteOrderError : public NotSupportedError {};
        class InvalidBufferError : public InvalidArgumentError {};
        class BufferOutOfBoundsError : public OutOfBoundsError {};

        using ConstructionResult =
            exl::mixed<BufferReader, NotSupportedByteOrderError, InvalidBufferError>;

        friend ConstructionResult;

        template <typename T>
        using ReadResult =
            exl::mixed<T, BufferOutOfBoundsError, NotSupportedByteOrderError>;

    public:
        static ConstructionResult Make(
            const uint8_t* buffer,
            size_t bufferSize,
            ByteOrder defaultByteOrder
        )
        {
            if (defaultByteOrder != ByteOrder::BE)
            {
                return NotSupportedByteOrderError {};
            }

            if (buffer == nullptr)
            {
                return InvalidBufferError {};
            }

            return ConstructionResult::make<BufferReader>(buffer, bufferSize, defaultByteOrder);
        }

        template <typename T>
        ReadResult<T> Read(ByteOrder byteOrder = ByteOrder::Default)
        {
            if (byteOrder == ByteOrder::Default)
            {
                return Read<T>(byteOrder_);
            }

            if (byteOrder_ == ByteOrder::BE)
            {
                return Read<ByteOrder::BE, T>();
            }

            return NotSupportedByteOrderError {};
        }

    private:
        using StepResult = exl::mixed<NoError, BufferOutOfBoundsError>;

    private:
        BufferReader(const uint8_t* buffer, size_t bufferSize, ByteOrder defaultByteOrder)
            : buffer_(buffer)
            , bufferSize_(bufferSize)
            , byteOrder_(defaultByteOrder)
            , pos_(0) {};

        StepResult PerformStep(size_t step);

    private:
        template <ByteOrder B, typename T>
        ReadResult<T> Read();

    private:
        const uint8_t* buffer_;
        size_t bufferSize_;
        ByteOrder byteOrder_;
        size_t pos_;
    };
}}