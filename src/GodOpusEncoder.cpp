#include "godot_cpp/classes/global_constants.hpp"
#include "godot_cpp/classes/label.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/variant/utility_functions.hpp"
#include "godot_cpp/variant/dictionary.hpp"

#include "GodOpusEncoder.hpp"

#define ALLOCATION_TAG "GD_OpusEncoder"

GodOpusEncoder::GodOpusEncoder() : m_max_packet_size(1024)
{
    m_output_buffer = new unsigned char[m_max_packet_size];
    int err;
    m_encoder = opus_encoder_create(48000, 1, OPUS_APPLICATION_VOIP, &err);
    OPUS_SET_BITRATE(4000);
}

GodOpusEncoder::~GodOpusEncoder()
{
    opus_encoder_destroy(m_encoder);
    delete[] m_output_buffer;
}

void GodOpusEncoder::_bind_methods()
{
    godot::ClassDB::bind_method(godot::D_METHOD("encode_stream_buffer", "stream_buffer"), &GodOpusEncoder::encode_stream_buffer);
}

godot::PackedByteArray GodOpusEncoder::encode_stream_buffer(const godot::PackedVector2Array &streamBuffer)
{
    std::lock_guard<std::mutex> guard(m_mtx);

    godot::PackedByteArray opusBytes;
    if (streamBuffer.size() < 1)
    {
        return opusBytes;
    }

    std::vector<float> pcmSamples(streamBuffer.size());
    for (int i = 0; i < streamBuffer.size(); i++)
    {
        pcmSamples[i] = streamBuffer[i].x;
    }

    opus_int32 opusPacketSize = opus_encode_float(m_encoder, pcmSamples.data(), pcmSamples.size(), m_output_buffer, m_max_packet_size);
    if (opusPacketSize < 0)
    {
        ERR_PRINT(godot::String("Opus encode failure: {0}").format(godot::Array::make(opus_strerror(opusPacketSize))));
        return opusBytes;
    }

    opusBytes.resize(opusPacketSize);
    memcpy(opusBytes.ptrw(), m_output_buffer, opusPacketSize);

    return opusBytes;
}