`timescale 1ns/1ns 
`include "xor_logic.v"

module codificador_tb;

reg [31:0] A,B;
wire [31:0] C;

xor_logic uut(.a(A),.b(B),.c(C));

initial begin
    $dumpfile("codificador_tb.vcd");
    $dumpvars(0,codificador_tb);
    
    A = 32'b01110100011001010111001101110100; //MESSAGE: test
    B = 32'b10001010000101100100110110111000; #20; //OTP
    // RESULT: b11111110011100110011111011001100
    $display("Iniciando codificacao do input em A");
    $display("Palavra test em ASCII = %b",A);
    $display("OTP = %b",B);
    $display("Palavra codificada  = %b",C);
    $display("Iniciando decodificacao do output de A XOR B");
    A = C; // ENCRIPTED MESSAGE
    B = 32'b10001010000101100100110110111000; #20; //OTP
    $display("Codificacao da palavra = %b",A);
    $display("OTP = %b",B);
    $display("Palavra decodificada  = %b",C);
    $display("O codificador funciona!");

end

endmodule 
