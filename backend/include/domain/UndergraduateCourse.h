#pragma once

#include "domain/Course.h"

// Concrete course type for undergraduate-level courses.
// Configures the undergraduate passing rule through its constructor.
class UndergraduateCourse : public Course {
public:
    UndergraduateCourse(
        int id,
        const std::string& code,
        const std::string& name,
        int credits
    );
};