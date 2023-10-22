#pragma once

#include <array>
#include <cstdint>
#include <memory>

namespace util {

template <class T, std::size_t N> class StaticDequeue {
  public:
    using value_type = T;
    using pointer = T *;
    using reference = T &;

    class Iterator {
      public:
        using value_type = StaticDequeue::value_type;
        using pointer = StaticDequeue::pointer;
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::size_t;
        using reference = StaticDequeue::reference;
        Iterator &operator++() noexcept {
            if (mCounter == mDequeue.size()) {
                return *this;
            }

            ++mPosition;
            ++mCounter;
            if (mPosition == mDequeue.size()) {
                mPosition = 0;
            }

            return *this;
        }

        const Iterator &operator++() const noexcept {
            if (mCounter == mDequeue.size()) {
                return *this;
            }

            ++mPosition;
            ++mCounter;
            if (mPosition == mDequeue.size()) {
                mPosition = 0;
            }

            return *this;
        }

        T *operator->() noexcept {
            return reinterpret_cast<T *>(
                &mDequeue.mArray[mPosition * kElementSize]);
        }

        const T *operator->() const noexcept {
            return reinterpret_cast<T *>(
                &mDequeue.mArray[mPosition * kElementSize]);
        }

        T &operator*() noexcept {
            return *(reinterpret_cast<T *>(
                &mDequeue.mArray[mPosition * kElementSize]));
        }

        const T &operator*() const noexcept {
            return (reinterpret_cast<T *>(
                &mDequeue.mArray[mPosition * kElementSize]));
        }

        bool operator==(const Iterator &other) const noexcept {
            return other.mCounter == mCounter;
        }

      private:
        Iterator(StaticDequeue &dequeue, std::size_t position,
                 std::size_t counter) noexcept
            : mDequeue{dequeue}, mPosition{position}, mCounter{counter} {}

        StaticDequeue &mDequeue;
        mutable std::size_t mPosition;
        mutable std::size_t mCounter;

        friend class StaticDequeue;
    };

    StaticDequeue() noexcept = default;
    StaticDequeue(const StaticDequeue &) noexcept = default;
    StaticDequeue(StaticDequeue &&) noexcept = default;
    StaticDequeue &operator=(const StaticDequeue &) noexcept = default;
    StaticDequeue &operator=(StaticDequeue &&) noexcept = default;

    Iterator begin() noexcept { return Iterator{*this, mFirst, 0}; }

    Iterator end() noexcept { return Iterator{*this, mFirst, mSize}; }

    const Iterator begin() const noexcept { return Iterator{*this, mFirst, 0}; }

    const Iterator end() const noexcept {
        return Iterator{*this, mFirst, mSize};
    }

    std::size_t size() const noexcept { return mSize; }

    bool empty() const noexcept { return mSize == 0; }

    bool full() const noexcept { return mSize == N; }

    T &pushBack(T &&value) noexcept {
        if (full()) {
            return back();
        }

        std::size_t next = mFirst + mSize;

        if (next >= N) {
            next -= N;
        }

        T *element = new (&mArray[next * kElementSize]) T{std::move(value)};
        ++mSize;

        return *element;
    }

    T &pushBack(const T &value) noexcept {
        if (full()) {
            return back();
        }

        std::size_t next = mFirst + mSize;

        if (next >= N) {
            next -= N;
        }

        T *element = new (&mArray[next * kElementSize]) T{value};
        ++mSize;

        return *element;
    }

    T &back() noexcept {
        if (empty()) {
            return *reinterpret_cast<T *>(&mArray[mFirst * kElementSize]);
        }

        std::size_t last = mFirst + mSize - 1;

        if (last >= N) {
            last -= N;
        }

        return *reinterpret_cast<T *>(&mArray[last * kElementSize]);
    }

    void popFront() noexcept {
        if (empty()) {
            return;
        }

        T &element = *reinterpret_cast<T *>(&mArray[mFirst * kElementSize]);
        element.~T();

        ++mFirst;

        if (mFirst == N) {
            mFirst = 0;
        }

        --mSize;
    }

    T &front() noexcept {
        return *reinterpret_cast<T *>(&mArray[mFirst * kElementSize]);
    }

  private:
    constexpr static std::size_t elementSize() {
        if constexpr ((sizeof(T) % alignof(T)) == 0) {
            return sizeof(T);
        } else {
            return sizeof(T) + (alignof(T) - (sizeof(T) % alignof(T)));
        }
    }

    constexpr static std::size_t kElementSize = elementSize();

    std::array<char, kElementSize * N> mArray;
    std::size_t mSize{0};
    std::size_t mFirst{0};
};

} // namespace util