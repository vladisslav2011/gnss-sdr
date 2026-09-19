/*!
 * \file object_pool.h
 * \brief Generic thread-safe object pool interface
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

#ifndef GNSS_SDR_OBJECT_POOL_H_
#define GNSS_SDR_OBJECT_POOL_H_

#include <mutex>
#include <vector>
#include <iostream>

/** \addtogroup Algorithms_Library
 * \{ */
/** \addtogroup Algorithm_libs algorithms_libs
 * \{ */

template<class T> class ObjectPool
{
public:
    ObjectPool(ObjectPool &) = delete;
    ObjectPool(ObjectPool &&) = delete;
    ObjectPool(const ObjectPool &) = delete;
    ObjectPool & operator=(const ObjectPool &) = delete;
    ObjectPool & operator=(ObjectPool &) = delete;
    ObjectPool & operator=(ObjectPool &&) = delete;
    static T take()
    {
        std::lock_guard<std::mutex> lock(get().d_mutex);
        T rv = std::move(get().d_store.back());
        get().d_store.pop_back();
        //std::cout<<"take: "<<(&rv)<<" s="<<rv.size()<<" d="<<rv.data()<<"\n";
        return rv;
    }
    static void release(T&& item)
    {
        std::lock_guard<std::mutex> lock(get().d_mutex);
        //std::cout<<"release: "<<(&item)<<" s="<<item.size()<<" d="<<item.data()<<"\n";
        get().d_store.emplace_back(std::move(item));
    }
    static void resize(size_t n)
    {
        std::lock_guard<std::mutex> lock(get().d_mutex);
        get().d_store.resize(n);
    }
    static void reserve(size_t n)
    {
        std::lock_guard<std::mutex> lock(get().d_mutex);
        get().d_store.reserve(n);
    }
    static size_t size()
    {
        std::lock_guard<std::mutex> lock(get().d_mutex);
        return get().d_store.size();
    }
    static void foreach(std::function<void(T&)> fn)
    {
        std::lock_guard<std::mutex> lock(get().d_mutex);
        for(auto& it:get().d_store)
            fn(it);
    }
    static ObjectPool& get()
    {
        static ObjectPool inst{};
        return inst;
    }
    std::vector<T> d_store{};
    std::mutex d_mutex{};
protected:
    ObjectPool() = default;
};

/** \} */
/** \} */
#endif  // GNSS_SDR_OBJECT_POOL_H_
