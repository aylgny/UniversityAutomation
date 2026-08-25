#include "domain/Instructor.h"

Instructor::Instructor(int id, const std::string& name)
    : Person(id, name) {
}

void Instructor::addCourse(Course* course) {
    if (course != nullptr) {
        courses.push_back(course);
    }
}

const std::vector<Course*>& Instructor::getCourses() const {
    return courses;
}