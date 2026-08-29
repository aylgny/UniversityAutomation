/*
 * Registers enrollment-related endpoints.
 * Handles course registration and enrollment retrieval.
 * Business rules are delegated to RegistrationController.
 */

#include "api/routes/EnrollmentRoutes.h"

#include <httplib.h>
#include <nlohmann/json.hpp>

#include "api/ApiUtils.h"

#include "application/ApplicationState.h"
#include "application/RegistrationController.h"

#include "domain/Student.h"
#include "domain/Course.h"
#include "domain/Enrollment.h"


namespace api {

    using json = nlohmann::json;


    void registerEnrollmentRoutes(
        httplib::Server& server,
        ApplicationState& state,
        RegistrationController& registrationController,
        const std::string& dataFile
    ) {

        // =====================================================
        // CREATE ENROLLMENT
        // =====================================================

        server.Post(
            "/api/enrollments",
            [
                &state,
                &registrationController,
                &dataFile
            ](
                const httplib::Request& request,
                httplib::Response& response
                ) {
                    try {
                        // Parse request body.
                        const json body =
                            json::parse(
                                request.body
                            );


                        const int studentId =
                            body.at("studentId")
                            .get<int>();

                        const int courseId =
                            body.at("courseId")
                            .get<int>();


                        // Find related domain objects.
                        Student* student =
                            state.findStudentById(
                                studentId
                            );

                        Course* course =
                            state.findCourseById(
                                courseId
                            );


                        if (
                            student ==
                            nullptr
                            ) {
                            sendError(
                                response,
                                "Student not found.",
                                404
                            );

                            return;
                        }


                        if (
                            course ==
                            nullptr
                            ) {
                            sendError(
                                response,
                                "Course not found.",
                                404
                            );

                            return;
                        }


                        // Delegate enrollment rules to the controller.
                        Enrollment* enrollment =
                            registrationController
                            .enroll(
                                *student,
                                *course,
                                state.getEnrollments()
                            );


                        // Persist the updated state.
                        state.saveToFile(
                            dataFile
                        );


                        sendJson(
                            response,
                            {
                                {"success", true},
                                {
                                    "enrollment",
                                    enrollmentToJson(
                                        *enrollment
                                    )
                                }
                            },
                            201
                        );
                    }
                    catch (
                        const std::logic_error& exception
                        ) {
                        // Business rule conflict.
                        sendError(
                            response,
                            exception.what(),
                            409
                        );
                    }
                    catch (
                        const std::exception& exception
                        ) {
                        // Invalid request or JSON.
                        sendError(
                            response,
                            exception.what(),
                            400
                        );
                    }
            }
        );


        // =====================================================
        // GET STUDENT ENROLLMENTS
        // =====================================================

        server.Get(
            R"(/api/students/(\d+)/enrollments)",
            [&state](
                const httplib::Request& request,
                httplib::Response& response
                ) {
                    try {
                        // Read student ID from the URL.
                        const int studentId =
                            std::stoi(
                                request.matches[1]
                                .str()
                            );


                        Student* student =
                            state.findStudentById(
                                studentId
                            );


                        if (
                            student ==
                            nullptr
                            ) {
                            sendError(
                                response,
                                "Student not found.",
                                404
                            );

                            return;
                        }


                        json result =
                            json::array();


                        // Collect this student's enrollments.
                        for (
                            const auto& enrollment :
                            state.getEnrollments()
                            ) {
                            if (
                                enrollment
                                ->getStudent()
                                ->getId() ==
                                studentId
                                ) {
                                result.push_back(
                                    enrollmentToJson(
                                        *enrollment
                                    )
                                );
                            }
                        }


                        sendJson(
                            response,
                            result
                        );
                    }
                    catch (
                        const std::exception& exception
                        ) {
                        sendError(
                            response,
                            exception.what(),
                            400
                        );
                    }
            }
        );


        // =====================================================
        // GET ENROLLMENT
        // =====================================================

        server.Get(
            R"(/api/enrollments/(\d+))",
            [&state](
                const httplib::Request& request,
                httplib::Response& response
                ) {
                    try {
                        // Read enrollment ID from the URL.
                        const int enrollmentId =
                            std::stoi(
                                request.matches[1]
                                .str()
                            );


                        Enrollment* enrollment =
                            state.findEnrollmentById(
                                enrollmentId
                            );


                        if (
                            enrollment ==
                            nullptr
                            ) {
                            sendError(
                                response,
                                "Enrollment not found.",
                                404
                            );

                            return;
                        }


                        sendJson(
                            response,
                            enrollmentToJson(
                                *enrollment
                            )
                        );
                    }
                    catch (
                        const std::exception& exception
                        ) {
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