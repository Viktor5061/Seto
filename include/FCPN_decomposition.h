/*
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE.txt', which is part of this source code package.
 */

#include "Utilities.h"
#include "../pblib/pb2cnf.h"
#include <algorithm>
#include <iomanip>
#include "../include/Pre_and_post_regions_generator.h"


class FCPN_decomposition {
private:

public:
    FCPN_decomposition(vector<vector<int32_t> *> *clauses,
                       int number_of_events,
                       vector<Region> *regions,
                       int number_of_states,
                       string file,
                       Pre_and_post_regions_generator *pprg,
                       map<int, int> *aliases,
                       map<int, ER> *ER);
    ~FCPN_decomposition();
};