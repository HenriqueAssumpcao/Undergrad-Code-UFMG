`timescale 1ns/1ns 
`include "feedback_register.v"
module feedback_test;
  reg clk,clr;
  wire out;
  integer i;
  feedback_register SR(clk,clr,out);
  
  initial
    begin clk=1'b0;#2 clr=0;#5 clr=1; end
  always #5 clk=~clk;
  initial
    begin
      $dumpfile("feedback_register.vcd");
      $dumpvars(0,feedback_test);
      #100 $finish;
    end
endmodule