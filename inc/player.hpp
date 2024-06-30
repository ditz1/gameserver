#pragma once
#include <string>
#include <cstdint>
#include "gameprotocol.hpp"

class Player {
public:
    Player();
    void SetId(int id);
    void SetState(int state);
    void SetMoveDir(int move_dir);
    void SetAtkDir(int atk_dir);
    void SetX(float x);
    void SetY(float y);
    void SetName(std::string name);
    int GetId();
    int GetState();
    int GetMoveDir();
    int GetAtkDir();
    float GetX();
    float GetY();
    std::string GetName();
private:
    void ConvertPlayerData(PlayerData data);
    int _id;
    int _state;
    int _move_dir;
    int _atk_dir;
    float _pos_x;
    float _pos_y;
    char* _tag;
    PlayerData _playerdata;
    std::string _name;
};
