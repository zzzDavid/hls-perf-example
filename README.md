# Example: HLS Performance Different than Expectation

This example demonstrates a scenario where the performance of HLS (High-Level Synthesis) may differ from initial expectations. The example focuses on matrix multiplication with accumulation interleaving.

In this context, accumulation interleaving involves reordering the reduction loop and introducing an intermediate buffer to hold partial sums. 


### Version 1: Pipelining the Reduction Loop
The first approach attempts to pipeline the reduction loop. The following code snippet illustrates this:

```cpp
  l_mm1_i0: for (int i0 = 0; i0 < 200; i0++) {	// L3
    float v4[220];	// L4
    #pragma HLS array_partition variable=v4 complete dim=1
    l_j0_init: for (int j0_init = 0; j0_init < 220; j0_init++) {	// L6
    #pragma HLS unroll
      v4[j0_init] = 0.000000;	// L7
    }
    l_S_k0_0_k0: for (int k0 = 0; k0 < 240; k0++) {	// L9
    #pragma HLS pipeline II=1
      l_j0: for (int j0 = 0; j0 < 220; j0++) {	// L10
        float v8 = v0[i0][k0];	// L11
        float v9 = v1[k0][j0];	// L12
        float v10 = v8 * v9;	// L13
        float v11 = v4[j0];	// L14
        float v12 = v11 + v10;	// L15
        v4[j0] = v12;	// L16
      }
    }
    l_j0_back: for (int j0_back = 0; j0_back < 220; j0_back++) {	// L19
    #pragma HLS unroll
      float v14 = v4[j0_back];	// L20
      v2[i0][j0_back] = v14;	// L21
    }
  }
```

```text
+------------------------+---------+---------+----------+-----------+-----------+-------+----------+
|                        |  Latency (cycles) | Iteration|  Initiation Interval  |  Trip |          |
|        Loop Name       |   min   |   max   |  Latency |  achieved |   target  | Count | Pipelined|
+------------------------+---------+---------+----------+-----------+-----------+-------+----------+
|- l_mm1_i0_l_S_k0_0_k0  |   192011|   192011|        16|          4|          1|  48000|       yes|
+------------------------+---------+---------+----------+-----------+-----------+-------+----------+
```

In this version, the Initiation Interval (II) is 4. This is expected due to the dependency between these lines:


```cpp
float v11 = v4[j0];	// L14
float v12 = v11 + v10;	// L15
v4[j0] = v12;	// L16
```

Since each k0 iteration reads and writes the same `v4[j0]` location, the next k0 iteration cannot start immediately (II=1). Floating-point addition takes 4 cycles, so we can only start the next iteration of the k0 loop after 4 cycles to maintain the data dependency on `v4[j0]`.

### Version 2: Unrolling the j0 Loop

To achieve II=1 for the `L_S_k0_0_k0` loop, a reasonable approach is to unroll the j0 loop by a factor of 55. This ensures that for each `k0` iteration, there are 4 `j0` loop iterations after unrolling, allowing the `l_j0` loop to be pipelined with II=1.


That gives us version 2:

```cpp
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
```

```text
+--------------------+---------+---------+----------+-----------+-----------+------+----------+
|                    |  Latency (cycles) | Iteration|  Initiation Interval  | Trip |          |
|      Loop Name     |   min   |   max   |  Latency |  achieved |   target  | Count| Pipelined|
+--------------------+---------+---------+----------+-----------+-----------+------+----------+
|- l_S_k0_0_k0_l_j0  |     1932|     1932|        15|          2|          1|   960|       yes|
+--------------------+---------+---------+----------+-----------+-----------+------+----------+
```

From the report, we see that the `l_j0` loop is pipelined and fused with `l_S_k0_0_k0` (as indicated by the loop name and trip count). However, the loop is pipelined with II=2, resulting in a higher overall latency (1.3 ms) compared to version 1 (0.639 ms).

I expected Version 2 to achieve pipelining with an II=1, but why didn't it?

## Reproduce the results

```
source /opt/xilinx/2022.1/Vitis_HLS/2022.1/settings64.sh
vitis_hls -f version1.tcl
vitis_hls -f version2.tcl
```