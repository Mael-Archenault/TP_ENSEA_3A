library IEEE;
use IEEE.STD_LOGIC_1164.ALL;



package dcpt_m_pkg is
    component DCPT_M
        generic ( M : integer := 4);
        Port ( clk : in STD_LOGIC;
               reset : in STD_LOGIC;
               ud : in STD_LOGIC;
               enable : in STD_LOGIC;
               cptr: out STD_LOGIC_VECTOR (M-1 downto 0));
    end component DCPT_M;
end package ;
