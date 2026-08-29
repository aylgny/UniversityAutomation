/*
 * Application entry point.
 * Loads existing data or creates sample data,
 * then starts the REST API server.
 */

#include <filesystem>
#include <iostream>
#include <string>

#include "application/ApplicationState.h"
#include "api/ApiServer.h"


int main() {
    try {

        const std::string dataFile =
            "data.json";

        ApplicationState state;


        // Load existing data if available.
        if (
            std::filesystem::exists(
                dataFile
            )
            ) {
            state.loadFromFile(
                dataFile
            );

            std::cout
                << "Application data loaded from data.json."
                << std::endl;
        }
        else {

            // Create initial sample data.
            state.seedSampleData();

            state.saveToFile(
                dataFile
            );

            std::cout
                << "Sample data initialized and saved to data.json."
                << std::endl;
        }


        // Start the REST API server.
        ApiServer server(
            state
        );

        server.run(
            8080
        );


        return 0;
    }
    catch (
        const std::exception& exception
        ) {

        // Handle startup or runtime errors.
        std::cerr
            << "Error: "
            << exception.what()
            << std::endl;

        return 1;
    }
}