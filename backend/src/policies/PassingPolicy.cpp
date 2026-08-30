#include "policies/PassingPolicy.h"

PassingPolicy::PassingPolicy(LetterGrade minimumPassingGrade)
    : minimumPassingGrade(minimumPassingGrade) {
}

LetterGrade PassingPolicy::getMinimumPassingGrade() const {
    return minimumPassingGrade;
}

bool PassingPolicy::isPassed(LetterGrade grade) const {
    // LetterGrade values are ordered from best to worst,
    // so lower or equal enum values satisfy the threshold.
    return static_cast<int>(grade) <= static_cast<int>(minimumPassingGrade);
}