#pragma once

#include <staircase/IMoving.hxx>
#include <staircase/IMovingFactory.hxx>
#include <staircase/Moving.hxx>

#include <algorithm>
#include <array>
#include <cstdint>

namespace staircase {

template <std::size_t N>
class StaticMovingFactory final : public IMovingFactory {
  public:
    StaticMovingFactory() {
        std::fill(std::begin(mOccupied), std::end(mOccupied), false);
    }
    StaticMovingFactory(const StaticMovingFactory &) noexcept = delete;
    StaticMovingFactory(StaticMovingFactory &&) noexcept = default;
    StaticMovingFactory &
    operator=(const StaticMovingFactory &) noexcept = delete;
    StaticMovingFactory &operator=(StaticMovingFactory &&) noexcept = default;

    ~StaticMovingFactory() = default;

    IMoving *create(BasicLights& lights, IMoving::Direction direction,
                    hal::Milliseconds duration) noexcept final {
        std::size_t index = 0;
        while (mOccupied[index] && (index < N)) {
            ++index;
        }

        if (index == N) {
            return nullptr;
        }

        mOccupied[index] = true;
        char *addr = &mData[index * kElementSize];
        Moving *moving = new (addr) Moving(lights, direction, duration);

        return moving;
    }
    void destroy(IMoving *moving) noexcept final {
        std::size_t index = 0;
        while ((&mData[index * kElementSize] !=
                reinterpret_cast<char *>(moving)) &&
               (index < N)) {
            ++index;
        }

        if (index == N) {
            return;
        }

        mOccupied[index] = false;
        Moving *ourMoving = reinterpret_cast<Moving *>(moving);
        ourMoving->~Moving();
    }

  private:
    constexpr static std::size_t elementSize() {
        if constexpr ((sizeof(Moving) % alignof(Moving)) == 0) {
            return sizeof(Moving);
        } else {
            return sizeof(Moving) +
                   (alignof(Moving) - (sizeof(Moving) % alignof(Moving)));
        }
    }

    constexpr static std::size_t kElementSize = elementSize();

    std::array<char, kElementSize * N> mData;
    std::array<bool, N> mOccupied;
};

} // namespace staircase