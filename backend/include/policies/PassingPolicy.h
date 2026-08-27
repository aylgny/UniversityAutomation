#pragma once

#include "domain/LetterGrade.h"

// Defines the minimum letter grade required to pass a course.
class PassingPolicy {
private:
    LetterGrade minimumPassingGrade;

public:
    // explicit prevents unintended implicit conversion from LetterGrade
    // to PassingPolicy through this single-argument constructor.
    explicit PassingPolicy(LetterGrade minimumPassingGrade);

    // Checks whether the given letter grade satisfies the passing threshold.
    bool isPassed(LetterGrade grade) const;

    LetterGrade getMinimumPassingGrade() const;
};