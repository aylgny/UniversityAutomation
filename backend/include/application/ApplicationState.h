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
    // ApplicationState owns the main domain objects.
    std::vector<std::unique_ptr<Student>> students;
    std::vector<std::unique_ptr<Course>> courses;
    std::vector<std::unique_ptr<Instructor>> instructors;
    std::vector<std::unique_ptr<Enrollment>> enrollments;

public:
    ApplicationState();

    // Defined in the .cpp file where the owned types are complete.
    ~ApplicationState();

    // Replaces the current state with predefined demonstration data.
    void seedSampleData();

    // Restores the application state from a JSON file.
    void loadFromFile(
        const std::string& filePath
    );

    // Persists the current application state to a JSON file.
    void saveToFile(
        const std::string& filePath
    ) const;


    // =====================================================
    // ACCESSORS
    // =====================================================

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


    // =====================================================
    // LOOKUPS
    // =====================================================

    // Lookup methods return nullptr when no matching object is found.
    Student* findStudentById(
        int id
    );

    const Student* findStudentById(
        int id
    ) const;


    Course* findCourseById(
        int id
    );

    const Course* findCourseById(
        int id
    ) const;


    Instructor* findInstructorById(
        int id
    );

    const Instructor* findInstructorById(
        int id
    ) const;


    Enrollment* findEnrollmentById(
        int id
    );

    const Enrollment* findEnrollmentById(
        int id
    ) const;
};