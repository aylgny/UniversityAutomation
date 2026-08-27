#include <gtest/gtest.h>

#include "application/GradingController.h"

#include "domain/UndergraduateStudent.h"
#include "domain/GraduateStudent.h"

#include "domain/UndergraduateCourse.h"
#include "domain/GraduateCourse.h"

#include "domain/Enrollment.h"
#include "domain/LetterGrade.h"


// =========================================================
// GRADING CONTROLLER TESTS
// =========================================================

TEST(
    GradingControllerTest,
    CalculatesWeightedFinalScoreAndLetterGradeForUndergraduateStudent
) {
    UndergraduateStudent student(
        1,
        "Undergraduate Student",
        3.20
    );

    UndergraduateCourse course(
        101,
        "CS301",
        "Software Engineering",
        5
    );

    Enrollment enrollment(
        1,
        &student,
        &course
    );

    GradingController controller;

    controller.configureExams(
        course,
        2
    );

    // Configure exam weights directly without a separate config object.
    controller.configureWeightedAverage(
        course,
        StudentType::UNDERGRADUATE,
        {
            {1, 0.40},
            {2, 0.60}
        }
    );

    controller.enterExamScore(
        enrollment,
        1,
        70.0
    );

    controller.enterExamScore(
        enrollment,
        2,
        90.0
    );

    controller.calculateFinalResult(
        enrollment
    );

    ASSERT_TRUE(
        enrollment.getFinalScore().has_value()
    );

    ASSERT_TRUE(
        enrollment.getLetterGrade().has_value()
    );

    EXPECT_DOUBLE_EQ(
        enrollment.getFinalScore().value(),
        82.0
    );

    EXPECT_EQ(
        enrollment.getLetterGrade().value(),
        LetterGrade::BA
    );
}


TEST(
    GradingControllerTest,
    CalculatesWeightedFinalScoreAndLetterGradeForGraduateStudent
) {
    GraduateStudent student(
        2,
        "Graduate Student",
        3.50
    );

    GraduateCourse course(
        201,
        "CS501",
        "Advanced Software Engineering",
        5
    );

    Enrollment enrollment(
        2,
        &student,
        &course
    );

    GradingController controller;

    controller.configureExams(
        course,
        2
    );

    controller.configureWeightedAverage(
        course,
        StudentType::GRADUATE,
        {
            {1, 0.50},
            {2, 0.50}
        }
    );

    controller.enterExamScore(
        enrollment,
        1,
        90.0
    );

    controller.enterExamScore(
        enrollment,
        2,
        90.0
    );

    controller.calculateFinalResult(
        enrollment
    );

    ASSERT_TRUE(
        enrollment.getFinalScore().has_value()
    );

    ASSERT_TRUE(
        enrollment.getLetterGrade().has_value()
    );

    EXPECT_DOUBLE_EQ(
        enrollment.getFinalScore().value(),
        90.0
    );

    EXPECT_EQ(
        enrollment.getLetterGrade().value(),
        LetterGrade::BA
    );
}


TEST(
    GradingControllerTest,
    CalculatesThresholdResultWhenThresholdIsPassed
) {
    UndergraduateStudent student(
        1,
        "Student",
        3.20
    );

    UndergraduateCourse course(
        101,
        "CS302",
        "Algorithms",
        5
    );

    Enrollment enrollment(
        1,
        &student,
        &course
    );

    GradingController controller;

    controller.configureExams(
        course,
        2
    );

    // Exam 1 is the threshold exam and the threshold value is 50.
    controller.configureThreshold(
        course,
        StudentType::UNDERGRADUATE,
        50.0,
        { 1 }
    );

    controller.enterExamScore(
        enrollment,
        1,
        60.0
    );

    controller.enterExamScore(
        enrollment,
        2,
        85.0
    );

    controller.calculateFinalResult(
        enrollment
    );

    ASSERT_TRUE(
        enrollment.getFinalScore().has_value()
    );

    ASSERT_TRUE(
        enrollment.getLetterGrade().has_value()
    );

    EXPECT_DOUBLE_EQ(
        enrollment.getFinalScore().value(),
        85.0
    );

    EXPECT_EQ(
        enrollment.getLetterGrade().value(),
        LetterGrade::BA
    );
}


