#ifndef GTP_INL
#define GTP_INL
inline u_int8_t gtpMsgHeader::version()const{
    return m_version >> 5;
}

inline void gtpMsgHeader::version(u_int8_t version){
    if(7 < version){
        std::cout << "invalid gtp version in header...exiting" << std::endl;
        exit(1);
    }
    m_version = version;
    m_version = m_version << 5;
}

inline gtpMsgType_t gtpMsgHeader::msgType()const{
    return m_msgType;
}
inline void gtpMsgHeader::msgType(gtpMsgType_t msgType){
    m_msgType = msgType;
}

inline u_int16_t gtpMsgHeader::len()const{
    return ntohs(m_len);
}
   
inline void gtpMsgHeader::len(u_int16_t len){
    m_len = htons(len);
}

inline u_int8_t gtpMsgHeader::seq()const{
    return m_seq[2];
}

inline void gtpMsgHeader::seq(u_int8_t seq){
    m_seq[0] = 0;
    m_seq[1] = 0;
    m_seq[2] = seq;
}
 
#endif //GTP_INL
