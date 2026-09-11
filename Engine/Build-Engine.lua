project "Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"

    files { 
        "Source/**.h", 
        "Source/**.cpp",
        "Source/glad.c",
        "Source/imgui.cpp",
        "Source/imgui_draw.cpp",
        "Source/imgui_widgets.cpp",
        "Source/imgui_tables.cpp",
        "Source/imgui_demo.cpp",
        "Source/imgui_impl_glfw.cpp",
        "Source/imgui_impl_opengl3.cpp"
    }

    includedirs {
        "Source",

        -- Dependencies
        "%{DependenciesDir}/GLFW/include",
        "%{DependenciesDir}/GLAD/include",
        "%{DependenciesDir}/Bullet/include",
        "%{DependenciesDir}/GLM/include",
        "%{DependenciesDir}/ImGui/include",
        "%{DependenciesDir}/SDL/include"
    }

    libdirs {
        "%{DependenciesDir}/GLFW/lib",
        "%{DependenciesDir}/Bullet/lib/%{cfg.buildcfg}",
        "%{DependenciesDir}/SDL/lib/x64"
    }

    links {
        "opengl32.lib",
        "glfw3.lib",
        "BulletDynamics_Debug.lib",
        "BulletCollision_Debug.lib",
        "LinearMath_Debug.lib",
        "Bullet3Common_Debug.lib",
        "SDL2.lib"
    }

    targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
    objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines { "DEBUG" }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "RELEASE" }
        runtime "Release"
        optimize "On"
        symbols "On"

    filter "configurations:Dist"
        defines { "DIST" }
        runtime "Release"
        optimize "On"
        symbols "Off"