#pragma once

#include "policies/CreditPolicy.h"

// Credit policy for undergraduate students.
class UndergraduateCreditPolicy : public CreditPolicy {
public:
    std::optional<int> getMaxCredits(double gpa) const override;
};