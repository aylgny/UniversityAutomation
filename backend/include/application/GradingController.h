#pragma once

#include "application/GradingMethodConfig.h"
#include "domain/StudentType.h"

class Course;
class Enrollment;

class GradingController {
public:
    void configureExams(
        Course& course,
        int examCount
    ) const;

    void configureGradingMethod(
        Course& course,
        StudentType studentType,
        const GradingMethodConfig& config
    ) const;

    void enterExamScore(
        Enrollment& enrollment,
        int examId,
        double score
    ) const;

    void calculateFinalResult(
        Enrollment& enrollment
    ) const;
};