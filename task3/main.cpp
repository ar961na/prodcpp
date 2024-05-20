#include <stdexcept>
#include <iostream>
#include <string>
#include <type_traits>


template<class... Types>
struct type_map;


template<class T, class... Types>
struct type_map<T, Types...>
{
    T value;
    type_map<Types...>* tail = nullptr;
    bool deleted = true;

    type_map();

    template<class U>
    void AddValue (U&& val)
    {
        if constexpr (std::is_same<T, U>::value)
        {
            deleted = false;
            value = val;
        }
        else
        {
            if constexpr (std::is_same<decltype(tail), type_map<>*>::value)
                throw std::runtime_error{"There is no such value in type_map"};
            else
                tail->template AddValue<U>(std::forward<U>(val));
        }
    }

    template<class U>
    U GetValue ()
    {
        if constexpr (std::is_same<T, U>::value)
            return deleted ? throw std::runtime_error{"There is no such value in type_map"} : value;
        else
        {
            if constexpr (std::is_same<decltype(tail), type_map<>*>::value)
                throw std::runtime_error{"There is no such value in type_map"};
            else
                return tail->template GetValue<U>();
        }
    }

    template<class U>
    bool Contains ()
    {
        if constexpr (std::is_same<T, U>::value)
            return true;
        else
        {
            if constexpr (std::is_same<decltype(tail), type_map<>*>::value)
                return false;
            else
                return tail->template Contains<U>();
        }
    }

    template<class U>
    void RemoveValue ()
    {
        if constexpr (std::is_same<T, U>::value)
            deleted = true;
        else
        {
            if constexpr (std::is_same<decltype(tail), type_map<>*>::value)
                throw std::runtime_error{"There is no such value in type_map"};
            else
                tail->template RemoveValue<U>();
        }
    }

};


template<class T, class... Types>
type_map<T, Types...>::type_map()
{
    if constexpr (sizeof...(Types) != 0)
        tail = new type_map<Types...>();
}

struct DataA
{
    std::string value;
};
struct DataB
{
    int value;
};

int main()
{
    type_map<int, DataA, double, DataB> myTypeMap;

    myTypeMap.AddValue<int>(42);
    myTypeMap.AddValue<double>(3.14);
    myTypeMap.AddValue<DataA>({"Hello, type_map!"});
    myTypeMap.AddValue<DataB>({10});

    std::cout << "Value for int: " << myTypeMap.GetValue<int>() << std::endl;
    std::cout << "Value for double: " << myTypeMap.GetValue<double>() << std::endl;
    std::cout << "Value for DataA: " << myTypeMap.GetValue<DataA>().value << std::endl;
    std::cout << "Value for DataB: " << myTypeMap.GetValue<DataB>().value << std::endl;

    std::cout << "Contains int? " << (myTypeMap.Contains<int>() ? "Yes" : "No") << std::endl;

    myTypeMap.RemoveValue<double>();

    std::cout << "Value for double after removal: " << std::flush << myTypeMap.GetValue<double>() << std::endl;

    return 0;
}