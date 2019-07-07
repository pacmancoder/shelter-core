#pragma once

#include <shelter-model/error.hpp>
#include <shelter-utils/byte_order.hpp>

#include <cstddef>
#include <cstdint>

namespace Shelter { namespace Utils
{
    class BufferReader
    {
    public:
        BufferReader(const uint8_t* buffer, size_t bufferSize, ByteOrder defaultByteOrder)
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
        T Read(ByteOrder byteOrder = ByteOrder::Default)
        {
            if (byteOrder == ByteOrder::Default)
            {
                return Read<T>(byteOrder_);
            }

            if (byteOrder_ == ByteOrder::BE)
            {
                return Read<ByteOrder::BE, T>();
            }

            ThrowLENotSupported();
        }

    private:
        [[noreturn]]
        static void ThrowLENotSupported();
        void PerformStep(size_t step);

    private:
        template <ByteOrder B, typename T>
        T Read();

    private:
        const uint8_t* buffer_;
        size_t bufferSize_;
        ByteOrder byteOrder_;
        size_t pos_;
    };
}}