// info.h
// Utilized for security reasons for subject objects 

#ifndef __INFO_H__
#define __INFO_H__
#include <cstddef>
#include "state.h"


struct Info {
    int row, col;
    int pnum;
    char name;
    int strength;
    int spaces;
};

#endif
