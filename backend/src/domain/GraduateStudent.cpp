#include "domain/GraduateStudent.h"

#include <memory>

#include "policies/GraduateCreditPolicy.h"
#include "policies/GraduateLetterGradePolicy.h"

GraduateStudent::GraduateStudent(
    int id,
    const std::string& name,
    double gpa
)

// The concrete student type selects the appropriate policies,
// while the Student base class owns and uses them.
    : Student(
        id,
        name,
        gpa,
        std::make_unique<GraduateCreditPolicy>(),
        std::make_unique<GraduateLetterGradePolicy>()
    ) {
}

StudentType GraduateStudent::getStudentType() const {
    return StudentType::GRADUATE;
}