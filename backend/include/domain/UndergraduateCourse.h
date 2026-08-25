#pragma once

#include "domain/Course.h"

class UndergraduateCourse : public Course {
public:
    UndergraduateCourse(
        int id,
        const std::string& code,
        const std::string& name,
        int credits
    );
};