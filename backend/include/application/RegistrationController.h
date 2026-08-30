#pragma once

#include <memory>
#include <vector>

class Student;
class Course;
class Enrollment;

// Coordinates the course registration use case.
class RegistrationController {
public:
    Enrollment* enroll(
        Student& student,
        Course& course,
        std::vector<std::unique_ptr<Enrollment>>& enrollments
    ) const;

private:
    // Calculates the total credits of the student's current enrollments.
    int calculateCurrentCredits(
        const Student& student,
        const std::vector<std::unique_ptr<Enrollment>>& enrollments
    ) const;
};