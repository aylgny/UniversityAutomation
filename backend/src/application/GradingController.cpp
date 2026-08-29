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

    // Do not recreate Exam objects when the configuration
    // has not actually changed. Existing ExamScore pointers
    // remain valid.
    if (
        static_cast<int>(
            course.getExams().size()
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

    // A course may use a different grading strategy
    // for undergraduate and graduate students.
    CourseGradingPolicy& gradingPolicy =
        course.getOrCreateGradingPolicy(studentType);

    // The grading policy owns the selected strategy.
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
        course.getOrCreateGradingPolicy(studentType);

    // Create the threshold strategy using the instructor's configuration
    // and transfer its ownership to the grading policy.
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

    const Exam* selectedExam = nullptr;

    // Find the exam that belongs to the enrollment's course.
    for (const auto& exam : course->getExams()) {
        if (exam->getId() == examId) {
            selectedExam = exam.get();
            break;
        }
    }

    if (selectedExam == nullptr) {
        throw std::invalid_argument(
            "Exam not found in this course."
        );
    }

    // Enrollment owns the ExamScore and updates it if it already exists.
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

    if (student == nullptr ||
        course == nullptr) {
        throw std::logic_error(
            "Enrollment is incomplete."
        );
    }

    const StudentType studentType =
        student->getStudentType();

    // Select the grading policy configured for this student type.
    const CourseGradingPolicy* gradingPolicy =
        course->getGradingPolicy(studentType);

    if (gradingPolicy == nullptr) {
        throw std::logic_error(
            "Grading policy has not been configured."
        );
    }

    // Numeric grade calculation is delegated to the selected strategy.
    const double finalScore =
        gradingPolicy->calculateGrade(
            enrollment.getExamScores()
        );

    // Letter-grade conversion is delegated to the student's
    // LetterGradePolicy.
    const LetterGrade letterGrade =
        student->calculateLetterGrade(
            finalScore
        );

    enrollment.setFinalScore(finalScore);
    enrollment.setLetterGrade(letterGrade);
}