TEST(
    GradingControllerTest,
    CalculatesThresholdResultWhenThresholdIsNotPassed
) {
    UndergraduateStudent student(
        1,
        "Student",
        3.20
    );

    UndergraduateCourse course(
        101,
        "CS302",
        "Algorithms",
        5
    );

    Enrollment enrollment(
        1,
        &student,
        &course
    );

    GradingController controller;

    controller.configureExams(
        course,
        2
    );

    controller.configureThreshold(
        course,
        StudentType::UNDERGRADUATE,
        50.0,
        { 1 }
    );

    controller.enterExamScore(
        enrollment,
        1,
        40.0
    );

    controller.enterExamScore(
        enrollment,
        2,
        90.0
    );

    controller.calculateFinalResult(
        enrollment
    );

    ASSERT_TRUE(
        enrollment.getFinalScore().has_value()
    );

    ASSERT_TRUE(
        enrollment.getLetterGrade().has_value()
    );

    EXPECT_DOUBLE_EQ(
        enrollment.getFinalScore().value(),
        40.0
    );

    EXPECT_EQ(
        enrollment.getLetterGrade().value(),
        LetterGrade::DD
    );
}


TEST(
    GradingControllerTest,
    ThrowsWhenGradingPolicyIsNotConfigured
) {
    UndergraduateStudent student(
        1,
        "Student",
        3.20
    );

    UndergraduateCourse course(
        101,
        "CS301",
        "Software Engineering",
        5
    );

    Enrollment enrollment(
        1,
        &student,
        &course
    );

    GradingController controller;

    controller.configureExams(
        course,
        2
    );

    controller.enterExamScore(
        enrollment,
        1,
        70.0
    );

    controller.enterExamScore(
        enrollment,
        2,
        90.0
    );

    EXPECT_THROW(
        controller.calculateFinalResult(
            enrollment
        ),
        std::logic_error
    );
}


TEST(
    GradingControllerTest,
    ThrowsWhenExamIdDoesNotExist
) {
    UndergraduateStudent student(
        1,
        "Student",
        3.20
    );

    UndergraduateCourse course(
        101,
        "CS301",
        "Software Engineering",
        5
    );

    Enrollment enrollment(
        1,
        &student,
        &course
    );

    GradingController controller;

    controller.configureExams(
        course,
        2
    );

    EXPECT_THROW(
        controller.enterExamScore(
            enrollment,
            99,
            80.0
        ),
        std::invalid_argument
    );
}


TEST(
    GradingControllerTest,
    ThrowsWhenExamScoreIsBelowZero
) {
    UndergraduateStudent student(
        1,
        "Student",
        3.20
    );

    UndergraduateCourse course(
        101,
        "CS301",
        "Software Engineering",
        5
    );

    Enrollment enrollment(
        1,
        &student,
        &course
    );

    GradingController controller;

    controller.configureExams(
        course,
        2
    );

    EXPECT_THROW(
        controller.enterExamScore(
            enrollment,
            1,
            -1.0
        ),
        std::invalid_argument
    );
}


TEST(
    GradingControllerTest,
    ThrowsWhenExamScoreIsAboveOneHundred
) {
    UndergraduateStudent student(
        1,
        "Student",
        3.20
    );

    UndergraduateCourse course(
        101,
        "CS301",
        "Software Engineering",
        5
    );

    Enrollment enrollment(
        1,
        &student,
        &course
    );

    GradingController controller;

    controller.configureExams(
        course,
        2
    );

    EXPECT_THROW(
        controller.enterExamScore(
            enrollment,
            1,
            101.0
        ),
        std::invalid_argument
    );
}


TEST(
    GradingControllerTest,
    ThrowsWhenExamCountIsNotPositive
) {
    UndergraduateCourse course(
        101,
        "CS301",
        "Software Engineering",
        5
    );

    GradingController controller;

    EXPECT_THROW(
        controller.configureExams(
            course,
            0
        ),
        std::invalid_argument
    );
}