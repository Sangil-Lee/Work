#include <memory>

#ifndef s7nodave_Optional_h
#define s7nodave_Optional_h

namespace s7nodave {

/**
 * Class template for a variable that might have a value or not.
 * 
 * This class template provides a replacement for std::optional or
 * boost::optional, so that we can avoid dependencies on C++ 17 or the Boost
 * library.
 * 
 * Please note that it does not provide all the constructors, methods, and
 * operators provided by C++ 17's std::optional. It only provides a baseline of
 * those, which is sufficient for our purposes.
 */
template <typename T>
class Optional {
public:
    /**
     * Creates an empty optional.
     */
    constexpr Optional() noexcept : engaged(false) {
    }

    /**
     * Creates an optional holding the specified value.
     */
    Optional(const T& value) : engaged(true) {
        new (std::addressof(this->value)) T(value);
    }

    /**
     * Creates an optional holding the specified value.
     */
    Optional(T&& value) : engaged(true) {
        new (std::addressof(this->value)) T(value);
    }

    /**
     * Creates an optional that is a copy of the specified optional.
     */
    Optional(const Optional& other) : engaged(other.engaged) {
        if (this->engaged) {
            new (std::addressof(this->value)) T(other.value);
        }
    }

    /**
     * Creates an optional that moves the value from the specified optional.
     */
    Optional(Optional&& other) : engaged(other.engaged) {
        if (this->engaged) {
            new (std::addressof(this->value)) T(std::move(other.value));
            other.engaged = false;
            other.value.~T();
        }
    }

    /**
     * Destructor.
     */
    ~Optional() {
        if (this->engaged) {
            this->value.~T();
        }
    }

    /**
     * Access the optional value.
     * 
     * Results in undefined behavior (access to uninitialized memory) if this
     * optional is not engaged (does not have a value).
     */
    constexpr const T* operator->() const {
        return &this->value;
    }

    /**
     * Access the optional value.
     * 
     * Results in undefined behavior (access to uninitialized memory) if this
     * optional is not engaged (does not have a value).
     */
    T* operator->() {
        return &this->value;
    }

    /**
     * Access the optional value.
     * 
     * Results in undefined behavior (access to uninitialized memory) if this
     * optional is not engaged (does not have a value).
     */
    constexpr const T& operator*() const& {
        return this->value;
    }

    /**
     * Access the optional value.
     * 
     * Results in undefined behavior (access to uninitialized memory) if this
     * optional is not engaged (does not have a value).
     */
    T& operator*() & {
        return this->value;
    }

    /**
     * Access the optional value.
     * 
     * Results in undefined behavior (access to uninitialized memory) if this
     * optional is not engaged (does not have a value).
     */
    constexpr const T&& operator*() const&& {
        return std::move(this->value);
    }

    /**
     * Access the optional value.
     * 
     * Results in undefined behavior (access to uninitialized memory) if this
     * optional is not engaged (does not have a value).
     */
    T&& operator*() && {
        return std::move(this->value);
    }

    /**
     * Explicit conversion to bool.
     * 
     * Tells whether this optional is engaged (has a value).
     */
    constexpr explicit operator bool() const noexcept {
        return this->engaged;
    }

    /**
     * Assignment operator.
     * 
     * Assigns the specified value to this optional.
     */
    Optional& operator=(const T& value) {
        if (this->engaged) {
            this->value = value;
        } else {
            new (std::addressof(this->value)) T(value);
            this->engaged = true;
        }
        return *this;
    }

    /**
     * Assignment operator.
     * 
     * Assigns the specified value to this optional.
     */
    Optional& operator=(T&& value) {
        if (this->engaged) {
            this->value = value;
        } else {
            new (std::addressof(this->value)) T(value);
            this->engaged = true;
        }
        return *this;
    }

    /**
     * Assignment operator.
     * 
     * Assigns the value of the specified optional to this optional.
     */
    Optional& operator=(const Optional<T>& other) {
        if (other.engaged) {
            if (this->engaged) {
                this->value = other.value;
            } else {
                new (std::addressof(this->value)) T(other.value);
                this->engaged = true;
            }
        } else if (this->engaged) {
            this->engaged = false;
            this->value.~T();
        }
        return *this;
    }

    /**
     * Assignment operator.
     * 
     * Assigns the value of the specified optional to this optional.
     */
    Optional& operator=(Optional<T>&& other) {
        if (other.engaged) {
            if (this->engaged) {
                this->value = std::move(other.value);
            } else {
                new (std::addressof(this->value)) T(std::move(other.value));
                this->engaged = true;
                other.engaged = false;
                other.value.~T();
            }
        } else if (this->engaged) {
            this->engaged = false;
            this->value.~T();
        }
        return *this;
    }

    /**
     * Equality operator.
     *
     * Compares the underlying values if both optionals are engaged.
     */
    bool operator==(const Optional<T>& other) const {
        if (this->engaged != other.engaged) {
            return false;
        }
        if (!this->engaged) {
            return true;
        }
        return this->value == other.value;
    }

private:
    bool engaged;

    union {
        char nullValue;
        T value;
    };
};

} // namespace s7nodave

#endif // s7nodave_Optional_h
