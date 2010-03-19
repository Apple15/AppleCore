///////////////////////////////////////
//  CopperToGold v1.0.0
//  by Nicolai Dutka
//  http://nicolaidutka.archongames.com
///////////////////////////////////////

#include "precompiled.h"

std::string CopperToGold(uint32 copper){
	// Convert copper to gold/silver/copper
	int32 gold = 0;
	int32 silver = 0;
	if(copper>9999){
		gold = floor(copper/10000.0);
		copper -= gold*10000;
	}
	if(copper>99){
		silver = floor(copper/100.0);
		copper -= silver*100;
	}
	std::stringstream ss;
	ss<< gold << "g " << silver << "s " << copper << "c";
	return ss.str();
}