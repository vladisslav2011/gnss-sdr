/*!
 * \file beidou_cnav1_ldpc.h
 * \brief B-CNAV1 NB-LDPC decoder (BDS-SIS-ICD-B1C-1.0 §6.2.2, Appendix)
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef GNSS_SDR_BEIDOU_CNAV1_LDPC_DECODER_H
#define GNSS_SDR_BEIDOU_CNAV1_LDPC_DECODER_H

#include "Beidou_CNAV1_ldpc.h"
#include <cstddef>
#include <cstdint>
#include <vector>

bool beidou_cnav1_ldpc_decode_200_100(const float* symbol_llr, int32_t num_bits, uint8_t* info_bits600);
bool beidou_cnav1_ldpc_decode_88_44(const float* symbol_llr, int32_t num_bits, uint8_t* info_bits264);
bool beidou_cnav1_ldpc_decode_200_100_codeword(const float* symbol_llr, int32_t num_bits, uint8_t* codeword_bits1200);
bool beidou_cnav1_ldpc_decode_88_44_codeword(const float* symbol_llr, int32_t num_bits, uint8_t* codeword_bits528);

namespace GaloisField64
{
constexpr uint8_t kZero = 0U;
constexpr uint8_t kOrder = 63U;
constexpr uint8_t kFieldSize = 64U;

// LUT values come from BDS-SIS-ICD-B1C-1.0 Appendix GF(2^6) mapping rules.
inline bool valid_symbol(uint8_t x)
{
    return x < kFieldSize;
}

inline uint8_t add(uint8_t a, uint8_t b)
{
    if (!valid_symbol(a) || !valid_symbol(b))
        {
            return kZero;
        }
    return static_cast<uint8_t>(a ^ b);
}

inline uint8_t mul(uint8_t a, uint8_t b)
{
    if (!valid_symbol(a) || !valid_symbol(b) || a == kZero || b == kZero)
        {
            return kZero;
        }
    const int32_t log_sum = static_cast<int32_t>(BEIDOU_CNAV1_GF64_LOG[a]) +
                            static_cast<int32_t>(BEIDOU_CNAV1_GF64_LOG[b]);
    return BEIDOU_CNAV1_GF64_EXP[log_sum % kOrder];
}

inline uint8_t inv(uint8_t a)
{
    if (!valid_symbol(a) || a == kZero)
        {
            return kZero;
        }
    const int32_t exponent = static_cast<int32_t>(kOrder) - static_cast<int32_t>(BEIDOU_CNAV1_GF64_LOG[a]);
    return BEIDOU_CNAV1_GF64_EXP[exponent % kOrder];
}
}  // namespace GaloisField64

struct BeidouCnav1LdpcGraph
{
    int32_t num_checks = 0;
    int32_t num_variables = 0;
    int32_t row_weight = 0;

    // Check-node (CSR-like) adjacency: [check_offsets[i], check_offsets[i+1]).
    std::vector<uint32_t> check_offsets;
    std::vector<uint16_t> check_to_var;
    std::vector<uint8_t> check_to_h;

    // Variable-node (CSC-like) adjacency: [var_offsets[j], var_offsets[j+1]).
    std::vector<uint32_t> var_offsets;
    std::vector<uint16_t> var_to_check;
    std::vector<uint32_t> var_to_edge;
    std::vector<uint8_t> var_to_h;
    std::vector<uint8_t> var_to_h_inv;
};

bool beidou_cnav1_ldpc_init_graph(
    int32_t num_checks,
    int32_t num_variables,
    int32_t row_weight,
    const uint16_t* icd_index,
    const uint8_t* icd_element,
    int32_t num_entries,
    BeidouCnav1LdpcGraph& graph);

const BeidouCnav1LdpcGraph& beidou_cnav1_ldpc_graph_200_100();
const BeidouCnav1LdpcGraph& beidou_cnav1_ldpc_graph_88_44();

#endif  // GNSS_SDR_BEIDOU_CNAV1_LDPC_DECODER_H
