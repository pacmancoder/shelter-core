#include <shelter-model/error.hpp>

using namespace Shelter;

Error::Error(Error::ErrorPtr nested)
    : nested_(std::move(nested)) {}

void Error::PrintDescription(std::ostream& out)
{
    out << "[";
    DescribeName(out);
    out << "] ";
    DescribeCause(out);
    out << "\n";
}

void Error::PrintErrorStack(std::ostream& out)
{
    PrintDescription(out);
    if (nested_)
    {
        nested_->PrintErrorStack(out);
    }
}

const char* Error::what() const noexcept
{
    return "Shelter runtime error: Process Shelter::Error to acquire additional information";
}

IndexOutOfBoundsError::IndexOutOfBoundsError(size_t index)
    : Error(nullptr)
    , index_(index) {}

void IndexOutOfBoundsError::DescribeCause(std::ostream& out) const
{
    out << "Index " << index_ << " out of bounds";
}

void IndexOutOfBoundsError::DescribeName(std::ostream& out) const
{
    out << "IndexOutOfBoundsError";
}

NotSupportedError::NotSupportedError(const char* what)
    : Error(nullptr)
    , what_(what) {}

void NotSupportedError::DescribeCause(std::ostream& out) const
{
    out << "Current implementation does not support " << what_;
}

void NotSupportedError::DescribeName(std::ostream& out) const
{
    out << "NotSupportedError";
}
