#pragma once

#include <optional>
#include <vector>

#include "domain/ExamScore.h"
#include "domain/LetterGrade.h"

class Student;
class Course;
class Exam;

class Enrollment {
private:
    int id;

    Student* student;
    Course* course;

    std::vector<ExamScore> examScores;

    std::optional<double> finalScore;
    std::optional<LetterGrade> letterGrade;

public:
    Enrollment(
        int id,
        Student* student,
        Course* course
    );

    int getId() const;

    Student* getStudent() const;
    Course* getCourse() const;

    void setExamScore(const Exam* exam, double score);

    const std::vector<ExamScore>& getExamScores() const;

    std::optional<double> getFinalScore() const;
    void setFinalScore(double score);

    std::optional<LetterGrade> getLetterGrade() const;
    void setLetterGrade(LetterGrade grade);
};