/*
 * Registers instructor-related endpoints.
 * Handles instructor data and assigned course retrieval.
 */

#include "api/routes/InstructorRoutes.h"

#include <httplib.h>
#include <nlohmann/json.hpp>

#include "api/ApiUtils.h"

#include "application/ApplicationState.h"

#include "domain/Instructor.h"
#include "domain/Course.h"


namespace api {

    using json = nlohmann::json;


    void registerInstructorRoutes(
        httplib::Server& server,
        ApplicationState& state
    ) {

        // =====================================================
        // GET INSTRUCTORS
        // =====================================================

        server.Get(
            "/api/instructors",
            [&state](
                const httplib::Request&,
                httplib::Response& response
                ) {
                    json result =
                        json::array();


                    for (
                        const auto& instructor :
                        state.getInstructors()
                        ) {
                        json courseIds =
                            json::array();


                        // Collect assigned course IDs.
                        for (
                            Course* course :
                            instructor->getCourses()
                            ) {
                            if (
                                course !=
                                nullptr
                                ) {
                                courseIds.push_back(
                                    course->getId()
                                );
                            }
                        }


                        result.push_back(
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
                                    "courseIds",
                                    courseIds
                                }
                            }
                        );
                    }


                    sendJson(
                        response,
                        result
                    );
            }
        );


        // =====================================================
        // GET INSTRUCTOR COURSES
        // =====================================================

        server.Get(
            R"(/api/instructors/(\d+)/courses)",
            [&state](
                const httplib::Request& request,
                httplib::Response& response
                ) {
                    try {
                        // Read instructor ID from the URL.
                        const int instructorId =
                            std::stoi(
                                request.matches[1]
                                .str()
                            );


                        Instructor* instructor =
                            state.findInstructorById(
                                instructorId
                            );


                        if (
                            instructor ==
                            nullptr
                            ) {
                            sendError(
                                response,
                                "Instructor not found.",
                                404
                            );

                            return;
                        }


                        json result =
                            json::array();


                        for (
                            Course* course :
                            instructor->getCourses()
                            ) {
                            if (
                                course ==
                                nullptr
                                ) {
                                continue;
                            }


                            result.push_back(
                                {
                                    {
                                        "id",
                                        course->getId()
                                    },
                                    {
                                        "code",
                                        course->getCode()
                                    },
                                    {
                                        "name",
                                        course->getName()
                                    },
                                    {
                                        "credits",
                                        course->getCredits()
                                    },
                                    {
                                        "examCount",
                                        course
                                            ->getExams()
                                            .size()
                                    }
                                }
                            );
                        }


                        sendJson(
                            response,
                            result
                        );
                    }
                    catch (
                        const std::exception& exception
                        ) {
                        // Handle invalid path parameters.
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