workspace "Interview"
configurations {"Debug", "Release"}
location "_build"
architecture "x86_64"
systemversion "latest"
language "C++"
cppdialect "c++17"
targetdir "../bin/%{cfg.buildcfg}"
includedirs {"src"}
warnings "Extra"
flags {"FatalWarnings"}
buildoptions{"/arch:AVX2", "/Zc:__cplusplus"}

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
    sysincludedirs {"ext/glfw/include"}
    libdirs {"ext/glfw/lib-vc2019"}
    links {"glfw3dll.lib"}