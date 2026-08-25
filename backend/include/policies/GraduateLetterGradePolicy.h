#pragma once

#include "policies/LetterGradePolicy.h"

class GraduateLetterGradePolicy : public LetterGradePolicy {
public:
    LetterGrade getLetterGrade(double score) const override;
};