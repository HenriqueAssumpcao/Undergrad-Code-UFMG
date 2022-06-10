`timescale 1ns/1ns 
`include "lfsr.v"
module lfsr_testbench_4bit;
    reg clk;
    reg n_rst;
    reg [3:0] polinomio;
    reg [3:0] reset;
    wire [3:0] test_computed_value;
    integer posedgectr;

    lfsr_4bit lfsr_iut(.clock(clk),
                  .reset(n_rst),
                  .taps(polinomio),
                  .reset_value(reset),
                  .computed_value(test_computed_value));
    initial
    begin
        #0 
        $monitor("LSFR_4BIT (clock_atual=%d): %d", posedgectr, test_computed_value);
        $dumpfile("test_4.vcd");
	    $dumpvars(0, lfsr_testbench_4bit);

        n_rst  = 1;
        clk    = 0;
        posedgectr= 0;

        #10
        reset = 4'b1111;
        polinomio = 4'b1100;
        n_rst = 0;

        #10
        n_rst = 1;

        #512
        $finish;
    end

    always #1 begin
        clk = !clk;
        if (clk) posedgectr = posedgectr + 1;
    end
endmodule
