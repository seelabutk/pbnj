#ifndef PBNJ_SUBJECT_H
#define PBNJ_SUBJECT_H

#include <pbnj.h>

namespace pbnj {
    class Subject {
        public:
            virtual OSPObject asOSPRayObject() = 0;
            std::string ID;
    };
}

#endif
