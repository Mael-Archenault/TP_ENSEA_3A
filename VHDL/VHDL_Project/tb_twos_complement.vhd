library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity tb_twos_complement is
end entity tb_twos_complement;


architecture arch of tb_twos_complement is
    
    component twos_complement
        generic (
            N : integer := 8  -- Number of bits
        );
        port (
            input_vector  : in  std_logic_vector(N-1 downto 0); -- Data input
            output_vector : out std_logic_vector(N-1 downto 0)  -- Two's complement output
        );
    end component twos_complement;

    signal din  : std_logic_vector(7 downto 0) := (others => '0');
    signal dout : std_logic_vector(7 downto 0);

begin
    u1: twos_complement
        generic map (
            N => 8
        )
        port map (
            input_vector  => din,
            output_vector => dout
        );

    process
    begin
        -- Test case 1: Input = 00000001 (1)
        din <= "00000001";
        wait for 10 ns;
        
        -- Test case 2: Input = 00000010 (2)
        din <= "00000010";
        wait for 10 ns;
        
        -- Test case 3: Input = 11111111 (-1 in two's complement)
        din <= "11111111";
        wait for 10 ns;
        
        -- Test case 4: Input = 10000000 (-128 in two's complement)
        din <= "10000000";
        wait for 10 ns;
        
        -- Test case 5: Input = 01111111 (127)
        din <= "01111111";
        wait for 10 ns;

        -- End simulation
        wait;
    end process;

end architecture arch;


