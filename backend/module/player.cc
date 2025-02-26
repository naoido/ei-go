#include "player.h"
#include "utils.h"

Player::Player()
{
    id = gen_id();
    is_ready = false;
    name = "名無しさん"
}
