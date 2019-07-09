#pragma once

namespace Shelter
{
    class NoError {};

    class Error {};
    class NotSupportedError : public Error {};
    class InvalidArgumentError : public Error {};
    class OutOfBoundsError : public Error {};
}