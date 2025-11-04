library ieee;
use ieee.std_logic_1164.all;

configuration fifo_cfg of fifo is
    for fifo_arch
        for seq : Sequencer
            use entity work.sequencer(archi_Mealy);
        end for;
    end for;
end configuration fifo_cfg;
