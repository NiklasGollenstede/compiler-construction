#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <cstdint>
#include <vector>
#include <map>
#include <string>
#include <cassert>
#include <exception>

#define ASSERT(expr)         assert((expr))
#define ASSERT_EQ(v0, v1)    assert((v0) == (v1))
#define ASSERT_NULL(ptr)     assert((ptr) == nullptr)
#define ASSERT_NOT_NULL(ptr) assert((ptr) != nullptr)
#define EXCEPT(message)      throw std::runtime_error((message))
#define PRINT(message)       std::cout << message << std::endl;

// Create an instance of enum E on the heap.
template<typename E>
E* enum_new(E enumval) {
	return new E(enumval);
}

// Throw an exception containing the given node's line position and description.
template<typename T>
void error(T* node, std::string const& description) {
  EXCEPT("Error in line " + std::to_string(node->line_number) + ": " + description);
}

// Error w/o line information.
inline void error(std::string const& description) { EXCEPT("Error: " + description); }

// Throw a critical error w/ default message.
inline void crash() { EXCEPT("A critical error occured."); }

template<typename T>
void crash(T* node) {
  EXCEPT("A critical error occured in line " + std::to_string(node->line_number) + ".");
}

#endif
