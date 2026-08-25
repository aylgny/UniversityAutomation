#pragma once

#include <vector>

class Student;
class Course;
class Enrollment;

class RegistrationController {
public:
    Enrollment* enroll(
        Student& student,
        Course& course,
        std::vector<Enrollment>& enrollments
    ) const;

private:
    int calculateCurrentCredits(
        const Student& student,
        const std::vector<Enrollment>& enrollments
    ) const;
};