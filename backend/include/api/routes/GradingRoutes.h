/*
 * Declares grading-related API routes.
 * Uses GradingController for grading operations.
 */

#pragma once

#include <string>


namespace httplib {
    class Server;
}


class ApplicationState;
class GradingController;


namespace api {

    // Registers grading-related endpoints on the HTTP server.
    void registerGradingRoutes(
        httplib::Server& server,
        ApplicationState& state,
        GradingController& gradingController,
        const std::string& dataFile
    );

}