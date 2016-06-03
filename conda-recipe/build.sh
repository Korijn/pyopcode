#!/bin/bash


BUILD_CONFIG=Release

if [ `uname` == Linux ]; then
    CC=${PREFIX}/bin/gcc
    CXX=${PREFIX}/bin/g++

    # FIXME refactor to reuse the python name (e.g. python3.5m)
    # FIXME detect any kind of suffix (m, or d)
    include_path=${PREFIX}/include/python${PY_VER}
    if [ ! -d $include_path ]; then
      # Control will enter here if $DIRECTORY doesn't exist.
      include_path=${PREFIX}/include/python${PY_VER}m
    fi

    PY_LIB="libpython${PY_VER}.so"
    library_file_path=${PREFIX}/lib/${PY_LIB}
    if [ ! -f $library_file_path ]; then
        library_file_path=${PREFIX}/lib/libpython${PY_VER}m.so
    fi

    cd pyopcode
    mkdir build
    cd build

    cmake ../src \
        -Wno-dev \
        -DCMAKE_BUILD_TYPE=${BUILD_CONFIG} \
        -DCMAKE_INSTALL_PREFIX=${PREFIX} \
        -DPYTHON_INCLUDE_DIR:PATH=${PREFIX}/include \
        -DPYTHON_LIBRARY:FILEPATH=${PYTHON_LIBRARY} \
        -DNUMPY_INCLUDE_DIR:PATH=${SP_DIR}/numpy/core/include \
        -DBOOST_ROOT:PATH=${PREFIX}/Library

fi


cd ..
cmake --build ./build --clean-first --config ${BUILD_CONFIG}
copy .\build\release\pyopcode.pyd .\pyopcode.pyd
cd ..

python setup.py install
