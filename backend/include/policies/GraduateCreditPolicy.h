#pragma once

#include "policies/CreditPolicy.h"

// Credit policy for graduate students.
class GraduateCreditPolicy : public CreditPolicy {
public:
    std::optional<int> getMaxCredits(double gpa) const override;
};