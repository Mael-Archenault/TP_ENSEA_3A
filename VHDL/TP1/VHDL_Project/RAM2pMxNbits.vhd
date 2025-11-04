library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.numeric_std.all;


entity RAM2pMxNbits is
    generic (
        N : integer := 8;  -- Number of bits per word
        M : integer := 4  -- Number of words
    );
    port (
        oe     : in  std_logic;
        cs_n      : in  std_logic;               
        rw_n     : in  std_logic;               
        addr    : in  std_logic_vector(M-1 downto 0); -- Address input
        din     : in  std_logic_vector(N-1 downto 0); -- Data input
        dout    : out std_logic_vector(N-1 downto 0)  -- Data output
    );
end entity RAM2pMxNbits;


architecture Behavioral of RAM2pMxNbits is
    type ram_type is array (0 to 2**M-1) of std_logic_vector(N-1 downto 0);
    signal ram : ram_type := (others => (others => '0'));
    signal dout_reg : std_logic_vector(N-1 downto 0) := (others => '0');
begin
    process (cs_n, rw_n, oe, addr, din)
    begin
        if cs_n = '0' then
            if rw_n = '0' then
                -- Write operation
                ram(to_integer(unsigned(addr))) <= din;
            else
                -- Read operation
                dout_reg <= ram(to_integer(unsigned(addr)));
            end if;
        end if;
    end process;

    dout <= dout_reg when oe = '1' else (others => 'Z');
end Behavioral;
