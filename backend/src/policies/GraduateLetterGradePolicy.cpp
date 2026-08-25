#include "policies/GraduateLetterGradePolicy.h"

LetterGrade GraduateLetterGradePolicy::getLetterGrade(double score) const {
    if (score >= 95.0) return LetterGrade::AA;
    if (score >= 90.0) return LetterGrade::BA;
    if (score >= 80.0) return LetterGrade::BB;
    if (score >= 70.0) return LetterGrade::CB;
    if (score >= 60.0) return LetterGrade::CC;

    return LetterGrade::F;
}