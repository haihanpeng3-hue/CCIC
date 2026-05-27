module int_reg #(
    parameter N = 1
)(
    input              clk,
    input              resetn,

    input  [N-1:0]     int_en,
    input  [N-1:0]     int_edga,
    input  [N-1:0]     int_pol,
    input  [N-1:0]     int_in,
    input  [N-1:0]     int_clr,
    input  [N-1:0]     int_set,
    output reg [N-1:0] int_out
);

reg [N-1:0] int_in_d;

wire [N-1:0] pos_edge;
wire [N-1:0] neg_edge;
wire [N-1:0] edge_hit;
wire [N-1:0] level_hit;
wire [N-1:0] int_hit;

assign pos_edge  =  int_in & ~int_in_d;
assign neg_edge  = ~int_in &  int_in_d;

assign edge_hit  = ( int_pol & pos_edge) | (~int_pol & neg_edge);
assign level_hit = ( int_pol & int_in)   | (~int_pol & ~int_in);

assign int_hit   = int_en & ((int_edga & edge_hit) | (~int_edga & level_hit));

always @(posedge clk or negedge resetn) begin
    if (!resetn) begin
        int_in_d <= {N{1'b0}};
        int_out  <= {N{1'b0}};
    end
    else begin
        int_in_d <= int_in;

        /*
         * clear has highest priority
         * set and detected interrupt can set int_out
         */
        int_out <= (int_out | int_hit | int_set) & ~int_clr;
    end
end

endmodule