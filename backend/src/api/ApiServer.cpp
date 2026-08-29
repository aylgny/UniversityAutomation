/*
 * Creates and starts the REST API server.
 * Registers route groups and connects them to application controllers.
 */

#include "api/ApiServer.h"

#include <iostream>
#include <string>

#include <httplib.h>

#include "application/ApplicationState.h"
#include "application/RegistrationController.h"
#include "application/GradingController.h"

#include "api/routes/SystemRoutes.h"
#include "api/routes/AuthRoutes.h"
#include "api/routes/StudentRoutes.h"
#include "api/routes/CourseRoutes.h"
#include "api/routes/InstructorRoutes.h"
#include "api/routes/EnrollmentRoutes.h"
#include "api/routes/GradingRoutes.h"


namespace {

    // File used for persistent application state.
    const std::string DATA_FILE =
        "data.json";

}


ApiServer::ApiServer(
    ApplicationState& state
)
    : state(state) {
}


void ApiServer::run(
    int port
) {
    // Create the HTTP server.
    httplib::Server server;


    // Application controllers handle use-case logic.
    RegistrationController
        registrationController;

    GradingController
        gradingController;


    // =====================================================
    // REGISTER ROUTES
    // =====================================================

    api::registerSystemRoutes(
        server
    );


    api::registerAuthRoutes(
        server,
        state
    );


    api::registerStudentRoutes(
        server,
        state
    );


    api::registerCourseRoutes(
        server,
        state
    );


    api::registerInstructorRoutes(
        server,
        state
    );


    api::registerEnrollmentRoutes(
        server,
        state,
        registrationController,
        DATA_FILE
    );


    api::registerGradingRoutes(
        server,
        state,
        gradingController,
        DATA_FILE
    );


    // =====================================================
    // START SERVER
    // =====================================================

    std::cout
        << "University Automation REST API running on port "
        << port
        << std::endl;


    // Accept requests on all available network interfaces.
    server.listen(
        "0.0.0.0",
        port
    );
}