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
    vorbis_comment_init(&vc);

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
    err = ogg_stream_packetin(&os,&header);
    if(err!=0){return err;}                                         
    err = ogg_stream_packetin(&os,&header_comm);
    if(err!=0){return err;}    
    err = ogg_stream_packetin(&os,&header_code);
    if(err!=0){return err;}    

    while(!eos){
      int result=ogg_stream_flush(&os,&og);
      if(result==0)break;
      outfile.write((const char*)og.header,og.header_len);
      outfile.write((const char*)og.body,og.body_len);
    }
    return 0;
}

void VorbisEncoder::add(const std::vector<float> pcmData){  
    float ** buffer=vorbis_analysis_buffer(&vd,pcmData.size()/channels);
    
    unsigned int c = 0;
    for(int i = 0; i<pcmData.size()/channels;i++){
        for(int j = 0; j<channels;j++){
            buffer[j][i]=pcmData[c];
            c++;
        }
    }
    vorbis_analysis_wrote(&vd,pcmData.size()/channels);    

    while(vorbis_analysis_blockout(&vd,&vb)>0){
        
        vorbis_analysis(&vb,NULL);
        vorbis_bitrate_addblock(&vb);

        while(vorbis_bitrate_flushpacket(&vd,&op)){
            ogg_stream_packetin(&os,&op);
            int result;
            while((result = ogg_stream_pageout(&os,&og)) > 0){
                outfile.write((const char*)og.header,og.header_len);
                outfile.write((const char*)og.body,og.body_len);
            }
        }
    }          
}

bool VorbisEncoder::close(){
    vorbis_analysis_wrote(&vd,0);
    while(vorbis_analysis_blockout(&vd,&vb)>0){        
        vorbis_analysis(&vb,NULL);
        vorbis_bitrate_addblock(&vb);

        while(vorbis_bitrate_flushpacket(&vd,&op)){
            ogg_stream_packetin(&os,&op);
            int result;
            while((result = ogg_stream_pageout(&os,&og)) > 0){
                outfile.write((const char*)og.header,og.header_len);
                outfile.write((const char*)og.body,og.body_len);
            }
        }
    }          
    
    int result=ogg_stream_flush(&os,&og);
    if(result!=0){
        outfile.write((const char*)og.header,og.header_len);
        outfile.write((const char*)og.body,og.body_len);
    }
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