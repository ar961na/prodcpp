#include <type_traits>
#include <tuple>
#include <concepts>

namespace TypeList
{

    template<class... Types>
    struct TypeList
    {
        // Получение элемента списка по его индексу
        template<std::size_t id>
        using get = typename std::tuple_element<id, std::tuple<Types...>>::type;

        // Получение размера списка
        static constexpr std::size_t size = sizeof...(Types);

        // Проверка наличия типа в списке (constexpr bool)
        template<class T>
        static constexpr bool is_contained = (std::is_same<T, Types>::value || ...);

        // Получение индекса типа в списке
        template<typename T, std::size_t id = 0>
        static constexpr std::size_t id_type()
        {
            if constexpr (std::is_same<T, get<id>>::value)
            return id;
            else
            return id_type<T, id + 1>();
        }

        template<typename T>
        static constexpr std::size_t get_id = id_type<T, 0>();

        // Добавление типа в конец списка
        template<class T>
        using push_back = TypeList<Types..., T>;

        // Добавление типа в начало списка
        template<class T>
        using push_front = TypeList<T, Types...>;
    };

}

int main() {
    using MyTypeList = TypeList::TypeList<int, double, char, bool>;

    static_assert(std::is_same<MyTypeList::get<0>, int>::value);
    static_assert(std::is_same<MyTypeList::get<1>, double>::value);
    static_assert(std::is_same<MyTypeList::get<2>, char>::value);
    static_assert(std::is_same<MyTypeList::get<3>, bool>::value);

    static_assert(MyTypeList::size == 4);

    static_assert(MyTypeList::is_contained<int>);
    static_assert(MyTypeList::is_contained<double>);
    static_assert(MyTypeList::is_contained<char>);
    static_assert(MyTypeList::is_contained<bool>);
    static_assert(!MyTypeList::is_contained<float>);

    static_assert(MyTypeList::get_id<int> == 0);
    static_assert(MyTypeList::get_id<double> == 1);
    static_assert(MyTypeList::get_id<char> == 2);
    static_assert(MyTypeList::get_id<bool> == 3);

    using NewTypeList = MyTypeList::push_back<float>;
    static_assert(NewTypeList::size == 5);
    static_assert(NewTypeList::is_contained<float>);
    static_assert(std::is_same<NewTypeList::get<4>, float>::value);

    using NewTypeList2 = MyTypeList::push_front<float>;
    static_assert(NewTypeList2::size == 5);
    static_assert(NewTypeList2::is_contained<float>);
    static_assert(std::is_same<NewTypeList2::get<0>, float>::value);

    return 0;
}