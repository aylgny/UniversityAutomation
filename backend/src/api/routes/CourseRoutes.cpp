/*
 * Registers course-related endpoints.
 * Handles retrieval and serialization of course data.
 */

#include "api/routes/CourseRoutes.h"

#include <httplib.h>
#include <nlohmann/json.hpp>

#include "api/ApiUtils.h"

#include "application/ApplicationState.h"

#include "domain/Course.h"
#include "domain/UndergraduateCourse.h"


namespace api {

    using json = nlohmann::json;


    void registerCourseRoutes(
        httplib::Server& server,
        ApplicationState& state
    ) {

        // =====================================================
        // GET COURSES
        // =====================================================

        server.Get(
            "/api/courses",
            [&state](
                const httplib::Request&,
                httplib::Response& response
                ) {
                    json result =
                        json::array();


                    for (
                        const auto& course :
                        state.getCourses()
                        ) {
                        // Detect the concrete course type.
                        const bool undergraduate =
                            dynamic_cast<
                            const UndergraduateCourse*
                            >(
                                course.get()
                                ) != nullptr;


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
                                    "type",
                                    undergraduate
                                        ? "UNDERGRADUATE"
                                        : "GRADUATE"
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
        );
    }

}