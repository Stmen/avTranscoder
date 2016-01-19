#include "Frame.hpp"

#include <stdexcept>

namespace avtranscoder
{

Frame::Frame()
    : _frame(NULL)
{
    allocateAVFrame();
}

Frame::Frame(const Frame& otherFrame)
    : _frame(NULL)
{
    // allocate frame
    allocateAVFrame();
    // check if the frame could be a valid video/audio frame
    if(otherFrame.getAVFrame().format == -1)
        return;
    // reference the other frame
    refFrame(otherFrame);
}

Frame::~Frame()
{
    if(_frame != NULL)
    {
#if LIBAVCODEC_VERSION_MAJOR > 54
        av_frame_free(&_frame);
#else
#if LIBAVCODEC_VERSION_MAJOR > 53
        avcodec_free_frame(&_frame);
#else
        av_free(_frame);
#endif
#endif
        _frame = NULL;
    }
}

unsigned char* Frame::getPlaneData(const size_t plane)
{
    AVBufferRef* buffer = av_frame_get_plane_buffer(_frame, plane);
    if(buffer != NULL)
        return buffer->data;
    return NULL;
}

void Frame::copyData(const Frame& frameToRef)
{
    const int ret = av_frame_copy(_frame, &frameToRef.getAVFrame());
    if(ret < 0)
    {
        throw std::ios_base::failure("Unable to copy data of other frame: " + getDescriptionFromErrorCode(ret));
    }
}

void Frame::copyProperties(const Frame& otherFrame)
{
    av_frame_copy_props(_frame, &otherFrame.getAVFrame());
}

void Frame::refFrame(const Frame& otherFrame)
{
    const int ret = av_frame_ref(_frame, &otherFrame.getAVFrame());
    if(ret < 0)
    {
        throw std::ios_base::failure("Unable to reference other frame: " + getDescriptionFromErrorCode(ret));
    }
}

void Frame::clear()
{
    av_frame_unref(_frame);
}

void Frame::allocateAVFrame()
{
#if LIBAVCODEC_VERSION_MAJOR > 54
    _frame = av_frame_alloc();
#else
    _frame = avcodec_alloc_frame();
#endif
    if(_frame == NULL)
    {
        throw std::runtime_error("Unable to allocate an empty Frame.");
    }
}
}
