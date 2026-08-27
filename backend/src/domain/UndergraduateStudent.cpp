#include "domain/UndergraduateStudent.h"

#include <memory>

#include "policies/UndergraduateCreditPolicy.h"
#include "policies/UndergraduateLetterGradePolicy.h"

UndergraduateStudent::UndergraduateStudent(
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
        std::make_unique<UndergraduateCreditPolicy>(),
        std::make_unique<UndergraduateLetterGradePolicy>()
    ) {
}

StudentType UndergraduateStudent::getStudentType() const {
    return StudentType::UNDERGRADUATE;
}