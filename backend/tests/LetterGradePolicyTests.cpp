#include <gtest/gtest.h>

#include "domain/LetterGrade.h"
#include "policies/UndergraduateLetterGradePolicy.h"
#include "policies/GraduateLetterGradePolicy.h"


// =========================================================
// UNDERGRADUATE LETTER GRADE POLICY TESTS
// =========================================================

TEST(UndergraduateLetterGradePolicyTest, Score90ReturnsAA) {
    UndergraduateLetterGradePolicy policy;

    EXPECT_EQ(
        policy.getLetterGrade(90.0),
        LetterGrade::AA
    );
}


TEST(UndergraduateLetterGradePolicyTest, ScoreJustBelow90ReturnsBA) {
    UndergraduateLetterGradePolicy policy;

    EXPECT_EQ(
        policy.getLetterGrade(89.99),
        LetterGrade::BA
    );
}


TEST(UndergraduateLetterGradePolicyTest, Score80ReturnsBA) {
    UndergraduateLetterGradePolicy policy;

    EXPECT_EQ(
        policy.getLetterGrade(80.0),
        LetterGrade::BA
    );
}


TEST(UndergraduateLetterGradePolicyTest, ScoreJustBelow80ReturnsBB) {
    UndergraduateLetterGradePolicy policy;

    EXPECT_EQ(
        policy.getLetterGrade(79.99),
        LetterGrade::BB
    );
}


TEST(UndergraduateLetterGradePolicyTest, Score70ReturnsBB) {
    UndergraduateLetterGradePolicy policy;

    EXPECT_EQ(
        policy.getLetterGrade(70.0),
        LetterGrade::BB
    );
}


TEST(UndergraduateLetterGradePolicyTest, Score60ReturnsCB) {
    UndergraduateLetterGradePolicy policy;

    EXPECT_EQ(
        policy.getLetterGrade(60.0),
        LetterGrade::CB
    );
}


TEST(UndergraduateLetterGradePolicyTest, Score50ReturnsCC) {
    UndergraduateLetterGradePolicy policy;

    EXPECT_EQ(
        policy.getLetterGrade(50.0),
        LetterGrade::CC
    );
}


TEST(UndergraduateLetterGradePolicyTest, Score45ReturnsDC) {
    UndergraduateLetterGradePolicy policy;

    EXPECT_EQ(
        policy.getLetterGrade(45.0),
        LetterGrade::DC
    );
}


TEST(UndergraduateLetterGradePolicyTest, Score40ReturnsDD) {
    UndergraduateLetterGradePolicy policy;

    EXPECT_EQ(
        policy.getLetterGrade(40.0),
        LetterGrade::DD
    );
}


TEST(UndergraduateLetterGradePolicyTest, ScoreJustBelow40ReturnsF) {
    UndergraduateLetterGradePolicy policy;

    EXPECT_EQ(
        policy.getLetterGrade(39.99),
        LetterGrade::F
    );
}


// =========================================================
// GRADUATE LETTER GRADE POLICY TESTS
// =========================================================

TEST(GraduateLetterGradePolicyTest, Score95ReturnsAA) {
    GraduateLetterGradePolicy policy;

    EXPECT_EQ(
        policy.getLetterGrade(95.0),
        LetterGrade::AA
    );
}


TEST(GraduateLetterGradePolicyTest, ScoreJustBelow95ReturnsBA) {
    GraduateLetterGradePolicy policy;

    EXPECT_EQ(
        policy.getLetterGrade(94.99),
        LetterGrade::BA
    );
}


TEST(GraduateLetterGradePolicyTest, Score90ReturnsBA) {
    GraduateLetterGradePolicy policy;

    EXPECT_EQ(
        policy.getLetterGrade(90.0),
        LetterGrade::BA
    );
}


TEST(GraduateLetterGradePolicyTest, ScoreJustBelow90ReturnsBB) {
    GraduateLetterGradePolicy policy;

    EXPECT_EQ(
        policy.getLetterGrade(89.99),
        LetterGrade::BB
    );
}


TEST(GraduateLetterGradePolicyTest, Score80ReturnsBB) {
    GraduateLetterGradePolicy policy;

    EXPECT_EQ(
        policy.getLetterGrade(80.0),
        LetterGrade::BB
    );
}


TEST(GraduateLetterGradePolicyTest, Score70ReturnsCB) {
    GraduateLetterGradePolicy policy;

    EXPECT_EQ(
        policy.getLetterGrade(70.0),
        LetterGrade::CB
    );
}


TEST(GraduateLetterGradePolicyTest, Score60ReturnsCC) {
    GraduateLetterGradePolicy policy;

    EXPECT_EQ(
        policy.getLetterGrade(60.0),
        LetterGrade::CC
    );
}


TEST(GraduateLetterGradePolicyTest, ScoreJustBelow60ReturnsF) {
    GraduateLetterGradePolicy policy;

    EXPECT_EQ(
        policy.getLetterGrade(59.99),
        LetterGrade::F
    );
}