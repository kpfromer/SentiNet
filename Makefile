
# Some meta info
# CPP Source for doing things like formatting code etc.
CPP_SRC=./cpp
# Directory for build
BUILD_TARGET=./build
# Cmake base command
CMAKE_TARGET=cmake -S . -B ${BUILD_TARGET} -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
# Compilation target
MAKE_TARGET=make -C ${BUILD_TARGET}
# Clean target
CMAKE_CLEAN_ALL=rm -rf ${BUILD_TARGET}
# Format suffixes
FILE_SUFFIXES=-iname *.h -o -iname *.cpp -o -iname *.hpp -o -iname *.cc -o -iname *.c
# Format target
FORMAT_TARGET=clang-format -i -sort-includes=false -style=file


all :: cmake compile

cmake:
	@${CMAKE_TARGET}

compile:
	@${MAKE_TARGET}

clean ::
	@${CMAKE_CLEAN_ALL}

format ::
	@find ${CPP_SRC} ${FILE_SUFFIXES} | xargs ${FORMAT_TARGET}

install-deps ::
	@sudo apt install cmake make curl libcurl4-gnutls-dev autoconf automake libtool g++ unzip libzmq3-dev

setup ::
	@./scripts/generate setup ./resources/templates
	@cp ./scripts/generate ~/.sentinet/bin

model ::
	@${CMAKE_TARGET} -DBUILD_MODEL=ON
	@${MAKE_TARGET}
