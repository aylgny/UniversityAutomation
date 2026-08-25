#pragma once

#include "domain/LetterGrade.h"

class PassingPolicy {
private:
    LetterGrade minimumPassingGrade;

public:
    explicit PassingPolicy(LetterGrade minimumPassingGrade);

    bool isPassed(LetterGrade grade) const;

    LetterGrade getMinimumPassingGrade() const;
};