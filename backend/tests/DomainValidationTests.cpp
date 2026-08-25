#include <gtest/gtest.h>

#include "domain/UndergraduateStudent.h"
#include "domain/UndergraduateCourse.h"
#include "domain/Exam.h"
#include "domain/ExamScore.h"
#include "domain/Enrollment.h"
#include "domain/LetterGrade.h"


// =========================================================
// STUDENT VALIDATION TESTS
// =========================================================

TEST(StudentValidationTest, ThrowsWhenGpaIsBelowZero) {
    EXPECT_THROW(
        UndergraduateStudent student(
            1,
            "Student",
            -0.1
        ),
        std::invalid_argument
    );
}


TEST(StudentValidationTest, ThrowsWhenGpaIsAboveFour) {
    EXPECT_THROW(
        UndergraduateStudent student(
            1,
            "Student",
            4.1
        ),
        std::invalid_argument
    );
}


TEST(StudentValidationTest, AcceptsBoundaryGpaValues) {
    EXPECT_NO_THROW(
        UndergraduateStudent student1(
            1,
            "Student 1",
            0.0
        )
    );

    EXPECT_NO_THROW(
        UndergraduateStudent student2(
            2,
            "Student 2",
            4.0
        )
    );
}


TEST(StudentValidationTest, ThrowsWhenLetterGradeScoreIsBelowZero) {
    UndergraduateStudent student(
        1,
        "Student",
        3.0
    );

    EXPECT_THROW(
        student.calculateLetterGrade(-1.0),
        std::invalid_argument
    );
}


TEST(StudentValidationTest, ThrowsWhenLetterGradeScoreIsAboveOneHundred) {
    UndergraduateStudent student(
        1,
        "Student",
        3.0
    );

    EXPECT_THROW(
        student.calculateLetterGrade(101.0),
        std::invalid_argument
    );
}


// =========================================================
// COURSE VALIDATION TESTS
// =========================================================

TEST(CourseValidationTest, ThrowsWhenCreditsAreZero) {
    EXPECT_THROW(
        UndergraduateCourse course(
            101,
            "CS101",
            "Course",
            0
        ),
        std::invalid_argument
    );
}


TEST(CourseValidationTest, ThrowsWhenCreditsAreNegative) {
    EXPECT_THROW(
        UndergraduateCourse course(
            101,
            "CS101",
            "Course",
            -5
        ),
        std::invalid_argument
    );
}


// =========================================================
// EXAM SCORE VALIDATION TESTS
// =========================================================

TEST(ExamScoreValidationTest, ThrowsWhenExamIsNull) {
    EXPECT_THROW(
        ExamScore score(
            nullptr,
            80.0
        ),
        std::invalid_argument
    );
}


TEST(ExamScoreValidationTest, ThrowsWhenScoreIsBelowZero) {
    Exam exam(
        1,
        "Midterm"
    );

    EXPECT_THROW(
        ExamScore score(
            &exam,
            -1.0
        ),
        std::invalid_argument
    );
}


TEST(ExamScoreValidationTest, ThrowsWhenScoreIsAboveOneHundred) {
    Exam exam(
        1,
        "Midterm"
    );

    EXPECT_THROW(
        ExamScore score(
            &exam,
            101.0
        ),
        std::invalid_argument
    );
}


TEST(ExamScoreValidationTest, SetScoreValidatesNewValue) {
    Exam exam(
        1,
        "Midterm"
    );

    ExamScore score(
        &exam,
        75.0
    );

    EXPECT_THROW(
        score.setScore(120.0),
        std::invalid_argument
    );

    EXPECT_DOUBLE_EQ(
        score.getScore(),
        75.0
    );
}


// =========================================================
// ENROLLMENT VALIDATION TESTS
// =========================================================

TEST(EnrollmentValidationTest, ThrowsWhenStudentIsNull) {
    UndergraduateCourse course(
        101,
        "CS101",
        "Course",
        5
    );

    EXPECT_THROW(
        Enrollment enrollment(
            1,
            nullptr,
            &course
        ),
        std::invalid_argument
    );
}


TEST(EnrollmentValidationTest, ThrowsWhenCourseIsNull) {
    UndergraduateStudent student(
        1,
        "Student",
        3.0
    );

    EXPECT_THROW(
        Enrollment enrollment(
            1,
            &student,
            nullptr
        ),
        std::invalid_argument
    );
}


TEST(EnrollmentValidationTest, ThrowsWhenFinalScoreIsBelowZero) {
    UndergraduateStudent student(
        1,
        "Student",
        3.0
    );

    UndergraduateCourse course(
        101,
        "CS101",
        "Course",
        5
    );

    Enrollment enrollment(
        1,
        &student,
        &course
    );

    EXPECT_THROW(
        enrollment.setFinalScore(-1.0),
        std::invalid_argument
    );
}


TEST(EnrollmentValidationTest, ThrowsWhenFinalScoreIsAboveOneHundred) {
    UndergraduateStudent student(
        1,
        "Student",
        3.0
    );

    UndergraduateCourse course(
        101,
        "CS101",
        "Course",
        5
    );

    Enrollment enrollment(
        1,
        &student,
        &course
    );

    EXPECT_THROW(
        enrollment.setFinalScore(101.0),
        std::invalid_argument
    );
}


TEST(EnrollmentValidationTest, UpdatingExistingExamScoreDoesNotCreateDuplicate) {
    UndergraduateStudent student(
        1,
        "Student",
        3.0
    );

    UndergraduateCourse course(
        101,
        "CS101",
        "Course",
        5
    );

    Exam exam(
        1,
        "Midterm"
    );

    Enrollment enrollment(
        1,
        &student,
        &course
    );

    enrollment.setExamScore(
        &exam,
        70.0
    );

    enrollment.setExamScore(
        &exam,
        85.0
    );

    ASSERT_EQ(
        enrollment.getExamScores().size(),
        1
    );

    EXPECT_DOUBLE_EQ(
        enrollment.getExamScores()[0].getScore(),
        85.0
    );
}