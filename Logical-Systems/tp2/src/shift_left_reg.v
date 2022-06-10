module shifting_4bit(clock,clear,E,A);
  input clock,clear,E;
  output A;
  reg A;
  reg B,C,D;
  always @(posedge clock)
    begin
      if(!clear) begin A<=0;B<=0;C<=0;D<=0;end
      else begin
        D<=E;
        C<=D;
        B<=C;
        A<=B;
      end
    end
endmodule

