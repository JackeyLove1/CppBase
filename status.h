#pragma once

#include <string_view>
#include <string>
#include <cassert>
#include <map>

class Status {
public:
    // Create a success status.
    Status()

    noexcept: state_(nullptr){}

    ~Status() { delete[]state_; }

    // Copy
    Status(const Status &rhs);

    Status &operator=(const Status &rhs);

    // Move
    Status(Status &&rhs) noexcept : state_(std::move(rhs.state_)){}

    Status &operator=(Status &&rhs) noexcept;

    std::string ToString();

    static Status OK() { return Status(); }

    bool ok() const {return state_ == nullptr; }

    // Return error status of an appropriate type.
    static Status NotFound(std::string_view msg, std::string_view msg2 = "") {
        return Status(kNotFound, msg, msg2);
    }
    static Status Corruption(std::string_view msg, std::string_view msg2 = "") {
        return Status(kCorruption, msg, msg2);
    }
    static Status NotSupported(std::string_view msg, std::string_view msg2 = "") {
        return Status(kNotSupported, msg, msg2);
    }
    static Status InvalidArgument(std::string_view msg, std::string_view msg2 = "") {
        return Status(kInvalidArgument, msg, msg2);
    }
    static Status IOError(std::string_view msg, std::string_view msg2 = "") {
        return Status(kIOError, msg, msg2);
    }
private:
    enum Code:uint8_t {
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
    const auto len1 = static_cast<uint32_t>(msg1.size());
    const auto len2 = static_cast<uint32_t>(msg2.size());
    const auto size = len1 + (len2 ? (len2 + 2) : 0);
    char * result = new char [size + 5];
    std::memcpy(result, &size, sizeof (size));
    result[4] = static_cast<char>(code);
    std::memcpy(result + 5, msg1.data(), len1);
    if (len2){
        result[5 + len1] = ':';
        result[6 + len1] = ' ';
        std::memcpy(result + 7 + len1, msg2.data(), len2);
    }
    state_ = result;
}

std::string Status::ToString() {
    if (state_ == nullptr){
        return "OK";
    }
   static std::map<uint8_t , std::string> m = {
            {kOk, "OK: "},
            {kNotFound, "NotFound: "},
            {kCorruption, "Corruption: "},
            {kNotSupported, "NotSupported: "},
            {kInvalidArgument, "InvalidArgument: "},
            {kIOError, "IOError"}
    };
    std::string result;
    const auto key  = static_cast<uint8_t>(code());
    if (m.find(key) != m.end()){
        result = m[key];
    }else {
        result = "Unknown type: ";
    }
    uint32_t length;
    std::memcpy(&length, state_, sizeof (length));
    result.append(state_ + 5, length);
    return result;
}