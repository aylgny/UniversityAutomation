#pragma once

#include "policies/LetterGradePolicy.h"

// Letter grade policy for graduate students.
class GraduateLetterGradePolicy : public LetterGradePolicy {
public:
    LetterGrade getLetterGrade(double score) const override;
};