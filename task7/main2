#include <iostream>
#include <algorithm>
#include <memory>
#include <set>
#include <stdexcept>
#include <vector>

class SetImplementation;

using AbstractPointer = std::unique_ptr<SetImplementation>;

class SetImplementation {
public:
    virtual void addElement(int element) = 0;

    virtual void deleteElement(int element) = 0;

    virtual bool haveElement(int element) = 0;

    virtual AbstractPointer Union(AbstractPointer other) = 0;

    virtual AbstractPointer Intersection(AbstractPointer other) = 0;

    virtual std::vector<int> getAllElements() = 0;

    virtual std::size_t getElementCount() = 0;
};

class SmallSet : public SetImplementation {
public:
    virtual void addElement(int element) override {
        if (!this->haveElement(element)) m_data.push_back(element);
    }

    virtual void deleteElement(int element) override {
        if (this->haveElement(element))
            m_data.erase(m_data.begin() + this->getIndex(element));
    }

    virtual bool haveElement(int element) override {
        for (std::size_t i = 0; i < m_data.size(); ++i)
            if (element == m_data.at(i)) return true;
        return false;
    }

    virtual AbstractPointer Union(AbstractPointer other) override {
        SmallSet result;
        for (auto element: m_data) result.addElement(element);
        for (auto element: other->getAllElements()) result.addElement(element);
        SetImplementation *pointer =
                static_cast<SetImplementation *>(new SmallSet(result));
        return AbstractPointer(pointer);
    }

    virtual AbstractPointer Intersection(AbstractPointer other) override {
        SmallSet result;
        for (auto element: m_data)
            if (other->haveElement(element)) result.addElement(element);
        SetImplementation *pointer =
                static_cast<SetImplementation *>(new SmallSet(result));
        return AbstractPointer(pointer);
    }

    virtual std::vector<int> getAllElements() override { return m_data; }

    virtual std::size_t getElementCount() override { return m_data.size(); }

private:
    std::vector<int> m_data;

    std::size_t getIndex(int element) {
        for (std::size_t i = 0; i < m_data.size(); ++i)
            if (element == m_data.at(i)) return i;
        throw std::runtime_error("no such element");
    }
};

class BigSet : public SetImplementation {
public:
    virtual void addElement(int element) override {
        if (!this->haveElement(element)) m_data.insert(element);
    }

    virtual void deleteElement(int element) override { m_data.erase(element); }

    virtual bool haveElement(int element) override { m_data.contains(element); }

    virtual AbstractPointer Union(AbstractPointer other) override {
        BigSet result;
        for (auto element: other->getAllElements()) result.addElement(element);
        SetImplementation *pointer =
                static_cast<SetImplementation *>(new BigSet(result));
        return AbstractPointer(pointer);
    }

    virtual AbstractPointer Intersection(AbstractPointer other) override {
        BigSet result;
        for (auto element: m_data)
            if (other->haveElement(element)) result.addElement(element);
        SetImplementation *pointer =
                static_cast<SetImplementation *>(new BigSet(result));
        return AbstractPointer(pointer);
    }

    virtual std::vector<int> getAllElements() {
        std::vector<int> result;
        for (auto iter = m_data.begin(); iter != m_data.end(); iter++)
            result.push_back(*iter);
        return result;
    }

    virtual std::size_t getElementCount() { return m_data.size(); }

private:
    std::set<int> m_data;
};

class Set {
public:
    Set() {
        implementation = std::unique_ptr<SetImplementation>(
                static_cast<SetImplementation *>(new SmallSet));
    }

    Set(Set &&other) noexcept: implementation{std::move(other.implementation)} {}

    void addElement(int element) {
        if (implementation->getElementCount() + 1 > high_limit) UseBig();
        implementation->addElement(element);
    }

    void deleteElement(int element) {
        if (implementation->getElementCount() < low_limit + 1) UseSmall();
        implementation->deleteElement(element);
    }

    bool haveElement(int element) { return implementation->haveElement(element); }

    Set Union(Set &other) {
        Set result = Set(implementation->Union(std::move(other.implementation)));
        if (result.implementation->getElementCount() > high_limit) result.UseBig();
        return result;
    }

    Set Intersection(Set &other) {
        Set result =
                Set(implementation->Intersection(std::move(other.implementation)));
        if (result.implementation->getElementCount() < low_limit) result.UseSmall();
        return result;
    }

    bool IsBigSetUsing() { return use_big_set; }

    void print() {
        for (auto element : implementation->getAllElements())
            std::cout << element << " ";
    }

private:
    std::unique_ptr<SetImplementation> implementation;

    explicit Set(std::unique_ptr<SetImplementation> impl)
            : implementation{std::move(impl)} {}

    static constexpr std::size_t high_limit = 8;
    static constexpr std::size_t low_limit = 4;
    bool use_big_set = false;

    void UseBig() {
        if (use_big_set) return;
        BigSet big_set;
        for (auto element: implementation->getAllElements())
            big_set.addElement(element);
        SetImplementation *pointer =
                static_cast<SetImplementation *>(new BigSet(big_set));
        implementation.reset();
        implementation = std::unique_ptr<SetImplementation>(pointer);
        use_big_set = true;
    }

    void UseSmall() {
        if (!use_big_set) return;
        SmallSet small_set;
        for (auto element: implementation->getAllElements())
            small_set.addElement(element);
        SetImplementation *pointer =
                static_cast<SetImplementation *>(new SmallSet(small_set));
        implementation.reset();
        implementation = std::unique_ptr<SetImplementation>(pointer);
        use_big_set = false;
    }
};

int main() {
    Set mySet;
    for (int i = 1; i <= 15; ++i) {
        mySet.addElement(i);
    }
    mySet.print();

    mySet.deleteElement(10);
    mySet.print();

    Set otherSet;
    otherSet.addElement(5);
    otherSet.addElement(20);

    Set unionSet = mySet.Union(otherSet);
    unionSet.print();

    Set intersectionSet = mySet.Intersection(otherSet);
    intersectionSet.print();
}
