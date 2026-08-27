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
};