#pragma once

#include <memory>
#include <optional>

#include "domain/Person.h"
#include "domain/StudentType.h"
#include "domain/LetterGrade.h"
#include "policies/CreditPolicy.h"
#include "policies/LetterGradePolicy.h"

class Student : public Person {
protected:
    double gpa;

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

    virtual ~Student() = default;

    double getGpa() const;

    virtual StudentType getStudentType() const = 0;

    std::optional<int> getMaxCredits() const;

    LetterGrade calculateLetterGrade(double score) const;
};