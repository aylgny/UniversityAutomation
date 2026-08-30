#include "policies/GraduateCreditPolicy.h"

std::optional<int> GraduateCreditPolicy::getMaxCredits(double gpa) const {
    // Graduate students with GPA above 3.5 may take up to 15 credits.
    if (gpa > 3.5) {
        return 15;
    }

    // Students with GPA between 3.0 and 3.5 may take up to 10 credits.
    if (gpa >= 3.0) {
        return 10;
    }

    // Students with GPA below 3.0 may take up to 6 credits.
    return 6;
}