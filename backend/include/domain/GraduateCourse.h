#pragma once

#include "domain/Course.h"

// Concrete course type for graduate-level courses.
class GraduateCourse : public Course {
public:
    GraduateCourse(
        int id,
        const std::string& code,
        const std::string& name,
        int credits
    );
};