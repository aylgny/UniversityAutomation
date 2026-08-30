#pragma once

#include <vector>

#include "strategies/GradeCalculationStrategy.h"

// Calculates the final grade using a threshold-based grading rule.
class ThresholdStrategy : public GradeCalculationStrategy {
private:
    double threshold;
    // Exams whose average must satisfy the configured threshold.
    std::vector<int> thresholdExamIds;

public:
    ThresholdStrategy(
        double threshold,
        const std::vector<int>& thresholdExamIds
    );

    double calculate(
        const std::vector<ExamScore>& scores
    ) const override;

    double getThreshold() const;

    const std::vector<int>&
        getThresholdExamIds() const;
};