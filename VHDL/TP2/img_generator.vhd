
library ieee;
use work.entree_sortie_pkg.all;

entity generator is
    generic (
        N_col : integer := 256;
        N_ligne : integer := 256
    );
end generator;

architecture arch_half of generator is
-- create a img half black half white
begin
    process
        variable image_out : IMAGE2D(1 to N_col, 1 to N_ligne);
    begin
        for i in 1 to N_ligne loop
            for j in 1 to N_col loop
                if j <= N_col / 2 then
                    image_out(i, j) := 0;
                else
                    image_out(i, j) := 255;
                end if;
            end loop;
        end loop;
        put_pixels("img_txt/half.txt", image_out, N_col, N_ligne, 8);
        wait;
    end process;
end arch_half;


architecture arch_center_rectangle of Generator is
begin
    process
        variable image_out : IMAGE2D(1 to N_col, 1 to N_ligne);
    begin
        for i in 1 to N_ligne loop
            for j in 1 to N_col loop
                if (i > N_ligne / 4) and (i < 3 * N_ligne / 4) and (j > N_col / 4) and (j < 3 * N_col / 4) then
                    image_out(i, j) := 0;
                else
                    image_out(i, j) := 255;
                end if;
            end loop;
        end loop;
        put_pixels("img_txt/center_rectangle.txt", image_out, N_col, N_ligne, 8);
        wait;
    end process;
end arch_center_rectangle;

architecture arch_diag_half of generator is
-- create a img with diagonal half black
begin
    process
        variable image_out : IMAGE2D(1 to N_col, 1 to N_ligne);
    begin
        for i in 1 to N_ligne loop
            for j in 1 to N_col loop
                if i>j then
                    image_out(i, j) := 0;
                else
                    image_out(i, j) := 255;
                end if;
            end loop;
        end loop;
        put_pixels("img_txt/diag_half.txt", image_out, N_col, N_ligne, 8);
        wait;
    end process;
end arch_diag_half;

architecture arch_diamond of generator is
-- create a img with a black diamond in the center
begin
    process
        variable image_out : IMAGE2D(1 to N_col, 1 to N_ligne);
    begin
        for i in 1 to N_ligne loop
            for j in 1 to N_col loop
                if abs(i - j) <= 50 and abs(i + j - (N_col + 1)) <= 50 then
                    image_out(i, j) := 0;
                else
                    image_out(i, j) := 255;
                end if;
            end loop;
        end loop;
        put_pixels("img_txt/diamond.txt", image_out, N_col, N_ligne, 8);
        wait;
    end process;
end arch_diamond;


