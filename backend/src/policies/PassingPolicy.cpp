#include "policies/PassingPolicy.h"

PassingPolicy::PassingPolicy(LetterGrade minimumPassingGrade)
    : minimumPassingGrade(minimumPassingGrade) {
}

LetterGrade PassingPolicy::getMinimumPassingGrade() const {
    return minimumPassingGrade;
}

bool PassingPolicy::isPassed(LetterGrade grade) const {
    return static_cast<int>(grade) <= static_cast<int>(minimumPassingGrade);
}