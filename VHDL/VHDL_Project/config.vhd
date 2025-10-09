library ieee;
use ieee.std_logic_1164.all;

configuration tb_fifo_cfg of tb_fifo is
    for behavior
        for u1 : fifo
            for fifo_arch
                for seq : Sequencer
                    use entity work.sequencer(archi_Moore);
                end for;
            end for;
        end for;
    end for;
end configuration tb_fifo_cfg;

