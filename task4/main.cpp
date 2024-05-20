#include <compare>
#include <iostream>
#include <atomic>

template<class Derived>
class counter
{
	static std::atomic<std::size_t> m_count;
public:
	counter()  { m_count++; }
	~counter() { m_count--; }
	static size_t count()
	{
		return m_count;
	}
};

template<class Derived>
std::atomic<std::size_t> counter<Derived>::m_count;

template<class Derived>
struct less_then_comparable
{
	auto operator<=>(const Derived& in) const
	{
		if (*(static_cast<const Derived*>(this)) < in)
			return std::weak_ordering::less;
		else if (in < *(static_cast<const Derived*>(this)))
			return std::weak_ordering::greater;
		else
			return std::weak_ordering::equivalent;
	}

	bool operator==(const Derived& in) const
	{
		return !(*(static_cast<const Derived*>(this)) < in) && !(in < *(static_cast<const Derived*>(this)));
	}
};

struct NamedDouble : public less_then_comparable<NamedDouble>, public counter<NamedDouble>
{
	const std::string name;
	const double value;
	NamedDouble(std::string name, double value)
	:	name {name}
	,	value {value}
	{}

	bool operator<(const NamedDouble& in) const
	{
		return value < in.value;
	}
};


std::ostream& operator<<(std::ostream& os, const NamedDouble& in)
{
	os << "name : \"" + in.name + "\", value : " << in.value;
	return os;
}

void test(const NamedDouble& d1, const NamedDouble& d2)
{
	std::cout << d1 << '\n' << d2 << std::endl;

	std::cout << "is " + d1.name + " equal to " + d2.name + " : " << (d1 == d2) << std::endl;
	std::cout << "is " + d1.name + " greater or equal to " + d2.name + " : " << (d1 >= d2) << std::endl;
	std::cout << "is " + d1.name + " less or equal to " + d2.name + " : " << (d1 <= d2) << std::endl;
	std::cout << "is " + d1.name + " not equal to " + d2.name + " : " << (d1 != d2) << std::endl;

	std::cout << "Number of instances of NamedDouble : " << counter<NamedDouble>::count() << std::endl;
}

int main()
{
	NamedDouble d1 ("first", 1.5);
	std::cout << "Number of instances of NamedDouble : " << counter<NamedDouble>::count() << std::endl;
	NamedDouble d2 ("second", 2.5);
	std::cout << "Number of instances of NamedDouble : " << counter<NamedDouble>::count() << std::endl;

	test(NamedDouble("first", 1.5), NamedDouble("second", 2.5));
	test(NamedDouble("first", 1.5), NamedDouble("second", 1.5));
	test(NamedDouble("first", 2.5), NamedDouble("second", 1.5));

	std::cout << "Number of instances of NamedDouble : " << counter<NamedDouble>::count() << std::endl;
}
