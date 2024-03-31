#include <iostream>

#include "my_vector.hpp"

namespace product
{
    class Product
    {
    public:
        Product(const char *name, int quantity, double price);

        friend std::ostream &operator<<(std::ostream &os, const Product &product);

    private:
        char *name_;
        int quantity_;
        double price_;
    };

    std::ostream &operator<<(std::ostream &os, const Product &product)
    {
        os << product.name_ << " " << product.quantity_ << " " << product.price_;
        return os;
    }

    Product::Product(const char *name, int quantity, double price) : name_(new char[strlen(name) + 1]),
                                                                     quantity_(quantity), price_(price)
    {}
}  // namespace product

namespace test_my_vector
{
    template<typename T>
    void test_my_vector(const T &arg1, const T &arg2)
    {
        containers::my_vector<T> vec;
        std::cout << "Vector created. Empty? " << (vec.empty() ? "Yes" : "No") << std::endl;

        // Добавляем элементы
        vec.push_back(arg1);
        vec.push_back(arg2);
        std::cout << "Size after push_back: " << vec.size() << std::endl;

        // Выводим элементы вектора
        std::cout << "Vector elements: ";
        for (std::size_t i = 0; i < vec.size(); ++i)
        {
            std::cout << vec[i] << " ";
        }
        std::cout << std::endl;

        // Операторы [] и доступа к размеру
        std::cout << "Element at index 0: " << vec[0] << std::endl;
        std::cout << "Vector size: " << vec.size() << std::endl;

        // Операции resize и reserve
//        vec.resize(3);
//        std::cout << "Size after resize(3): " << vec.size() << std::endl;
        vec.reserve(10);
        std::cout << "Capacity after reserve(10): " << vec.capacity() << std::endl;

        // Операция pop_back
        vec.pop_back();
        std::cout << "Size after pop_back: " << vec.size() << std::endl;

        // Операция clear
        vec.clear();
        std::cout << "Size after clear: " << vec.size() << std::endl;
        std::cout << "Empty? " << (vec.empty() ? "Yes" : "No") << std::endl;
    }
} // namespace test_my_vector

namespace test_my_vector_default_constructible
{
    template<typename T>
    void test_my_vector(const T &arg1, const T &arg2)
    {
        containers::my_vector<T> vec;
        std::cout << "Vector created. Empty? " << (vec.empty() ? "Yes" : "No") << std::endl;

        // Добавляем элементы
        vec.push_back(arg1);
        vec.push_back(arg2);
        std::cout << "Size after push_back: " << vec.size() << std::endl;

        // Выводим элементы вектора
        std::cout << "Vector elements: ";
        for (std::size_t i = 0; i < vec.size(); ++i)
        {
            std::cout << vec[i] << " ";
        }
        std::cout << std::endl;

        // Операторы [] и доступа к размеру
        std::cout << "Element at index 0: " << vec[0] << std::endl;
        std::cout << "Vector size: " << vec.size() << std::endl;

        // Операции resize и reserve
        vec.resize(3);
        std::cout << "Size after resize(3): " << vec.size() << std::endl;
        vec.reserve(10);
        std::cout << "Capacity after reserve(10): " << vec.capacity() << std::endl;

        // Операция pop_back
        vec.pop_back();
        std::cout << "Size after pop_back: " << vec.size() << std::endl;

        // Операция clear
        vec.clear();
        std::cout << "Size after clear: " << vec.size() << std::endl;
        std::cout << "Empty? " << (vec.empty() ? "Yes" : "No") << std::endl;
    }
} // namespace test_my_vector_default_constructible



int main()
{
    containers::my_vector<int> v;
    v.push_back(2);
    const int n{3};
    v.push_back(n);
    std::cout << v << std::endl;

    test_my_vector_default_constructible::test_my_vector<int>(5, 10);
    test_my_vector::test_my_vector<product::Product>(product::Product("asdf", 4, 12.0),
                                                     product::Product("qwe", -1, 7.5));

    return 0;
}
