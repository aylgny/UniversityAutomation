/*
 * Declares course-related API routes.
 * Provides access to course information.
 */

#pragma once


namespace httplib {
    class Server;
}


class ApplicationState;


namespace api {

    // Registers course routes.
    void registerCourseRoutes(
        httplib::Server& server,
        ApplicationState& state
    );

}