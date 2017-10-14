FROM ubuntu:latest AS rapidjson

RUN apt-get update && \
    apt-get install -y \
            # build-essential $(: all : compile tool) \
            # Compilation tool for C/C++ programs
            cmake $(: all : compile tool) \
            # python $(: pistache : running tests) \
            # libtbb-dev $(: embree ospray : threading framework) \
            # libglu1-mesa-dev $(: embree ospray : OpenGL requirements) \
            # freeglut3-dev $(: embree ospray : OpenGL requirements) \
            # mesa-common-dev $(: embree ospray : OpenGL requirements) \
            # libc6-dev $(: enchiladas : pthreads) \
    && \
    rm -rf /var/lib/apt/lists/*

RUN apt-get update && \
    apt-get install -y \
            build-essential $(: all : compile tool) \
            # python $(: pistache : running tests) \
            # libtbb-dev $(: embree ospray : threading framework) \
            # libglu1-mesa-dev $(: embree ospray : OpenGL requirements) \
            # freeglut3-dev $(: embree ospray : OpenGL requirements) \
            # mesa-common-dev $(: embree ospray : OpenGL requirements) \
            # libc6-dev $(: enchiladas : pthreads) \
    && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /opt/
COPY rapidjson /opt/rapidjson
WORKDIR /opt/rapidjson/build/
RUN true && \
    cmake .. \
          -DRAPIDJSON_BUILD_EXAMPLES:BOOL=OFF \
          -DRAPIDJSON_BUILD_TESTS:BOOL=OFF \
    && \
    make && \
    make install DESTDIR=/stage

FROM ubuntu:latest AS ispc
WORKDIR /opt/
ADD ispc-v1.9.1-linux.tar.gz /opt/
RUN mv ispc-v1.9.1-linux ispc
WORKDIR /opt/ispc/
RUN mkdir -p /stage/usr/bin && \
    cp /opt/ispc/ispc /stage/usr/bin/

FROM ubuntu:latest AS embree
WORKDIR /opt/
ADD embree-2.16.4.x86_64.linux.tar.gz /opt/
RUN mv embree-2.16.4.x86_64.linux embree
WORKDIR /opt/embree/

FROM ubuntu:latest AS ospray

RUN apt-get update && \
    apt-get install -y \
            build-essential \
            # Compilation tool for C/C++ programs
            cmake \
            # python $(: pistache : running tests) \
            # libtbb-dev $(: embree ospray : threading framework) \
            # libglu1-mesa-dev $(: embree ospray : OpenGL requirements) \
            # freeglut3-dev $(: embree ospray : OpenGL requirements) \
            # mesa-common-dev $(: embree ospray : OpenGL requirements) \
            # libc6-dev $(: enchiladas : pthreads) \
    && \
    rm -rf /var/lib/apt/lists/*

COPY --from=ispc /stage /
COPY --from=embree /opt/embree /opt/embree

WORKDIR /opt/
COPY ospray /opt/ospray
WORKDIR /opt/ospray/build
RUN true && \
    cmake .. \
          -Dembree_DIR=/opt/embree \
          -DOSPRAY_ENABLE_APPS:BOOL=OFF \
          -DOSPRAY_TASKING_SYSTEM=OpenMP \
    && \
    make && \
    make install DESTDIR=/stage

FROM ubuntu:latest AS pbnj
COPY --from=rapidjson /stage /
COPY --from=embree /opt/embree /opt/embree
COPY --from=ospray /stage /

RUN apt-get update && \
    apt-get install -y \
            python3-dev \
    && \
    rm -rf /var/lib/apt/lists/*

RUN apt-get update && \
    apt-get install -y \
            cmake \
    && \
    rm -rf /var/lib/apt/lists/*

RUN apt-get update && \
    apt-get install -y \
            build-essential \
    && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /opt/pbnj/
COPY cmake ./cmake
COPY include ./include
COPY src ./src
COPY CMakeLists.txt ./CMakeLists.txt
WORKDIR /opt/pbnj/build
RUN true && \
    cmake .. \
          -DUSE_NETCDF:BOOL=OFF \
          -DBUILD_EXAMPLES:BOOL=OFF \
          -Dembree_DIR=/opt/embree \
    && \
    make && \
    make install

RUN apt-get update && \
    apt-get install -y \
            python3-pip \
    && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /opt/pbnj/
COPY pbnj.py ./pbnj.py
COPY pbnj_wrap.cxx ./pbnj_wrap.cxx
COPY setup.py ./setup.py
WORKDIR /opt/pbnj/
RUN true && \
    python3 -m pip install virtualenv && \
    python3 -m virtualenv -p python3 venv

RUN true && \
    env LD_RUN_PATH=/usr/local/lib:/opt/embree/lib venv/bin/pip install --verbose .
COPY test.py ./test.py
CMD ["venv/bin/python3", "test.py"]
