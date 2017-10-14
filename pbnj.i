%module pbnj

%include <std_string.i>
%include <std_vector.i>

%{
#include "pbnj.h"
#include "Configuration.h"
#include "Camera.h"
#include "ConfigReader.h"
#include "DataFile.h"
#include "Renderer.h"
#include "TimeSeries.h"
#include "TransferFunction.h"
#include "Volume.h"

namespace pbnj {
void init(void) {
  int argc;
  const char *argv[1];
  const std::string dummy = "dummy";

  argc = 1;
  argv[0] = dummy.c_str();

  pbnj::pbnjInit(&argc, argv);
}
};

%}

namespace pbnj {

void init(void);

};

%include "include/Configuration.h"
%include "include/Camera.h"
%include "include/ConfigReader.h"
%include "include/DataFile.h"
%include "include/Renderer.h"
%include "include/TimeSeries.h"
%include "include/TransferFunction.h"
%include "include/Volume.h"
