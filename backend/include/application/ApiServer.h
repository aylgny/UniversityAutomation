#pragma once

class ApplicationState;

// Exposes application use cases through a REST/JSON API.
class ApiServer {
private:
    ApplicationState& state;

public:
    explicit ApiServer(
        ApplicationState& state
    );

    void run(
        int port = 8080
    );
};