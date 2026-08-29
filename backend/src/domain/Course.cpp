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

    if (id <= 0) {
        throw std::invalid_argument(
            "Course id must be positive."
        );
    }

    if (code.empty()) {
        throw std::invalid_argument(
            "Course code cannot be empty."
        );
    }

    if (name.empty()) {
        throw std::invalid_argument(
            "Course name cannot be empty."
        );
    }

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


bool Course::isPassed(
    LetterGrade grade
) const {
    return passingPolicy.isPassed(
        grade
    );
}


void Course::createExams(
    int examCount
) {
    if (examCount <= 0) {
        throw std::invalid_argument(
            "Exam count must be positive."
        );
    }

    exams.clear();

    for (
        int i = 1;
        i <= examCount;
        ++i
        ) {
        exams.push_back(
            std::make_unique<Exam>(
                i,
                "Exam " +
                std::to_string(i)
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

    for (
        const auto& policy :
        gradingPolicies
        ) {
        if (
            policy->getStudentType() ==
            studentType
            ) {
            return policy.get();
        }
    }

    return nullptr;
}


CourseGradingPolicy&
Course::getOrCreateGradingPolicy(
    StudentType studentType
) {

    for (
        const auto& policy :
        gradingPolicies
        ) {
        if (
            policy->getStudentType() ==
            studentType
            ) {
            return *policy;
        }
    }

    /*
     * Course owns the policy through unique_ptr.
     * The actual CourseGradingPolicy object keeps a stable address
     * even if the vector itself reallocates.
     */
    gradingPolicies.push_back(
        std::make_unique<CourseGradingPolicy>(
            studentType
        )
    );

    return *gradingPolicies.back();
}