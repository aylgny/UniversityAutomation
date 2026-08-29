/*
 * Declares the REST API server.
 * Holds the shared application state and starts the HTTP server.
 */

#pragma once

class ApplicationState;


class ApiServer {
public:
    explicit ApiServer(
        ApplicationState& state
    );

    void run(
        int port
    );

private:
    // Shared application data.
    ApplicationState& state;
};