#ifndef PBNJ_SUBJECT_H
#define PBNJ_SUBJECT_H

#include <pbnj.h>
#include <string>

namespace pbnj {
    class Subject {
        public:
            Subject() { ID = createID(); }
            virtual ~Subject() {};
            virtual OSPObject asOSPRayObject() = 0;
            std::string ID;
    };
}

#endif
