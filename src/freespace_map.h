//
//  freespace_map.h
//  FRITRecreation
//
//  Created by Christopher Harris on 4/19/15.
//  Copyright (c) 2015 University of Michigan. All rights reserved.
//

#ifndef __FRITRecreation__freespace_map__
#define __FRITRecreation__freespace_map__

#include <iostream>

#include "map.h"

class Freespace_Map : public Map {
public:
    Freespace_Map(std::istream& stream);
    Freespace_Map(int width, int height);
    
    ~Freespace_Map();
    
    virtual void observe(int x, int y) override;
    virtual bool isConnected(int x, int y, int nx, int ny) const override;
    
    virtual void clearObservations();
private:
    void generateOArray();
    void deleteOArray();
    
    bool** m_observed;
};

#endif /* defined(__FRITRecreation__freespace_map__) */
