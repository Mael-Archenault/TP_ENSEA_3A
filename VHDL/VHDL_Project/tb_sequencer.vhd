library ieee;
use ieee.std_logic_1164.all;

entity tb_sequencer is
end entity;


architecture arch of tb_sequencer is

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
    end component;

    signal clk: std_logic := '0';
    signal reset: std_logic := '0';
    signal enread: std_logic := '0';
    signal enwrite: std_logic := '0';
    signal req: std_logic := '0';

    signal ack: std_logic;
    signal rw_n: std_logic;
    signal oe: std_logic;
    signal incwrite: std_logic;
    signal incread: std_logic;
    signal hl: std_logic;
    signal selread: std_logic;
    signal cs_n: std_logic;

begin
    u1: Sequencer
        port map (
            clk => clk,
            reset => reset,
            enread => enread,
            enwrite => enwrite,
            req => req,

            ack => ack,
            rw_n => rw_n,
            oe => oe,
            incwrite => incwrite,
            incread => incread,
            hl => hl,
            selread => selread,
            cs_n => cs_n
        );
    -- Clock generation
    clk_process : process
    begin
        while now < 200 ns loop
            clk <= '0';
            wait for 10 ns;
            clk <= '1';
            wait for 10 ns;
        end loop;
        wait;
    end process clk_process;

    -- Test sequence
    stim_process : process
    begin
        -- Initial reset
        reset <= '1';
        wait for 5 ns;
        reset <= '0';
        enread<= '1';
        wait for 10 ns;
        enread<= '0';


        wait for 30 ns;

        enwrite<='1';
        req<='0';

        wait for 10 ns;

        enwrite<= '0';
        wait for 30 ns;
        enread<= '1';
        wait for 10 ns;
        enread <= '0';
        wait for 20 ns;
        req <= '1';
        wait for 10 ns;


        wait;
    end process stim_process;
end architecture;

       


