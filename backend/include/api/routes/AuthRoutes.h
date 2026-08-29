/*
 * Declares authentication-related API routes.
 * Currently provides demo login functionality.
 */

#pragma once


namespace httplib {
    class Server;
}


class ApplicationState;


namespace api {

    // Registers authentication routes.
    void registerAuthRoutes(
        httplib::Server& server,
        ApplicationState& state
    );

}