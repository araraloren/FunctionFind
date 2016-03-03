#ifndef CCSSEQQUEUE_H
#define CCSSEQQUEUE_H

#include <ccseqqueue.h>
#include <ctime>

ccNamespaceDeclare(cc)

//初始化值
const std::size_t SSQ_INIT_MAX_LEN     = 1024;
const int SSQ_INIT_CAS_WAIT = 200;//ms

//队列
template <typename T>
class SSeqQueue: public cc::SeqQueue<T>
{
public:
	SSeqQueue(const std::size_t max_len = SSQ_INIT_MAX_LEN, const int cas_wait = SSQ_INIT_CAS_WAIT)
		:cc::SeqQueue<T>(max_len, cas_wait)
		,size_(0)
    {}

    inline
    std::size_t count() const
    {
		return size_.get();
    }

    void
    push(const T& ref)
    {
        assert(size_.get() != this->cc::SeqQueue<T>::m_length);
		this->cc::SeqQueue<T>::push(ref);
		size_.inc();
    }

    void
    pop(T& ref)
    {
        assert(size_.get() != 0);
		this->cc::SeqQueue<T>::pop(ref);
		size_.dec();
    }

private:
    //禁止拷贝 赋值
	SSeqQueue(const SSeqQueue&);
	SSeqQueue& operator = (const SSeqQueue&);

protected:
	cc::Atomic<size_t>	size_;
};

ccNamespaceEnd(cc)


#endif // SSeqQueue

