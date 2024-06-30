#pragma once
#include <cstdint>


typedef enum {
    p1,
    p2,
    p3,
    p4,
} pids;

typedef enum {
    alive,
    dead,
    crouch,
    jump,
} states;

typedef enum {
    down,
    right,
    left,
    up,
} move_dirs;

typedef enum {
    not_atk,
    atk_right,
    atk_left,
    atk_up,
} atk_dirs;

typedef union PlayerData {
    uint8_t data[16];
    struct {
        uint8_t pid;
        uint8_t state;
        uint8_t mv_dir;
        uint8_t atk_dir;
        float pos_x;
        float pos_y;
        char tag[3];
        char end;
    } b;
} PlayerData;

// protocol will be 16 bytes total for player sends

//// first 4 bytes
// 1 byte
// pid = 00, 01, 10, 11
//        1,  2,  3,  4

// 1 byte
// state =   00,    01,     10,    11
//          alive, dead,  crouch,  jump

// 1 byte
// mv_dir =   00,   01,      10,    11
//         down,   right,    left,  up

// 1 byte
// atk_dir =  00,         01,      10,    11
//           not/atk,   right,    left,  up

/// second 4 bytes
// pos_x
// f16

/// another 4 bytes
// pos_y
// f16

// last 4 bytes
// 4 bytes
// player name or something havent decided yet 

// 1 byte could be like joining lobby or whatever like special case
// last two bytes of those 4 are /n