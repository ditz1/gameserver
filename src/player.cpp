// player.cpp
#include "../inc/player.hpp"
#include <cstring>

Player::Player() {

}

void Player::SetId(int id) {
    _id = id;
}

void Player::SetState(int state) {
    _state = state;
}

void Player::SetMoveDir(int move_dir) {
    _move_dir = move_dir;
}

void Player::SetAtkDir(int atk_dir) {
    _atk_dir = atk_dir;
}

void Player::SetX(float x) {
    _pos_x = x;
}

void Player::SetY(float y) {
    _pos_y = y;
}

void Player::SetName(std::string name) {
    _name = name;
}

int Player::GetId() {
    return _id;
}

int Player::GetState() {
    return _state;
}

int Player::GetMoveDir() {
    return _move_dir;
}

int Player::GetAtkDir() {
    return _atk_dir;
}

float Player::GetX() {
    return _pos_x;
}

float Player::GetY() {
    return _pos_y;
}

std::string Player::GetName() {
    return _name;
}

// typedef union PlayerData {
//     uint8_t data[16];
//     struct {
//         uint8_t pid;
//         uint8_t state;
//         uint8_t mv_dir;
//         uint8_t atk_dir;
//         float pos_x;
//         float pos_y;
//         char tag[3];
//         char end;
//     } proto;
// } PlayerData;

void Player::ConvertPlayerData(PlayerData data) {
    _id = data.b.pid;
    _state = data.b.state;
    _move_dir = data.b.mv_dir;
    _atk_dir = data.b.atk_dir;
    _pos_x = data.b.pos_x;
    _pos_y = data.b.pos_y;
    std::memcpy(_tag, data.b.tag, 3);
}