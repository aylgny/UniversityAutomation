#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "domain/UndergraduateStudent.h"
#include "domain/UndergraduateCourse.h"
#include "domain/Enrollment.h"
#include "domain/LetterGrade.h"

#include "application/RegistrationController.h"
#include "application/GradingController.h"
#include "application/WeightedAverageConfig.h"
#include "application/ThresholdConfig.h"


std::string letterGradeToString(LetterGrade grade) {
    switch (grade) {
    case LetterGrade::AA: return "AA";
    case LetterGrade::BA: return "BA";
    case LetterGrade::BB: return "BB";
    case LetterGrade::CB: return "CB";
    case LetterGrade::CC: return "CC";
    case LetterGrade::DC: return "DC";
    case LetterGrade::DD: return "DD";
    case LetterGrade::F:  return "F";
    }

    return "UNKNOWN";
}


int main() {
    try {
        UndergraduateStudent student(
            1,
            "Aylin",
            3.20
        );

        RegistrationController registrationController;
        GradingController gradingController;


        // =====================================================
        // DEMO 1 - WEIGHTED AVERAGE STRATEGY
        // =====================================================

        std::cout
            << "--- Weighted Average Strategy Demo ---"
            << std::endl;

        UndergraduateCourse weightedCourse(
            101,
            "CS301",
            "Software Engineering",
            5
        );

        std::vector<Enrollment> weightedEnrollments;

        Enrollment* weightedEnrollment =
            registrationController.enroll(
                student,
                weightedCourse,
                weightedEnrollments
            );

        std::cout
            << "Enrollment successful."
            << std::endl;

        gradingController.configureExams(
            weightedCourse,
            2
        );

        WeightedAverageConfig weightedConfig;

        weightedConfig.weights = {
            {1, 0.40},
            {2, 0.60}
        };

        gradingController.configureGradingMethod(
            weightedCourse,
            StudentType::UNDERGRADUATE,
            weightedConfig
        );

        gradingController.enterExamScore(
            *weightedEnrollment,
            1,
            70.0
        );

        gradingController.enterExamScore(
            *weightedEnrollment,
            2,
            90.0
        );

        gradingController.calculateFinalResult(
            *weightedEnrollment
        );

        const double weightedFinalScore =
            weightedEnrollment
            ->getFinalScore()
            .value();

        const LetterGrade weightedLetterGrade =
            weightedEnrollment
            ->getLetterGrade()
            .value();

        std::cout
            << "Final Score: "
            << weightedFinalScore
            << std::endl;

        std::cout
            << "Letter Grade: "
            << letterGradeToString(
                weightedLetterGrade
            )
            << std::endl;

        std::cout
            << "Passed: "
            << (
                weightedCourse.isPassed(
                    weightedLetterGrade
                )
                ? "Yes"
                : "No"
                )
            << std::endl;


        // =====================================================
        // DEMO 2 - THRESHOLD STRATEGY
        // =====================================================

        std::cout
            << "\n--- Threshold Strategy Demo ---"
            << std::endl;

        UndergraduateCourse thresholdCourse(
            102,
            "CS302",
            "Algorithms",
            5
        );

        std::vector<Enrollment> thresholdEnrollments;

        Enrollment* thresholdEnrollment =
            registrationController.enroll(
                student,
                thresholdCourse,
                thresholdEnrollments
            );

        std::cout
            << "Enrollment successful."
            << std::endl;

        gradingController.configureExams(
            thresholdCourse,
            2
        );

        ThresholdConfig thresholdConfig;

        thresholdConfig.threshold = 50.0;
        thresholdConfig.thresholdExamIds = { 1 };

        gradingController.configureGradingMethod(
            thresholdCourse,
            StudentType::UNDERGRADUATE,
            thresholdConfig
        );

        // Exam 1 is the threshold exam.
        gradingController.enterExamScore(
            *thresholdEnrollment,
            1,
            60.0
        );

        // Since threshold is passed,
        // this exam determines the final score.
        gradingController.enterExamScore(
            *thresholdEnrollment,
            2,
            85.0
        );

        gradingController.calculateFinalResult(
            *thresholdEnrollment
        );

        const double thresholdFinalScore =
            thresholdEnrollment
            ->getFinalScore()
            .value();

        const LetterGrade thresholdLetterGrade =
            thresholdEnrollment
            ->getLetterGrade()
            .value();

        std::cout
            << "Final Score: "
            << thresholdFinalScore
            << std::endl;

        std::cout
            << "Letter Grade: "
            << letterGradeToString(
                thresholdLetterGrade
            )
            << std::endl;

        std::cout
            << "Passed: "
            << (
                thresholdCourse.isPassed(
                    thresholdLetterGrade
                )
                ? "Yes"
                : "No"
                )
            << std::endl;
    }
    catch (const std::exception& exception) {
        std::cerr
            << "Error: "
            << exception.what()
            << std::endl;

        return 1;
    }

    return 0;
}