#pragma once

#include <hal/BinaryValue.hxx>

namespace hal {

class IBinaryValueWriter {
  public:
    IBinaryValueWriter() = default;

    IBinaryValueWriter(const IBinaryValueWriter &) = delete;
    IBinaryValueWriter(IBinaryValueWriter &&) = delete;
    IBinaryValueWriter &operator=(const IBinaryValueWriter &) = delete;
    IBinaryValueWriter &operator=(IBinaryValueWriter &&) = delete;

    virtual ~IBinaryValueWriter() = default;

    virtual void writeValue(BinaryValue value) noexcept = 0;
};

} // namespace hal