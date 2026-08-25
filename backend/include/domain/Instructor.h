#pragma once

#include <vector>
#include "domain/Person.h"

class Course;

class Instructor : public Person {
private:
    std::vector<Course*> courses;

public:
    Instructor(int id, const std::string& name);

    void addCourse(Course* course);

    const std::vector<Course*>& getCourses() const;
};