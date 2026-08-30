#include "policies/CourseGradingPolicy.h"

#include <stdexcept>
#include <utility>

CourseGradingPolicy::CourseGradingPolicy(
    StudentType studentType
)
    : studentType(studentType),
    strategy(nullptr) {
}

StudentType CourseGradingPolicy::getStudentType() const {
    return studentType;
}

void CourseGradingPolicy::setStrategy(
    std::unique_ptr<GradeCalculationStrategy> strategy
) {
    if (!strategy) {
        throw std::invalid_argument(
            "Grade calculation strategy cannot be null."
        );
    }

    // Transfer ownership of the new strategy to this policy.
    this->strategy = std::move(strategy);
}

bool CourseGradingPolicy::hasStrategy() const {
    return strategy != nullptr;
}

const GradeCalculationStrategy*
CourseGradingPolicy::getStrategy() const {
    return strategy.get();
}

double CourseGradingPolicy::calculateGrade(
    const std::vector<ExamScore>& scores
) const {
    // Grade calculation is not possible until a strategy is configured.
    if (!strategy) {
        throw std::logic_error(
            "Grading strategy has not been configured."
        );
    }

    // Delegate the calculation algorithm to the selected strategy.
    return strategy->calculate(scores);
}