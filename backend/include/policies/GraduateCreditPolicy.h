#pragma once

#include "policies/CreditPolicy.h"

// Credit policy for graduate students.
// Defines the maximum credit limit according to GPA.
class GraduateCreditPolicy : public CreditPolicy {
public:
    std::optional<int> getMaxCredits(double gpa) const override;
};