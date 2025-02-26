#include "player.h"

Player::Player()
{
    id = Utils::gen_id();
    name = "名無しさん";
    is_ready = false;   
}
