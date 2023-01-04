#pragma once

#include <string_view>

class Status {
public:
    // Create a success status.
    Status()

    noexcept: state_(nullptr){}

    ~Status() { delete[]state_; }

    // Copy
    Status(const Status &rhs)

    Status &operator=(const Status &rhs);

    // Move
    Status(Status &&rhs) noexcept : state_(std::move(rhs.state_)){}

    Status &operator=(Status &&rhs) noexcept;

    std::string ToString();

    static Status OK() { return Status(); }

private:
    enum Code {
        kOk = 0,
        kNotFound = 1,
        kCorruption = 2,
        kNotSupported = 3,
        kInvalidArgument = 4,
        kIOError = 5
    };

    Code code() const {
        return (state_ == nullptr) ? kOk : static_cast<Code>(state_[4]);
    }

    Status(Code code, std::string_view msg1, std::string_view msg2);

    static const char *CopyState(const char *s);

    // OK status has a null state_.  Otherwise, state_ is a new[] array
    // of the following form:
    //    state_[0..3] == length of message
    //    state_[4]    == code
    //    state_[5..]  == message
    const char *state_;
};

const char * Status::CopyState(const char *state) {
    uint32_t size;
    std::memcpy(&size, state, sizeof (size));
    // 5: state_[0..3] == length of message state_[4] == code
    char * result = new char [size + 5];
    std::memcpy(result, state, size + 5);
    return result;
}

inline Status::Status(const Status &rhs) {
    state_ = (rhs.state_ == nullptr) ? nullptr : CopyState(rhs.state_);
}

inline Status& Status::operator=(const Status& rhs){
    if (state_ != rhs.state_){
        delete [] state_;
        state_ = (rhs.state_ == nullptr) ? nullptr : CopyState(rhs.state_);
    }
    return *this;
}

inline Status& Status::operator=(Status &&rhs) noexcept {
    std::swap(state_, rhs.state_);
    return *this;
}

Status::Status(Code code, std::string_view msg1, std::string_view msg2) {
    assert(code != kOk);
}