#include <memory>
#include <vector>
#include <set>
#include <algorithm>
#include <iostream>
#include <iterator>

// Абстрактная реализация
class SetImpl {
public:
    virtual ~SetImpl() = default;

    virtual void add(int element) = 0;
    virtual void remove(int element) = 0;
    virtual bool contains(int element) const = 0;
    virtual std::unique_ptr<SetImpl> clone() const = 0;
    virtual std::unique_ptr<SetImpl> unionWith(const SetImpl* other) const = 0;
    virtual std::unique_ptr<SetImpl> intersectionWith(const SetImpl* other) const = 0;
    virtual void print() const = 0;
    virtual size_t size() const = 0;
    virtual std::vector<int> getElements() const = 0; // Новый метод для получения элементов
};

// Первая конкретная реализация: массив
class ArraySetImpl : public SetImpl {
public:
    void add(int element) override {
        if (!contains(element)) {
            elements.push_back(element);
        }
    }

    void remove(int element) override {
        elements.erase(std::remove(elements.begin(), elements.end(), element), elements.end());
    }

    bool contains(int element) const override {
        return std::find(elements.begin(), elements.end(), element) != elements.end();
    }

    std::unique_ptr<SetImpl> clone() const override {
        return std::make_unique<ArraySetImpl>(*this);
    }

    std::unique_ptr<SetImpl> unionWith(const SetImpl* other) const override {
        auto result = std::make_unique<ArraySetImpl>(*this);
        for (const auto& elem : other->getElements()) {
            result->add(elem);
        }
        return result;
    }

    std::unique_ptr<SetImpl> intersectionWith(const SetImpl* other) const override {
        auto result = std::make_unique<ArraySetImpl>();
        for (const auto& elem : elements) {
            if (other->contains(elem)) {
                result->add(elem);
            }
        }
        return result;
    }

    void print() const override {
        for (int elem : elements) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    }

    size_t size() const override {
        return elements.size();
    }

    std::vector<int> getElements() const override {
        return elements;
    }

private:
    std::vector<int> elements;
};

// Вторая конкретная реализация: хэш-таблица
class HashSetImpl : public SetImpl {
public:
    void add(int element) override {
        elements.insert(element);
    }

    void remove(int element) override {
        elements.erase(element);
    }

    bool contains(int element) const override {
        return elements.find(element) != elements.end();
    }

    std::unique_ptr<SetImpl> clone() const override {
        return std::make_unique<HashSetImpl>(*this);
    }

    std::unique_ptr<SetImpl> unionWith(const SetImpl* other) const override {
        auto result = std::make_unique<HashSetImpl>(*this);
        for (const auto& elem : other->getElements()) {
            result->add(elem);
        }
        return result;
    }

    std::unique_ptr<SetImpl> intersectionWith(const SetImpl* other) const override {
        auto result = std::make_unique<HashSetImpl>();
        for (const auto& elem : elements) {
            if (other->contains(elem)) {
                result->add(elem);
            }
        }
        return result;
    }

    void print() const override {
        for (int elem : elements) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    }

    size_t size() const override {
        return elements.size();
    }

    std::vector<int> getElements() const override {
        return std::vector<int>(elements.begin(), elements.end());
    }

private:
    std::set<int> elements;
};

// Абстракция
class Set {
public:
    Set();

    void add(int element);
    void remove(int element);
    bool contains(int element) const;
    Set unionWith(const Set& other) const;
    Set intersectionWith(const Set& other) const;
    void print() const;

private:
    std::unique_ptr<SetImpl> impl;
    void switchImplementationIfNeeded();
};

// Реализация методов класса Set
Set::Set() : impl(std::make_unique<ArraySetImpl>()) {}

void Set::add(int element) {
    impl->add(element);
    switchImplementationIfNeeded();
}

void Set::remove(int element) {
    impl->remove(element);
    switchImplementationIfNeeded();
}

bool Set::contains(int element) const {
    return impl->contains(element);
}

Set Set::unionWith(const Set& other) const {
    Set result;
    result.impl = impl->unionWith(other.impl.get());
    return result;
}

Set Set::intersectionWith(const Set& other) const {
    Set result;
    result.impl = impl->intersectionWith(other.impl.get());
    return result;
}

void Set::print() const {
    impl->print();
}

void Set::switchImplementationIfNeeded() {
    const size_t threshold = 10;
    if (impl->size() > threshold && dynamic_cast<ArraySetImpl*>(impl.get())) {
        auto newImpl = std::make_unique<HashSetImpl>();
        for (int elem : impl->getElements()) {
            newImpl->add(elem);
        }
        impl = std::move(newImpl);
    } else if (impl->size() <= threshold && dynamic_cast<HashSetImpl*>(impl.get())) {
        auto newImpl = std::make_unique<ArraySetImpl>();
        for (int elem : impl->getElements()) {
            newImpl->add(elem);
        }
        impl = std::move(newImpl);
    }
}

// Пример использования
int main() {
    Set mySet;
    for (int i = 1; i <= 15; ++i) {
        mySet.add(i);
    }
    mySet.print();

    mySet.remove(10);
    mySet.print();

    Set otherSet;
    otherSet.add(5);
    otherSet.add(20);

    Set unionSet = mySet.unionWith(otherSet);
    unionSet.print();

    Set intersectionSet = mySet.intersectionWith(otherSet);
    intersectionSet.print();

    return 0;
}
