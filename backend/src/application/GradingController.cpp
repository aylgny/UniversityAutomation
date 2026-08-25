#include "application/GradingController.h"

#include <memory>
#include <stdexcept>
#include <type_traits>

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
    course.createExams(examCount);
}


void GradingController::configureGradingMethod(
    Course& course,
    StudentType studentType,
    const GradingMethodConfig& config
) const {

    CourseGradingPolicy& gradingPolicy =
        course.getOrCreateGradingPolicy(studentType);

    std::visit(
        [&gradingPolicy](const auto& concreteConfig) {

            using ConfigType =
                std::decay_t<decltype(concreteConfig)>;

            if constexpr (
                std::is_same_v<
                ConfigType,
                WeightedAverageConfig
                >
                ) {
                gradingPolicy.setStrategy(
                    std::make_unique<
                    WeightedAverageStrategy
                    >(concreteConfig.weights)
                );
            }

            else if constexpr (
                std::is_same_v<
                ConfigType,
                ThresholdConfig
                >
                ) {
                gradingPolicy.setStrategy(
                    std::make_unique<
                    ThresholdStrategy
                    >(
                        concreteConfig.threshold,
                        concreteConfig.thresholdExamIds
                    )
                );
            }
        },
        config
    );
}


void GradingController::enterExamScore(
    Enrollment& enrollment,
    int examId,
    double score
) const {

    Course* course = enrollment.getCourse();

    if (course == nullptr) {
        throw std::logic_error(
            "Enrollment does not reference a course."
        );
    }

    const Exam* selectedExam = nullptr;

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

    CourseGradingPolicy* gradingPolicy =
        course->getGradingPolicy(studentType);

    if (gradingPolicy == nullptr) {
        throw std::logic_error(
            "Grading policy has not been configured."
        );
    }

    const double finalScore =
        gradingPolicy->calculateGrade(
            enrollment.getExamScores()
        );

    const LetterGrade letterGrade =
        student->calculateLetterGrade(
            finalScore
        );

    enrollment.setFinalScore(finalScore);
    enrollment.setLetterGrade(letterGrade);
}