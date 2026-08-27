#pragma once

#include "domain/Course.h"

// Concrete course type for undergraduate-level courses.
class UndergraduateCourse : public Course {
public:
    UndergraduateCourse(
        int id,
        const std::string& code,
        const std::string& name,
        int credits
    );
};