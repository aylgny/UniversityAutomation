#include "application/RegistrationController.h"

#include <stdexcept>

#include "domain/Student.h"
#include "domain/Course.h"
#include "domain/Enrollment.h"

int RegistrationController::calculateCurrentCredits(
    const Student& student,
    const std::vector<Enrollment>& enrollments
) const {
    int totalCredits = 0;

    for (const auto& enrollment : enrollments) {
        if (enrollment.getStudent() == &student) {
            totalCredits += enrollment.getCourse()->getCredits();
        }
    }

    return totalCredits;
}

Enrollment* RegistrationController::enroll(
    Student& student,
    Course& course,
    std::vector<Enrollment>& enrollments
) const {
    for (const auto& enrollment : enrollments) {
        if (enrollment.getStudent() == &student &&
            enrollment.getCourse() == &course) {
            throw std::logic_error(
                "Student is already enrolled in this course."
            );
        }
    }

    const int currentCredits =
        calculateCurrentCredits(student, enrollments);

    const auto maxCredits =
        student.getMaxCredits();

    if (maxCredits.has_value() &&
        currentCredits + course.getCredits() > maxCredits.value()) {
        throw std::logic_error(
            "Credit limit would be exceeded."
        );
    }

    const int newEnrollmentId =
        static_cast<int>(enrollments.size()) + 1;

    enrollments.emplace_back(
        newEnrollmentId,
        &student,
        &course
    );

    return &enrollments.back();
}