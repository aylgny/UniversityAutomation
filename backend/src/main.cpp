#include <filesystem>
#include <iostream>
#include <string>

#include "application/ApplicationState.h"
#include "application/ApiServer.h"


int main() {
    try {

        const std::string dataFile =
            "data.json";

        ApplicationState state;


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

            state.seedSampleData();

            state.saveToFile(
                dataFile
            );

            std::cout
                << "Sample data initialized and saved to data.json."
                << std::endl;
        }


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

        std::cerr
            << "Error: "
            << exception.what()
            << std::endl;

        return 1;
    }
}