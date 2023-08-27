#pragma once

#include <hal/BinaryValue.hxx>

namespace hal {

class IBinaryValueReader {
  public:
    IBinaryValueReader() = default;

    IBinaryValueReader(const IBinaryValueReader &) = delete;
    IBinaryValueReader(IBinaryValueReader &&) = delete;
    IBinaryValueReader &operator=(const IBinaryValueReader &) = delete;
    IBinaryValueReader &operator=(IBinaryValueReader &&) = delete;

    virtual ~IBinaryValueReader() = default;

    virtual BinaryValue readValue() noexcept = 0;
};

} // namespace hal