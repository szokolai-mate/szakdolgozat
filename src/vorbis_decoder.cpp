#include <VorbisDecoder.h>

//!\todo TODO: errors and exceptions

VorbisDecoder::VorbisDecoder()
{
    vorbis_info_init(&vi);
    vorbis_comment_init(&vc);
}

int VorbisDecoder::addToHeader(ogg_packet &packet)
{
    return vorbis_synthesis_headerin(&vi, &vc, &packet);
}

std::vector<std::string> VorbisDecoder::getComments() const
{
    std::vector<std::string> res;
    char **ptr = vc.user_comments;
    if(!ptr)return res;
    while (*ptr)
    {
        res.push_back(std::string{*ptr});
        ++ptr;
    }
    return res;
}

unsigned int VorbisDecoder::getChannels() const{
    return vi.channels;
}

unsigned int VorbisDecoder::getSampleRate() const{
    return vi.rate;
}

std::string VorbisDecoder::getVendor() const{
    return std::string{vc.vendor};
}

void VorbisDecoder::clear(){
    vorbis_block_clear(&vb);
    vorbis_dsp_clear(&vd);
    vorbis_comment_clear(&vc);
    vorbis_info_clear(&vi);
}

int VorbisDecoder::initDecoding(){
    int err = vorbis_synthesis_init(&vd, &vi);
    vorbis_block_init(&vd, &vb);
    return err;
}

std::vector<float> VorbisDecoder::decode(ogg_packet & packet){
    std::vector<float> res;
    if (vorbis_synthesis(&vb, &packet) != 0)  return res;
    vorbis_synthesis_blockin(&vd, &vb);
    res.reserve(vorbis_packet_blocksize(&vi,&packet));    

    float ** pcm;
    int samples;

    while ((samples = vorbis_synthesis_pcmout(&vd, &pcm))>0) {
        for (int i = 0; i < samples; i++) {
            for (int j = 0; j < vi.channels; j++) {
                res.push_back(pcm[j][i]);
            }
        }
        vorbis_synthesis_read(&vd, samples);                      
    }
    return res;
}


VorbisDecoder::~VorbisDecoder(){
    clear();
}