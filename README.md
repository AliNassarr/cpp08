*this project was done by alnassar..*

# C++ - Module 08: Templated Containers, Iterators and Algorithms

## Description
**C++ Module 08** introduces the standard template library (**STL**), specifically focusing on **Containers**, **Iterators**, and **Algorithms** in C++98.

In Modules 00 through 07, standard containers (`<vector>`, `<list>`, `<map>`, etc.) and standard algorithms (`<algorithm>`) were strictly forbidden. In Module 08, they become the central subject of study. The exercises demonstrate:
1. Writing algorithms that operate polymorphically across different STL sequence containers via iterators (`ex00`).
2. Designing custom container-like abstractions that efficiently compute metrics using STL algorithms (`ex01`).
3. Extending existing STL container adapters (`std::stack`) to expose underlying iterator capabilities without breaking encapsulation or standard interface expectations (`ex02`).

---

## 📑 Summary of Exercises

| Exercise | Primary Topic | Key Deliverables | Evaluation Focus |
| :--- | :--- | :--- | :--- |
| **[ex00: Easy find](ex00/)** | Function Templates & STL Algorithms | `easyfind.hpp`, `main.cpp`, `Makefile` | Searching arbitrary integer containers via iterators with `std::find`; throwing exception when value is absent; const-correctness. |
| **[ex01: Span](ex01/)** | Container Encapsulation & Range Operations | `Span.hpp`, `Span.cpp`, `main.cpp`, `Makefile` | Managing fixed capacity; `addNumber`; bulk addition via iterator ranges; $O(N \log N)$ `shortestSpan` and $O(N)$ `longestSpan`; verified with $\ge 10{,}000$ numbers. |
| **[ex02: Mutated stack](ex02/)** | Container Adapters & Custom Iterators | `MutantStack.hpp`, `main.cpp`, `Makefile` | Inheriting `std::stack`; exposing underlying container `c` iterators (`iterator`, `const_iterator`, reverse variants); output matching `std::list`. |

---

## 🛠️ Exercises Overview

### [Exercise 00: Easy find](ex00/)
- **Header**: `easyfind.hpp` (implemented completely in header as a template).
- **Function Template**:
  ```cpp
  template <typename T>
  typename T::iterator easyfind(T& container, int value);

  template <typename T>
  typename T::const_iterator easyfind(const T& container, int value);
  ```
- **Behavior**:
  - Accepts any sequence container of integers `T` (e.g., `std::vector<int>`, `std::list<int>`, `std::deque<int>`).
  - Calls `std::find(container.begin(), container.end(), value)`.
  - If the element is found, returns an iterator to its first occurrence.
  - If not found, throws `std::runtime_error("Value not found in container")`.
  - Const overload ensures read-only containers can also be searched safely.

### [Exercise 01: Span](ex01/)
- **Files**: `Span.hpp`, `Span.cpp`
- **Class**: `Span`
- **Behavior & Member Functions**:
  - `Span(unsigned int n)`: Allocates storage with a maximum capacity of `n` integers.
  - `void addNumber(int number)`: Adds a single integer. Throws `std::length_error` if full.
  - `template <typename InputIterator> void addRange(InputIterator first, InputIterator last)`: Adds a range of elements via iterators in a single operation. Throws `std::length_error` if capacity would be exceeded. Also overloaded as `addNumber(first, last)` for evaluator compatibility.
  - `int shortestSpan() const`: Sorts a copy of elements and finds the minimum difference between adjacent numbers ($O(N \log N)$). Throws `std::logic_error` if fewer than 2 elements exist.
  - `int longestSpan() const`: Finds difference between `*std::max_element` and `*std::min_element` ($O(N)$). Throws `std::logic_error` if fewer than 2 elements exist.
  - Orthodox Canonical Form fully implemented (default constructor, copy constructor, assignment operator, destructor).

### [Exercise 02: Mutated stack](ex02/)
- **Header**: `MutantStack.hpp`
- **Class Template**: `MutantStack<T, Container = std::deque<T> >`
- **Behavior**:
  - Inherits from `std::stack<T, Container>`.
  - `std::stack` is a **container adapter** that does not provide iterators by design. However, its underlying container member `c` is `protected`.
  - `MutantStack` exposes iterator typedefs and methods delegating directly to `this->c`:
    - `iterator begin()`, `iterator end()`
    - `const_iterator begin() const`, `const_iterator end() const`
    - `reverse_iterator rbegin()`, `reverse_iterator rend()`
    - `const_reverse_iterator rbegin() const`, `const_reverse_iterator rend() const`
  - Allows iterating from bottom-to-top (`begin` to `end`) or top-to-bottom (`rbegin` to `rend`).
  - Can be copied directly to a standard `std::stack<T>`.

---

## 📋 Evaluation Sheet Checklist

- [x] **Prerequisites**:
  - Compiles cleanly with `c++ -Wall -Wextra -Werror -std=c++98`.
  - Strict C++98 standard; no C++11 auto, lambdas, range-for, or nullptr.
  - Standard Orthodox Canonical Form where applicable.
  - Zero memory leaks.

- [x] **Exercise 00**:
  - Files named `Makefile`, `main.cpp`, `easyfind.hpp`.
  - `easyfind` is a template taking container `T` and `int`.
  - Correct use of `typename T::iterator` to resolve dependent types.
  - Returns iterator when element is found.
  - Throws exception when element is not found.
  - Tested with `std::vector<int>` and `std::list<int>`.

- [x] **Exercise 01**:
  - Files named `Makefile`, `main.cpp`, `Span.hpp`, `Span.cpp`.
  - Class `Span` storing up to $N$ elements.
  - `shortestSpan` and `longestSpan` calculate spans correctly.
  - Throws exception when adding beyond capacity.
  - Throws exception when computing spans on 0 or 1 element.
  - Bulk addition with iterators implemented.
  - Tested with $\ge 10{,}000$ numbers (tested with 15,000 numbers).

- [x] **Exercise 02**:
  - Files named `Makefile`, `main.cpp`, `MutantStack.hpp`.
  - `MutantStack` inherits from `std::stack`.
  - Iterators (`begin()`, `end()`, etc.) are exposed and functional.
  - Subject `main.cpp` code executes and produces exact output.
  - Comparison with `std::list` produces identical sequence.

---

## 🚀 Compilation & Running

Each exercise contains its own independent Makefile:

```bash
# Exercise 00: easyfind
cd ex00 && make
./easyfind

# Exercise 01: span
cd ../ex01 && make
./span

# Exercise 02: mutantstack
cd ../ex02 && make
./mutantstack
```
