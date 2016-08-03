//
// Created by codeh on 2016/7/21.
//

#include <cstring>
#include "market_data.h"

void market_data::update_data(const CThostFtdcDepthMarketDataField &data) {
    if(std::strcmp(data.InstrumentID,_data.InstrumentID) == 0){
        memcpy(&_data,&data,sizeof(CThostFtdcDepthMarketDataField));
    }
}

