#pragma once

#include <string>

class Person {
protected:
    int id;
    std::string name;

public:
    Person(int id, const std::string& name);

    virtual ~Person() = default;

    int getId() const;
    const std::string& getName() const;
};