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

    if (thresholdAverage < threshold) {
        return thresholdAverage;
    }

    if (otherCount == 0) {
        throw std::invalid_argument(
            "No non-threshold exam score was found."
        );
    }

    return otherTotal / otherCount;
}