#include "../Log/logger.h"
#include "Catridge.h"
#include <iostream>
#include <fstream>

uint8_t cart_data[MAX_CART_SIZE];
bool is_loaded = false;
