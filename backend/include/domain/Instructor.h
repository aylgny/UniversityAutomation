#pragma once

#include <vector>
#include "domain/Person.h"

class Course;

// Represents an instructor and the courses they teach.
class Instructor : public Person {
private:
    // Non-owning references to the courses assigned to this instructor.
    std::vector<Course*> courses;

public:
    Instructor(int id, const std::string& name);

    // Adds a course assignment if the pointer is valid.
    void addCourse(Course* course);

    const std::vector<Course*>& getCourses() const;
};