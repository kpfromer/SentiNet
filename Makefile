
CPP_SRC=./cpp
BUILD_TARGET=./build
CMAKE_TARGET=cmake -S . -B ${BUILD_TARGET}
MAKE_TARGET=make -C ${BUILD_TARGET}

CMAKE_CLEAN=rm -rf ${BUILD_TARGET}/CMakeFiles ${BUILD_TARGET}/cmake_install.cmake \
	    ${BUILD_TARGET}/Makefile ${BUILD_TARGET}/CMakeCache.txt ${BUILD_TARGET}/cpp
CLEAN_ALL=rm -rf ${BUILD_TARGET}
CLEAN=rm -rf ${BUILD_TARGET}

all :: keep-cmake
	@echo "Removing Cmake Resources"
	@${CMAKE_CLEAN}

clean ::
	@${CLEAN}

keep-cmake ::
	@echo "Gathering Source Files and Compile Time instructions"
	@${CMAKE_TARGET}
	@echo "Making Files generated by Cmake"
	@${MAKE_TARGET}

format ::
	@find ./cpp -iname "*.proto" -o -iname "*.cpp" -o -iname "*.hpp" -o -iname "*.h" -o -iname "*.cc" -o -iname "*.c" | xargs clang-format -i -sort-includes=false -style=file
