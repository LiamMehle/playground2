
/*
Large chunks of this are implemented sloppily.
It does, however, work.
Mainly useful as a sanity check/known good sample.
*/


module adder_component( input A, input B, input carry_in, output C, output carry_out );

wire a_xor_b;

assign   a_xor_b =    A ^ B; // factor out common
assign         C =   a_xor_b ^ carry_in;
assign carry_out = ( a_xor_b & carry_in ) | ( A & B);

endmodule

module adder( input [7:0]A, input [7:0]B, output [7:0]C );
wire [8:0] carry;

assign carry[0] = 0; // hardcode no carry for zero-th element (from element -1)

genvar i;
generate
for( i=7; i>=0; i=i-1 )
begin
	adder_component adder_comp( A[i], B[i], carry[i], C[i], carry[i+1] );
end
endgenerate

endmodule

module main;
reg  [7:0] A;
reg  [7:0] B;
wire [7:0] C;
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
