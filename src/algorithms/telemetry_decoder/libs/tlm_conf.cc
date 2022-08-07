/*!
 * \file tlm_conf.cc
 * \brief Class that contains all the configuration parameters for generic
 * telemetry decoder block.
 * \author Carles Fernandez, 2020. cfernandez(at)cttc.es
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

#include "tlm_conf.h"
#include <utility>


void Tlm_Conf::SetFromConfiguration(const ConfigurationInterface *configuration,
    const std::string &role)
{
    const std::string default_dumpname("telemetry");
    dump_filename = configuration->property(role + ".dump_filename", default_dumpname);
    dump = configuration->property(role + ".dump", false);
    dump_mat = configuration->property(role + ".dump_mat", dump);
    remove_dat = configuration->property(role + ".remove_dat", false);
    dump_crc_stats = configuration->property(role + ".dump_crc_stats", false);
    const std::string default_crc_stats_dumpname("telemetry_crc_stats");
    dump_crc_stats_filename = configuration->property(role + ".dump_crc_stats_filename", default_crc_stats_dumpname);
    enable_navdata_monitor = configuration->property("NavDataMonitor.enable_monitor", false);
    if (configuration->property("Channels_1B.count", 0) > 0)
        {
            there_are_e1_channels = true;
        }

    if (configuration->property("Channels_E6.count", 0) > 0)
        {
            there_are_e6_channels = true;
        }
    tow_to_trk = configuration->property("GNSS-SDR.tow_to_trk", false);
    std::string override_str = configuration->property(role + ".override_health", std::string(""));
    size_t s=0;
    size_t e=0;
    if (override_str.size())
        {
            while (true)
                {
                    e = override_str.find(",", s);
                    if(e == std::string::npos)
                    {
                        override_health[std::stoi(override_str.substr(s, override_str.size() - s))] = true;
                        break;
                    }
                    override_health[std::stoi(override_str.substr(s, e - s))] = true;
                    s = e + 1;
                }
        }
    ecc_errors_reject = configuration->property(role + ".ecc_reject", 1);
    ecc_errors_reject = (ecc_errors_reject < 1) ? 1 : ecc_errors_reject;
    ecc_errors_resync = configuration->property(role + ".ecc_resync", 6);
    ecc_errors_resync = (ecc_errors_resync < 1) ? 1 : ecc_errors_resync;
}
