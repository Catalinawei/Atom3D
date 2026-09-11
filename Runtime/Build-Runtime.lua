project "Runtime"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"

    files { 
        "Source/**.h", 
        "Source/**.cpp" 
    }

    includedirs {
        "Source",
        "../Engine/Source",
        "%{DependenciesDir}/GLFW/include",
        "%{DependenciesDir}/GLAD/include",
        "%{DependenciesDir}/Bullet/include",
        "%{DependenciesDir}/GLM/include",
        "%{DependenciesDir}/ImGui/include",
        "%{DependenciesDir}/SDL/include"
    }

    -- ═══════════════════════════════════════════
    -- ADD LIBRARY DIRECTORIES
    -- ═══════════════════════════════════════════
    libdirs {
        "%{DependenciesDir}/GLFW/lib",
        "%{DependenciesDir}/Bullet/lib/%{cfg.buildcfg}",
        "%{DependenciesDir}/SDL/lib/x64"
    }

    -- ═══════════════════════════════════════════
    -- ADD LINKS
    -- ═══════════════════════════════════════════
    links {
        "Engine",
        "opengl32.lib",
        "glfw3.lib"
    }

    filter "configurations:Debug"
        links {
            "BulletDynamics_Debug.lib",
            "BulletCollision_Debug.lib",
            "LinearMath_Debug.lib",
            "Bullet3Common_Debug.lib",
            "SDL2.lib"
        }

    filter "configurations:Release"
        links {
            "BulletDynamics.lib",
            "BulletCollision.lib",
            "LinearMath.lib",
            "Bullet3Common.lib"
        }

    filter "configurations:Dist"
        links {
            "BulletDynamics.lib",
            "BulletCollision.lib",
            "LinearMath.lib",
            "Bullet3Common.lib"
        }

    targetdir ("%{cfg.buildcfg}")
    objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

    postbuildcommands {
        "{COPY} ../Assets ../bin/" .. OutputDir .. "/Assets"
 }

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