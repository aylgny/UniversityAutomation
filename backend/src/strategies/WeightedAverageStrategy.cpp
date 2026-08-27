#include "strategies/WeightedAverageStrategy.h"

#include <cmath>
#include <stdexcept>

#include "domain/Exam.h"

WeightedAverageStrategy::WeightedAverageStrategy(
    const std::map<int, double>& weights
)
    : weights(weights) {

    if (weights.empty()) {
        throw std::invalid_argument(
            "Weights cannot be empty."
        );
    }

    double totalWeight = 0.0;

    // Validate each weight and calculate the total.
    for (const auto& [examId, weight] : weights) {
        if (weight < 0.0 || weight > 1.0) {
            throw std::invalid_argument(
                "Each weight must be between 0 and 1."
            );
        }

        totalWeight += weight;
    }

    // Use a tolerance because floating-point values may not sum exactly to 1.0.
    if (std::abs(totalWeight - 1.0) > 0.0001) {
        throw std::invalid_argument(
            "Weights must sum to 1.0."
        );
    }
}

double WeightedAverageStrategy::calculate(
    const std::vector<ExamScore>& scores
) const {

    if (scores.empty()) {
        throw std::invalid_argument(
            "Exam scores cannot be empty."
        );
    }

    double finalScore = 0.0;

    for (const auto& examScore : scores) {
        const Exam* exam = examScore.getExam();

        // Find the configured weight for this exam.
        const auto weightIt = weights.find(exam->getId());

        if (weightIt == weights.end()) {
            throw std::invalid_argument(
                "Weight is missing for an exam."
            );
        }

        finalScore +=
            examScore.getScore() * weightIt->second;
    }

    return finalScore;
}

const std::map<int, double>&
WeightedAverageStrategy::getWeights() const {
    return weights;
}