add_rules("mode.debug", "mode.release")

add_requires("glfw 3.3.4")
add_requires("opencv 4.6.0")

add_includedirs("../")

if is_host("linux") then
    add_includedirs("/usr/include/opencv4")
    add_links("opencv_core")
    add_links("opencv_highgui")
    add_links("opencv_imgproc", "opencv_videoio", "opencv_imgcodecs")
end

target("xrecorder-opengl")
    set_kind("binary")
    add_files("xr-opengl-recorder.cpp")
    add_packages("glfw", "opencv")