#include "DynamicArray.h"
#include "HashMap.h"
#include <iostream>
#include <string>

struct Person {
    std::string name;
    int age;
};

bool r(const Person& a, const Person& b) {
    return a.name < b.name;
}

bool p(const Person& a, const Person& b) {
    return a.age > b.age;
}
int main() {
    std::cout << "×ÒÎ ÏÐÎÈÑÕÎÄÆÈÒ" << std::endl;

    DynamicArray<int> arr;
    arr.append(10);
    arr.append(20);
    arr.append(30);

    for (size_t i = 0; i < arr.size(); i++) {
        std::cout << "arr[" << i << "] = " << arr[2 - i] << std::endl;
    }

    std::cout << "HashMap test\n" << std::endl;
    HashMap<int, std::string> map;
    map.insert(1, "one");
    map.insert(2, "two");
    map.insert(3, "three");

    if (auto value = map.find(2)) {
        std::cout << "Found key 2: " << *value << std::endl;
    }

    map.erase(2);

    if (map.find(2) == nullptr) {
        std::cout << "Key 2 not found (after erase)" << std::endl;
    }

    DynamicArray<Person> people;

    people.append({ "Alice", 25 });
    people.append({ "Bob", 30 });
    people.append({ "Charlie", 22 });
    people.append({ "David", 28 });

    people.removeAt(1);
    people.sort(r);

    for (size_t i = 0; i < people.size(); ++i) {
        std::cout << people[i].name << " - " << people[i].age << std::endl;
    }

    std::cout << std::endl;
    people.sort(p);



    for (size_t i = 0; i < people.size(); ++i) {
        std::cout << people[i].name << " - " << people[i].age << std::endl;
    }
    return 0;
}