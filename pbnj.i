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

enum CONFSTATE {ERROR_NODATA, ERROR_MULTISET, SINGLE_NOVAR, SINGLE_VAR,
                MULTI_NOVAR, MULTI_VAR};

class Configuration {
public:
  Configuration(std::string filename);
  ~Configuration();

  CONFSTATE getConfigState();

  std::string configFilename;

  std::string dataFilename;
  std::vector<std::string> globbedFilenames;
  std::string dataVariable;
  int dataXDim;
  int dataYDim;
  int dataZDim;

  int imageWidth;
  int imageHeight;
  std::string imageFilename;
  std::vector<unsigned char> bgColor;

  std::vector<float> colorMap;
  std::vector<float> opacityMap;
  float opacityAttenuation;

  unsigned int samples;

  float cameraX;
  float cameraY;
  float cameraZ;

  float cameraUpX;
  float cameraUpY;
  float cameraUpZ;

  std::vector<float> isosurfaceValues;
};

class Camera {
public:
  Camera(int width, int height);
  ~Camera();

  void setPosition(float x, float y, float z);
  void setUpVector(float x, float y, float z);
  void setOrbitRadius(float radius);
  void centerView();

  OSPCamera asOSPRayObject();

  int imageWidth;
  int imageHeight;

  std::string ID;
};

class ConfigReader {
 public:
  ConfigReader();
  ~ConfigReader();

  void parseConfigFile(std::string filename,
                       rapidjson::Document& config);
};

enum FILETYPE {UNKNOWN, BINARY, NETCDF};


class DataFile {

 public:
  DataFile(int x, int y, int z);
  ~DataFile();

  void loadFromFile(std::string filename, std::string variable="",
                    bool memmap=false);
  void calculateStatistics();
  void printStatistics();

  // experimental
  void bin(unsigned int num_bins);

  std::string filename;
  FILETYPE filetype;

  int xDim;
  int yDim;
  int zDim;
  long int numValues;

  float minVal; // these should
  float maxVal; //
  float avgVal; // eventually be
  float stdDev; //
  float *data;  // template types

  bool statsCalculated;
};


enum IMAGETYPE {INVALID, PIXMAP, PNG};


class Renderer {
 public:
  Renderer();
  ~Renderer();

  void setBackgroundColor(unsigned char r, unsigned char g, unsigned char b);
  void setBackgroundColor(std::vector<unsigned char> bgColor);
  void setVolume(pbnj::Volume *v);
  void setIsosurface(pbnj::Volume *v, std::vector<float> &isoValues);
  void setCamera(pbnj::Camera *c);
  void setSamples(unsigned int spp);

  void render();
  void renderToBuffer(unsigned char **buffer);
  void renderToPNGObject(std::vector<unsigned char> &png);
  void renderImage(std::string imageFilename);

  int cameraWidth;
  int cameraHeight;
};


class TimeSeries {

 public:
  TimeSeries(std::vector<std::string> filenames, int x, int y, int z);
  TimeSeries(std::vector<std::string> filenames, std::string varname,
             int x, int y, int z);
  ~TimeSeries();

  pbnj::Volume *getVolume(unsigned int index);
  int getVolumeIndex(std::string filename);
  unsigned int getLength();
  void setMaxMemory(unsigned int gigabytes);

  // attributes for volumes to receive when loaded
  std::vector<float> colorMap;
  std::vector<float> opacityMap;
  float opacityAttenuation;
  bool doMemoryMap;

  void setColorMap(std::vector<float> &map);
  void setOpacityMap(std::vector<float> &map);
  void setOpacityAttenuation(float attenuation);
  void setMemoryMapping(bool toMMap);
};

class Volume {

 public:
  //Volume(DataFile *df);
  Volume(std::string filename, int x, int y, int z,
         bool memmap=false);
  Volume(std::string filename, std::string var_name, int x, int y,
         int z, bool memmap=false);
  ~Volume();

  void attenuateOpacity(float amount);
  void setColorMap(std::vector<float> &map);
  void setOpacityMap(std::vector<float> &map);
  std::vector<int> getBounds();
  OSPVolume asOSPRayObject();

  std::string ID;
};

};
