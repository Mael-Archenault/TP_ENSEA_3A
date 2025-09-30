library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

use work.DCPT_M_PKG.all;

entity genadr is
    generic (M : integer := 200);

    port (
        reset : in STD_LOGIC;
        clk : in STD_LOGIC;
        incread : in STD_LOGIC;
        incwrite : in STD_LOGIC;
        selread : in STD_LOGIC;
        adrg: out STD_LOGIC_VECTOR (M-1 downto 0)
    );
end genadr;


architecture Behavioral of genadr is
    signal cptr1_out: STD_LOGIC_VECTOR (M-1 downto 0);
    signal cptr2_out: STD_LOGIC_VECTOR (M-1 downto 0);
    begin
    dcpt1: DCPT_M
        generic map (
            M => M
        )
        port map (
            clk    => clk,
            reset  => reset,
            ud     => '1',
            enable => incread,
            cptr   => cptr1_out
        );

    dcpt2: DCPT_M
        generic map (
            M => M
        )
        port map (
            clk    => clk,
            reset  => reset,
            ud     => '1',
            enable => incwrite,
            cptr   => cptr2_out
        );

    adrg <= cptr1_out when selread = '1' else cptr2_out;
end Behavioral;
