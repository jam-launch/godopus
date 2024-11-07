#include "godot_cpp/classes/global_constants.hpp"
#include "godot_cpp/classes/label.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/variant/utility_functions.hpp"
#include "godot_cpp/variant/dictionary.hpp"

#include "GodOpusDecoder.hpp"

#define ALLOCATION_TAG "GD_OpusEncoder"

GodOpusDecoder::GodOpusDecoder() : m_max_output_size(5760)
{
    m_output_buffer = new float[m_max_output_size];
    int err;
    m_decoder = opus_decoder_create(48000, 1, &err);
    if (err != OPUS_OK)
    {
        ERR_PRINT(godot::String("Opus decoder creation failure: {0}").format(godot::Array::make(opus_strerror(err))));
    }
}

GodOpusDecoder::~GodOpusDecoder()
{
    opus_decoder_destroy(m_decoder);
    delete[] m_output_buffer;
}

void GodOpusDecoder::_bind_methods()
{
    godot::ClassDB::bind_method(godot::D_METHOD("decode_as_stream_buffer", "opus_packet"), &GodOpusDecoder::decode_as_stream_buffer);
}

godot::PackedVector2Array GodOpusDecoder::decode_as_stream_buffer(const godot::PackedByteArray &opusPkt)
{
    std::lock_guard<std::mutex> guard(m_mtx);

    godot::PackedVector2Array streamBuffer;
    if (opusPkt.size() < 1)
    {
        return streamBuffer;
    }

    opus_int32 decodedSampleCount = opus_decode_float(m_decoder, opusPkt.ptr(), opusPkt.size(), m_output_buffer, m_max_output_size, 0);
    if (decodedSampleCount < 0)
    {
        ERR_PRINT(godot::String("Opus encode failure: {0}").format(godot::Array::make(opus_strerror(decodedSampleCount))));
        return streamBuffer;
    }

    for (int i = 0; i < decodedSampleCount; i++)
    {
        streamBuffer.push_back(godot::Vector2(m_output_buffer[i], m_output_buffer[i]));
    }
    return streamBuffer;
}