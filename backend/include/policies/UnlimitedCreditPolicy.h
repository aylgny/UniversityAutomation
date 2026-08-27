#pragma once

#include "policies/CreditPolicy.h"

// Credit policy representing the absence of a maximum credit limit.
class UnlimitedCreditPolicy : public CreditPolicy {
public:
    // Returns no value because this policy does not impose a credit limit.
    std::optional<int> getMaxCredits(double gpa) const override;
};