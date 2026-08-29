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
     * Course owns grading policies for different student types.
     *
     * unique_ptr keeps ownership explicit and keeps CourseGradingPolicy
     * objects at stable memory addresses even if the vector reallocates.
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

    // Recreates the exams belonging to this course.
    void createExams(
        int examCount
    );

    // Provides read-only access to the owned exams.
    const std::vector<std::unique_ptr<Exam>>&
        getExams() const;

    /*
     * Returns the grading policy for the given student type,
     * or nullptr if no policy has been configured.
     */
    const CourseGradingPolicy*
        getGradingPolicy(
            StudentType studentType
        ) const;

    /*
     * Returns the existing policy or creates
     * and owns a new one when necessary.
     */
    CourseGradingPolicy&
        getOrCreateGradingPolicy(
            StudentType studentType
        );
};