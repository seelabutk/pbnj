#ifndef PBNJ_SUBJECT_H
#define PBNJ_SUBJECT_H

#include <pbnj.h>
#include <string>
#include <vector>

namespace pbnj {
    class Subject {
        public:
            Subject() { ID = createID(); }
            virtual ~Subject() {};
            virtual OSPObject asOSPRayObject() = 0;
            virtual bool isSurface() = 0;
            virtual std::string getRenderType() = 0;
            virtual std::vector<long unsigned int> getBounds()
            {
                return std::vector<long unsigned int> {1, 1, 1};
            }
            std::string ID;
    };
}

#endif
