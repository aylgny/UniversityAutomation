#include "domain/ExamScore.h"

#include <stdexcept>

ExamScore::ExamScore(const Exam* exam, double score)
    : exam(exam), score(score) {

    if (exam == nullptr) {
        throw std::invalid_argument("Exam cannot be null.");
    }

    if (score < 0.0 || score > 100.0) {
        throw std::invalid_argument("Score must be between 0 and 100.");
    }
}

const Exam* ExamScore::getExam() const {
    return exam;
}

double ExamScore::getScore() const {
    return score;
}

void ExamScore::setScore(double score) {
    if (score < 0.0 || score > 100.0) {
        throw std::invalid_argument("Score must be between 0 and 100.");
    }

    // Update the stored score after validation.
    this->score = score;
}