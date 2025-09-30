library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity tb_DCPT_M is
end entity tb_DCPT_M;


architecture arch of tb_DCPT_M is

    component DCPT_M
        generic (
            M : integer := 4  -- Number of words
        );
        port (
            clk   : in  std_logic;
            reset : in  std_logic;
            ud    : in  std_logic;  -- Up/Down control
            enable: in  std_logic;
            cptr  : out std_logic_vector(M-1 downto 0)
        );
    end component DCPT_M;

    signal clk    : std_logic := '0';
    signal reset  : std_logic := '0';
    signal ud     : std_logic := '1';  -- Start counting up
    signal enable : std_logic := '1';
    signal cptr   : std_logic_vector(3 downto 0);

begin

    u1: DCPT_M
        generic map (
            M => 4
        )
        port map (
            clk    => clk,
            reset  => reset,
            ud     => ud,
            enable => enable,
            cptr   => cptr
        );

    process
    begin
        -- Clock generation
        while now < 500 ns loop
            clk <= '0';
            wait for 5 ns;
            clk <= '1';
            wait for 5 ns;
        end loop;
        wait;
    end process;
    process
    begin
        -- Test sequence
        reset <= '1';
        wait for 10 ns;
        reset <= '0';
        wait for 100 ns;
        ud <= '0';  -- Change direction to down
        wait for 50 ns;
        reset <= '1'; -- Reset the counter
        wait for 10 ns;
        reset <= '0';
        wait for 40 ns;
        enable <= '0'; -- Disable counting
        wait for 50 ns;
        enable <= '1'; -- Enable counting
        wait;
        

    end process;


end arch ; -- arch