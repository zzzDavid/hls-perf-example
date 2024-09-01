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
void mm(
  double v0[1024][1024],
  double v1[1024][1024],
  double v2[1024][1024]
) {     // L2
  double v5[1024];      // L4
  #pragma HLS array_partition variable=v5 cyclic factor=32 dim=1
  #pragma HLS array_partition variable=v1 cyclic factor=32 dim=2
  #pragma HLS array_partition variable=v2 cyclic factor=32 dim=2
  i: for (int i0 = 0; i0 < 1024; i0++) {    // L3
    j_init: for (int j0_init = 0; j0_init < 1024; j0_init++) {        // L6
    #pragma HLS pipeline II=1
    #pragma HLS unroll factor=32
      v5[j0_init] = v2[i0][j0_init];   // L7
    }
    k: for (int k0 = 0; k0 < 1024; k0++) {     // L9
      double v9 = v0[i0][k0];  // L11
      l_j0: for (int j0 = 0; j0 < 1024; j0++) {  // L10
      #pragma HLS pipeline II=1
      #pragma HLS unroll factor=32
        v5[j0] += v9 * v1[k0][j0];   // L17
      }
    }
    j_back: for (int j0_back = 0; j0_back < 1024; j0_back++) {        // L20
    #pragma HLS pipeline II=1
    #pragma HLS unroll factor=32
      double v16 = v5[j0_back];  // L21
      v2[i0][j0_back] = v16;  // L22
    }
  }
}

void mm2(
  double v0[1024][1024],
  double v1[1024][1024],
  double v2[1024][1024]
) {     // L2
  double v5[1024];      // L4
  #pragma HLS array_partition variable=v5 complete dim=1
  #pragma HLS array_partition variable=v1 complete dim=2
  #pragma HLS array_partition variable=v2 complete dim=2
  i: for (int i0 = 0; i0 < 1024; i0++) {    // L3
    j_init: for (int j0_init = 0; j0_init < 1024; j0_init++) {        // L6
    #pragma HLS unroll
      v5[j0_init] = v2[i0][j0_init];   // L7
    }
    k: for (int k0 = 0; k0 < 1024; k0++) {     // L9
    #pragma HLS pipeline II=4
      double v9 = v0[i0][k0];  // L11
      l_j0: for (int j0 = 0; j0 < 1024; j0++) {  // L10
        v5[j0] += v9 * v1[k0][j0];   // L17
      }
    }
    j_back: for (int j0_back = 0; j0_back < 1024; j0_back++) {        // L20
    #pragma HLS unroll
      double v16 = v5[j0_back];  // L21
      v2[i0][j0_back] = v16;  // L22
    }
  }
}


void kernel_gemm(
  double v26[1024][1024],
  double v27[1024][1024],
  double v28[1024][1024]
) {     // L39
  mm(v26, v27, v28);    // L41
}