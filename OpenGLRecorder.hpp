
#ifndef __OPENGL_RECORDER_HPP__XRECORDER
#define __OPENGL_RECORDER_HPP__XRECORDER

#include <string>
#include <vector>

#include <opencv2/opencv.hpp>
#include <GL/gl.h>
#include <GL/glu.h>

namespace xrecorder {

template <uint32_t W, uint32_t H, uint8_t FPS = 60>
class OpenGLRecorder {

public:
    OpenGLRecorder(std::string fileName = "xrecorder") {
        // 创建VideoWriter对象
        if (fileName == "") {
            fileName = "xrecorder";
        }

        fileName += ".mkv";

        int fourcc = cv::VideoWriter::fourcc('X', '2', '6', '4'); // x264 is a opensource impl for H.264 
        __mVideoWriter.open(fileName, fourcc, FPS, cv::Size(W, H), true);

        if (!__mVideoWriter.isOpened()) {
            std::cerr << "Error opening video file for output" << std::endl;
        }

        // Note: Mat(int rows/h, int cols/w, int type)
        __mFrame = cv::Mat(H, W, CV_8UC3); // MJPG、XVID、H264 not support CV_8UC4

        __mBuff.resize(3840 * 2160 * 4, 0);
    }

    ~OpenGLRecorder() {
        // save file and release res
        __mVideoWriter.release();
    }

public:
    void captureFrameData() {
        // Note:
        //      Mat(int rows/h, int cols/w, int type)
        //      cv::Rect(x, y, width, height)
        GLint viewport[4];

        glGetIntegerv(GL_VIEWPORT, viewport);

        glReadPixels(viewport[0], viewport[1], viewport[2], viewport[3], GL_RGBA, GL_UNSIGNED_BYTE, __mBuff.data());

        auto frame = cv::Mat(viewport[3], viewport[2], CV_8UC4, __mBuff.data());

        std::memcpy(frame.data, __mBuff.data(), viewport[2] * viewport[3] * 4);

        cv::cvtColor(frame, frame, cv::COLOR_RGBA2BGR);
        cv::flip(frame, frame, 0);

        __mFrame.setTo(cv::Scalar(0));

        if (viewport[2] < W && viewport[3] < H) {
            cv::Rect roi((W - viewport[2]) / 2, (H - viewport[3]) / 2, viewport[2], viewport[3]);
            frame.copyTo(__mFrame(roi));
        } else {
            cv::resize( // TODO: optimize
                frame,
                __mFrame,
                cv::Size(W, H)
            );
        }

        __mVideoWriter.write(__mFrame);
    }

private:
    std::vector<GLubyte> __mBuff;
    cv::Mat __mFrame;
    cv::VideoWriter __mVideoWriter;
};

} // xrecorder namespace end

#endif