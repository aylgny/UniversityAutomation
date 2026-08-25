#pragma once

#include "domain/Student.h"

class GraduateStudent : public Student {
public:
    GraduateStudent(
        int id,
        const std::string& name,
        double gpa
    );

    StudentType getStudentType() const override;
};