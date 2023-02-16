cmake_minimum_required(VERSION 3.8)

project(SMSMirrorMode VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)

file(GLOB SMS_MIRROR_MODE_SRC
    "src/*.c"
    "src/*.cpp"
    "include/*.h"
    "include/*.hxx"
)

add_executable(SMSMirrorMode ${SMS_MIRROR_MODE_SRC})
add_subdirectory(lib/BetterSunshineEngine)

target_link_libraries(SMSMirrorMode PUBLIC BetterSunshineEngine)

target_include_directories(SMSMirrorMode PUBLIC "include")

target_compile_options(SMSMirrorMode PUBLIC ${SMS_COMPILE_FLAGS})
target_link_options(SMSMirrorMode PUBLIC ${SMS_LINK_FLAGS})

list(APPEND SMS_MIRROR_MODE_CONFIG_DEFINES "KURIBO_NO_TYPES" "SMS_MIRROR_MODE_VERSION=\"v1.0\"")

target_compile_definitions(SMSMirrorMode PUBLIC ${SMS_MIRROR_MODE_CONFIG_DEFINES})

add_custom_target(${PROJECT_NAME}.kxe ALL DEPENDS ${PROJECT_NAME})
add_custom_command(
    TARGET ${PROJECT_NAME}.kxe
    COMMAND ${PROJECT_SOURCE_DIR}/lib/BetterSunshineEngine/tools/KuriboConverter.exe ARGS ${PROJECT_NAME} ${PROJECT_NAME}.kxe ${PROJECT_SOURCE_DIR}/lib/BetterSunshineEngine/maps/${SMS_REGION}.map
)
