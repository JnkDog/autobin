#ifndef __IdentitiedTrash_H__
#define __IdentitiedTrash_H__

#include <string>
#include <vector>
#include <optional>
#include <ctime>

/*
 * This is a data templete.
 * 
 * Coded by Kuihong Chen 
 */

enum trash_type_set {
    HOUSEHOLD_FOOD_WASTE = 0,
    RESIDUAL_WASTE = 1,
    HAZARDOUS_WASTE = 2,
    RECYCLABLE_WASTE = 3
}

class IdentifiedTrash
{
public:
        string name;
        time_t date;
        trash_type_set type;
        float accuracy;
        string location;
};

#endif