#include "status.h"
#include <cassert>

int main() {
    {
        Status ok = Status::OK();
        auto ok2 = std::move(ok);
        assert(ok2.ok());
    }
    return 0;
}