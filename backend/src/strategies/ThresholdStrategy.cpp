#include "strategies/ThresholdStrategy.h"

#include <stdexcept>

#include "domain/Exam.h"

ThresholdStrategy::ThresholdStrategy(
    double threshold,
    const std::vector<int>& thresholdExamIds
)
    : threshold(threshold),
    thresholdExamIds(thresholdExamIds) {

    if (threshold < 0.0 || threshold > 100.0) {
        throw std::invalid_argument(
            "Threshold must be between 0 and 100."
        );
    }

    if (thresholdExamIds.empty()) {
        throw std::invalid_argument(
            "At least one threshold exam must be specified."
        );
    }
}

double ThresholdStrategy::calculate(
    const std::vector<ExamScore>& scores
) const {

    if (scores.empty()) {
        throw std::invalid_argument(
            "Exam scores cannot be empty."
        );
    }

    double thresholdTotal = 0.0;
    int thresholdCount = 0;

    double otherTotal = 0.0;
    int otherCount = 0;

    for (const auto& examScore : scores) {
        const Exam* exam = examScore.getExam();

        bool isThresholdExam = false;

        // Separate threshold exams from the remaining exams.
        for (int examId : thresholdExamIds) {
            if (exam->getId() == examId) {
                isThresholdExam = true;
                break;
            }
        }

        if (isThresholdExam) {
            thresholdTotal += examScore.getScore();
            ++thresholdCount;
        }
        else {
            otherTotal += examScore.getScore();
            ++otherCount;
        }
    }

    if (thresholdCount == 0) {
        throw std::invalid_argument(
            "No threshold exam score was found."
        );
    }

    const double thresholdAverage =
        thresholdTotal / thresholdCount;

    // If the threshold is not met, other exam scores are ignored.
    if (thresholdAverage < threshold) {
        return thresholdAverage;
    }

    if (otherCount == 0) {
        throw std::invalid_argument(
            "No non-threshold exam score was found."
        );
    }

    // Once the threshold is satisfied, use the average of the other exams.
    return otherTotal / otherCount;
}

bool ThresholdStrategy::forcesFailure(
    const std::vector<ExamScore>& scores
) const {

    if (scores.empty()) {
        throw std::invalid_argument(
            "Exam scores cannot be empty."
        );
    }

    double thresholdTotal = 0.0;
    int thresholdCount = 0;

    for (const auto& examScore : scores) {
        const Exam* exam =
            examScore.getExam();

        for (int examId : thresholdExamIds) {
            if (exam->getId() == examId) {
                thresholdTotal +=
                    examScore.getScore();

                ++thresholdCount;

                break;
            }
        }
    }

    if (thresholdCount == 0) {
        throw std::invalid_argument(
            "No threshold exam score was found."
        );
    }

    const double thresholdAverage =
        thresholdTotal / thresholdCount;

    return thresholdAverage < threshold;
}

double ThresholdStrategy::getThreshold() const {
    return threshold;
}


const std::vector<int>&
ThresholdStrategy::getThresholdExamIds() const {
    return thresholdExamIds;
}