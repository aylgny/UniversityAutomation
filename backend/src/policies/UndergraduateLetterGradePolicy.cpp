#include "policies/UndergraduateLetterGradePolicy.h"

LetterGrade UndergraduateLetterGradePolicy::getLetterGrade(double score) const {
    // Convert the numeric score according to undergraduate grade ranges.
    if (score >= 90.0) return LetterGrade::AA;
    if (score >= 80.0) return LetterGrade::BA;
    if (score >= 70.0) return LetterGrade::BB;
    if (score >= 60.0) return LetterGrade::CB;
    if (score >= 50.0) return LetterGrade::CC;
    if (score >= 45.0) return LetterGrade::DC;
    if (score >= 40.0) return LetterGrade::DD;

    return LetterGrade::F;
}