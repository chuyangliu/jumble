#ifndef JUMBLE_SHARED_PTR_HPP_INCLUDED
#define JUMBLE_SHARED_PTR_HPP_INCLUDED

#include "jumble/util/common.hpp"
#include <cstddef>
#include <functional>
#include <utility>

JUMBLE_NAMESPACE_BEGIN

template<typename>
class SharedPtr;

template<typename T>
void swap(SharedPtr<T>& lhs, SharedPtr<T>& rhs) noexcept {
    lhs.swap(rhs);
}

template<typename T>
void defaultDeleter(T* p) noexcept {
    delete p;
}

template<typename T, typename... Args>
SharedPtr<T> makeShared(Args&&... args) {
    return SharedPtr<T>(new T(std::forward<Args>(args)...));
}

// Simplified version of std::shared_ptr.
template<typename T>
class SharedPtr {
    friend void swap<T>(SharedPtr<T>& lhs, SharedPtr<T>& rhs) noexcept;

public:
    // Default ctor
    SharedPtr() noexcept : ptr(nullptr), refCnt(nullptr), deleter(defaultDeleter<T>) {}

    // Contruct from raw pointer (optional custom deleter)
    explicit SharedPtr(T* p, const std::function<void(T*) noexcept>& d = defaultDeleter<T>)
    : ptr(p), refCnt(new std::size_t(1)), deleter(d) {}

    // Copy ctor (deleter copy shall not throw)
    SharedPtr(const SharedPtr& other) noexcept
    : ptr(other.ptr), refCnt(other.refCnt), deleter(other.deleter) {
        ++*refCnt;
    }

    // Move ctor (deleter move shall not throw)
    SharedPtr(SharedPtr&& other) noexcept
    : ptr(other.ptr), refCnt(other.refCnt), deleter(std::move(other.deleter)) {
        other.ptr = nullptr;
        other.refCnt = nullptr;
    }

    // Move- and copy-assignment operator (deleter swap shall not throw)
    SharedPtr& operator=(SharedPtr rhs) noexcept {
        swap(rhs);
        return *this;
    }

    // Dtor
    ~SharedPtr() noexcept {
        decreaseAndDestory();
    }

    // Swap members with another SharedPtr (deleter swap shall not throw)
    void swap(SharedPtr& rhs) noexcept {
        using std::swap;
        swap(ptr, rhs.ptr);
        swap(refCnt, rhs.refCnt);
        swap(deleter, rhs.deleter);
    }

    // Safe bool conversion
    explicit operator bool() const noexcept {
        return ptr != nullptr;
    }

    // Dereference operator
    T& operator*() const noexcept {
        return *ptr;
    }

    // Member access operator
    T* operator->() const noexcept {
        return ptr;
    }

    // Return underlying pointer
    T* get() const noexcept {
        return ptr;
    }

    // Whether reference count is one
    bool unique() const noexcept {
        return 1 == useCount();
    }

    // Return reference count
    std::size_t useCount() const noexcept {
        return refCnt ? *refCnt : 0;
    }

    // Reset pointer
    void reset(T* p = nullptr) {
        decreaseAndDestory();
        if (p) {
            ptr = p;
            refCnt = new std::size_t(1);
        }
    }

    // Reset pointer with custom deleter
    void reset(T* p, const std::function<void(T*) noexcept>& d) {
        reset(p);
        deleter = d;
    }

private:
    void decreaseAndDestory() noexcept {
        if (refCnt && --*refCnt == 0) {
            delete refCnt;
            deleter(ptr);
        }
        refCnt = nullptr;
        ptr = nullptr;
    }

private:
    T *ptr;
    std::size_t *refCnt;
    std::function<void(T*) noexcept> deleter;
};

JUMBLE_NAMESPACE_END

#endif
