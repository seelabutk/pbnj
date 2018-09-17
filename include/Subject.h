#ifndef PBNJ_SUBJECT_H
#define PBNJ_SUBJECT_H

#include <pbnj.h>
#include <algorithm>
#include <string>
#include <vector>

namespace pbnj {
    class Subject {
        public:
            Subject() { ID = createID(); specular = 0.f; }
            virtual ~Subject() {};
            virtual OSPObject asOSPRayObject() = 0;
            virtual bool isSurface() = 0;
            virtual std::string getRenderType() = 0;
            virtual std::vector<long unsigned int> getBounds()
            {
                return std::vector<long unsigned int> {1, 1, 1};
            }
            virtual void setSpecular(float specular)
            {
                this->specular = std::max(0.f, std::min(1.f, specular));
            }
            std::string ID;
            float specular;
    };
}

#endif
