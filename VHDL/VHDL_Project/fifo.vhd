library ieee;
use ieee.std_logic_1164.all;
use work.cpnt_pkg.all;

entity fifo is
    generic (
        N : integer := 8;  -- Number of bits per word
        M : integer := 4   -- Number of words
    );
    port (
        clk     : in  std_logic;
        reset   : in  std_logic;
        din    : in  std_logic_vector(N-1 downto 0);               
        req    : in  std_logic;              

        ack: out std_logic;
        dout  : out std_logic_vector(N-1 downto 0);               
        hl : out std_logic;               
        fast: out std_logic;
        slow: out std_logic

    );
end fifo;


architecture fifo_arch of fifo is

    signal register_out, tcc_out: std_logic_vector(N-1 downto 0);
    signal genhl_read_out, genhl_write_out: std_logic;
    signal seq_cs_out, seq_rw_out, seq_oe_out, seq_selread_out, seq_incread_out,  seq_incwrite_out : std_logic;
    signal genaddr_out : std_logic_vector(M-1 downto 0);

    begin
    regN: Nbits_register
        generic map (N => N)
        port map (
            clk => clk,
            reset => reset,
            d => din,
            q => register_out
        );

    TCC: twos_complement
        generic map(N => N)
        port map(
            input_vector => register_out,
            output_vector => tcc_out
        );

    gen_hl: genhl
        generic map(M=>250)
        port map(
            reset => reset,
            clk => clk,
            enread => genhl_read_out,
            enwrite => genhl_write_out
        );

    seq: Sequencer
    port map (
        clk => clk,
        reset => reset,
        enread => genhl_read_out,
        enwrite => genhl_write_out,
        req => req,

        ack => ack, 
        rw_n => seq_rw_out,
        oe => seq_oe_out,
        incwrite => seq_incwrite_out,
        incread => seq_incread_out,
        hl => hl,
        selread => seq_selread_out,
        cs_n => seq_cs_out
    );

    gen_addr: genadr
    generic map(M=>M)
    port map(
        reset => reset,
        clk => clk,
        incread => seq_incread_out,
        incwrite => seq_incwrite_out,
        selread => seq_selread_out,
        adrg => genaddr_out
    );

    fast_slow: fastslow
    generic map(M=>M)
    port map(
        reset => reset,
        clk => clk,
        incread => seq_incread_out,
        incwrite => seq_incwrite_out,
        fast => fast,
        slow => slow
    );

    ram: RAM2pMxNbits
        generic map(
            N => N,
            M => M
        )
        port map(
            oe => seq_oe_out,
            cs_n => seq_cs_out,
            rw_n => seq_rw_out,
            addr => genaddr_out,
            din => tcc_out,
            dout => dout
        );

end architecture fifo_arch;
