#include "domain/Student.h"

#include <stdexcept>
#include <utility>

Student::Student(
    int id,
    const std::string& name,
    double gpa,
    std::unique_ptr<CreditPolicy> creditPolicy,
    std::unique_ptr<LetterGradePolicy> letterGradePolicy
)
    : Person(id, name),
    gpa(gpa),

    // Transfer ownership of the policy objects to this Student.
    creditPolicy(std::move(creditPolicy)),
    letterGradePolicy(std::move(letterGradePolicy)) {

    // GPA is expected to be within the standard 0.0 - 4.0 range.
    if (gpa < 0.0 || gpa > 4.0) {
        throw std::invalid_argument("GPA must be between 0.0 and 4.0.");
    }

    // A Student must always have a credit policy.
    if (!this->creditPolicy) {
        throw std::invalid_argument("Credit policy cannot be null.");
    }

    // A Student must always have a letter grade policy.
    if (!this->letterGradePolicy) {
        throw std::invalid_argument("Letter grade policy cannot be null.");
    }
}

double Student::getGpa() const {
    return gpa;
}

std::optional<int> Student::getMaxCredits() const {
    // Credit calculation is delegated to the configured policy.
    return creditPolicy->getMaxCredits(gpa);
}

LetterGrade Student::calculateLetterGrade(double score) const {
    if (score < 0.0 || score > 100.0) {
        throw std::invalid_argument("Score must be between 0 and 100.");
    }

    // Letter grade calculation is delegated to the configured policy.
    return letterGradePolicy->getLetterGrade(score);
}