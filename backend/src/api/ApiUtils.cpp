/*
 * Implements shared API helper functions.
 * Provides CORS handling, JSON responses and domain serialization.
 */

#include "api/ApiUtils.h"

#include <stdexcept>

#include "domain/Enrollment.h"
#include "domain/Student.h"
#include "domain/Course.h"
#include "domain/ExamScore.h"
#include "domain/Exam.h"


namespace api {


    // =====================================================
    // CORS
    // =====================================================

    void addCorsHeaders(
        httplib::Response& response
    ) {
        response.set_header(
            "Access-Control-Allow-Origin",
            "*"
        );

        response.set_header(
            "Access-Control-Allow-Headers",
            "Content-Type"
        );

        response.set_header(
            "Access-Control-Allow-Methods",
            "GET, POST, PUT, OPTIONS"
        );
    }


    // =====================================================
    // JSON RESPONSE
    // =====================================================

    void sendJson(
        httplib::Response& response,
        const json& body,
        int status
    ) {
        response.status =
            status;

        // Serialize JSON into the response body.
        response.set_content(
            body.dump(),
            "application/json"
        );

        addCorsHeaders(
            response
        );
    }


    // =====================================================
    // ERROR RESPONSE
    // =====================================================

    void sendError(
        httplib::Response& response,
        const std::string& message,
        int status
    ) {
        sendJson(
            response,
            {
                {"success", false},
                {"error", message}
            },
            status
        );
    }


    // =====================================================
    // STUDENT TYPE
    // =====================================================

    std::string studentTypeToString(
        StudentType type
    ) {
        if (
            type ==
            StudentType::UNDERGRADUATE
            ) {
            return "UNDERGRADUATE";
        }

        return "GRADUATE";
    }


    StudentType parseStudentType(
        const std::string& value
    ) {
        if (
            value ==
            "UNDERGRADUATE"
            ) {
            return StudentType::UNDERGRADUATE;
        }

        if (
            value ==
            "GRADUATE"
            ) {
            return StudentType::GRADUATE;
        }

        throw std::invalid_argument(
            "Unsupported student type."
        );
    }


    // =====================================================
    // LETTER GRADE
    // =====================================================

    std::string letterGradeToString(
        LetterGrade grade
    ) {
        switch (grade) {

        case LetterGrade::AA:
            return "AA";

        case LetterGrade::BA:
            return "BA";

        case LetterGrade::BB:
            return "BB";

        case LetterGrade::CB:
            return "CB";

        case LetterGrade::CC:
            return "CC";

        case LetterGrade::DC:
            return "DC";

        case LetterGrade::DD:
            return "DD";

        case LetterGrade::F:
            return "F";
        }

        return "UNKNOWN";
    }


    // =====================================================
    // ENROLLMENT JSON
    // =====================================================

    json enrollmentToJson(
        const Enrollment& enrollment
    ) {
        json result;


        result["id"] =
            enrollment.getId();

        result["studentId"] =
            enrollment
            .getStudent()
            ->getId();

        result["courseId"] =
            enrollment
            .getCourse()
            ->getId();


        // Serialize exam scores.
        result["examScores"] =
            json::array();


        for (
            const auto& examScore :
            enrollment.getExamScores()
            ) {
            result["examScores"]
                .push_back(
                    {
                        {
                            "examId",
                            examScore
                                .getExam()
                                ->getId()
                        },
                        {
                            "score",
                            examScore
                                .getScore()
                        }
                    }
                );
        }


        // Final result may not exist yet.
        if (
            enrollment
            .getFinalScore()
            .has_value()
            ) {
            result["finalScore"] =
                enrollment
                .getFinalScore()
                .value();
        }
        else {
            result["finalScore"] =
                nullptr;
        }


        if (
            enrollment
            .getLetterGrade()
            .has_value()
            ) {
            const LetterGrade grade =
                enrollment
                .getLetterGrade()
                .value();


            result["letterGrade"] =
                letterGradeToString(
                    grade
                );


            // Passing depends on the course policy.
            result["passed"] =
                enrollment
                .getCourse()
                ->isPassed(
                    grade
                );
        }
        else {
            result["letterGrade"] =
                nullptr;

            result["passed"] =
                nullptr;
        }


        return result;
    }

}