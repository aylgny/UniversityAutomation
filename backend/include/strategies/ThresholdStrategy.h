#pragma once

#include <vector>

#include "strategies/GradeCalculationStrategy.h"

class ThresholdStrategy : public GradeCalculationStrategy {
private:
    double threshold;
    std::vector<int> thresholdExamIds;

public:
    ThresholdStrategy(
        double threshold,
        const std::vector<int>& thresholdExamIds
    );

    double calculate(
        const std::vector<ExamScore>& scores
    ) const override;
};