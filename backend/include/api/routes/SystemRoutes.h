/*
 * Declares system-level API routes.
 * Includes CORS preflight and health-check endpoints.
 */

#pragma once


namespace httplib {
    class Server;
}


namespace api {

    // Registers system-level routes.
    void registerSystemRoutes(
        httplib::Server& server
    );

}