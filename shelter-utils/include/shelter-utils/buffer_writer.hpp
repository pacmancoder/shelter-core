#pragma once

#include <shelter-utils/byte_order.hpp>

#include <cstddef>
#include <cstdint>

namespace Shelter { namespace Utils
{
    class BufferWriter
    {
    public:
        BufferWriter(uint8_t* buffer, size_t bufferSize, ByteOrder defaultByteOrder)
            : buffer_(buffer)
            , bufferSize_(bufferSize)
            , byteOrder_(defaultByteOrder)
            , pos_(0)
        {
            if (defaultByteOrder != ByteOrder::BE)
            {
                ThrowLENotSupported();
            }
        };

        template <typename T>
        void Write(T value, ByteOrder byteOrder = ByteOrder::Default)
        {
            if (byteOrder == ByteOrder::Default)
            {
                Write<T>(value, byteOrder_);
                return;
            }

            if (byteOrder == ByteOrder::BE)
            {
                Write<ByteOrder::BE, T>(value);
                return;
            }

            ThrowLENotSupported();
        }

    private:
        [[noreturn]]
        static void ThrowLENotSupported();
        void PerformStep(size_t step);

    private:
        template <ByteOrder B, typename T>
        void Write(T value);

    private:
        uint8_t* buffer_;
        size_t bufferSize_;
        ByteOrder byteOrder_;
        size_t pos_;
    };
}}