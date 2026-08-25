#pragma once

#include <optional>

class CreditPolicy {
public:
    virtual ~CreditPolicy() = default;

    virtual std::optional<int> getMaxCredits(double gpa) const = 0;
};