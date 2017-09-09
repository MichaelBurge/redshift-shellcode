-- 68719476737 -> b1a3
create or replace function pf_show_move(move bigint) returns varchar stable as $$
from_file = (move & 0xFFFFFFFF) % 8
from_rank = (move & 0xFFFFFFFF) / 8
to_file = (move >> 32) % 8
to_rank = (move >> 32) / 8

return (chr(from_file + ord('a')) + str(from_rank+1)) + (chr(to_file + ord('a')) + str(to_rank+1))
$$ language plpythonu;

