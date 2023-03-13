#include <iostream>
#include <vector>
#include <memory>

template<typename Ret, typename... Args>
class Function
{
    class Base
    {
    public:
        virtual ~Base() { }
        virtual Ret operator() (Args... args) const = 0;
    };

    template <typename T>
    class Based : public Base
    {
    public:
        Based(const T& t) : object_(t) {}
        Based(T&& t) : object_(std::move(t)) {}

        Ret operator() (Args... args) const override
        {
            return object_(args...);
        };
    private:
        T object_;
    };

public:
    template <typename T>
    Function(const T& object) : object_(std::make_unique<Based<T>>(object)) {}

    template <typename T>
    Function(T&& object) : object_(std::make_unique<Based<T>>(std::move(object))) {}

    Ret operator() (Args... args) const
    {
        return (*object_)(args...);
    }

private:
    std::unique_ptr<Base> object_;
};


template<typename Ret>
class Function<Ret>
{
    class Base
    {
    public:
        virtual ~Base() { }
        virtual Ret operator()() const = 0;
    };

    template <typename T>
    class Based : public Base
    {
    public:
        Based(const T& t) : object_(t) {}
        Based(T&& t) : object_(std::move(t)) {}

        Ret operator()() const override
        {
            return object_();
        };
    private:
        T object_;
    };

public:
    template <typename T>
    Function(const T& object) : object_(std::make_unique<Based<T>>(object)) {}

    template <typename T>
    Function(T&& object) : object_(std::make_unique<Based<T>>(std::move(object))) {}

    Ret operator()() const
    {
        return (*object_)();
    }

private:
    std::unique_ptr<Base> object_;
};

int main()
{
    int value = 10;
    const auto lambda1 = [value](int a){ std::cout << "value = " << value << std::endl; };
    Function<void, int> fun1(lambda1);
    fun1(123);

    const auto lambda2 = [](){ std::cout << "lambda 2" << std::endl; };
    Function<void> fun2(lambda2);
    fun2();
}
