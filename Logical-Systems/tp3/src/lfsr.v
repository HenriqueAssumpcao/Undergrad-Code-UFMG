 module lfsr_8bit(
    input   clock,
    input   reset,
    input   [7:0] taps,
    input   [7:0] reset_value,
    output  reg [7:0] computed_value);

    
    always @ (posedge clock, negedge reset)
    begin
        if (!reset)
        begin
            computed_value <= reset_value;
        end
        else
        begin
            computed_value <= computed_value >> 1;
            computed_value[7] <= computed_value[0] ^
                                 ((taps[6]) ? computed_value[1] : 0) ^
                                 ((taps[5]) ? computed_value[2] : 0) ^
                                 ((taps[4]) ? computed_value[3] : 0) ^
                                 ((taps[3]) ? computed_value[4] : 0) ^
                                 ((taps[2]) ? computed_value[5] : 0) ^
                                 ((taps[1]) ? computed_value[6] : 0) ^
                                 ((taps[0]) ? computed_value[7] : 0);
        end
    end
endmodule 

module lfsr_4bit(
    input   clock,
    input   reset,
    input   [3:0] taps,
    input   [3:0] reset_value,
    output  reg [3:0] computed_value);

    
    always @ (posedge clock, negedge reset)
    begin
        if (!reset)
        begin
            computed_value <= reset_value;
        end
        else
        begin
            computed_value <= computed_value >> 1;
            computed_value[3] <= computed_value[0] ^
                                 ((taps[2]) ? computed_value[1] : 0) ^
                                 ((taps[1]) ? computed_value[2] : 0) ^
                                 ((taps[0]) ? computed_value[3] : 0);
        end
    end
endmodule 

module lfsr_3bit(
    input   clock,
    input   reset,
    input   [2:0] taps,
    input   [2:0] reset_value,
    output  reg [2:0] computed_value);

    
    always @ (posedge clock, negedge reset)
    begin
        if (!reset)
        begin
            computed_value <= reset_value;
        end
        else
        begin
            computed_value <= computed_value >> 1;
            computed_value[2] <= computed_value[0] ^
                                 ((taps[1]) ? computed_value[1] : 0) ^
                                 ((taps[0]) ? computed_value[2] : 0);
        end
    end
endmodule 
