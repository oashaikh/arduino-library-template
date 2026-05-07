// Tiny stub-runner that lets each test file register tests in a global vector.
#include <cstdio>
#include <vector>
#include <string>
#include <functional>

#include "test_runner.h"

std::vector<TestCase>& tests() {
    static std::vector<TestCase> v;
    return v;
}

int main() {
    int failed = 0;
    for (auto& t : tests()) {
        try {
            t.fn();
            std::printf("[OK] %s\n", t.name);
        } catch (const std::exception& e) {
            std::printf("[FAIL] %s: %s\n", t.name, e.what());
            ++failed;
        } catch (...) {
            std::printf("[FAIL] %s: unknown error\n", t.name);
            ++failed;
        }
    }
    std::printf("---\n%zu tests, %d failed\n", tests().size(), failed);
    return failed == 0 ? 0 : 1;
}
