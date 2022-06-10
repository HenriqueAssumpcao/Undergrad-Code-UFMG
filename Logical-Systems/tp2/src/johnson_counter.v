module johnson_counter(clk,clr,out);
  input clk,clr;
  output out;
  reg A,B,C,D;
  always @(posedge clk)
    begin
      if(!clr) begin A<=0;B<=0;C<=0;D<=0;end
      else begin
        D<=C;
        C<=B;
        B<=A;
        A<=(~D);
      end
    end
  assign out=D;
endmodule