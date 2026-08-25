#pragma once

class Exam;

class ExamScore {
private:
    const Exam* exam;
    double score;

public:
    ExamScore(const Exam* exam, double score);

    const Exam* getExam() const;
    double getScore() const;

    void setScore(double score);
};