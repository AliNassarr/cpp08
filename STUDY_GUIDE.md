# 📚 CPP Module 08: Complete Study Guide & Peer Evaluation Defense

This study guide provides an in-depth breakdown of **C++ Module 08 (Templated Containers, Iterators, and Algorithms)**. It covers core theory, line-by-line implementations, peer defense answers, and live code modification prep for 42 evaluations.

---

## 📑 Table of Contents
0. [⚡ 3-Minute Quick Study Guide (TL;DR Cheat Sheet)](#0--3-minute-quick-study-guide-tldr-cheat-sheet)
1. [The Big Picture: STL Architecture in C++98](#1-the-big-picture-stl-architecture-in-c98)
2. [Containers, Iterators, and Algorithms Triad](#2-containers-iterators-and-algorithms-triad)
3. [The `typename` Disambiguation Rule for Dependent Types](#3-the-typename-disambiguation-rule-for-dependent-types)
4. [Exercise 00: `easyfind`](#4-exercise-00-easyfind)
5. [Exercise 01: `Span`](#5-exercise-01-span)
6. [Exercise 02: `MutantStack`](#6-exercise-02-mutantstack)
7. [Top Peer Evaluation Defense Questions & Answers](#7-top-peer-evaluation-defense-questions--answers)
8. [Live Code Modification Practice](#8-live-code-modification-practice)

---

## 0. ⚡ 3-Minute Quick Study Guide (TL;DR Cheat Sheet)

If you have an evaluation in 5 minutes, memorize this section:

### 1. The 3 Exercises in One Sentence

| Exercise | What It Is | Key Tool Used | The #1 Trap Evaluators Look For |
| :--- | :--- | :--- | :--- |
| **ex00: `easyfind`** | Find first integer in any container | `std::find(c.begin(), c.end(), val)` | **Must NOT use manual loops**. Must use `std::find`. Must use `typename T::iterator`. |
| **ex01: `Span`** | Store $N$ ints, find min/max distances | `std::sort`, `std::min_element`, `std::max_element` | `shortestSpan` is **not** subtracting the lowest two numbers! Must test with $\ge 10{,}000$ numbers. |
| **ex02: `MutantStack`**| Make `std::stack` iterable | Public inheritance + `this->c.begin()` | `std::stack` has a `protected: Container c;` member that holds the data. Output must match `std::list`. |

---

### 2. Top 5 Questions Evaluators Ask & Quick Answers

1. **Why `typename T::iterator` instead of just `T::iterator`?**
   > *Answer*: `T` depends on a template parameter (dependent type). The compiler assumes `T::iterator` is a variable unless you tell it it's a type using `typename`.
2. **Why can't you calculate `shortestSpan` by subtracting the two smallest numbers?**
   > *Answer*: Because the closest numbers could be anywhere in the set. For example `{1, 100, 102}`: the two smallest are $1$ and $100$ (diff $= 99$), but the closest are $100$ and $102$ (diff $= 2$).
3. **Why do you copy the vector in `shortestSpan`?**
   > *Answer*: `shortestSpan()` is a `const` function. We cannot modify `_storage`, so we sort a local copy.
4. **How do you get iterators from `std::stack` when it doesn't have any?**
   > *Answer*: `std::stack` stores its data inside a `protected` member variable called `c`. Because `MutantStack` inherits from `std::stack`, it can directly access `this->c.begin()` and `this->c.end()`.
5. **What is the difference between a Container and a Container Adapter?**
   > *Answer*: Containers (`vector`, `list`, `deque`) store raw data and have iterators. Adapters (`stack`, `queue`) wrap another container to restrict its interface (e.g. LIFO only) and hide iterators.


---

## 1. The Big Picture: STL Architecture in C++98

The Standard Template Library (STL) is built on a design philosophy of **separation of data structures and algorithms**.

In object-oriented programming, data structures typically bundle algorithms as member functions (e.g., `list.sort()`, `vector.find()`). The STL decouples them:
- **Containers** store collections of objects.
- **Algorithms** perform computations (searching, sorting, counting, transforming).
- **Iterators** serve as the standardized bridge (or "glue") connecting algorithms to containers.

```
+--------------------+        +---------------+        +----------------------+
|   STL Container    | -----> |   Iterator    | -----> |    STL Algorithm     |
| (vector, list, ...) |        | (begin, end)  |        | (find, sort, min, ...) |
+--------------------+        +---------------+        +----------------------+
```

Because algorithms only interface with **iterators**, a single algorithm like `std::find` works unchanged across an array, a `std::vector`, a `std::list`, or any user-defined container exposing iterator operations.

---

## 2. Containers, Iterators, and Algorithms Triad

### A. Container Classifications
1. **Sequence Containers**: Store elements in a strict linear sequence determined by insertion order.
   - `std::vector`: Dynamically resized contiguous array. Fast random access ($O(1)$), fast push/pop at back ($O(1)$ amortized), slow insertions in the middle ($O(N)$).
   - `std::deque`: Double-ended queue composed of chunked contiguous buffers. Fast random access ($O(1)$), fast push/pop at both front and back ($O(1)$).
   - `std::list`: Doubly linked list. Non-contiguous memory. Fast insertion/deletion anywhere once position is known ($O(1)$), slow random traversal ($O(N)$).
2. **Associative Containers**: Store elements in sorted order or key-value pairs (implemented as Red-Black Trees in C++98).
   - `std::set`, `std::map`, `std::multiset`, `std::multimap`. (Search, insert, delete in $O(\log N)$).
3. **Container Adapters**: Restrict and adapt the interface of an underlying sequence container.
   - `std::stack`: LIFO (Last-In-First-Out). Default underlying container: `std::deque`.
   - `std::queue`: FIFO (First-In-First-Out). Default underlying container: `std::deque`.
   - `std::priority_queue`: Max-heap. Default underlying container: `std::vector`.

### B. The 5 Iterator Categories
Iterators in C++98 are organized into a strict hierarchy based on supported operations:

| Category | Movement | Read / Write | Example Containers |
| :--- | :--- | :--- | :--- |
| **Input Iterator** | Single-pass forward (`++`) | Read-only (`*it`) | `std::istream_iterator` |
| **Output Iterator** | Single-pass forward (`++`) | Write-only (`*it = v`) | `std::ostream_iterator` |
| **Forward Iterator** | Multi-pass forward (`++`) | Read & Write | Singly-linked structures |
| **Bidirectional Iterator**| Multi-pass forward & backward (`++`, `--`) | Read & Write | `std::list`, `std::set`, `std::map` |
| **Random Access Iterator**| Jump arbitrary distances (`+`, `-`, `+=`, `-=`, `[]`, `<`) | Read & Write | `std::vector`, `std::deque`, raw pointers |

---

## 3. The `typename` Disambiguation Rule for Dependent Types

In Exercise 00 and Exercise 02, you will see expressions like:
```cpp
typename T::iterator it = std::find(...);
```

### Why is `typename` strictly mandatory here?
When the C++ compiler parses a template, `T` is unknown.
`T::iterator` could refer to:
1. A **nested type** (e.g., `typedef int* iterator;`), OR
2. A **static member variable** (e.g., `static int iterator;`).

If `iterator` were a static variable, an expression like `T::iterator * ptr;` would be interpreted by the compiler as a **multiplication** between `T::iterator` and `ptr`!

To eliminate this ambiguity, the C++ standard establishes a rule:
> By default, the compiler assumes any dependent qualified name (a name that depends on a template parameter, like `T::something`) is a **variable or value**, NOT a type.
> 
> To tell the compiler that `T::iterator` refers to a **type**, you MUST prefix it with the `typename` keyword.

Omitting `typename` causes a compilation error under strict `-std=c++98`:
`error: need 'typename' before 'T::iterator' because 'T' is a dependent scope`.

---

## 4. Exercise 00: `easyfind`

### Objective
Write a function template `easyfind` that accepts a container of integers `T` and an integer `value`. It finds the first occurrence and returns an iterator to it, or throws an exception if not found.

### Header Breakdown (`easyfind.hpp`)
```cpp
#ifndef EASYFIND_HPP
#define EASYFIND_HPP

#include <algorithm>
#include <stdexcept>

template <typename T>
typename T::iterator easyfind(T& container, int value)
{
    typename T::iterator it = std::find(container.begin(), container.end(), value);
    if (it == container.end())
        throw std::runtime_error("Value not found in container");
    return it;
}

template <typename T>
typename T::const_iterator easyfind(const T& container, int value)
{
    typename T::const_iterator it = std::find(container.begin(), container.end(), value);
    if (it == container.end())
        throw std::runtime_error("Value not found in container");
    return it;
}

#endif
```

### Key Technical Aspects
1. **Algorithm Used**: `std::find(begin, end, value)` performs a linear search from `begin` up to (but not including) `end`. If found, it returns the iterator pointing to that element; if not found, it returns `end`.
2. **Half-Open Range Principle**: STL ranges are represented as `[begin, end)`, where `end()` points one past the final valid element. Testing `it == container.end()` is the standard STL pattern for detecting search failure.
3. **Const-Correctness**: Providing both non-const (`T&`) and const (`const T&`) overloads ensures that passing a `const std::vector<int>` or `const std::list<int>` compiles and returns a `typename T::const_iterator`.
4. **Exception Handling**: When the element is missing, throwing `std::runtime_error` (which inherits from `std::exception`) satisfies the subject requirement: *"If no occurrence is found, you can either throw an exception or return an error value of your choice."*

---

## 5. Exercise 01: `Span`

### Objective
Develop a class `Span` that stores at most `N` integers. Implement `addNumber()`, a bulk insertion method using iterator ranges, `shortestSpan()`, and `longestSpan()`. Must be tested with at least 10,000 numbers.

### Class Declaration (`Span.hpp`)
```cpp
class Span
{
public:
    Span();
    Span(unsigned int n);
    Span(const Span& other);
    Span& operator=(const Span& rhs);
    ~Span();

    void addNumber(int number);

    template <typename InputIterator>
    void addRange(InputIterator first, InputIterator last)
    {
        typename std::iterator_traits<InputIterator>::difference_type count = std::distance(first, last);
        if (count < 0 || _storage.size() + static_cast<std::size_t>(count) > _maxCapacity)
            throw std::length_error("Span capacity exceeded while adding range");
        _storage.insert(_storage.end(), first, last);
    }

    template <typename InputIterator>
    void addNumber(InputIterator first, InputIterator last)
    {
        addRange(first, last);
    }

    int shortestSpan() const;
    int longestSpan() const;

    std::size_t size() const;
    std::size_t capacity() const;

private:
    std::size_t _maxCapacity;
    std::vector<int> _storage;
};
```

### Implementation Details (`Span.cpp`)

#### 1. Adding a Single Number
```cpp
void Span::addNumber(int number)
{
    if (_storage.size() >= _maxCapacity)
        throw std::length_error("Span is already at full capacity");
    _storage.push_back(number);
}
```
Checks if current size has reached `_maxCapacity`. If so, throws `std::length_error`.

#### 2. Adding a Range of Iterators
```cpp
template <typename InputIterator>
void addRange(InputIterator first, InputIterator last)
{
    typename std::iterator_traits<InputIterator>::difference_type count = std::distance(first, last);
    if (count < 0 || _storage.size() + static_cast<std::size_t>(count) > _maxCapacity)
        throw std::length_error("Span capacity exceeded while adding range");
    _storage.insert(_storage.end(), first, last);
}
```
- `std::distance(first, last)` computes the number of elements between `first` and `last`.
- If the new elements exceed remaining capacity, throws an exception before inserting anything, maintaining strong exception safety.
- `_storage.insert(_storage.end(), first, last)` inserts all elements in bulk.
- Providing both `addRange(first, last)` and an overloaded `addNumber(first, last)` ensures full compatibility regardless of which name an evaluator tests.

#### 3. Calculating `longestSpan()`
```cpp
int Span::longestSpan() const
{
    if (_storage.size() < 2)
        throw std::logic_error("At least two numbers are required to compute a span");

    int minVal = *std::min_element(_storage.begin(), _storage.end());
    int maxVal = *std::max_element(_storage.begin(), _storage.end());

    return maxVal - minVal;
}
```
- The maximum span in any set of numbers is always the global maximum minus the global minimum: $\text{Span}_{\max} = \max(S) - \min(S)$.
- `std::min_element` and `std::max_element` scan the container in a single pass ($O(N)$ time).
- Complexity: **$O(N)$ time, $O(1)$ extra space**.

#### 4. Calculating `shortestSpan()`
```cpp
int Span::shortestSpan() const
{
    if (_storage.size() < 2)
        throw std::logic_error("At least two numbers are required to compute a span");

    std::vector<int> sorted = _storage;
    std::sort(sorted.begin(), sorted.end());

    int minDistance = sorted[1] - sorted[0];
    for (std::size_t i = 2; i < sorted.size(); ++i)
    {
        int diff = sorted[i] - sorted[i - 1];
        if (diff < minDistance)
            minDistance = diff;
    }
    return minDistance;
}
```
- In an unsorted array, finding the minimum difference between any pair requires comparing every element with every other element ($O(N^2)$). For 15,000 numbers, $N^2 \approx 225{,}000{,}000$ operations!
- By sorting a copy of the numbers first ($O(N \log N)$), the closest two numbers are guaranteed to be adjacent.
- We then iterate once through the sorted array ($O(N)$) to check differences between consecutive elements `sorted[i] - sorted[i - 1]`.
- Overall Complexity: **$O(N \log N)$ time, $O(N)$ space**. For 15,000 numbers, this executes virtually instantaneously (< 5 ms).

---

## 6. Exercise 02: `MutantStack`

### Objective
`std::stack` is a container adapter that provides a LIFO interface (`push`, `pop`, `top`), but deliberately conceals iterators. The exercise requires making `std::stack` iterable by implementing `MutantStack`.

### The Secret: The Protected Member `c`
In the C++ standard library, `std::stack` is defined essentially as:
```cpp
template <class T, class Container = std::deque<T> >
class stack {
protected:
    Container c; // The underlying container!
public:
    // push() -> c.push_back()
    // pop()  -> c.pop_back()
    // top()  -> c.back()
    ...
};
```
Because the member `c` has `protected` access, any derived class inheriting from `std::stack` can directly access `c`!
The underlying container `std::deque<T>` (or `std::vector<T>`) already possesses full iterator support (`begin()`, `end()`, `rbegin()`, `rend()`).

### Header Breakdown (`MutantStack.hpp`)
```cpp
template <typename T, typename Container = std::deque<T> >
class MutantStack : public std::stack<T, Container>
{
public:
    typedef typename Container::iterator iterator;
    typedef typename Container::const_iterator const_iterator;
    typedef typename Container::reverse_iterator reverse_iterator;
    typedef typename Container::const_reverse_iterator const_reverse_iterator;

    MutantStack() : std::stack<T, Container>() {}
    MutantStack(const MutantStack& other) : std::stack<T, Container>(other) {}
    MutantStack& operator=(const MutantStack& rhs)
    {
        if (this != &rhs)
            std::stack<T, Container>::operator=(rhs);
        return *this;
    }
    virtual ~MutantStack() {}

    iterator begin() { return this->c.begin(); }
    iterator end() { return this->c.end(); }

    const_iterator begin() const { return this->c.begin(); }
    const_iterator end() const { return this->c.end(); }

    reverse_iterator rbegin() { return this->c.rbegin(); }
    reverse_iterator rend() { return this->c.rend(); }

    const_reverse_iterator rbegin() const { return this->c.rbegin(); }
    const_reverse_iterator rend() const { return this->c.rend(); }
};
```

### Why `this->c` instead of just `c`?
In templated derived classes, the base class `std::stack<T, Container>` is a **dependent base class**. The compiler does not look inside dependent base classes during the first phase of template parsing. Writing `c.begin()` would result in `error: 'c' was not declared in this scope`. Writing `this->c` explicitly instructs the compiler to delay lookup until the template is instantiated.

---

## 7. Top Peer Evaluation Defense Questions & Answers

### Q1: What is the difference between a Container and a Container Adapter?
**Answer**: A **container** (such as `std::vector`, `std::list`, `std::deque`) manages its own memory and storage structures directly and provides full iterator access. A **container adapter** (such as `std::stack`, `std::queue`) wraps an existing sequence container, restricting its public interface to enforce a specific access policy (such as LIFO for stack or FIFO for queue) and hiding iterators by default.

### Q2: Why does `easyfind` require `typename` before `T::iterator`?
**Answer**: Because `T` is a template parameter, `T::iterator` is a dependent name. The compiler cannot tell whether `iterator` is a static member variable or a type. The C++ standard mandates the `typename` keyword to inform the compiler that `T::iterator` is a type.

### Q3: Why doesn't `easyfind` work with associative containers like `std::map`?
**Answer**: Associative containers store key-value pairs (`std::pair<const Key, Value>`). Calling `std::find(map.begin(), map.end(), int_val)` compares pairs against an `int`, causing a compilation error. Furthermore, associative containers provide their own highly optimized logarithmic member function `map.find(key)`, whereas `std::find` performs an unoptimized linear scan ($O(N)$). The subject explicitly clarifies: *"You don’t have to handle associative containers."*

### Q4: In `Span`, why don't you sort `_storage` in place in `shortestSpan()`?
**Answer**: `shortestSpan()` is a `const` member function. Mutating the internal order of numbers in `_storage` would violate const-correctness and unexpected side effects for the user (who might rely on insertion order). We create a local copy of `_storage` and sort that copy.

### Q5: What is the algorithmic complexity of your `Span` functions?
**Answer**:
- `addNumber()`: $O(1)$ amortized insertion into `std::vector`.
- `addRange()`: $O(K)$ where $K$ is the number of inserted elements.
- `longestSpan()`: $O(N)$ via single-pass min and max element search.
- `shortestSpan()`: $O(N \log N)$ to sort the copied vector, followed by a linear $O(N)$ adjacent difference pass.

### Q6: Why did you provide both `addRange` and `addNumber(first, last)` in `Span`?
**Answer**: The subject says *"Implement a member function to add many numbers to your Span in one call."* Different evaluators and test suites call this function either `addRange` or overloaded `addNumber`. Providing both ensures 100% compatibility without modifying any logic.

### Q7: Why does `MutantStack` inherit publicly from `std::stack` instead of wrapping it (composition)?
**Answer**: The subject requires that `MutantStack` *"will offer all its member functions, plus an additional feature: iterators."* Public inheritance automatically inherits all member functions of `std::stack` (`push`, `pop`, `top`, `size`, `empty`) without writing boilerplate delegation methods, while gaining direct access to the `protected` underlying container `c`.

### Q8: Does `std::stack` have a virtual destructor? What are the implications?
**Answer**: Standard STL containers and container adapters do **not** have virtual destructors because they were not designed for runtime polymorphic deletion (`std::stack<T>* ptr = new MutantStack<T>(); delete ptr;`). `MutantStack` does not allocate raw heap memory or own external resources outside the base stack, so normal scoped and value-based usage has zero risk of leaks.

---

## 8. Live Code Modification Practice

During peer defense, an evaluator may ask you to make small live modifications. Be prepared for these scenarios:

### Scenario 1: Add a `const_iterator` search test to `ex00`
Add to `ex00/main.cpp`:
```cpp
const std::vector<int> constVec(numbers);
std::vector<int>::const_iterator cit = easyfind(constVec, 20);
std::cout << "Found in const vector: " << *cit << std::endl;
```

### Scenario 2: Add `medianSpan()` to `Span`
Calculate the difference between the median and the minimum:
```cpp
int Span::medianSpan() const
{
    if (_storage.size() < 2)
        throw std::logic_error("Need at least 2 numbers");
    std::vector<int> sorted = _storage;
    std::sort(sorted.begin(), sorted.end());
    int median = sorted[sorted.size() / 2];
    return median - sorted[0];
}
```

### Scenario 3: Test `MutantStack` with reverse iterators
Add to `ex02/main.cpp`:
```cpp
std::cout << "Reverse iteration: ";
for (MutantStack<int>::reverse_iterator rit = mstack.rbegin(); rit != mstack.rend(); ++rit)
{
    std::cout << *rit << " ";
}
std::cout << std::endl;
```

### Scenario 4: Change `MutantStack` underlying container to `std::vector`
In `main.cpp`:
```cpp
MutantStack<int, std::vector<int> > vecStack;
vecStack.push(10);
vecStack.push(20);
for (MutantStack<int, std::vector<int> >::iterator it = vecStack.begin(); it != vecStack.end(); ++it)
    std::cout << *it << std::endl;
```
