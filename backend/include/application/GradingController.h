#pragma once

#include <map>
#include <vector>

#include "domain/StudentType.h"

class Course;
class Enrollment;

// Coordinates grading-related use cases.
// The controller manages the flow, while grading calculations
// are delegated to grading strategies and policies.
class GradingController {
public:
    // Creates the requested number of exams for a course.
    void configureExams(
        Course& course,
        int examCount
    ) const;

    // Configures weighted-average grading for a specific student type.
    void configureWeightedAverage(
        Course& course,
        StudentType studentType,
        const std::map<int, double>& weights
    ) const;

    // Configures threshold-based grading for a specific student type.
    void configureThreshold(
        Course& course,
        StudentType studentType,
        double threshold,
        const std::vector<int>& thresholdExamIds
    ) const;

    // Adds or updates an exam score for an enrollment.
    void enterExamScore(
        Enrollment& enrollment,
        int examId,
        double score
    ) const;

    // Calculates and stores both the numeric final score
    // and the corresponding letter grade.
    void calculateFinalResult(
        Enrollment& enrollment
    ) const;
};