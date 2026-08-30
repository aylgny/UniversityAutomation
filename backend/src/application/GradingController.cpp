#include "application/GradingController.h"

#include <memory>
#include <stdexcept>

#include "domain/Course.h"
#include "domain/Enrollment.h"
#include "domain/Exam.h"
#include "domain/Student.h"

#include "policies/CourseGradingPolicy.h"

#include "strategies/WeightedAverageStrategy.h"
#include "strategies/ThresholdStrategy.h"


void GradingController::configureExams(
    Course& course,
    int examCount
) const {

    if (examCount <= 0) {
        throw std::invalid_argument(
            "Exam count must be positive."
        );
    }

    /*
     * Do not recreate Exam objects when the requested
     * configuration is already active.
     *
     * ExamScore keeps pointers to Exam objects, so preserving
     * the existing Exam instances also preserves those pointers.
     */
    if (
        static_cast<int>(
            course
            .getExams()
            .size()
            ) == examCount
        ) {
        return;
    }

    course.createExams(
        examCount
    );
}


void GradingController::configureWeightedAverage(
    Course& course,
    StudentType studentType,
    const std::map<int, double>& weights
) const {

    /*
     * A Course may use a different grading strategy
     * for each StudentType.
     */
    CourseGradingPolicy& gradingPolicy =
        course.getOrCreateGradingPolicy(
            studentType
        );

    gradingPolicy.setStrategy(
        std::make_unique<WeightedAverageStrategy>(
            weights
        )
    );
}


void GradingController::configureThreshold(
    Course& course,
    StudentType studentType,
    double threshold,
    const std::vector<int>& thresholdExamIds
) const {

    CourseGradingPolicy& gradingPolicy =
        course.getOrCreateGradingPolicy(
            studentType
        );

    gradingPolicy.setStrategy(
        std::make_unique<ThresholdStrategy>(
            threshold,
            thresholdExamIds
        )
    );
}


void GradingController::enterExamScore(
    Enrollment& enrollment,
    int examId,
    double score
) const {

    Course* course =
        enrollment.getCourse();

    if (course == nullptr) {
        throw std::logic_error(
            "Enrollment does not reference a course."
        );
    }

    const Exam* selectedExam =
        nullptr;

    /*
     * Find the Exam belonging to the Course
     * referenced by the Enrollment.
     */
    for (
        const auto& exam :
        course->getExams()
        ) {
        if (
            exam->getId() ==
            examId
            ) {
            selectedExam =
                exam.get();

            break;
        }
    }

    if (
        selectedExam ==
        nullptr
        ) {
        throw std::invalid_argument(
            "Exam not found in this course."
        );
    }

    // Enrollment updates an existing score or creates a new one.
    enrollment.setExamScore(
        selectedExam,
        score
    );
}


void GradingController::calculateFinalResult(
    Enrollment& enrollment
) const {

    Student* student =
        enrollment.getStudent();

    Course* course =
        enrollment.getCourse();

    if (
        student == nullptr ||
        course == nullptr
        ) {
        throw std::logic_error(
            "Enrollment is incomplete."
        );
    }

    // Every configured exam must have a score before final grading.
    if (
        enrollment
        .getExamScores()
        .size() !=
        course
        ->getExams()
        .size()
        ) {
        throw std::logic_error(
            "All exam scores must be entered before calculating the final result."
        );
    }

    const StudentType studentType =
        student->getStudentType();

    const CourseGradingPolicy* gradingPolicy =
        course->getGradingPolicy(
            studentType
        );

    if (
        gradingPolicy ==
        nullptr
        ) {
        throw std::logic_error(
            "Grading policy has not been configured."
        );
    }

    const GradeCalculationStrategy* strategy =
        gradingPolicy->getStrategy();

    if (
        strategy ==
        nullptr
        ) {
        throw std::logic_error(
            "Grading strategy has not been configured."
        );
    }

    /*
     * If the active grading strategy defines a mandatory
     * threshold and the student fails that threshold,
     * the result is directly F.
     *
     * In this case no numeric final score is defined.
     */
    if (
        strategy->forcesFailure(
            enrollment.getExamScores()
        )
        ) {

        enrollment.clearFinalScore();

        enrollment.setLetterGrade(
            LetterGrade::F
        );

        return;
    }

    /*
     * If no forced failure exists, calculate the normal
     * numeric final score and convert it to a letter grade
     * according to the student's grading policy.
     */
    const double finalScore =
        gradingPolicy->calculateGrade(
            enrollment.getExamScores()
        );

    const LetterGrade letterGrade =
        student->calculateLetterGrade(
            finalScore
        );

    enrollment.setFinalScore(
        finalScore
    );

    enrollment.setLetterGrade(
        letterGrade
    );
}