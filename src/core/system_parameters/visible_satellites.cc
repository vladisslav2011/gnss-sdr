/*!
 * \file visible_satellites.cc
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


#include "visible_satellites.h"

Visible_Satellites::Map_Type Visible_Satellites::items{};

Visible_Satellites::Visible_Satellites()
{
}

const Visible_Satellites::Map_Type& Visible_Satellites::get()
{
    return items;
}

void Visible_Satellites::add(char system, int PRN, double az, double el)
{
    uint32_t key = PRN | (static_cast<uint32_t>(system) << 24);
    auto found = items.find(key);
    if (found == items.end())
        {
            items.emplace(key, Item_Type({system, PRN, az, el}));
        }
    else
        {
            found->second.az = az;
            found->second.el = el;
        }
}

void Visible_Satellites::remove(char system, int PRN)
{
    uint32_t key = PRN | (static_cast<uint32_t>(system) << 24);
    items.erase(key);
}
