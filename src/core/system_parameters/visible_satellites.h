/*!
 * \file visible_satellites.h
 * \brief  Visible satellite list
 * \author Vladislav P, 2026. vladisslav2011@gmail.com
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


#ifndef GNSS_SDR_VISIBLE_SATELLITES
#define GNSS_SDR_VISIBLE_SATELLITES

#include <cstdint>
#include <map>

/** \addtogroup Core
 * \{ */
/** \addtogroup System_Parameters
 * \{ */


class Visible_Satellites
{
public:
    using Item_Type = struct {char system; int PRN; double az; double el;};
    using Map_Type = std::map<uint32_t,Item_Type>;
    static const Map_Type & get();
    static void add(char system, int PRN, double az, double el);
    static void remove(char system, int PRN);
    inline Visible_Satellites(const Visible_Satellites&) = delete;
    inline void operator=(const Visible_Satellites&) = delete;

private:
    static Map_Type items;
    Visible_Satellites();
};

/** \} */
/** \} */
#endif  // GNSS_SDR_VISIBLE_SATELLITES
