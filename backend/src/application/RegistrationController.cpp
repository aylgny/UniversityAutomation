#include "application/RegistrationController.h"

#include <stdexcept>

#include "domain/Student.h"
#include "domain/Course.h"
#include "domain/Enrollment.h"


int RegistrationController::calculateCurrentCredits(
    const Student& student,
    const std::vector<std::unique_ptr<Enrollment>>& enrollments
) const {

    int totalCredits = 0;

    // Sum credits only from courses belonging to this student.
    for (const auto& enrollment : enrollments) {

        if (
            enrollment->getStudent() ==
            &student
            ) {
            totalCredits +=
                enrollment
                ->getCourse()
                ->getCredits();
        }
    }

    return totalCredits;
}


Enrollment* RegistrationController::enroll(
    Student& student,
    Course& course,
    std::vector<std::unique_ptr<Enrollment>>& enrollments
) const {

    // Prevent duplicate registration for the same student and course.
    for (const auto& enrollment : enrollments) {

        if (
            enrollment->getStudent() == &student &&
            enrollment->getCourse() == &course
            ) {
            throw std::logic_error(
                "Student is already enrolled in this course."
            );
        }
    }


    const int currentCredits =
        calculateCurrentCredits(
            student,
            enrollments
        );


    // Credit-limit calculation is delegated to the student's CreditPolicy.
    const auto maxCredits =
        student.getMaxCredits();

    // Unlimited policies return no maximum credit value.
    if (
        maxCredits.has_value() &&
        currentCredits + course.getCredits() >
        maxCredits.value()
        ) {
        throw std::logic_error(
            "Credit limit would be exceeded."
        );
    }


    const int newEnrollmentId =
        static_cast<int>(
            enrollments.size()
            ) + 1;


    auto enrollment =
        std::make_unique<Enrollment>(
            newEnrollmentId,
            &student,
            &course
        );


    // Keep a non-owning pointer before transferring ownership.
    Enrollment* enrollmentPtr =
        enrollment.get();


    // Application-level collection takes ownership of Enrollment.
    enrollments.push_back(
        std::move(enrollment)
    );


    return enrollmentPtr;
}