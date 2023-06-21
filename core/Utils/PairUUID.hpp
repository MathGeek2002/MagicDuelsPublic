#pragma once

#include <algorithm>

// return unique id for pair of numbers
int PairUUID(int x,int y) 
{
    if(x > y)
        std::swap( x, y );
    
    return y * y + x;
}