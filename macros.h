#ifndef CPPBASE_MACROS_H
#define CPPBASE_MACROS_H

#pragma once
#include <type_traits>
#include <functional>
#include <memory>
#include <utility>

#define LIKELY(x)      __builtin_expect(!!(x), 1)
#define UNLIKELY(x)    __builtin_expect(!!(x), 0)

#endif //CPPBASE_MACROS_H