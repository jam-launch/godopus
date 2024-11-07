#pragma once

#include <mutex>
#include <memory>

#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/classes/global_constants.hpp"
#include "godot_cpp/core/binder_common.hpp"
#include "godot_cpp/variant/packed_byte_array.hpp"
#include "godot_cpp/variant/packed_vector2_array.hpp"

#include "opus.h"

class GodOpusDecoder : public godot::RefCounted
{
    GDCLASS(GodOpusDecoder, godot::RefCounted)

public:
    GodOpusDecoder();
    ~GodOpusDecoder();

    godot::PackedVector2Array decode_as_stream_buffer(const godot::PackedByteArray &buf);

protected:
    static void _bind_methods();

private:
    int m_max_output_size;
    float *m_output_buffer;
    std::mutex m_mtx;
    OpusDecoder *m_decoder;
};