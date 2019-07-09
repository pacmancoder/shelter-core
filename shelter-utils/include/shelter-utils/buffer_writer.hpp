#pragma once

#include <shelter-model/error.hpp>
#include <shelter-utils/byte_order.hpp>

#include <exl/mixed.hpp>

#include <cstddef>
#include <cstdint>

namespace Shelter { namespace Utils
{
    class BufferWriterError : public Error {};

    class BufferWriter
    {
    public:

    public:
        class NotSupportedByteOrderError : public NotSupportedError {};
        class InvalidBufferError : public InvalidArgumentError {};
        class BufferOutOfBoundsError : public OutOfBoundsError {};

        using ConstructionResult =
            exl::mixed<BufferWriter, NotSupportedByteOrderError, InvalidBufferError>;

        friend ConstructionResult;

        using WriteResult =
            exl::mixed<NoError, BufferOutOfBoundsError, NotSupportedByteOrderError>;

    public:
        static ConstructionResult Make(
            uint8_t* buffer,
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

            return ConstructionResult::make<BufferWriter>(buffer, bufferSize, defaultByteOrder);
        }

        template <typename T>
        WriteResult Write(T value, ByteOrder byteOrder = ByteOrder::Default)
        {
            if (byteOrder == ByteOrder::Default)
            {
                return Write<T>(value, byteOrder_);
            }

            if (byteOrder == ByteOrder::BE)
            {
                return Write<ByteOrder::BE, T>(value);
            }

            return NotSupportedByteOrderError {};
        }

    private:
        using StepResult = exl::mixed<NoError, BufferOutOfBoundsError>;

    private:
        BufferWriter(uint8_t* buffer, size_t bufferSize, ByteOrder defaultByteOrder)
            : buffer_(buffer)
            , bufferSize_(bufferSize)
            , byteOrder_(defaultByteOrder)
            , pos_(0) {};

    private:
        StepResult PerformStep(size_t step);

    private:
        template <ByteOrder B, typename T>
        WriteResult Write(T value);

    private:
        uint8_t* buffer_;
        size_t bufferSize_;
        ByteOrder byteOrder_;
        size_t pos_;
    };
}}