library ieee;
use ieee.std_logic_1164.all;


entity Sequencer is
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
end entity Sequencer;




architecture archi_Moore of Sequencer is
    type state_type is (IDLE, WRITING, READING1, WAITING, READING2);
    signal current_state, next_state : state_type;

begin
    state_register: process(clk, reset)
    begin
        if reset = '1' then
            current_state <= IDLE;
        elsif rising_edge(clk) then
            current_state <= next_state;
        end if;
    end process state_register;

    -- Next state logic process
    next_state_logic: process(current_state, enread, enwrite, req)
    begin
        case current_state is
            when IDLE =>
                -- Give read priority over write: check enread first
                if enread = '1' then
                    next_state <= READING1;
                elsif enwrite = '1'  and req = '0' then
                    next_state <= WRITING;
                else
                    next_state <= IDLE;
                end if;

            when WRITING =>
                next_state <= WAITING;

            when READING1 =>
                next_state <= IDLE;

            when WAITING =>
                if enread = '1' then
                    next_state <= READING2;
                elsif req = '0' then
                    next_state <= WAITING;
                else
                    next_state <= IDLE;
                end if;

            when READING2 =>
                next_state <= WAITING;

            when others =>
                next_state <= IDLE; 
        end case;
    end process next_state_logic;

    -- Output logic process (Moore machine)
    output_logic: process(current_state)
    begin
        ack <= '1';
        rw_n <= '0';
        oe <= '0';
        incwrite <= '0';
        incread <= '0';
        hl <= '0';
        selread <= '0';
        cs_n <= '1';

        case current_state is
            when IDLE =>
                ack <= '1';
                rw_n <= '0';
                oe <= '0';
                incwrite <= '0';
                incread <= '0';
                hl <= '0';
                selread <= '0';
                cs_n <= '1';

            when WRITING =>
                ack <= '0';
                rw_n <= '0';
                oe <= '0';
                incwrite <= '1';
                incread <= '0';
                hl <= '0';
                selread <= '0';
                cs_n <= '0';

            when READING1 =>
                ack <= '1';
                rw_n <= '1';
                oe <= '1';
                incwrite <= '0';
                incread <= '1';
                hl <= '1';
                selread <= '1';
                cs_n <= '0';

            when WAITING =>
                ack <= '0';
                rw_n <= '0';
                oe <= '0';
                incwrite <= '0';
                incread <= '0';
                hl <= '0';
                selread <= '0';
                cs_n <= '1';

            when others => --READING2 state
                ack <= '0';
                rw_n <= '1';
                oe <= '1';
                incwrite <= '0';
                incread <= '1';
                hl <= '0';
                selread <= '1';
                cs_n <= '0';
        end case;
    end process;
end architecture;

architecture archi_Mealy of Sequencer is
    type state_type is (IDLE, WRITING, READING1, WAITING, READING2);
    signal current_state, next_state : state_type;

begin
    state_register: process(clk, reset)
    begin
        if reset = '1' then
            current_state <= IDLE;
        elsif rising_edge(clk) then
            current_state <= next_state;
        end if;
    end process state_register;

    -- Next state logic process
    next_state_logic: process(current_state, enread, enwrite, req)
    begin
        case current_state is
            when IDLE =>
                -- Give read priority over write: check enread first
                if enread = '1' then
                    next_state <= READING1;
                elsif enwrite = '1'  and req = '0' then
                    next_state <= WRITING;
                else
                    next_state <= IDLE;
                end if;

            when WRITING =>
                next_state <= WAITING;

            when READING1 =>
                next_state <= IDLE;

            when WAITING =>
                if enread = '1' then
                    next_state <= READING2;
                elsif req = '0' then
                    next_state <= WAITING;
                else
                    next_state <= IDLE;
                end if;

            when READING2 =>
                next_state <= WAITING;

            when others =>
                next_state <= IDLE; 
        end case;
    end process next_state_logic;

    -- Output logic process (Mealy machine)
    -- Use a single combinational process so outputs can depend on current_state and inputs
    output_logic: process(current_state, enread, enwrite, req)
    begin
        -- default values
        ack <= '1';
        rw_n <= '0';
        oe <= '0';
        incwrite <= '0';
        incread <= '0';
        hl <= '0';
        selread <= '0';
        cs_n <= '1';

        case current_state is
            when IDLE =>
                -- If a read is enabled, give it priority
                if enread = '1' then
                    ack <= '1';
                    rw_n <= '1';
                    oe <= '1';
                    incread <= '1';
                    hl <= '1';
                    selread <= '1';
                    cs_n <= '0';
                    incwrite <= '0';
                elsif enwrite = '1' and req = '0' then
                    -- Accept a write
                    ack <= '0';
                    rw_n <= '0';
                    oe <= '0';
                    incwrite <= '1';
                    cs_n <= '0';
                else
                    -- remain idle outputs (defaults already set)
                    null;
                end if;

            when WRITING =>
                ack <= '0';
                rw_n <= '0';
                oe <= '0';
                incwrite <= '1';
                cs_n <= '0';

            when READING1 =>
                ack <= '1';
                rw_n <= '1';
                oe <= '1';
                incread <= '1';
                hl <= '1';
                selread <= '1';
                cs_n <= '0';

            when WAITING =>
                -- In WAITING, behaviour may depend on enread/req
                if enread = '1' then
                    -- prepare for a second read
                    ack <= '0';
                    rw_n <= '1';
                    oe <= '1';
                    incread <= '1';
                    selread <= '1';
                    cs_n <= '0';
                elsif req = '0' then
                    -- keep waiting for a write request completion
                    ack <= '0';
                    rw_n <= '0';
                    oe <= '0';
                    cs_n <= '1';
                else
                    -- default WAITING outputs
                    ack <= '0';
                    cs_n <= '1';
                end if;

            when READING2 =>
                ack <= '0';
                rw_n <= '1';
                oe <= '1';
                incread <= '1';
                selread <= '1';
                cs_n <= '0';

            when others =>
                -- safe defaults
                ack <= '1';
                rw_n <= '0';
                oe <= '0';
                incwrite <= '0';
                incread <= '0';
                hl <= '0';
                selread <= '0';
                cs_n <= '1';
        end case;
    end process output_logic;
end architecture;