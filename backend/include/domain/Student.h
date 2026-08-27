#pragma once

#include <memory>
#include <optional>

#include "domain/Person.h"
#include "domain/StudentType.h"
#include "domain/LetterGrade.h"
#include "policies/CreditPolicy.h"
#include "policies/LetterGradePolicy.h"


// Abstract base class for all student types.
// Stores common student data and delegates changeable business rules
// to credit and letter grade policy objects.
class Student : public Person {
protected:
    double gpa;


    // Student owns these policy objects.
    // They encapsulate changeable credit limit and letter grade rules.
    std::unique_ptr<CreditPolicy> creditPolicy;
    std::unique_ptr<LetterGradePolicy> letterGradePolicy;

public:
    Student(
        int id,
        const std::string& name,
        double gpa,
        std::unique_ptr<CreditPolicy> creditPolicy,
        std::unique_ptr<LetterGradePolicy> letterGradePolicy
    );

    // Student is used as a polymorphic base class.
    virtual ~Student() = default;

    double getGpa() const;

    // Implemented by concrete student types such as
    // UndergraduateStudent and GraduateStudent.
    virtual StudentType getStudentType() const = 0;


    // Returns no value when the applied credit policy defines no limit.
    std::optional<int> getMaxCredits() const;

    // Delegates score-to-letter-grade conversion to LetterGradePolicy.
    LetterGrade calculateLetterGrade(double score) const;
};