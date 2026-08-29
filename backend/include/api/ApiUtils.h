/*
 * Declares common API helper functions.
 * Handles JSON responses, errors, enum conversion and serialization.
 */

#pragma once

#include <string>

#include <httplib.h>
#include <nlohmann/json.hpp>

#include "domain/StudentType.h"
#include "domain/LetterGrade.h"


class Enrollment;


namespace api {

    using json = nlohmann::json;


    // Adds common CORS headers.
    void addCorsHeaders(
        httplib::Response& response
    );


    // Sends a JSON response.
    void sendJson(
        httplib::Response& response,
        const json& body,
        int status = 200
    );


    // Sends a standard error response.
    void sendError(
        httplib::Response& response,
        const std::string& message,
        int status
    );


    // Converts StudentType to API text.
    std::string studentTypeToString(
        StudentType type
    );


    // Converts API text to StudentType.
    StudentType parseStudentType(
        const std::string& value
    );


    // Converts LetterGrade to API text.
    std::string letterGradeToString(
        LetterGrade grade
    );


    // Serializes an Enrollment object.
    json enrollmentToJson(
        const Enrollment& enrollment
    );

}