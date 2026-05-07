#pragma once

#include <stdexcept>
#include <string>
#include <vector>
#include <functional>

struct TestCase {
    const char* name;
    std::function<void()> fn;
};

std::vector<TestCase>& tests();

#define ASSERT_TRUE(expr) do { if (!(expr)) throw std::runtime_error("ASSERT_TRUE failed: " #expr); } while (0)
#define ASSERT_FALSE(expr) do { if ((expr)) throw std::runtime_error("ASSERT_FALSE failed: " #expr); } while (0)
#define ASSERT_EQ(a, b) do { auto _a = (a); auto _b = (b); if (!(_a == _b)) throw std::runtime_error("ASSERT_EQ failed: " #a " != " #b); } while (0)

#define TEST(name) \
    static void name(); \
    namespace { struct Reg_##name { Reg_##name() { tests().push_back({#name, name}); } }; \
    static Reg_##name reg_##name; } \
    static void name()
