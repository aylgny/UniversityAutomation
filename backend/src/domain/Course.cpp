#include "domain/Course.h"

#include <stdexcept>
#include <utility>

#include "domain/Exam.h"

Course::Course(
    int id,
    const std::string& code,
    const std::string& name,
    int credits,
    PassingPolicy passingPolicy
)
    : id(id),
    code(code),
    name(name),
    credits(credits),
    passingPolicy(std::move(passingPolicy)) {

    // A valid course must have a positive credit value.
    if (credits <= 0) {
        throw std::invalid_argument(
            "Course credits must be positive."
        );
    }
}

Course::~Course() = default;

int Course::getId() const {
    return id;
}

const std::string& Course::getCode() const {
    return code;
}

const std::string& Course::getName() const {
    return name;
}

int Course::getCredits() const {
    return credits;
}

bool Course::isPassed(LetterGrade grade) const {
    return passingPolicy.isPassed(grade);
}

void Course::createExams(int examCount) {
    if (examCount <= 0) {
        throw std::invalid_argument(
            "Exam count must be positive."
        );
    }

    exams.clear();

    for (int i = 1; i <= examCount; ++i) {
        exams.push_back(
            std::make_unique<Exam>(
                i,
                "Exam " + std::to_string(i)
            )
        );
    }
}

const std::vector<std::unique_ptr<Exam>>&
Course::getExams() const {
    return exams;
}

const CourseGradingPolicy*
Course::getGradingPolicy(
    StudentType studentType
) const {

    for (const auto& policy : gradingPolicies) {
        if (policy.getStudentType() == studentType) {
            return &policy;
        }
    }

    return nullptr;
}

CourseGradingPolicy&
Course::getOrCreateGradingPolicy(
    StudentType studentType
) {
    for (auto& policy : gradingPolicies) {
        if (policy.getStudentType() == studentType) {
            return policy;
        }
    }

    // Construct the new policy directly inside the vector.
    gradingPolicies.emplace_back(
        studentType
    );

    return gradingPolicies.back();
}