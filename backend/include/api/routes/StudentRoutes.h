/*
 * Declares student-related API routes.
 * Provides access to student information.
 */

#pragma once


namespace httplib {
    class Server;
}


class ApplicationState;


namespace api {

    // Registers student endpoints on the HTTP server.
    void registerStudentRoutes(
        httplib::Server& server,
        ApplicationState& state
    );

}