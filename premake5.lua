workspace "Interview"
configurations {"Debug", "Release"}
location "_build"
architecture "x86_64"
systemversion "latest"
language "C++"
cppdialect "c++17"
targetdir "_bin/%{cfg.buildcfg}"
includedirs {"src"}
warnings "Extra"
flags {"FatalWarnings"}
linkoptions {"/ignore:4099"}

filter {"configurations:Debug"}
optimize "Off"
symbols "Full"
filter {"configurations:Release"}
optimize "Full"
symbols "Full"
filter{}


project "Example"
    kind "ConsoleApp"
    files {"src/**"}
    defines {"GLEW_STATIC"}
    staticruntime "on"
    externalincludedirs {"ext/glfw/include", "ext/glew/include"}
    libdirs {"ext/glfw/lib-vc2019", "ext/glew/lib/Release/x64"}
    links {"glfw3_mt", "glew32s", "opengl32", "glu32"}