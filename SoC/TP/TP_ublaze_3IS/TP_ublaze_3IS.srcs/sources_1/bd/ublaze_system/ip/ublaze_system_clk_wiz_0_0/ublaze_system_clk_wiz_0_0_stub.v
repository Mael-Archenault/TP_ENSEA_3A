// Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2019.1.2 (win64) Build 2615518 Fri Aug  9 15:55:25 MDT 2019
// Date        : Fri Nov  7 15:24:43 2025
// Host        : c306-pc1 running 64-bit major release  (build 9200)
// Command     : write_verilog -force -mode synth_stub
//               u:/Documents/SoC/TP/TP_ublaze_3IS/TP_ublaze_3IS.srcs/sources_1/bd/ublaze_system/ip/ublaze_system_clk_wiz_0_0/ublaze_system_clk_wiz_0_0_stub.v
// Design      : ublaze_system_clk_wiz_0_0
// Purpose     : Stub declaration of top-level module interface
// Device      : xc7a35tcpg236-1
// --------------------------------------------------------------------------------

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// The synthesis directives are for Synopsys Synplify support to prevent IO buffer insertion.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
module ublaze_system_clk_wiz_0_0(clk_out1, reset, locked, clk_in1)
/* synthesis syn_black_box black_box_pad_pin="clk_out1,reset,locked,clk_in1" */;
  output clk_out1;
  input reset;
  output locked;
  input clk_in1;
endmodule
