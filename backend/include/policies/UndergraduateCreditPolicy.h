#pragma once

#include "policies/CreditPolicy.h"

// Credit policy for undergraduate students.
// Defines the maximum credit limit according to GPA.
class UndergraduateCreditPolicy : public CreditPolicy {
public:
    std::optional<int> getMaxCredits(double gpa) const override;
};