#ifndef _AV_TRANSCODER_FRAME_FRAME_HPP_
#define _AV_TRANSCODER_FRAME_FRAME_HPP_

#include <AvTranscoder/common.hpp>

extern "C" {
#include <libavutil/frame.h>
}

namespace avtranscoder
{

/**
 * @brief This class describes decoded (raw) audio or video data.
 * This class is abstract.
 * @see VideoFrame
 * @see AudioFrame
 */
class AvExport Frame
{
public:
    /**
     * @brief Allocate an empty frame.
     * @warning This only allocates the AVFrame itself, not the data buffers.
     * Depending on the case, we could manipulate frame with data allocated elsewhere.
     * For example, an empty frame is given to a decoder, which is responsible to allocate and free the data buffers.
     */
    Frame();

    virtual ~Frame();

    /**
     * @brief Allocate the buffer of the frame.
     */
    virtual void allocateData() = 0;

    /**
     * @brief Free the buffer of the frame.
     */
    virtual void freeData() = 0;

    /**
     * @brief Get the size in bytes that a video/audio buffer of the given frame properties would occupy if allocated.
     * @warning This methods does not guarantee that the buffer is actually allocated.
     */
    virtual size_t getSize() const = 0;

    /**
     * @brief Get all the data of the frame.
     */
    unsigned char** getData() { return _frame->data; }

    /**
     * @brief Returns the size in byte.
     * For video, size in bytes of each picture line.
     * For audio, size in bytes of each plane.
     * @note For audio, only linesize[0] may be set. For planar audio, each channel
     * plane must be the same size.
     */
    int* getLineSize() const { return _frame->linesize; }

    /**
     * @brief Copy the data of the given Frame.
     * This function does not allocate anything: the current frame must be already initialized and
     * allocated with the same parameters as the given frame, to be ready for memcpy instructions.
     * @note It copies the frame data (i.e. the contents of the data / extended data arrays), not any other properties.
     * @see copyProperties
     */
    void copyData(const Frame& frameToRef);

    /**
     * @brief Copy all the fields that do not affect the data layout in the buffers.
     * @warning The info checked when copying data of an other frame (width/height, channels...) are not copied.
     */
    void copyProperties(const Frame& otherFrame);

    /**
     * @return If it corresponds to a valid audio frame.
     * @see AudioFrame
     */
    bool isAudioFrame() const;

    /**
     * @return If it corresponds to a valid video frame.
     * @see VideoFrame
     */
    bool isVideoFrame() const;

#ifndef SWIG
    AVFrame& getAVFrame() { return *_frame; }
    const AVFrame& getAVFrame() const { return *_frame; }
    const unsigned char** getData() const { return const_cast<const unsigned char**>(_frame->data); }
#endif

private:
    void allocateAVFrame();
    void freeAVFrame();

protected:
    AVFrame* _frame;
    bool _dataAllocated;
};
}

#endif
