#pragma once

#include <mutex>
#include <memory>

#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/classes/global_constants.hpp"
#include "godot_cpp/core/binder_common.hpp"
#include "godot_cpp/variant/packed_byte_array.hpp"
#include "godot_cpp/variant/packed_vector2_array.hpp"

#include "opus.h"

class GodOpusEncoder : public godot::RefCounted
{
    GDCLASS(GodOpusEncoder, godot::RefCounted)

public:
    GodOpusEncoder();
    ~GodOpusEncoder();

    godot::PackedByteArray encode_stream_buffer(const godot::PackedVector2Array &buf);

protected:
    static void _bind_methods();

private:
    int m_max_packet_size;
    unsigned char *m_output_buffer;
    std::mutex m_mtx;
    OpusEncoder *m_encoder;
};