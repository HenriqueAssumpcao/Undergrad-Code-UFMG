module feedback_register(clock, clear,out);
  input clock,clear;
  output out;
  reg A,B,C,D;
  always @(posedge clock)
    begin
      if(!clear) begin A<=1;B<=0;C<=0;D<=0;end
    else begin
      D<=A;
      C<=D;
      B<=C;
      A<=B;
    end
    end
  assign out=A;
endmodule
        

