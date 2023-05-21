#include <iostream>
#include <memory>
#include <vector>

template<typename Ret, typename... Args>
class Function
{
	class Base
	{
	public:
		virtual ~Base() = default;
		virtual Ret operator()(Args... args) const = 0;
	};

	template<typename T>
	class Based : public Base
	{
	public:
		explicit Based(const T &obj)
			: object_(obj)
		{}
		explicit Based(T &&obj)
			: object_(std::move(obj))
		{}

		Ret operator()(Args... args) const override { return object_(args...); };

	private:
		T object_;
	};

public:
	Function(const Function &function)
		: object_(std::make_unique(function.object_))
	{}

	Function(Function &&function) noexcept
		: object_(std::move(function.object_))
	{}

	template<typename T>
	explicit Function(const T &object)
		: object_(std::make_unique<Based<T>>(object))
	{}

	template<typename T>
	explicit Function(T &&object)
		: object_(std::make_unique<Based<T>>(std::forward<T>(object)))
	{}

	Ret operator()(Args... args) const { return (*object_)(args...); }

private:
	std::unique_ptr<Base> object_;
};


template<typename Ret>
class Function<Ret>
{
	class Base
	{
	public:
		virtual ~Base() {}
		virtual Ret operator()() const = 0;
	};

	template<typename T>
	class Based : public Base
	{
	public:
		Based(const T &t)
			: object_(t)
		{}
		Based(T &&t)
			: object_(std::move(t))
		{}

		Ret operator()() const override { return object_(); };

	private:
		T object_;
	};

public:
	template<typename T>
	Function(const T &object)
		: object_(std::make_unique<Based<T>>(object))
	{}

	template<typename T>
	Function(T &&object)
		: object_(std::make_unique<Based<T>>(std::move(object)))
	{}

	Ret operator()() const { return (*object_)(); }

private:
	std::unique_ptr<Base> object_;
};

int main()
{
	int value = 10;
	const auto lambda1 = [value](int a) {
		std::cout << "value = " << value << std::endl;
	};
	Function<void, int> fun1(lambda1);
	fun1(123);

	const auto lambda2 = []() {
		std::cout << "lambda 2" << std::endl;
	};
	Function<void> fun2(lambda2);
	fun2();
}
