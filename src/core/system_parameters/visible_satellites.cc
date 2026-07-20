/*!
 * \file visible_satellites.cc
 * \brief  Visible satellite list
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
    auto found = items.find({system, static_cast<char>(PRN)});
    if (found == items.end())
        {
            items.emplace(std::pair<char, char>({system, static_cast<char>(PRN)}), Item_Type({system, static_cast<char>(PRN), az, el}));
        }
    else
        {
            found->second.az = az;
            found->second.el = el;
        }
}

void Visible_Satellites::remove(char system, int PRN)
{
    items.erase({system, static_cast<char>(PRN)});
}
