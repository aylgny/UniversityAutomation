/*
 * Declares enrollment-related API routes.
 * Uses RegistrationController for enrollment operations.
 */

#pragma once

#include <string>


namespace httplib {
    class Server;
}


class ApplicationState;
class RegistrationController;


namespace api {

    // Registers enrollment routes.
    void registerEnrollmentRoutes(
        httplib::Server& server,
        ApplicationState& state,
        RegistrationController& registrationController,
        const std::string& dataFile
    );

}