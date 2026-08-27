#pragma once

#include <memory>
#include <string>
#include <vector>

class Student;
class Course;
class Instructor;
class Enrollment;

// Owns the main application data and manages persistent storage.
class ApplicationState {
private:
    std::vector<std::unique_ptr<Student>> students;
    std::vector<std::unique_ptr<Course>> courses;
    std::vector<std::unique_ptr<Instructor>> instructors;
    std::vector<std::unique_ptr<Enrollment>> enrollments;

public:
    ApplicationState();

    // Defined in the .cpp file where owned types are complete.
    ~ApplicationState();

    void seedSampleData();

    void loadFromFile(
        const std::string& filePath
    );

    void saveToFile(
        const std::string& filePath
    ) const;

    const std::vector<std::unique_ptr<Student>>&
        getStudents() const;

    const std::vector<std::unique_ptr<Course>>&
        getCourses() const;

    const std::vector<std::unique_ptr<Instructor>>&
        getInstructors() const;

    std::vector<std::unique_ptr<Enrollment>>&
        getEnrollments();

    const std::vector<std::unique_ptr<Enrollment>>&
        getEnrollments() const;

    Student* findStudentById(
        int id
    ) const;

    Course* findCourseById(
        int id
    ) const;

    Instructor* findInstructorById(
        int id
    ) const;

    Enrollment* findEnrollmentById(
        int id
    ) const;
};