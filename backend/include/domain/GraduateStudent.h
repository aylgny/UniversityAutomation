#pragma once

#include "domain/Student.h"

// Concrete student type for graduate students.
// Configures graduate-specific policies through its constructor.
class GraduateStudent : public Student {
public:
    GraduateStudent(
        int id,
        const std::string& name,
        double gpa
    );

    // Returns the concrete student category.
    StudentType getStudentType() const override;
};