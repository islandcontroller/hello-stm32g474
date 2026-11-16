cmake_minimum_required(VERSION 3.20)

# Project setup
project(hello-stm32g474
	LANGUAGES C ASM
)

# Filenames configuration
set(CMAKE_EXECUTABLE_SUFFIX ".elf")
set(CMAKE_HEXFILE_SUFFIX ".hex")
set(CMAKE_LISTING_SUFFIX ".lst")
set(CMAKE_MAPFILE_SUFFIX ".map")

# Language configuration
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

# Output target
add_executable(${PROJECT_NAME})

# Source files (exclude build outputs and file templates)
file(GLOB_RECURSE TARGET_SOURCES *.c *.S)
list(FILTER TARGET_SOURCES EXCLUDE REGEX "build\/.*")
list(FILTER TARGET_SOURCES EXCLUDE REGEX "Controller\/.*\/Template\/.*")
target_sources(${PROJECT_NAME} PRIVATE ${TARGET_SOURCES})

# Include paths
target_include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_SOURCE_DIR}
	Controller/
	Controller/STM32G4xx
	Controller/STM32G4xx/Core
	Controller/STM32G4xx/Peripheral/inc
	hw_layer/
)

# Common compiler/linker settings
set(MACHINE_OPTIONS
	-mcpu=cortex-m4
	-march=armv7e-m
	-mfpu=fpv4-sp-d16
	-mthumb
	-mfloat-abi=hard

	-specs=nano.specs
	-specs=nosys.specs
	
	-nostartfiles
)

# Compiler configuration
target_compile_definitions(${PROJECT_NAME} PRIVATE
	-DSTM32G474xx # STM32G474RET6
	-DUSE_HAL_DRIVER
)
target_compile_options(${PROJECT_NAME} PRIVATE
	${MACHINE_OPTIONS}

	-fdata-sections
	-ffunction-sections
	-flto

	-Wall
	-Wextra

	-Og
	-g
)

# Linker configuration
set(LINKER_FILE Controller/STM32G4xx/linker_script_stm32g474xe.ld) # STM32G474RET6
cmake_path(REMOVE_FILENAME LINKER_FILE OUTPUT_VARIABLE LINKER_BASEDIR)
cmake_path(GET LINKER_FILE FILENAME LINKER_FILE)
target_link_directories(${PROJECT_NAME} PRIVATE	${LINKER_BASEDIR})
target_link_options(${PROJECT_NAME} PRIVATE
	${MACHINE_OPTIONS}
	
	-T${LINKER_FILE}

	-Wl,--as-needed
	-Wl,--gc-sections
	-Wl,--print-memory-usage
	
	-Wl,-Map=${PROJECT_NAME}${CMAKE_MAPFILE_SUFFIX},--cref
)

# Post-Build: register generated mapfile
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND true
	BYPRODUCTS ${PROJECT_NAME}${CMAKE_MAPFILE_SUFFIX}
)

# Post-Build: print section sizes
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_SIZE_UTIL} ${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}
)

# Post-Build: generate listings
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_OBJDUMP} -d -S ${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX} > ${PROJECT_NAME}${CMAKE_LISTING_SUFFIX}
	BYPRODUCTS ${PROJECT_NAME}${CMAKE_LISTING_SUFFIX}
)

# Post-Build: generate HEX file
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_OBJCOPY} -O ihex ${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX} ${PROJECT_NAME}${CMAKE_HEXFILE_SUFFIX}
)