#include <gtest/gtest.h>

#include <vector>

#include "application/RegistrationController.h"

#include "domain/UndergraduateStudent.h"
#include "domain/GraduateStudent.h"

#include "domain/UndergraduateCourse.h"
#include "domain/GraduateCourse.h"

#include "domain/Enrollment.h"


// =========================================================
// REGISTRATION CONTROLLER TESTS
// =========================================================

TEST(RegistrationControllerTest, EnrollsStudentWhenCreditLimitIsNotExceeded) {
    UndergraduateStudent student(
        1,
        "Student",
        3.50
    );

    UndergraduateCourse course(
        101,
        "CS101",
        "Introduction to Computer Science",
        5
    );

    std::vector<Enrollment> enrollments;

    RegistrationController controller;

    Enrollment* enrollment =
        controller.enroll(
            student,
            course,
            enrollments
        );

    ASSERT_NE(enrollment, nullptr);

    EXPECT_EQ(
        enrollment->getStudent(),
        &student
    );

    EXPECT_EQ(
        enrollment->getCourse(),
        &course
    );

    EXPECT_EQ(
        enrollments.size(),
        1
    );
}


TEST(RegistrationControllerTest, RejectsDuplicateEnrollment) {
    UndergraduateStudent student(
        1,
        "Student",
        3.50
    );

    UndergraduateCourse course(
        101,
        "CS101",
        "Introduction to Computer Science",
        5
    );

    std::vector<Enrollment> enrollments;

    RegistrationController controller;

    controller.enroll(
        student,
        course,
        enrollments
    );

    EXPECT_THROW(
        controller.enroll(
            student,
            course,
            enrollments
        ),
        std::logic_error
    );

    EXPECT_EQ(
        enrollments.size(),
        1
    );
}


TEST(RegistrationControllerTest, AllowsUndergraduateStudentUpToMaximumCreditLimit) {
    UndergraduateStudent student(
        1,
        "Student",
        2.50
    );

    UndergraduateCourse course1(
        101,
        "CS101",
        "Course 1",
        10
    );

    UndergraduateCourse course2(
        102,
        "CS102",
        "Course 2",
        10
    );

    std::vector<Enrollment> enrollments;

    RegistrationController controller;

    EXPECT_NO_THROW(
        controller.enroll(
            student,
            course1,
            enrollments
        )
    );

    EXPECT_NO_THROW(
        controller.enroll(
            student,
            course2,
            enrollments
        )
    );

    EXPECT_EQ(
        enrollments.size(),
        2
    );
}


TEST(RegistrationControllerTest, RejectsUndergraduateStudentWhenCreditLimitWouldBeExceeded) {
    UndergraduateStudent student(
        1,
        "Student",
        2.50
    );

    UndergraduateCourse course1(
        101,
        "CS101",
        "Course 1",
        10
    );

    UndergraduateCourse course2(
        102,
        "CS102",
        "Course 2",
        10
    );

    UndergraduateCourse course3(
        103,
        "CS103",
        "Course 3",
        5
    );

    std::vector<Enrollment> enrollments;

    RegistrationController controller;

    controller.enroll(
        student,
        course1,
        enrollments
    );

    controller.enroll(
        student,
        course2,
        enrollments
    );

    EXPECT_THROW(
        controller.enroll(
            student,
            course3,
            enrollments
        ),
        std::logic_error
    );

    EXPECT_EQ(
        enrollments.size(),
        2
    );
}


TEST(RegistrationControllerTest, AppliesGraduateCreditLimitCorrectly) {
    GraduateStudent student(
        1,
        "Graduate Student",
        3.20
    );

    GraduateCourse course1(
        201,
        "CS501",
        "Graduate Course 1",
        5
    );

    GraduateCourse course2(
        202,
        "CS502",
        "Graduate Course 2",
        5
    );

    GraduateCourse course3(
        203,
        "CS503",
        "Graduate Course 3",
        5
    );

    std::vector<Enrollment> enrollments;

    RegistrationController controller;

    EXPECT_NO_THROW(
        controller.enroll(
            student,
            course1,
            enrollments
        )
    );

    EXPECT_NO_THROW(
        controller.enroll(
            student,
            course2,
            enrollments
        )
    );

    EXPECT_THROW(
        controller.enroll(
            student,
            course3,
            enrollments
        ),
        std::logic_error
    );

    EXPECT_EQ(
        enrollments.size(),
        2
    );
}


TEST(RegistrationControllerTest, FailedEnrollmentDoesNotModifyEnrollmentCollection) {
    UndergraduateStudent student(
        1,
        "Student",
        1.50
    );

    UndergraduateCourse course1(
        101,
        "CS101",
        "Course 1",
        10
    );

    UndergraduateCourse course2(
        102,
        "CS102",
        "Course 2",
        10
    );

    std::vector<Enrollment> enrollments;

    RegistrationController controller;

    controller.enroll(
        student,
        course1,
        enrollments
    );

    EXPECT_EQ(
        enrollments.size(),
        1
    );

    EXPECT_THROW(
        controller.enroll(
            student,
            course2,
            enrollments
        ),
        std::logic_error
    );

    EXPECT_EQ(
        enrollments.size(),
        1
    );
}