library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

use work.CHECK_PKG.all;


entity Nbits_register is
    generic (
        N : integer := 8  -- Number of bits
    );
    port (
        clk     : in  STD_LOGIC;
        reset   : in  STD_LOGIC;
        d       : in  STD_LOGIC_VECTOR(N-1 downto 0);
        q       : out STD_LOGIC_VECTOR(N-1 downto 0)
    );
end entity Nbits_register;


architecture Behavioral of Nbits_register is
    signal q_reg : STD_LOGIC_VECTOR(N-1 downto 0);
begin

    setup_check: process
    begin
        check_setup(clk, d, 3 ns);
    end process;

    hold_check: process
    begin
        check_hold(clk, d, 3 ns);
    end process;

    process(clk)
    begin
        if rising_edge(clk) then
            if reset = '1' then
                q_reg <= (others => '0');
            else
                q_reg <= d;
            end if;
        end if;
    end process;

    q <= q_reg;

end architecture Behavioral;
