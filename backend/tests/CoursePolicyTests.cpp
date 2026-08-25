#include <gtest/gtest.h>

#include <map>
#include <memory>
#include <vector>

#include "domain/UndergraduateCourse.h"
#include "domain/GraduateCourse.h"
#include "domain/Exam.h"
#include "domain/ExamScore.h"
#include "domain/StudentType.h"
#include "domain/LetterGrade.h"

#include "policies/CourseGradingPolicy.h"

#include "strategies/WeightedAverageStrategy.h"


// =========================================================
// PASSING POLICY TESTS
// =========================================================

TEST(PassingPolicyTest, UndergraduateCoursePassesWithDD) {
    UndergraduateCourse course(
        101,
        "CS101",
        "Course",
        5
    );

    EXPECT_TRUE(
        course.isPassed(
            LetterGrade::DD
        )
    );
}


TEST(PassingPolicyTest, UndergraduateCourseFailsWithF) {
    UndergraduateCourse course(
        101,
        "CS101",
        "Course",
        5
    );

    EXPECT_FALSE(
        course.isPassed(
            LetterGrade::F
        )
    );
}


TEST(PassingPolicyTest, GraduateCoursePassesWithCC) {
    GraduateCourse course(
        201,
        "CS501",
        "Graduate Course",
        5
    );

    EXPECT_TRUE(
        course.isPassed(
            LetterGrade::CC
        )
    );
}


TEST(PassingPolicyTest, GraduateCourseFailsWithDC) {
    GraduateCourse course(
        201,
        "CS501",
        "Graduate Course",
        5
    );

    EXPECT_FALSE(
        course.isPassed(
            LetterGrade::DC
        )
    );
}


// =========================================================
// COURSE EXAM TESTS
// =========================================================

TEST(CourseTest, CreatesRequestedNumberOfExams) {
    UndergraduateCourse course(
        101,
        "CS101",
        "Course",
        5
    );

    course.createExams(3);

    ASSERT_EQ(
        course.getExams().size(),
        3
    );

    EXPECT_EQ(
        course.getExams()[0]->getId(),
        1
    );

    EXPECT_EQ(
        course.getExams()[1]->getId(),
        2
    );

    EXPECT_EQ(
        course.getExams()[2]->getId(),
        3
    );
}


TEST(CourseTest, ThrowsWhenExamCountIsZero) {
    UndergraduateCourse course(
        101,
        "CS101",
        "Course",
        5
    );

    EXPECT_THROW(
        course.createExams(0),
        std::invalid_argument
    );
}


// =========================================================
// COURSE GRADING POLICY TESTS
// =========================================================

TEST(CourseGradingPolicyTest, InitiallyHasNoStrategy) {
    CourseGradingPolicy policy(
        StudentType::UNDERGRADUATE
    );

    EXPECT_FALSE(
        policy.hasStrategy()
    );
}


TEST(CourseGradingPolicyTest, ThrowsWhenCalculatingWithoutStrategy) {
    CourseGradingPolicy policy(
        StudentType::UNDERGRADUATE
    );

    const std::vector<ExamScore> scores;

    EXPECT_THROW(
        policy.calculateGrade(scores),
        std::logic_error
    );
}


TEST(CourseGradingPolicyTest, ThrowsWhenSettingNullStrategy) {
    CourseGradingPolicy policy(
        StudentType::UNDERGRADUATE
    );

    EXPECT_THROW(
        policy.setStrategy(nullptr),
        std::invalid_argument
    );
}


TEST(CourseGradingPolicyTest, DelegatesCalculationToConfiguredStrategy) {
    CourseGradingPolicy policy(
        StudentType::UNDERGRADUATE
    );

    std::map<int, double> weights = {
        {1, 0.40},
        {2, 0.60}
    };

    policy.setStrategy(
        std::make_unique<WeightedAverageStrategy>(
            weights
        )
    );

    Exam exam1(
        1,
        "Midterm"
    );

    Exam exam2(
        2,
        "Final"
    );

    std::vector<ExamScore> scores = {
        ExamScore(
            &exam1,
            70.0
        ),

        ExamScore(
            &exam2,
            90.0
        )
    };

    EXPECT_TRUE(
        policy.hasStrategy()
    );

    EXPECT_DOUBLE_EQ(
        policy.calculateGrade(scores),
        82.0
    );
}


// =========================================================
// STUDENT TYPE SPECIFIC GRADING POLICY TESTS
// =========================================================

TEST(CourseTest, CreatesSeparateGradingPoliciesForStudentTypes) {
    UndergraduateCourse course(
        101,
        "CS101",
        "Course",
        5
    );

    CourseGradingPolicy& undergraduatePolicy =
        course.getOrCreateGradingPolicy(
            StudentType::UNDERGRADUATE
        );

    CourseGradingPolicy& graduatePolicy =
        course.getOrCreateGradingPolicy(
            StudentType::GRADUATE
        );

    EXPECT_NE(
        &undergraduatePolicy,
        &graduatePolicy
    );

    EXPECT_EQ(
        undergraduatePolicy.getStudentType(),
        StudentType::UNDERGRADUATE
    );

    EXPECT_EQ(
        graduatePolicy.getStudentType(),
        StudentType::GRADUATE
    );
}


TEST(CourseTest, ReturnsExistingGradingPolicyForSameStudentType) {
    UndergraduateCourse course(
        101,
        "CS101",
        "Course",
        5
    );

    CourseGradingPolicy& first =
        course.getOrCreateGradingPolicy(
            StudentType::UNDERGRADUATE
        );

    CourseGradingPolicy& second =
        course.getOrCreateGradingPolicy(
            StudentType::UNDERGRADUATE
        );

    EXPECT_EQ(
        &first,
        &second
    );
}