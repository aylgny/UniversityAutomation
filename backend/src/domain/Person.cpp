#include "domain/Person.h"

#include <stdexcept>


Person::Person(
    int id,
    const std::string& name
)
    : id(id),
    name(name) {

    if (id <= 0) {
        throw std::invalid_argument(
            "Person id must be positive."
        );
    }

    if (name.empty()) {
        throw std::invalid_argument(
            "Person name cannot be empty."
        );
    }
}


int Person::getId() const {
    return id;
}


const std::string& Person::getName() const {
    return name;
}