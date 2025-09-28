#ifndef INCLUDED_PYLRU_RESULT_H_
#define INCLUDED_PYLRU_RESULT_H_

#include <stdexcept>

namespace pylru {

enum class Status { FOUND, NOT_FOUND };

template <typename T> class Result {
  private:
    Status status_;
    T value_;

  public:
    // Constructor for found result
    Result(T value) : status_(Status::FOUND), value_(value) {}

    // Constructor for not found result
    static Result not_found() { return Result(Status::NOT_FOUND, T{}); }

    // Status checking
    bool is_found() const { return status_ == Status::FOUND; }
    bool is_not_found() const { return status_ == Status::NOT_FOUND; }
    Status status() const { return status_; }

    // Value access (throws if not found)
    T value() const {
        if (status_ == Status::NOT_FOUND) {
            throw std::runtime_error(
                "Attempted to access value of NOT_FOUND result");
        }
        return value_;
    }

    // Safe value access with default
    T value_or(T default_value) const {
        return (status_ == Status::FOUND) ? value_ : default_value;
    }

    // Conversion to bool for easy checking
    explicit operator bool() const { return is_found(); }

  private:
    // Private constructor for not found case
    Result(Status status, T value) : status_(status), value_(value) {}
};

} // namespace pylru

#endif
