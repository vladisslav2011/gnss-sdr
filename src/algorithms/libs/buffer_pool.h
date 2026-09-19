/*!
 * \file buffer_pool.h
 * \brief Generic thread-safe volk_gnsssdr::vector-based buffer pool interface
 * \author Vladislav P, 2026. vladisslav2011(at)gmail.com
 *
 *
 * -----------------------------------------------------------------------------
 *
 * GNSS-SDR is a Global Navigation Satellite System software-defined receiver.
 * This file is part of GNSS-SDR.
 *
 * Copyright (C) 2010-2020  (see AUTHORS file for a list of contributors)
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * -----------------------------------------------------------------------------
 */

#ifndef GNSS_SDR_BUFFER_POOL_H_
#define GNSS_SDR_BUFFER_POOL_H_

#include <volk_gnsssdr/volk_gnsssdr_alloc.h>  // for volk_gnsssdr::vector
#include "object_pool.h"

/** \addtogroup Algorithms_Library
 * \{ */
/** \addtogroup Algorithm_libs algorithms_libs
 * \{ */

template<class T> class BufferPool: public ObjectPool<volk_gnsssdr::vector<T>>
{
public:
    BufferPool(BufferPool &) = delete;
    BufferPool(BufferPool &&) = delete;
    BufferPool(const BufferPool &) = delete;
    BufferPool & operator=(const BufferPool &) = delete;
    BufferPool & operator=(BufferPool &) = delete;
    BufferPool & operator=(BufferPool &&) = delete;
    static void resize_buffers(size_t n)
    {
        resize_buffers_(n);
    }
    static void reserve_buffers(size_t n)
    {
        std::lock_guard<std::mutex> lock(get().d_mutex);
        if (get().d_store.empty())
            {
                return;
            }
        if (get().d_store[0].capacity() >= n)
            {
                return;
            }
        get().foreach ([&](volk_gnsssdr::vector<T> buffer){
            buffer.reserve(n);
        });
    }
    static size_t buffer_size()
    {
        std::lock_guard<std::mutex> lock(get().d_mutex);
        if (get().d_store.empty())
            {
                return 0;
            }
        return get().d_store[0].size();
    }
    static size_t buffer_capacity()
    {
        std::lock_guard<std::mutex> lock(get().d_mutex);
        if (get().d_store.empty())
            {
                return 0;
            }
        return get().d_store[0].capacity();
    }
    using ObjectPool<volk_gnsssdr::vector<T>>::take;
    using ObjectPool<volk_gnsssdr::vector<T>>::release;
private:
    BufferPool():ObjectPool<volk_gnsssdr::vector<T>>()
    {
    }
    void resize_buffers_(size_t n)
    {
        std::lock_guard<std::mutex> lock(d_mutex);
        if (d_store.empty())
            {
                return;
            }
        if (d_store[0].size() == n)
            {
                return;
            }
        foreach ([&](volk_gnsssdr::vector<T> buffer){
            buffer.resize(n);
        });
    }
    using ObjectPool<volk_gnsssdr::vector<T>>::get;
    using ObjectPool<volk_gnsssdr::vector<T>>::d_mutex;
    using ObjectPool<volk_gnsssdr::vector<T>>::d_store;
};

/** \} */
/** \} */
#endif  // GNSS_SDR_BUFFER_POOL_H_
