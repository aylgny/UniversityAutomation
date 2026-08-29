/*
 * Registers common system endpoints.
 * Handles CORS preflight requests and API health checks.
 */

#include "api/routes/SystemRoutes.h"

#include <httplib.h>

#include "api/ApiUtils.h"


namespace api {

    void registerSystemRoutes(
        httplib::Server& server
    ) {

        // =====================================================
        // CORS PREFLIGHT
        // =====================================================

        server.Options(
            R"(.*)",
            [](
                const httplib::Request&,
                httplib::Response& response
                ) {
                    addCorsHeaders(
                        response
                    );

                    // No response body is required.
                    response.status =
                        204;
            }
        );


        // =====================================================
        // HEALTH CHECK
        // =====================================================

        server.Get(
            "/api/health",
            [](
                const httplib::Request&,
                httplib::Response& response
                ) {
                    sendJson(
                        response,
                        {
                            {"success", true},
                            {"status", "UP"}
                        }
                    );
            }
        );
    }

}