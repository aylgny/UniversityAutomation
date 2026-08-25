#pragma once

#include "policies/CreditPolicy.h"

class UnlimitedCreditPolicy : public CreditPolicy {
public:
    std::optional<int> getMaxCredits(double gpa) const override;
};