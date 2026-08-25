#pragma once

#include <memory>
#include <string>
#include <vector>

#include "domain/LetterGrade.h"
#include "domain/StudentType.h"
#include "policies/PassingPolicy.h"

class Exam;
class CourseGradingPolicy;

class Course {
protected:
    int id;
    std::string code;
    std::string name;
    int credits;

    PassingPolicy passingPolicy;

    std::vector<std::unique_ptr<Exam>> exams;
    std::vector<std::unique_ptr<CourseGradingPolicy>> gradingPolicies;

public:
    Course(
        int id,
        const std::string& code,
        const std::string& name,
        int credits,
        PassingPolicy passingPolicy
    );

    virtual ~Course();

    int getId() const;
    const std::string& getCode() const;
    const std::string& getName() const;
    int getCredits() const;

    bool isPassed(LetterGrade grade) const;

    void createExams(int examCount);

    const std::vector<std::unique_ptr<Exam>>& getExams() const;

    CourseGradingPolicy* getGradingPolicy(
        StudentType studentType
    ) const;

    CourseGradingPolicy& getOrCreateGradingPolicy(
        StudentType studentType
    );
};