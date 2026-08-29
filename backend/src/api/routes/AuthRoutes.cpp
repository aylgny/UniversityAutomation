/*
 * Registers authentication endpoints.
 * Handles demo student and instructor login requests.
 */

#include "api/routes/AuthRoutes.h"

#include <httplib.h>
#include <nlohmann/json.hpp>

#include "api/ApiUtils.h"

#include "application/ApplicationState.h"

#include "domain/Student.h"
#include "domain/Instructor.h"


namespace api {

    using json = nlohmann::json;


    void registerAuthRoutes(
        httplib::Server& server,
        ApplicationState& state
    ) {

        // =====================================================
        // DEMO LOGIN
        // =====================================================

        server.Post(
            "/api/login",
            [&state](
                const httplib::Request& request,
                httplib::Response& response
                ) {
                    try {
                        // Parse login credentials.
                        const json body =
                            json::parse(
                                request.body
                            );


                        const std::string username =
                            body.at("username")
                            .get<std::string>();

                        const std::string password =
                            body.at("password")
                            .get<std::string>();


                        /*
                         * Demo credentials only.
                         * No JWT or password hashing is used.
                         */


                         // =================================================
                         // UNDERGRADUATE
                         // =================================================

                        if (
                            username == "undergrad" &&
                            password == "1234"
                            ) {
                            Student* student =
                                state.findStudentById(
                                    1
                                );


                            if (
                                student ==
                                nullptr
                                ) {
                                sendError(
                                    response,
                                    "Demo undergraduate student was not found.",
                                    500
                                );

                                return;
                            }


                            sendJson(
                                response,
                                {
                                    {"success", true},
                                    {
                                        "user",
                                        {
                                            {
                                                "id",
                                                student->getId()
                                            },
                                            {
                                                "name",
                                                student->getName()
                                            },
                                            {
                                                "role",
                                                "STUDENT"
                                            },
                                            {
                                                "studentType",
                                                studentTypeToString(
                                                    student
                                                        ->getStudentType()
                                                )
                                            },
                                            {
                                                "gpa",
                                                student->getGpa()
                                            },
                                            {
                                                "maxCredits",
                                                student
                                                    ->getMaxCredits()
                                                    .value()
                                            }
                                        }
                                    }
                                }
                            );

                            return;
                        }


                        // =================================================
                        // GRADUATE
                        // =================================================

                        if (
                            username == "graduate" &&
                            password == "1234"
                            ) {
                            Student* student =
                                state.findStudentById(
                                    4
                                );


                            if (
                                student ==
                                nullptr
                                ) {
                                sendError(
                                    response,
                                    "Demo graduate student was not found.",
                                    500
                                );

                                return;
                            }


                            sendJson(
                                response,
                                {
                                    {"success", true},
                                    {
                                        "user",
                                        {
                                            {
                                                "id",
                                                student->getId()
                                            },
                                            {
                                                "name",
                                                student->getName()
                                            },
                                            {
                                                "role",
                                                "STUDENT"
                                            },
                                            {
                                                "studentType",
                                                studentTypeToString(
                                                    student
                                                        ->getStudentType()
                                                )
                                            },
                                            {
                                                "gpa",
                                                student->getGpa()
                                            },
                                            {
                                                "maxCredits",
                                                student
                                                    ->getMaxCredits()
                                                    .value()
                                            }
                                        }
                                    }
                                }
                            );

                            return;
                        }


                        // =================================================
                        // INSTRUCTOR
                        // =================================================

                        if (
                            username == "instructor" &&
                            password == "1234"
                            ) {
                            Instructor* instructor =
                                state.findInstructorById(
                                    1
                                );


                            if (
                                instructor ==
                                nullptr
                                ) {
                                sendError(
                                    response,
                                    "Demo instructor was not found.",
                                    500
                                );

                                return;
                            }


                            sendJson(
                                response,
                                {
                                    {"success", true},
                                    {
                                        "user",
                                        {
                                            {
                                                "id",
                                                instructor->getId()
                                            },
                                            {
                                                "name",
                                                instructor->getName()
                                            },
                                            {
                                                "role",
                                                "INSTRUCTOR"
                                            }
                                        }
                                    }
                                }
                            );

                            return;
                        }


                        // Reject invalid credentials.
                        sendError(
                            response,
                            "Invalid username or password.",
                            401
                        );
                    }
                    catch (
                        const std::exception& exception
                        ) {
                        // Handle malformed requests.
                        sendError(
                            response,
                            exception.what(),
                            400
                        );
                    }
            }
        );
    }

}