//
// Created by codeh on 2016/8/3.
//

#ifndef NB2016_CONTRACT_LIST_H
#define NB2016_CONTRACT_LIST_H


#include <ThostFtdcUserApiStruct.h>

class contract {
private:
    CThostFtdcInstrumentField _instrument;
    CThostFtdcInstrumentCommissionRateField _commission;
    CThostFtdcInstrumentMarginRateField _margin;
};


#endif //NB2016_CONTRACT_LIST_H
