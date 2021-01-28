set(SOURCES
    SmartEVSE2.X/bootloader.c
    SmartEVSE2.X/EVSE.c
    SmartEVSE2.X/GLCD.c
    SmartEVSE2.X/modbus.c
    SmartEVSE2.X/OneWire.c
    SmartEVSE2.X/utils.c
)

add_executable(version2 ${SOURCES})
target_include_directories(version2 PRIVATE SmartEVSE2.X/)

target_link_libraries(version2)
