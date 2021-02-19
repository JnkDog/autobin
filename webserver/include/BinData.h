#ifndef __BinData_H__
#define __BinData_H__

#include <string>
#include <vector>
#include <optional>
#include <ctime>
#include "./IdentitiedTrash.h"

using namespace std;
using namespace IdentifiedTrash;

/*
 * This is a console class to store data.
 * 
 * Coded by Kuihong Chen 
 */

enum save_type_set {JSON_STORE, DB_STORE};

class TrashData
{
private:
        vector<IdentifiedTrash> data;
        save_type_set saveType;
public:
        void setSaveType();
        void add();
        void save();
        void fetchData();
}

#endif
