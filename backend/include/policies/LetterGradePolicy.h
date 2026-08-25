#pragma once

#include "domain/LetterGrade.h"

class LetterGradePolicy {
public:
    virtual ~LetterGradePolicy() = default;

    virtual LetterGrade getLetterGrade(double score) const = 0;
};