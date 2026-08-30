#pragma once

#include "domain/Course.h"

// Concrete course type for graduate-level courses.
// Configures the graduate passing rule through its constructor.
class GraduateCourse : public Course {
public:
    GraduateCourse(
        int id,
        const std::string& code,
        const std::string& name,
        int credits
    );
};