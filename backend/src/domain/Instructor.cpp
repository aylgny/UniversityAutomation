#include "domain/Instructor.h"

Instructor::Instructor(int id, const std::string& name)
    : Person(id, name) {
}

void Instructor::addCourse(Course* course) {

    // Ignore invalid course references.
    if (course == nullptr) {
        return;
    }

    // Prevent assigning the same course more than once.
    for (const Course* existingCourse : courses) {
        if (existingCourse == course) {
            return;
        }
    }

    courses.push_back(course);
}

const std::vector<Course*>& Instructor::getCourses() const {
    return courses;
}