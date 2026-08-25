#pragma once

#include "domain/Student.h"

class UndergraduateStudent : public Student {
public:
    UndergraduateStudent(
        int id,
        const std::string& name,
        double gpa
    );

    StudentType getStudentType() const override;
};