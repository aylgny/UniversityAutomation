#pragma once

#include <map>

#include "strategies/GradeCalculationStrategy.h"

class WeightedAverageStrategy : public GradeCalculationStrategy {
private:
    std::map<int, double> weights;

public:
    explicit WeightedAverageStrategy(
        const std::map<int, double>& weights
    );

    double calculate(
        const std::vector<ExamScore>& scores
    ) const override;
};