#pragma once

class Exam;

// Represents a score associated with a specific exam.
class ExamScore {
private:
    // Non-owning, read-only reference to the related Exam.
    const Exam* exam;
    double score;

public:
    ExamScore(const Exam* exam, double score);

    const Exam* getExam() const;
    double getScore() const;

    void setScore(double score);
};