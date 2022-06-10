`timescale 1ns/1ns 
`include "lfsr.v"
module lfsr_testbench_8bit;
    reg clk;
    reg n_rst;
    reg [7:0] polinomio;
    reg [7:0] reset;
    wire [7:0] test_computed_value;
    integer posedgectr;

    lfsr_8bit lfsr_iut(.clock(clk),
                  .reset(n_rst),
                  .taps(polinomio),
                  .reset_value(reset),
                  .computed_value(test_computed_value));
    initial
    begin
        #0 
        $monitor("LSFR_8BIT (clock_atual=%d): %d", posedgectr, test_computed_value);
        $dumpfile("test_8.vcd");
	    $dumpvars(0, lfsr_testbench_8bit);

        n_rst  = 1;
        clk    = 0;
        posedgectr= 0;

        #10
        reset = 8'b11111111;
        polinomio = 8'b10111000;
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
