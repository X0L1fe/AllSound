#include <iostream>
extern "C" {
    #include <libavformat/avformat.h>
    #include <libavcodec/avcodec.h>
    #include <libavutil/avutil.h>
    #include <libavutil/opt.h>
    #include <libavutil/channel_layout.h>
    #include <libswresample/swresample.h>
}

#include "audio_decoder.h"



AudioBuffer AudioDecoder::decode(const std::string& path) {
    AudioBuffer buffer;

    AVFormatContext* fmt = nullptr;

    int err = avformat_open_input(&fmt, path.c_str(), nullptr, nullptr);

    if (err != 0) {
        char errbuf[256];
        av_strerror(err, errbuf, sizeof(errbuf));
        std::cerr << "FFmpeg error: " << errbuf << "\n";
        std::cout << path;
        return buffer;
    }

    avformat_find_stream_info(fmt, nullptr);

    int streamIndex = -1;

    for (unsigned i = 0; i < fmt->nb_streams; i++) {
        if (fmt->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            streamIndex = i;
            break;
        }
    }

    if (streamIndex == -1) {
        std::cerr << "Аудио поток не найден\n";
        return buffer;
    }

    AVCodecParameters* codecpar = fmt->streams[streamIndex]->codecpar;
    const AVCodec* codec = avcodec_find_decoder(codecpar->codec_id);

    AVCodecContext* ctx = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(ctx, codecpar);
    avcodec_open2(ctx, codec, nullptr);

    SwrContext* swr = swr_alloc();

// входной layout
    AVChannelLayout in_layout = ctx->ch_layout;

    // выходной layout (mono)
    AVChannelLayout out_layout;
    av_channel_layout_default(&out_layout, 1);

    // настройки
    av_opt_set_chlayout(swr, "in_chlayout", &in_layout, 0);
    av_opt_set_chlayout(swr, "out_chlayout", &out_layout, 0);

    av_opt_set_int(swr, "in_sample_rate", ctx->sample_rate, 0);
    av_opt_set_int(swr, "out_sample_rate", 44100, 0);

    av_opt_set_sample_fmt(swr, "in_sample_fmt", ctx->sample_fmt, 0);
    av_opt_set_sample_fmt(swr, "out_sample_fmt", AV_SAMPLE_FMT_FLT, 0);

    swr_init(swr);

    AVPacket* pkt = av_packet_alloc();
    AVFrame* frame = av_frame_alloc();

    buffer.sampleRate = 44100;

    while (av_read_frame(fmt, pkt) >= 0) {
        if (pkt->stream_index != streamIndex) {
            av_packet_unref(pkt);
            continue;
        }
        avcodec_send_packet(ctx, pkt);

        while (avcodec_receive_frame(ctx, frame) == 0) {
            int out_samples = swr_get_out_samples(swr, frame->nb_samples);

            float* out_buffer = (float*)av_malloc(sizeof(float) * out_samples);
            
            uint8_t* out_arr[1] = {(uint8_t*)out_buffer};

            int samples = swr_convert(
                swr,
                out_arr,
                out_samples,
                (const uint8_t**)frame->data,
                frame->nb_samples
            );

            for (int i = 0; i < samples; i++) {
                buffer.samples.push_back(out_buffer[i]);
            }

            av_free(out_buffer);
        }

        av_packet_unref(pkt);
    }

    av_frame_free(&frame);
    av_packet_free(&pkt);
    swr_free(&swr);
    avcodec_free_context(&ctx);
    avformat_close_input(&fmt);

    return buffer;
}