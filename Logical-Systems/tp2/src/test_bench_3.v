`timescale 1ns/1ns 
`include "johnson_counter.v"
module johnson_test;
  reg clk,clr;
  wire out;
  integer i;
  johnson_counter SR(clk,clr,out);
  
  initial
    begin clk=1'b0;#2 clr=0;#5 clr=1; end
  always #5 clk=~clk;
  
  initial
    begin
      $dumpfile("johnson_counter.vcd");
      $dumpvars(0,johnson_test);
      #200 $finish;
    end
endmodule