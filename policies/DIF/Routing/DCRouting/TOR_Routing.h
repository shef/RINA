#pragma once

#include "IntDCRouting.h"


namespace NSPSimpleDC {

    class TOR_Routing : public IntDCRouting {
        void activeNeigh(const DCAddr &dst);
        void inactiveNeigh(const DCAddr &dst);
        void startMyLinks();

        set<DCAddr> getNotOptimalDst(map<DCAddr, tableNode> *  table);

        void printAtEnd();
    };

}
