#pragma once
#include<iostream>
#include "Runner.h"
#include"GridHashing/HashGrid.h"

struct GameObject {
    Runner run;
    HashGrid worldGrid;
};
extern GameObject gameObject;
