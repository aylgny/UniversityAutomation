#pragma once

#include <memory>
#include <string>
#include <vector>

#include "domain/LetterGrade.h"
#include "domain/StudentType.h"
#include "policies/PassingPolicy.h"

class Exam;
class CourseGradingPolicy;

// Base domain class representing common course data and behavior.
// A Course owns its exams and student-type-specific grading policies.
class Course {
protected:
    int id;
    std::string code;
    std::string name;
    int credits;

    // Defines the minimum letter grade required to pass this course.
    PassingPolicy passingPolicy;

    // Course owns the Exam objects created for this course.
    std::vector<std::unique_ptr<Exam>> exams;

    // Stores grading configurations for different student types.
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

    // Delegates the pass/fail decision to PassingPolicy.
    bool isPassed(LetterGrade grade) const;

    // Recreates the exams belonging to this course.
    void createExams(int examCount);

    // Provides read-only access to the owned exams.
    const std::vector<std::unique_ptr<Exam>>& getExams() const;

    // Returns the grading policy for the given student type,
    // or nullptr if no policy has been configured.
    CourseGradingPolicy* getGradingPolicy(
        StudentType studentType
    ) const;

    // Returns the existing policy or creates one when necessary.
    CourseGradingPolicy& getOrCreateGradingPolicy(
        StudentType studentType
    );
};