#include "domain/Exam.h"

Exam::Exam(int id, const std::string& name)
    : id(id), name(name) {
}

int Exam::getId() const {
    return id;
}

const std::string& Exam::getName() const {
    return name;
}