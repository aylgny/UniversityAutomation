#pragma once

#include <optional>

// Abstract policy interface for determining a student's maximum credit limit.
// Concrete implementations define the actual credit rules.
class CreditPolicy {
public:
    // Virtual destructor enables safe polymorphic destruction.
    virtual ~CreditPolicy() = default;

    // Returns the maximum allowed credits for the given GPA.
    // std::nullopt represents an unlimited credit policy.
    virtual std::optional<int> getMaxCredits(double gpa) const = 0;
};