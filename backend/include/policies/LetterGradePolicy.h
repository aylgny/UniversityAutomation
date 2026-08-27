#pragma once

#include "domain/LetterGrade.h"

// Abstract policy interface for converting a numeric score
// into a letter grade.
class LetterGradePolicy {
public:
    // Virtual destructor enables safe polymorphic destruction.
    virtual ~LetterGradePolicy() = default;

    // Concrete policies define the score-to-letter-grade mapping.
    virtual LetterGrade getLetterGrade(double score) const = 0;
};