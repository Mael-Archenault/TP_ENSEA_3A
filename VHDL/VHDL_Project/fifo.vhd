library ieee;
use library ieee;
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


architecture behavior of fifo is

    signal register_out, 2cc_out: std_logic_vector(N-1 downto 0);
    signal genhl_read_out, genhl_write_out: std_logic;



    reg: Nbits_register
        generic map (N => N);
        port map (
            clk => clk,
            reset => reset,
            d => din,
            q => register_out
        );

    2CC: twos_complement
        generic map(N => N);
        port map (
            input_vector => register_out,
            output_vector => 2cc_out
        );

    genhl: genhl
        generic map(M=>200);
        port map(
            reset => reset,
            clk => clk,
            enread => genhl_read_out,
            enwrite => genhl_write_out
        );


end architecture behavior;
