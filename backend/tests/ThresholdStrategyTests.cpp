#include <gtest/gtest.h>

#include <vector>

#include "domain/Exam.h"
#include "domain/ExamScore.h"
#include "strategies/ThresholdStrategy.h"


// =========================================================
// THRESHOLD STRATEGY TESTS
// =========================================================

TEST(ThresholdStrategyTest, ReturnsOtherExamAverageWhenThresholdIsPassed) {
    Exam thresholdExam(1, "Threshold Exam");
    Exam otherExam(2, "Other Exam");

    std::vector<ExamScore> scores = {
        ExamScore(&thresholdExam, 60.0),
        ExamScore(&otherExam, 85.0)
    };

    ThresholdStrategy strategy(
        50.0,
        { 1 }
    );

    const double result =
        strategy.calculate(scores);

    EXPECT_DOUBLE_EQ(result, 85.0);
}


TEST(ThresholdStrategyTest, ReturnsThresholdAverageWhenThresholdIsNotPassed) {
    Exam thresholdExam(1, "Threshold Exam");
    Exam otherExam(2, "Other Exam");

    std::vector<ExamScore> scores = {
        ExamScore(&thresholdExam, 40.0),
        ExamScore(&otherExam, 90.0)
    };

    ThresholdStrategy strategy(
        50.0,
        { 1 }
    );

    const double result =
        strategy.calculate(scores);

    EXPECT_DOUBLE_EQ(result, 40.0);
}


TEST(ThresholdStrategyTest, CalculatesAverageOfMultipleThresholdExams) {
    Exam thresholdExam1(1, "Threshold Exam 1");
    Exam thresholdExam2(2, "Threshold Exam 2");
    Exam otherExam(3, "Other Exam");

    std::vector<ExamScore> scores = {
        ExamScore(&thresholdExam1, 40.0),
        ExamScore(&thresholdExam2, 60.0),
        ExamScore(&otherExam, 90.0)
    };

    ThresholdStrategy strategy(
        55.0,
        { 1, 2 }
    );

    const double result =
        strategy.calculate(scores);

    EXPECT_DOUBLE_EQ(result, 50.0);
}


TEST(ThresholdStrategyTest, ReturnsAverageOfOtherExamsWhenThresholdIsPassed) {
    Exam thresholdExam(1, "Threshold Exam");
    Exam otherExam1(2, "Other Exam 1");
    Exam otherExam2(3, "Other Exam 2");

    std::vector<ExamScore> scores = {
        ExamScore(&thresholdExam, 70.0),
        ExamScore(&otherExam1, 80.0),
        ExamScore(&otherExam2, 90.0)
    };

    ThresholdStrategy strategy(
        50.0,
        { 1 }
    );

    const double result =
        strategy.calculate(scores);

    EXPECT_DOUBLE_EQ(result, 85.0);
}


TEST(ThresholdStrategyTest, ThresholdExactlyEqualToLimitIsConsideredPassed) {
    Exam thresholdExam(1, "Threshold Exam");
    Exam otherExam(2, "Other Exam");

    std::vector<ExamScore> scores = {
        ExamScore(&thresholdExam, 50.0),
        ExamScore(&otherExam, 90.0)
    };

    ThresholdStrategy strategy(
        50.0,
        { 1 }
    );

    const double result =
        strategy.calculate(scores);

    EXPECT_DOUBLE_EQ(result, 90.0);
}


TEST(ThresholdStrategyTest, ThrowsWhenThresholdIsBelowZero) {
    EXPECT_THROW(
        ThresholdStrategy strategy(
            -1.0,
            { 1 }
        ),
        std::invalid_argument
    );
}


TEST(ThresholdStrategyTest, ThrowsWhenThresholdIsAboveOneHundred) {
    EXPECT_THROW(
        ThresholdStrategy strategy(
            101.0,
            { 1 }
        ),
        std::invalid_argument
    );
}


TEST(ThresholdStrategyTest, ThrowsWhenThresholdExamListIsEmpty) {
    EXPECT_THROW(
        ThresholdStrategy strategy(
            50.0,
            {}
        ),
        std::invalid_argument
    );
}


TEST(ThresholdStrategyTest, ThrowsWhenScoreListIsEmpty) {
    ThresholdStrategy strategy(
        50.0,
        { 1 }
    );

    const std::vector<ExamScore> scores;

    EXPECT_THROW(
        strategy.calculate(scores),
        std::invalid_argument
    );
}


TEST(ThresholdStrategyTest, ThrowsWhenThresholdExamScoreIsMissing) {
    Exam otherExam(2, "Other Exam");

    std::vector<ExamScore> scores = {
        ExamScore(&otherExam, 85.0)
    };

    ThresholdStrategy strategy(
        50.0,
        { 1 }
    );

    EXPECT_THROW(
        strategy.calculate(scores),
        std::invalid_argument
    );
}


TEST(ThresholdStrategyTest, ThrowsWhenThresholdIsPassedButNoOtherExamExists) {
    Exam thresholdExam(1, "Threshold Exam");

    std::vector<ExamScore> scores = {
        ExamScore(&thresholdExam, 70.0)
    };

    ThresholdStrategy strategy(
        50.0,
        { 1 }
    );

    EXPECT_THROW(
        strategy.calculate(scores),
        std::invalid_argument
    );
}