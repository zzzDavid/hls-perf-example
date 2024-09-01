
//===------------------------------------------------------------*- C++ -*-===//
//
// Automatically generated file for High-level Synthesis (HLS).
//
//===----------------------------------------------------------------------===//
#include <algorithm>
#include <ap_axi_sdata.h>
#include <ap_fixed.h>
#include <ap_int.h>
#include <hls_math.h>
#include <hls_stream.h>
#include <math.h>
#include <stdint.h>
using namespace std;
void mm1(
  float v0[200][240],
  float v1[240][220],
  float v2[200][220]
) {	// L2
#pragma HLS array_partition variable=v1 type=cyclic factor=55 dim=2
#pragma HLS array_partition variable=v2 type=cyclic factor=55 dim=2
  l_mm1_i0: for (int i0 = 0; i0 < 200; i0++) {	// L3
    float v4[220];	// L4
    #pragma HLS array_partition variable=v4 type=cyclic factor=55 dim=1
    l_j0_init: for (int j0_init = 0; j0_init < 220; j0_init++) {	// L6
    #pragma HLS unroll factor=55
    #pragma HLS pipeline II=1
      v4[j0_init] = 0.000000;	// L7
    }
    l_S_k0_0_k0: for (int k0 = 0; k0 < 240; k0++) {	// L9
      l_j0: for (int j0 = 0; j0 < 220; j0++) {	// L10
      #pragma HLS pipeline II=1
      #pragma HLS unroll factor=55
        float v8 = v0[i0][k0];	// L11
        float v9 = v1[k0][j0];	// L12
        float v10 = v8 * v9;	// L13
        float v11 = v4[j0];	// L14
        float v12 = v11 + v10;	// L15
        v4[j0] = v12;	// L16
      }
    }
    l_j0_back: for (int j0_back = 0; j0_back < 220; j0_back++) {	// L19
    #pragma HLS unroll factor=55
    #pragma HLS pipeline II=1
      float v14 = v4[j0_back];	// L20
      v2[i0][j0_back] = v14;	// L21
    }
  }
}


void kernel_gemm(
  float v24[200][240],
  float v25[240][220],
  float out_AB[200][220]
) {	// L39
  mm1(v24, v25, out_AB);	// L41
}

