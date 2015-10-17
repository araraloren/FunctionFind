#ifndef CCATOMIC_H
#define CCATOMIC_H

#include <ccconfig.h>

#if __cplusplus >= 201103L
#   include <atomic>
#endif


ccNamespaceDeclare(cc)

//namespace {
    //a simple atmoic
    template <typename AT>
    class Atomic
    {
    public:
        Atomic()
            :data_(0)
        {

        }

        explicit Atomic(AT value)
            :data_(value)
        {

        }

		~Atomic()
		{

		}

		inline AT
        get() const
		{
#if __cplusplus >= 201103L
			return std::atomic_load(&data_);
#else
			return data_;
#endif
		}

		inline void
		set(AT value)
		{
#if __cplusplus >= 201103L
			return std::atomic_store(&data_, value);
#else
			data_ = value;
#endif
		}

        //fetch and add
        inline AT
        inc()
        {
#if __cplusplus >= 201103L
            return std::atomic_fetch_add(&data_, (AT)1);
#else
#   if (defined CC_PF_LINUX) || (defined CC_PF_MINGW32) && (defined CC_C_GCC)
            return __sync_fetch_and_add(&data_, 1);
#   elif (defined CC_PF_WIN32) || (defined CC_PF_MFC)
            return InterlockedExchangeAdd(&data_, 1);
#   endif
#endif
        }

        //fetch and sub
        inline AT
        dec()
        {
#if __cplusplus >= 201103L
            return std::atomic_fetch_sub(&data_, (AT)1);
#else
#   if (defined CC_PF_LINUX) || (defined CC_PF_MINGW32) && (defined CC_C_GCC)
            return __sync_fetch_and_sub(&data_, 1);
#   elif (defined CC_PF_WIN32) || (defined CC_PF_MFC)
            return InterlockedExchangeAdd(&data_, -1);
#   endif
#endif
        }

        inline AT
        add(AT value)
        {
#if __cplusplus >= 201103L
            return std::atomic_fetch_add(&data_, value);
#else
#   if (defined CC_PF_LINUX) || (defined CC_PF_MINGW32) && (defined CC_C_GCC)
            return __sync_fetch_and_add(&data_, value);
#   elif (defined CC_PF_WIN32) || (defined CC_PF_MFC)
            return InterlockedExchangeAdd(&data_, value);
#   endif
#endif
        }

        inline AT
        sub(AT value)
        {
#if __cplusplus >= 201103L
            return std::atomic_fetch_sub(&data_, value);
#else
#   if (defined CC_PF_LINUX) || (defined CC_PF_MINGW32) && (defined CC_C_GCC)
            return __sync_fetch_and_sub(&data_, 1);
#   elif (defined CC_PF_WIN32) || (defined CC_PF_MFC)
            return InterlockedExchangeAdd(&data_, -value);
#   endif
#endif
        }

        //warning !!
        //c++11 will change (*compare)
        inline AT
        cas(AT* compare, AT set)
        {
#if __cplusplus >= 201103L
            return std::atomic_compare_exchange_strong(&data_, compare, set);
#else
#   if (defined CC_PF_LINUX) || (defined CC_PF_MINGW32) && (defined CC_C_GCC)
            return __sync_bool_compare_and_swap(&data_, *compare, set);
#   elif (defined CC_PF_WIN32) || (defined CC_PF_MFC)
            return (*compare == InterlockedCompareExchange(&data_, set, *compare))
#   endif
#endif
        }

    protected:
#if __cplusplus >= 201103L
        std::atomic<AT> data_;
#else
        AT  data_;
#endif
    };
//}


ccNamespaceEnd(cc)

#endif // CCATOMIC_H
