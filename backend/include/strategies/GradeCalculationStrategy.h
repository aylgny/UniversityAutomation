#pragma once

#include <vector>

#include "domain/ExamScore.h"

class GradeCalculationStrategy {
public:
    virtual ~GradeCalculationStrategy() = default;

    virtual double calculate(
        const std::vector<ExamScore>& scores
    ) const = 0;
};