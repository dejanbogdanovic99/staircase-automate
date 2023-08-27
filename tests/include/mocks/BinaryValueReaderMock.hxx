#include <gmock/gmock.h>

#include <hal/BinaryValue.hxx>
#include <hal/IBinaryValueReader.hxx>

namespace tests {
namespace mocks {

class BinaryValueReaderMock : public hal::IBinaryValueReader {
  public:
    MOCK_METHOD(hal::BinaryValue, readValue, (), (noexcept));
};

} // namespace mocks
} // namespace tests