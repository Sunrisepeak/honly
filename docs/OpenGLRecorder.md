## OpenGLRecorder 文档

| 库                                       | 版本信息   | 类型      | 依赖          | 描述                                             | 文档                           |
| ---------------------------------------- | ---------- | --------- | ------------- | ------------------------------------------------ | ------------------------------ |
| [OpenGLRecorder.hpp](OpenGLRecorder.hpp) | pre-v0.0.1 | 图片/视频 | OpenCV/OpenGL | 用于捕获/录制/显示OpenGL帧缓冲区, 生成图片或视频 | [Docs](docs/OpenGLRecorder.md) |



### 1.依赖&配置

- opencv

```bash
sudo apt-get install libopencv-dev # linux/ubuntu
```

#### xmake 配置

```lua
add_requires("opencv")
```

或

~~~lua
if is_host("linux") then
    add_includedirs("/usr/include/opencv4")
end

add_links("opencv_core")
add_links("opencv_highgui")
add_links("opencv_imgproc", "opencv_videoio", "opencv_imgcodecs")
~~~

#### cmake 配置

```cmake
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
```



### 2.用法简介

#### 2.1 引用头文件

- OpenGLRecorder.hpp

#### 2.2 基本用法

```cpp
// 创建一个opengl帧录制器xr
// 分辨率: 1920 X 1080
// 帧率: 60FPS
// 文件名: xrecorder
xrecorder::OpenGLRecorder<1920, 1080> xr;

while (flag) {

    // 你的opengl渲染操作

    xr.captureFrameData(); // 捕获当前opengl帧缓冲区数据
    xr.saveToVideo();      // 保存当前帧数据到视频
    //xr.saveToImg();      // 保存当前帧数据生成图片
}
```



#### 2.3 使用示例

- [录制opengl帧缓冲区](https://github.com/Sunrisepeak/Hanim/blob/b457f802e5a7e4facff4479bd5f1b54ab159d372/examples/opengl/main.cpp#L75)

- [录制imgui (opengl后端) 帧缓冲区](https://github.com/Sunrisepeak/Hanim/blob/b457f802e5a7e4facff4479bd5f1b54ab159d372/examples/imgui/main.cpp#L100)



### 3. 接口介绍

#### 3.1 构造器接口

**`xrecorder::OpenGLRecorder<W, H, FPS>(fileName, autoDetectFormat)`**

- `模板参数 W`: 分辨率的宽
- `模板参数 H`: 分辨率的高
- `模板参数 FPS`: 视频的帧率, 默认为60FPS
- `fileName`： 生产视频/图片的 文件名(不含后缀), 默认为"xrecorder"
- `autoDetectFormat`: 是否根据文件名自动检测格式, 默认为false
  - false: 使用默认格式mkv, 支持断续/中断, 文件略大
  - true: 根据`fileName`中的后缀自动检测选择格式, 不保证为期望格式

#### 3.2 基础信息接口

> xrecorder::OpenGLRecorder<W, H, FPS>::method 这里省略模板参数

**`void xrecorder::OpenGLRecorder::setFlipStatus(bool)`**

> 设置图像的翻转状态

**`bool xrecorder::OpenGLRecorder::getFlipStatus()`**

> 获取图像的翻转状态, 默认状态是true

**`int xrecorder::OpenGLRecorder::getW(bool)`**

> 获取分辨率的宽W

**`int xrecorder::OpenGLRecorder::getH()`**

> 获取分辨率的高H

**`int xrecorder::OpenGLRecorder::getFPS()`**

> 获取视频的FPS



#### 3.3 数据处理接口

**`void xrecorder::OpenGLRecorder::captureFrameData()`**

> 捕获当前opengl的帧缓冲区数据
>
> 注: 当opengl帧缓冲区大于录制器的分辨时 会对宽和高进行等比例缩放

**`void xrecorder::OpenGLRecorder::saveToImg() const`**

> 保存当前帧为一个图片, 并保存到本地
>
> 图片文件名: 录制器名.帧数.jpg

**`void xrecorder::OpenGLRecorder::saveToVideo() const`**

> 保存当前帧到视频文件中

**`void xrecorder::OpenGLRecorder::show() const`**

> 显示当前帧



### 4. 完整的例子

#### 4.1 源码 - `examples/xr-opengl-recorder.cpp`

```cpp
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <OpenGLRecorder.hpp>

int main() {
    if (!glfwInit()) {
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(400, 400, "XRecorder - OpenGLRecorder", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    xrecorder::OpenGLRecorder<1920, 1080> xr;

    unsigned char i = 0;
    
    while (!glfwWindowShouldClose(window)) {
        glClearColor((i % 255) * 1.0f, (i % 255) * 1.0f, (i % 255) * 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        i++;
        
        xr.captureFrameData();
        xr.saveToVideo();
        //xr.saveToImg();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
```



#### 4.2 录制的视频文件 播放效果 - xrecorder.mkv

![](imgs/xr-opengl-recorder.gif)



### 5.Other

[XRecorder](https://github.com/Sunrisepeak/XRecorder)

[Github](https://github.com/Sunrisepeak)
