SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

cmake -DCMAKE_TOOLCHAIN_FILE="bsp/cmake/n32g435.cmake" ${SCRIPT_DIR} "${@:2}"
