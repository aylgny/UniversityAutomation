/*
 * Declares instructor-related API routes.
 * Provides instructor and assigned course information.
 */

#pragma once


namespace httplib {
    class Server;
}


class ApplicationState;


namespace api {

    // Registers instructor routes.
    void registerInstructorRoutes(
        httplib::Server& server,
        ApplicationState& state
    );

}