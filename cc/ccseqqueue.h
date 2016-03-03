
#ifndef CCSEQQUEUE_H
#define CCSEQQUEUE_H

#include <ccconfig.h>
#include <cctypedef.h>
#include <ccpublic.h>
#include <ccatomic.h>
#include <cassert>

//cc namespace
ccNamespaceDeclare(cc)

/*
 * 队列标志
 * 队列使用四个标志来确保元素的push以及pop正确
*/
enum flags_value_t
{
    FV_CAN_PUT = 1,
    FV_PUT_NOW = 2,
    FV_CAN_GET = 3,
    FV_GET_NOW = 4,
};

//初始化值
const size_t SQ_INIT_MAX_LEN = 1024;
const int SQ_INIT_CAS_WAIT = 200;//ms

//队列
template <typename T, typename PT = cc::uint32_t>
class SeqQueue
{
public:
	typedef cc::Atomic<cc::uint8_t>		flag_t;
	typedef cc::Atomic<PT>	pos_t;

public:
	SeqQueue(const size_t max_len = SQ_INIT_MAX_LEN, const int cas_wait = SQ_INIT_CAS_WAIT)
        : m_cas_wait(cas_wait),
          m_front(0),
          m_back(0),
          m_length(__formatLength(max_len)),
          m_sq(nullptr),
          m_flags(nullptr)
    {}

    ~SeqQueue()
    {
        __release();
    }

	inline size_t
	size() const
    {
        return m_length;
    }

	void
	init()
    {
        try{
            if(!m_sq)
                m_sq = new T[m_length];
			if(!m_flags){
				m_flags =	\
				reinterpret_cast<flag_t*>(operator new(m_length * sizeof(flag_t)));
				for (size_t i = 0;i < m_length;++ i) {
					new(m_flags + i) flag_t(FV_CAN_PUT);
				}
			}
        }catch(...){
            __release();
            throw;
        }
    }

	void
	push(const T& ref)
    {
        assert(m_sq);

        uint8_t U_CAN_PUT = FV_CAN_PUT;

        //获取当前的下一个pos
        register PT curr_pos = m_front.inc() % m_length;

        //判断元素是否可以赋值	
        while(! ((m_flags + curr_pos)->cas(&U_CAN_PUT, FV_PUT_NOW))) {
			cc::microSleep(m_cas_wait);
#if __cplusplus >= 201103L
            U_CAN_PUT = FV_CAN_PUT;
#endif
		}

        //给当前元素赋值
        *(m_sq + curr_pos)      = ref;
		(m_flags + curr_pos)->set(FV_CAN_GET);
    }

	void
	pop(T& ref)
    {
        assert(m_sq);

        uint8_t U_CAN_GET = FV_CAN_GET;

        register PT curr_pos = m_back.inc() % m_length;

        while(! ((m_flags + curr_pos)->cas(&U_CAN_GET, FV_GET_NOW))) {
			cc::microSleep(m_cas_wait);
#if __cplusplus >= 201103L
            U_CAN_GET = FV_CAN_GET;
#endif
		}

        ref = *(m_sq + curr_pos);
		(m_flags + curr_pos)->set( FV_CAN_PUT);
    }

protected:
    /*
     * 将长度格式化成2的幂次倍，这样下标的位置就会在0-max之间循环
    */
	inline size_t
	__formatLength(size_t len)
    {
		size_t ret = 1;

        while(ret < len){
            ret <<= 1;
        }

        return ret;
    }

	inline void
	__release()
    {
        delete[] m_sq;
		for (size_t i = 0;i < m_length;++ i) {
			(m_flags + i)->~Atomic();
		}
        delete m_flags;
    }

private:
    //禁止拷贝 赋值
    SeqQueue(const SeqQueue&);
    SeqQueue& operator = (const SeqQueue&);

protected:
	int		m_cas_wait; //cas需要等待的时间

	pos_t   m_front;    //队列push-pos
	pos_t   m_back;     //队列pop-pos

	size_t  m_length;   //队列长度

	T*      m_sq;       //队列元素数组
	flag_t* m_flags;    //队列元素标志
};

//cc namespace
ccNamespaceEnd(cc)

#endif // SEQQUEUE_H

