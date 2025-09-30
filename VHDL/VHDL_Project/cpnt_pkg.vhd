library IEEE;
use IEEE.STD_LOGIC_1164.ALL;



package cpnt_pkg is
    component DCPT_M
        generic ( M : integer := 4);
        Port ( clk : in STD_LOGIC;
               reset : in STD_LOGIC;
               ud : in STD_LOGIC;
               enable : in STD_LOGIC;
               cptr: out STD_LOGIC_VECTOR (M-1 downto 0));
    end component DCPT_M;

    component RAM2pMxNbits
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
    end component RAM2pMxNbits;

    component GENHL
        generic( M : integer := 200);
        port (
            reset : in STD_LOGIC;
            clk : in STD_LOGIC;
            enread : out STD_LOGIC;
            enwrite : out STD_LOGIC
        );
    end component GENHL;

    component genadr
        generic (M : integer := 200);

        port (
            reset : in STD_LOGIC;
            clk : in STD_LOGIC;
            incread : in STD_LOGIC;
            incwrite : in STD_LOGIC;
            selread : in STD_LOGIC;
            adrg: out STD_LOGIC_VECTOR (M-1 downto 0)
        );
    end component genadr;

    component fastslow
        generic (M : integer := 4);
        port(
            reset : in std_logic;
            clk: in std_logic;
            incread : in std_logic;
            incwrite : in std_logic;
            fast : out std_logic;
            slow : out std_logic
        );
    end component fastslow;
    
    component twos_complement
        GENERIC (N : INTEGER := 8);
        Port ( input_vector : in STD_LOGIC_VECTOR (N-1 downto 0);
            output_vector : out STD_LOGIC_VECTOR (N-1 downto 0));

    end component twos_complement;

    component Nbits_register
        generic (
            N : integer := 8  -- Number of bits
        );
        port (
            clk     : in  STD_LOGIC;
            reset   : in  STD_LOGIC;
            d       : in  STD_LOGIC_VECTOR(N-1 downto 0);
            q       : out STD_LOGIC_VECTOR(N-1 downto 0)
        );
    end component Nbits_register;

    component Sequencer
        port (
            clk: in std_logic;
            reset: in std_logic;
            enread: in std_logic;
            enwrite: in std_logic;
            req: in std_logic;

            ack: out std_logic; 
            rw_n: out std_logic;
            oe: out std_logic;
            incwrite: out std_logic;
            incread: out std_logic;
            hl: out std_logic;
            selread: out std_logic;
            cs_n: out std_logic
        );
    end component Sequencer;

end package ;

