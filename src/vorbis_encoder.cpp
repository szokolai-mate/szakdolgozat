#include <VorbisEncoder.h>

VorbisEncoder::VorbisEncoder(const unsigned int & channels,const unsigned int & sampleRate, const float & quality) : channels(channels) , sampleRate(sampleRate){
    vorbis_info_init(&vi);
    float internalQuality;
    if(quality>1.0f){
        internalQuality = 1.0f;
    }
    if(quality<-0.1f){
        internalQuality = -0.1f;
    }
    vorbis_encode_init_vbr(&vi,channels,sampleRate,internalQuality);
    vorbis_analysis_init(&vd,&vi);
    vorbis_block_init(&vd,&vb);

    //OGG
    srand(time(NULL));
    ogg_stream_init(&os,rand());
}

bool VorbisEncoder::open(const std::string & filename){
    outfile.open(filename, std::ios::binary);
    return outfile.is_open();
}

void VorbisEncoder::addComment(std::string key,std::string value){
    vorbis_comment_add_tag(&vc,key.c_str(),value.c_str());    
}

int VorbisEncoder::initEncoding(){
    ogg_packet header;
    ogg_packet header_comm;
    ogg_packet header_code;

    int err;

    err = vorbis_analysis_headerout(&vd,&vc,&header,&header_comm,&header_code);
    if(err!=0){return err;}
    err = ogg_stream_packetin(&os,&header); /* automatically placed in its own
                                         page */
    if(err!=0){return err;}                                         
    err = ogg_stream_packetin(&os,&header_comm);
    if(err!=0){return err;}    
    err = ogg_stream_packetin(&os,&header_code);
    if(err!=0){return err;}    

    /* This ensures the actual
     * audio data will start on a new page, as per spec
     */
    while(!eos){
      int result=ogg_stream_flush(&os,&og);
      if(result==0)break;
      outfile.write((const char*)og.header,og.header_len);
      outfile.write((const char*)og.body,og.body_len);
    }
}

void VorbisEncoder::add(const std::vector<float> pcmData){
    /* expose the buffer to submit data */
    float **buffer=vorbis_analysis_buffer(&vd,pcmData.size()/channels);
    
    /* uninterleave samples */
    unsigned int c = 0;
    for(int i = 0; i<pcmData.size()/channels;i++){
        for(int j = 0; j<channels;j++){
            buffer[j][i]=pcmData[c];
            c++;
        }
    }
    vorbis_analysis_wrote(&vd,pcmData.size()/channels);

    while(vorbis_analysis_blockout(&vd,&vb)==1){
        
              /* analysis, assume we want to use bitrate management */
              vorbis_analysis(&vb,NULL);
              vorbis_bitrate_addblock(&vb);
        
              while(vorbis_bitrate_flushpacket(&vd,&op)){
        
                /* weld the packet into the bitstream */
                ogg_stream_packetin(&os,&op);
        
                /* write out pages (if any) */
                while(!eos){
                  int result=ogg_stream_pageout(&os,&og);
                  if(result==0)break;
                  outfile.write((const char*)og.header,og.header_len);
                  outfile.write((const char*)og.body,og.body_len);
        
                  /* this could be set above, but for illustrative purposes, I do
                     it here (to show that vorbis does know where the stream ends) */
        
                  if(ogg_page_eos(&og))eos=1;
                }
            }
        }
}

bool VorbisEncoder::close(){
    vorbis_analysis_wrote(&vd,0);
    outfile.close();
    ogg_stream_clear(&os);
    vorbis_block_clear(&vb);
    vorbis_dsp_clear(&vd);
    vorbis_comment_clear(&vc);
    vorbis_info_clear(&vi);
    return !outfile.is_open();
}

VorbisEncoder::~VorbisEncoder(){
    outfile.close();
    ogg_stream_clear(&os);
    vorbis_block_clear(&vb);
    vorbis_dsp_clear(&vd);
    vorbis_comment_clear(&vc);
    vorbis_info_clear(&vi);
}