#include <gtest/gtest.h>

#include <map>
#include <vector>

#include "domain/Exam.h"
#include "domain/ExamScore.h"
#include "strategies/WeightedAverageStrategy.h"


// =========================================================
// WEIGHTED AVERAGE STRATEGY TESTS
// =========================================================

TEST(WeightedAverageStrategyTest, CalculatesWeightedAverageCorrectly) {
    Exam midterm(1, "Midterm");
    Exam finalExam(2, "Final");

    std::vector<ExamScore> scores = {
        ExamScore(&midterm, 70.0),
        ExamScore(&finalExam, 90.0)
    };

    std::map<int, double> weights = {
        {1, 0.40},
        {2, 0.60}
    };

    WeightedAverageStrategy strategy(weights);

    const double result =
        strategy.calculate(scores);

    EXPECT_DOUBLE_EQ(result, 82.0);
}


TEST(WeightedAverageStrategyTest, CalculatesAnotherWeightedAverageCorrectly) {
    Exam exam1(1, "Exam 1");
    Exam exam2(2, "Exam 2");
    Exam exam3(3, "Exam 3");

    std::vector<ExamScore> scores = {
        ExamScore(&exam1, 80.0),
        ExamScore(&exam2, 70.0),
        ExamScore(&exam3, 90.0)
    };

    std::map<int, double> weights = {
        {1, 0.30},
        {2, 0.30},
        {3, 0.40}
    };

    WeightedAverageStrategy strategy(weights);

    const double result =
        strategy.calculate(scores);

    EXPECT_DOUBLE_EQ(result, 81.0);
}


TEST(WeightedAverageStrategyTest, ThrowsWhenWeightsAreEmpty) {
    const std::map<int, double> weights;

    EXPECT_THROW(
        WeightedAverageStrategy strategy(weights),
        std::invalid_argument
    );
}


TEST(WeightedAverageStrategyTest, ThrowsWhenWeightsDoNotSumToOne) {
    const std::map<int, double> weights = {
        {1, 0.40},
        {2, 0.40}
    };

    EXPECT_THROW(
        WeightedAverageStrategy strategy(weights),
        std::invalid_argument
    );
}


TEST(WeightedAverageStrategyTest, ThrowsWhenWeightIsNegative) {
    const std::map<int, double> weights = {
        {1, -0.20},
        {2, 1.20}
    };

    EXPECT_THROW(
        WeightedAverageStrategy strategy(weights),
        std::invalid_argument
    );
}


TEST(WeightedAverageStrategyTest, ThrowsWhenWeightIsGreaterThanOne) {
    const std::map<int, double> weights = {
        {1, 1.20},
        {2, -0.20}
    };

    EXPECT_THROW(
        WeightedAverageStrategy strategy(weights),
        std::invalid_argument
    );
}


TEST(WeightedAverageStrategyTest, ThrowsWhenScoreListIsEmpty) {
    const std::map<int, double> weights = {
        {1, 0.50},
        {2, 0.50}
    };

    WeightedAverageStrategy strategy(weights);

    const std::vector<ExamScore> scores;

    EXPECT_THROW(
        strategy.calculate(scores),
        std::invalid_argument
    );
}


TEST(WeightedAverageStrategyTest, ThrowsWhenWeightIsMissingForExam) {
    Exam midterm(1, "Midterm");
    Exam finalExam(2, "Final");

    std::vector<ExamScore> scores = {
        ExamScore(&midterm, 70.0),
        ExamScore(&finalExam, 90.0)
    };

    const std::map<int, double> weights = {
        {1, 1.0}
    };

    WeightedAverageStrategy strategy(weights);

    EXPECT_THROW(
        strategy.calculate(scores),
        std::invalid_argument
    );
}