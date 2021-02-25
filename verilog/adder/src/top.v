module adder_component( input A, input B, input carry_in, output C, output carry_out );

wire a_xor_b;

assign   a_xor_b =    A ^ B; // factor out common
assign         C =   a_xor_b ^ carry_in;
assign carry_out = ( a_xor_b & carry_in ) | ( A & B);

endmodule

module adder( input [0:8-1]A, input [0:8-1]B, output [0:8-1]C );
reg        zero;
wire [0:8] carry;

initial
begin
	zero <= 0;
end

assign carry[8] = zero; // hardcode no carry for first element (from element -1)

genvar i;
generate
for( i = 7; i >= 0; i = i - 1 )//            bits are indexed 'backwards'
begin //                         there is also no bit -1 so they're all shifted
	adder_component adder_comp( A[i], B[i], carry[i+1], C[i], carry[i] );
end
endgenerate

endmodule

module main;
reg  [0:8-1] A;
reg  [0:8-1] B;
wire [0:8-1] C;
adder uut( A, B, C );

initial
begin
	A <= 8'd02;
	B <= 8'd02;
#1;
    $display("---------------");
    $display("A=%x", A);
    $display("B=%x", B);
    $display("C=%x", C);
	A <= 8'd01;
	B <= 8'd03;
#1;
    $display("---------------");
    $display("A=%x", A);
    $display("B=%x", B);
    $display("C=%x", C);
	A <= 8'b1001_0010;
	B <= 8'b1010_1011;
#1;
    $display("---------------");
    $display("A=%x", A);
    $display("B=%x", B);
    $display("C=%x", C);
end

endmodule

/*
module main_b;
reg value;
initial
begin
    value <= 1;
    #1;
    $display("Hello, World %d", value);
    $finish ;
end
endmodule
*/
