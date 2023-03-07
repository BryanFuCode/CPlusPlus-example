#include <iostream>
#include <vector>

using namespace std;

class Product
{
private:
    int price;
    string name;

public:
    Product(string _name, int _price) : name(_name), price(_price)
    {
        cout << "product has been constructed" << endl;
    }

    // 拷贝构造函数
    Product(const Product &other) : name(other.name), price(other.price)
    {
        cout << "product has been copy constructed" << endl;
    }

    // 移动构造函数
    Product(Product &&other) : name(other.name), price(other.price)
    {
        cout << "product has been move constructed" << endl;
    }

    ~Product() {}
};

int main()
{
    vector<Product> p1;
    cout << "emplace_back:" << endl;
    p1.emplace_back("apple", 10);

    vector<Product> p2;
    cout << "push_back:" << endl;
    p2.push_back(Product("banana", 20));
}