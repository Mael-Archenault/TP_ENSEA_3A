library ieee;
use ieee.std_logic_1164.all;


entity tb_fifo is
end tb_fifo;


architecture behavior of tb_fifo is

    constant N : integer := 8;  
    constant M : integer := 4;  

    signal clk     : std_logic := '0';
    signal reset   : std_logic := '1';
    signal din    : std_logic_vector(N-1 downto 0) := (others => '0');               
    signal req    : std_logic := '1';              

    signal ack: std_logic;
    signal dout  : std_logic_vector(N-1 downto 0);               
    signal hl : std_logic;               
    signal fast: std_logic;
    signal slow: std_logic;

    constant clk_period : time := 20 ns;

    component fifo
        generic (
            N : integer := 8;  
            M : integer := 4   
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
    end component fifo;

begin
    u1: fifo
        generic map (
            N => N,
            M => M
        )
        port map (
            clk => clk,
            reset => reset,
            din => din,
            req => req,
            ack => ack,
            dout => dout,
            hl => hl,
            fast => fast,
            slow => slow
        );

    clk_process :process
    begin
        while now<16000 ns loop
            clk <= '0';
            wait for clk_period/2;
            clk <= '1';
            wait for clk_period/2;
        end loop;
        wait;
    end process;

    stim_proc: process
    begin		
        reset <= '1';
        wait for clk_period*2;
        reset <= '0';
        wait for clk_period*2;

        din <= "00000001";
	wait for clk_period/4;
        req <= '0';
        wait for clk_period/2;
        req <= '1';
        wait for clk_period*2;

        din <= "00000010";
	wait for clk_period/4;
        req <= '0';
        wait for clk_period/2;
        req <= '1';
        wait for clk_period*2;

        din <= "00000011";
	wait for clk_period/4;
        req <= '0';
        wait for clk_period/2;
        req <= '1';

        wait;

end process;

end behavior;