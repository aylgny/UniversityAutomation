#pragma once

#include "policies/CreditPolicy.h"

class UndergraduateCreditPolicy : public CreditPolicy {
public:
    std::optional<int> getMaxCredits(double gpa) const override;
};