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

    /*
     * Enrollment owns the ExamScore collection.
     * Existing scores are updated instead of duplicated.
     */
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

    /*
     * Final result can only be calculated after a score
     * has been entered for every Exam in the Course.
     */
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

    /*
     * Numeric final score calculation is delegated
     * to the configured GradeCalculationStrategy.
     */
    const double finalScore =
        gradingPolicy->calculateGrade(
            enrollment.getExamScores()
        );

    /*
     * Letter grade conversion is delegated
     * to the Student's LetterGradePolicy.
     */
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