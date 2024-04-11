#include <gmock/gmock.h>

#include <hal/IPersistence.hxx>

#include <cstdint>
#include <string>

namespace tests {
namespace mocks {

class PersistenceMock : public hal::IPersistence {
  public:
    MOCK_METHOD(bool, keyExists, (const std::string &), (const, noexcept));
    MOCK_METHOD(std::int32_t, getValue, (const std::string &),
                (const, noexcept));
    MOCK_METHOD(void, setValue, (const std::string &, std::int32_t),
                (noexcept));
};

} // namespace mocks
} // namespace tests