#pragma once

#include <memory>
#include <vector>

#include "domain/StudentType.h"
#include "domain/ExamScore.h"
#include "strategies/GradeCalculationStrategy.h"

class CourseGradingPolicy {
private:
    StudentType studentType;
    std::unique_ptr<GradeCalculationStrategy> strategy;

public:
    explicit CourseGradingPolicy(
        StudentType studentType
    );

    StudentType getStudentType() const;

    void setStrategy(
        std::unique_ptr<GradeCalculationStrategy> strategy
    );

    bool hasStrategy() const;

    const GradeCalculationStrategy* getStrategy() const;

    double calculateGrade(
        const std::vector<ExamScore>& scores
    ) const;
};