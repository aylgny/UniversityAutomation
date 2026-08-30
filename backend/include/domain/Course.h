#pragma once

#include <memory>
#include <string>
#include <vector>

#include "domain/LetterGrade.h"
#include "domain/StudentType.h"
#include "policies/PassingPolicy.h"
#include "policies/CourseGradingPolicy.h"

class Exam;


// Base domain class representing common course data and behavior.
// A Course owns its exams and grading policies.
class Course {
protected:
    int id;
    std::string code;
    std::string name;
    int credits;

    // Defines the minimum letter grade required to pass this course.
    PassingPolicy passingPolicy;

    // Course owns Exam objects.
    std::vector<std::unique_ptr<Exam>> exams;

    /*
     * Stores grading policies for different student types.
     * unique_ptr keeps ownership explicit and policy addresses stable
     * even if the vector reallocates.
     */
    std::vector<std::unique_ptr<CourseGradingPolicy>>
        gradingPolicies;

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

    // Delegates the pass/fail decision to PassingPolicy.
    bool isPassed(
        LetterGrade grade
    ) const;

    // Replaces the current exam set with the requested number of exams.
    void createExams(
        int examCount
    );

    // Provides read-only access to the owned exams.
    const std::vector<std::unique_ptr<Exam>>&
        getExams() const;

    // Returns nullptr when no policy exists for the given student type.
    const CourseGradingPolicy*
        getGradingPolicy(
            StudentType studentType
        ) const;

    // Returns the existing policy or creates one when necessary.
    CourseGradingPolicy&
        getOrCreateGradingPolicy(
            StudentType studentType
        );
};