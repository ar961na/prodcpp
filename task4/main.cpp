#include <iostream>
#include <cstdlib>
#include <concepts>
#include <cassert>

template<class T>
struct counter {
private:
    static std::size_t cnt;
public:
    counter() { cnt++; }
    ~counter() { cnt--; }

    static std::size_t count() { return cnt; }
};

template<class T>
std::size_t counter<T>::cnt = 0;

template<class T>
concept LessImplemented = requires(T t1, T t2){
    { t1 < t2 } -> std::convertible_to<bool>;
};

template<class T> //requires LessImplemented<T>
struct less_than_comparable {
public:
    // оператор < уже определен
    // friend bool operator<(const T& lhs, const T& rhs) { return lhs < rhs; }

    friend bool operator>(const T &t1, const T &t2) { return t2 < t1; }

    friend bool operator>=(const T &t1, const T &t2) { return !(t1 < t2); }

    friend bool operator<=(const T &t1, const T &t2) { return !(t1 > t2); }

    friend bool operator!=(const T &t1, const T &t2) { return t1 > t2 || t1 < t2; }

    friend bool operator==(const T &t1, const T &t2) { return !(t1 != t2); }
};

class Number : public less_than_comparable<Number>, public counter<Number> {
public:
    Number(int value) : m_value{value} {}
    int value() const { return m_value; }
    bool operator<(Number const &other) const { return m_value < other.m_value; }

private:
    int m_value;
};

int main() {
    Number one{1};
    Number two{2};
    Number three{3};
    Number four{4};

    assert(one >= one);
    assert(three <= four);
    assert(two == two);
    assert(three > two);
    assert(one < two);
    std::cout << "Count: " << counter<Number>::count() << std::endl;
    return 0;
}