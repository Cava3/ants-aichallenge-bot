#pragma once

#include "Location.h"

class Food {
    private :
        Location _loc;
        int _predatoryAndId = 0;

    public :
        int id;

        Food(int id, Location loc) {
            this->id = id;
            _loc = loc;
        };

        Location getLocation() {
            return _loc;
        };

        int getPredatoryAntId() {
            return _predatoryAndId;
        };
};
