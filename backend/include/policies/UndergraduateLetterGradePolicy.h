#pragma once

#include "policies/LetterGradePolicy.h"

// Letter grade policy for undergraduate students.
class UndergraduateLetterGradePolicy : public LetterGradePolicy {
public:
    LetterGrade getLetterGrade(double score) const override;
};