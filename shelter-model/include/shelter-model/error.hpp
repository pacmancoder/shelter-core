#pragma once

#include <memory>
#include <ostream>
#include <exception>

namespace Shelter
{
    class Error : public std::exception
    {
    public:
        using ErrorPtr = std::unique_ptr<Error>;

    public:
        explicit Error(ErrorPtr nested = nullptr);

        void PrintDescription(std::ostream& out);
        void PrintErrorStack(std::ostream& out);

    private:
        virtual void DescribeCause(std::ostream& out) const = 0;
        virtual void DescribeName(std::ostream& out) const = 0;

    private:
        const char* what() const noexcept override;

    private:
        ErrorPtr nested_;
    };

    class IndexOutOfBoundsError : public Error
    {
    public:
        explicit IndexOutOfBoundsError(size_t index);

    private:
        void DescribeCause(std::ostream& out) const override;
        void DescribeName(std::ostream& out) const override;

    private:
        size_t index_;
    };

    class NotSupportedError : public Error
    {
    public:
        explicit NotSupportedError(const char* what);

    private:
        void DescribeCause(std::ostream& out) const override;
        void DescribeName(std::ostream& out) const override;

    private:
        const char* what_;
    };

}