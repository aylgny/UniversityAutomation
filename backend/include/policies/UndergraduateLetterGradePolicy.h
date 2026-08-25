#pragma once

#include "policies/LetterGradePolicy.h"

class UndergraduateLetterGradePolicy : public LetterGradePolicy {
public:
    LetterGrade getLetterGrade(double score) const override;
};