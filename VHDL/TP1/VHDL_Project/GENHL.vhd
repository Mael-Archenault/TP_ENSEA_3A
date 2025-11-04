library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

use work.dcpt_m_pkg.all;


entity GENHL is
    generic( M : integer := 200);
    port (
        reset : in STD_LOGIC;
        clk : in STD_LOGIC;
        enread : out STD_LOGIC;
        enwrite : out STD_LOGIC
    );

end GENHL;

architecture Behavioral of GENHL is
    signal cptr_out: STD_LOGIC_VECTOR (7 downto 0);
    signal comp_out: STD_LOGIC;
    signal reset_sig: STD_LOGIC;

begin
    u1: DCPT_M
        generic map (
            M => 8
        )
        port map (
            clk    => clk,
            reset  => reset_sig,
            ud     => '1',
            enable => '1',
            cptr   => cptr_out
        );

    comp_out <= '1' when cptr_out = std_logic_vector(to_unsigned(M-1,8)) else '0';
    reset_sig <= reset or comp_out;
    enwrite <= not comp_out;
    enread <= comp_out;

end Behavioral;
