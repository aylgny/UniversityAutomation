#pragma once

#include <map>

#include "strategies/GradeCalculationStrategy.h"

// Calculates the final grade using exam-specific weights.
class WeightedAverageStrategy : public GradeCalculationStrategy {
private:
    // Maps each exam id to its contribution to the final grade.
    std::map<int, double> weights;

public:
    explicit WeightedAverageStrategy(
        const std::map<int, double>& weights
    );

    double calculate(
        const std::vector<ExamScore>& scores
    ) const override;

    // Used when persisting the grading configuration.
    const std::map<int, double>& getWeights() const;
};