#pragma once

#include <string>

// Common base class for system users.
// Stores attributes shared by students and instructors.
class Person {
protected:
    int id;
    std::string name;

public:
    Person(int id, const std::string& name);

    // Virtual destructor allows safe destruction through a Person pointer.
    virtual ~Person() = default;

    int getId() const;
    const std::string& getName() const;
};