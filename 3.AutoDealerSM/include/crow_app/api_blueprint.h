#ifndef API_BLUEPRINT_H
#define API_BLUEPRINT_H

#include <crow.h>
#include "crow_app/middleware.h"

class ApiBlueprint {
public:
    static void register_blueprints(crow::App& app);

}

#endif
