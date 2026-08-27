#pragma once

#include "domain/Student.h"

// Concrete student type representing graduate students.
// It selects the graduate-specific policies through its constructor.
class GraduateStudent : public Student {
public:
    GraduateStudent(
        int id,
        const std::string& name,
        double gpa
    );

    StudentType getStudentType() const override;
};