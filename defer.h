#pragma once

#include <utility>

namespace base{
    template<typename Func>
    class DeferClass {
    public:
        explicit DeferClass(Func &&func) : func_(std::move(func)) {}

        ~DeferClass() { func_(); }

    private:
        Func func_;
    };

    template<typename Func>
    inline DeferClass<Func> defer(Func &&func) {
        return DeferClass<Func>(std::forward<Func>(func));
    }
}