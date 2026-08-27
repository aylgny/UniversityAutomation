#pragma once

#include "domain/Student.h"

// Concrete student type representing undergraduate students.
// It selects the undergraduate-specific policies through its constructor.

class UndergraduateStudent : public Student {
public:
    UndergraduateStudent(
        int id,
        const std::string& name,
        double gpa
    );

    // Returns the concrete student category.
    StudentType getStudentType() const override;
};