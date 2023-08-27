#include <gmock/gmock.h>

#include <hal/BinaryValue.hxx>
#include <hal/IBinaryValueWriter.hxx>

namespace tests {
namespace mocks {

class BinaryValueWriterMock : public hal::IBinaryValueWriter {
  public:
    MOCK_METHOD(void, writeValue, (hal::BinaryValue), (noexcept));
};

} // namespace mocks
} // namespace tests