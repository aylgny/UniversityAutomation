#pragma once

#include <vector>

#include "domain/ExamScore.h"


// Strategy interface for calculating a final numeric grade
// from a collection of exam scores.
class GradeCalculationStrategy {
public:
    virtual ~GradeCalculationStrategy() = default;

    // Concrete strategies implement their own calculation algorithm.
    virtual double calculate(
        const std::vector<ExamScore>& scores
    ) const = 0;

    // Indicates whether the grading rule forces the student to fail.
    // Most strategies do not have such a rule.
    virtual bool forcesFailure(
        const std::vector<ExamScore>& scores
    ) const {
        return false;
    }
};