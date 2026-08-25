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
    creditPolicy(std::move(creditPolicy)),
    letterGradePolicy(std::move(letterGradePolicy)) {

    if (gpa < 0.0 || gpa > 4.0) {
        throw std::invalid_argument("GPA must be between 0.0 and 4.0.");
    }

    if (!this->creditPolicy) {
        throw std::invalid_argument("Credit policy cannot be null.");
    }

    if (!this->letterGradePolicy) {
        throw std::invalid_argument("Letter grade policy cannot be null.");
    }
}

double Student::getGpa() const {
    return gpa;
}

std::optional<int> Student::getMaxCredits() const {
    return creditPolicy->getMaxCredits(gpa);
}

LetterGrade Student::calculateLetterGrade(double score) const {
    if (score < 0.0 || score > 100.0) {
        throw std::invalid_argument("Score must be between 0 and 100.");
    }

    return letterGradePolicy->getLetterGrade(score);
}