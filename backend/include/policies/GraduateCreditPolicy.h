#pragma once

#include "policies/CreditPolicy.h"

class GraduateCreditPolicy : public CreditPolicy {
public:
    std::optional<int> getMaxCredits(double gpa) const override;
};