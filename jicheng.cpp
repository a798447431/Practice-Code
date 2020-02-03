/*************************************************************************
	> File Name: jicheng.cpp
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月06日 星期六 15时43分12秒
 ************************************************************************/

#include <iostream>
using namespace std;


class Animal  {
public:
    Animal(string &name) : _name(name) {
        cout << "Animal Constructor" << endl;
    }
    string name() {return this->_name; }
    ~Animal() {
        cout << "Animal Distructor" << endl;
    }
private:
    string _name;  
};

class Cat : public Animal {
public :
    Cat() : Animal("cat-1") {
        cout << "Cat Constructor" << endl;
    }
    ~Cat() {
        cout << "Animal Distructor" << endl;
    }
}; 

ostream &operator<<(ostream &out, Cat &a) {
    out << "<Class Cat>";
    out << a.name();
    return out;
}

int main() {
    Cat c;
    cout << c.name() << endl;
    return 0;
}
