cmake_minimum_required(VERSION 3.14)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(AssetDeploy)
include(FetchContent)

# Setup UWP platform
set(CMAKE_SYSTEM_NAME WindowsStore)
set(CMAKE_SYSTEM_VERSION 10.0.19041.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch shared UWP deps
FetchContent_Declare(UwpLibs
    GIT_REPOSITORY https://github.com/worleydl/uwp-dep.git
    GIT_TAG c070e24502da4dcce30bfe5dbcb4f5ff61857cfa
)
FetchContent_MakeAvailable(UwpLibs)

# Define project
project(inferno-uwp LANGUAGES CXX)
set_directory_properties(PROPERTIES VS_STARTUP_PROJECT ${PROJECT_NAME})

# Config for main recomp build
#set(INFERNO_ASSET_DIR CACHE PATH "")
set(INFERNO_DIR CACHE PATH "")
#if(NOT INFERNO_ASSET_DIR)
#	message(FATAL_ERROR "INFERNO_ASSET_DIR must point to a folder containing required runtime assets.")
#endif()

if(NOT INFERNO_DIR)
	message(FATAL_ERROR "INFERNO_DIR must point to the Inferno build folder (../bin)")
endif()

add_executable(${PROJECT_NAME} WIN32
	src/main.cpp
)

set(BinDeps
	${CMAKE_CURRENT_BINARY_DIR}/_deps/uwplibs-src/x64/bin/dxil.dll
	${CMAKE_CURRENT_BINARY_DIR}/_deps/uwplibs-src/x64/bin/dxcompiler.dll
	${CMAKE_CURRENT_BINARY_DIR}/_deps/uwplibs-src/x64/bin/libuwp.dll
	${CMAKE_CURRENT_BINARY_DIR}/_deps/uwplibs-src/x64/bin/SDL3.dll

	src/Package.appxmanifest
)
source_group("BinDeps" FILES ${BinDeps})
set_source_files_properties(${BinDeps} PROPERTIES
	VS_COPY_TO_OUT_DIR Always
	VS_DEPLOYMENT_CONTENT TRUE
	VS_DEPLOYMENT_LOCATION "."
)

set(PkgAssets
	Assets/LockScreenLogo.scale-200.png
	Assets/Square44x44Logo.targetsize-24_altform-unplated.png
	Assets/Square44x44Logo.scale-200.png
	Assets/Square150x150Logo.scale-200.png
	Assets/StoreLogo.png
)
source_group("PkgAssets" FILES ${PkgAssets})
set_source_files_properties(${PkgAssets} PROPERTIES
	#VS_COPY_TO_OUT_DIR Always
	VS_DEPLOYMENT_CONTENT TRUE
	VS_DEPLOYMENT_LOCATION "Assets"
)

target_sources(${PROJECT_NAME} PRIVATE
	${BinDeps}
	${PkgAssets}
)

target_include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_CURRENT_BINARY_DIR}/_deps/uwplibs-src/x64/include
)

set(VCPKG_ROOT ${CMAKE_SOURCE_DIR}/../vcpkg_installed/x64-windows-static-md/x64-windows-static-md)
set(VCPKG_ROOT_ALT ${CMAKE_SOURCE_DIR}/../vcpkg_installed/x64-windows-static-md/x64-windows)
set(BinLibs
	${CMAKE_CURRENT_BINARY_DIR}/_deps/uwplibs-src/x64/lib/libuwp.lib
	${CMAKE_CURRENT_BINARY_DIR}/_deps/uwplibs-src/x64/lib/SDL3.lib

	# Inferno main
	${INFERNO_DIR}/Imgui/x64/$<CONFIG>/Imgui.lib
	${INFERNO_DIR}/Inferno.Core/x64/$<CONFIG>/Inferno.Core.lib
	${INFERNO_DIR}/Inferno/x64/$<CONFIG>/Inferno.lib

	# Inferno vcpkg
	debug ${VCPKG_ROOT}/debug/lib/c4core.lib
	debug ${VCPKG_ROOT}/debug/lib/D3D12MAd.lib
	debug ${VCPKG_ROOT}/debug/lib/DirectX-Guids.lib
	debug ${VCPKG_ROOT}/debug/lib/DirectX-Headers.lib
	debug ${VCPKG_ROOT}/debug/lib/DirectXTex.lib
	debug ${VCPKG_ROOT}/debug/lib/DirectXTK12.lib
	debug ${VCPKG_ROOT}/debug/lib/fmtd.lib
	debug ${VCPKG_ROOT}/debug/lib/lodepng.lib
	debug ${VCPKG_ROOT}/debug/lib/lodepng-c.lib
	debug ${VCPKG_ROOT}/debug/lib/ryml.lib
	debug ${VCPKG_ROOT}/debug/lib/spdlogd.lib

	optimized ${VCPKG_ROOT}/lib/c4core.lib
	optimized ${VCPKG_ROOT}/lib/D3D12MA.lib
	optimized ${VCPKG_ROOT}/lib/DirectX-Guids.lib
	optimized ${VCPKG_ROOT}/lib/DirectX-Headers.lib
	optimized ${VCPKG_ROOT}/lib/DirectXTex.lib
	optimized ${VCPKG_ROOT}/lib/DirectXTK12.lib
	optimized ${VCPKG_ROOT}/lib/fmt.lib
	optimized ${VCPKG_ROOT}/lib/lodepng.lib
	optimized ${VCPKG_ROOT}/lib/lodepng-c.lib
	optimized ${VCPKG_ROOT}/lib/ryml.lib
	optimized ${VCPKG_ROOT}/lib/spdlog.lib

	${VCPKG_ROOT_ALT}/lib/dxcompiler.lib

	dxgi.lib
	WindowsApp.lib
	OneCore.lib
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${BinLibs})

# Data deployment (this still requires some manual fetching of appropriate files)
#uwp_pkg(${PROJECT_NAME} ${INFERNO_ASSET_DIR}/assets "assets")
#uwp_pkg(${PROJECT_NAME} ${INFERNO_ASSET_DIR}/d1 "d1")
#uwp_pkg(${PROJECT_NAME} ${INFERNO_ASSET_DIR}/d2 "d2")
#uwp_pkg(${PROJECT_NAME} ${INFERNO_ASSET_DIR}/mods "mods")
#uwp_pkg(${PROJECT_NAME} ${INFERNO_ASSET_DIR}/shaders "shaders")

