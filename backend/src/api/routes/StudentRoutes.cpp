/*
 * Registers student-related endpoints.
 * Handles retrieval and serialization of student data.
 */

#include "api/routes/StudentRoutes.h"

#include <httplib.h>
#include <nlohmann/json.hpp>

#include "api/ApiUtils.h"

#include "application/ApplicationState.h"

#include "domain/Student.h"


namespace api {

    using json = nlohmann::json;


    void registerStudentRoutes(
        httplib::Server& server,
        ApplicationState& state
    ) {

        // =====================================================
        // GET STUDENTS
        // =====================================================

        server.Get(
            "/api/students",
            [&state](
                const httplib::Request&,
                httplib::Response& response
                ) {
                    json result =
                        json::array();


                    // Serialize all students.
                    for (
                        const auto& student :
                        state.getStudents()
                        ) {
                        result.push_back(
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
                                    "gpa",
                                    student->getGpa()
                                },
                                {
                                    "studentType",
                                    studentTypeToString(
                                        student
                                            ->getStudentType()
                                    )
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