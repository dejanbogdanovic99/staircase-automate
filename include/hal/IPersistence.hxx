#pragma once

#include <cstdint>
#include <string>

namespace hal {

class IPersistence {
  public:
    virtual ~IPersistence() = default;

    virtual bool keyExists(const std::string &key) const noexcept = 0;
    virtual std::int32_t getValue(const std::string &key) const noexcept = 0;
    virtual void setValue(const std::string &key,
                          std::int32_t value) noexcept = 0;
};

} // namespace hal