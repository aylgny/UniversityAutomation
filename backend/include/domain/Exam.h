#pragma once

#include <string>

// Represents an exam definition belonging to a course.
class Exam {
private:
    int id;
    std::string name;

public:
    Exam(int id, const std::string& name);

    int getId() const;
    const std::string& getName() const;
};