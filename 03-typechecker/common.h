#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <cstdint>
#include <vector>
#include <map>
#include <string>

#define ASSERT(expr)         assert((expr))
#define ASSERT_EQ(v0, v1)    assert((v0) == (v1))
#define ASSERT_NULL(ptr)     assert((ptr) == nullptr)
#define ASSERT_NOT_NULL(ptr) assert((ptr) != nullptr)
#define LOG(message)         std::cout << message << std::endl

#endif
