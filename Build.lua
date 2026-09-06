-- premake5.lua
workspace "Atom3D"
    architecture "x64"
    configurations { "Debug", "Release", "Dist" }
    startproject "Runtime"

    DependenciesDir = "%{wks.location}/Dependencies"

    filter "system:windows"
        buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "Engine"
    include "Engine/Build-Engine.lua"
group ""

include "Runtime/Build-Runtime.lua"