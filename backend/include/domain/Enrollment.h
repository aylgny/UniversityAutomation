#pragma once

#include <optional>
#include <vector>

#include "domain/ExamScore.h"
#include "domain/LetterGrade.h"

class Student;
class Course;
class Exam;

// Represents the relationship between a student and a course.
// Stores data that belongs specifically to this enrollment.
class Enrollment {
private:
    int id;

    // Non-owning references to the related student and course.
    Student* student;
    Course* course;

    // Enrollment owns the exam scores for this student-course relationship.
    std::vector<ExamScore> examScores;

    // Available only after final grading has been completed.
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

    // Updates an existing score or adds a new one for the given exam.
    void setExamScore(const Exam* exam, double score);

    const std::vector<ExamScore>& getExamScores() const;

    std::optional<double> getFinalScore() const;
    void setFinalScore(double score);

    void clearFinalScore();

    std::optional<LetterGrade> getLetterGrade() const;
    void setLetterGrade(LetterGrade grade);
};