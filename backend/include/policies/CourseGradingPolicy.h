#pragma once

#include <memory>
#include <vector>

#include "domain/StudentType.h"
#include "domain/ExamScore.h"
#include "strategies/GradeCalculationStrategy.h"

// Stores the grading configuration for a specific student type.
// Delegates final grade calculation to a configurable strategy.
class CourseGradingPolicy {
private:
    StudentType studentType;
    // Owns the currently configured grade calculation strategy.
    std::unique_ptr<GradeCalculationStrategy> strategy;

public:
    explicit CourseGradingPolicy(
        StudentType studentType
    );

    StudentType getStudentType() const;

    // Replaces the current grading strategy.
    void setStrategy(
        std::unique_ptr<GradeCalculationStrategy> strategy
    );

    bool hasStrategy() const;

    // Returns a non-owning pointer to the configured strategy.
    const GradeCalculationStrategy* getStrategy() const;

    // Delegates grade calculation to the configured strategy.
    double calculateGrade(
        const std::vector<ExamScore>& scores
    ) const;
};