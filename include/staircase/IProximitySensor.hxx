#pragma once

#include <hal/Timing.hxx>

namespace staircase {

class IProximitySensor {
  public:
    virtual ~IProximitySensor() = default;
    virtual bool hasStateChanged() const noexcept = 0;
    virtual bool isClose() const noexcept = 0;
    virtual bool isFar() const noexcept = 0;
    virtual void update(hal::Milliseconds delta) noexcept = 0;
};

} // namespace staircase