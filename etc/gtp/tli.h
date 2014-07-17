#ifndef TLI_H
#define TLI_H
//! modelling the type length instance header of information elements
class TLI{
public:
    TLI(){}
    ~TLI(){}

    void type(u_int8_t type){
        m_type = type;
    }
    u_int8_t type()const{
        return m_type;
    }
    void len(u_int16_t len){
        m_len = htons(len);
    }
    u_int16_t len()const{
        return ntohs(m_len);
    }
    void inst(u_int8_t inst){
        m_inst = inst;
    }
    u_int8_t inst()const{
        return m_inst;
    }

private:

    u_int8_t    m_type;
    u_int16_t   m_len;
    u_int8_t    m_inst;

}__attribute__((packed));

#endif //TLI_H
