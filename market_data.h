//
// Created by codeh on 2016/7/21.
//

#ifndef NB2016_MARKET_DATA_H
#define NB2016_MARKET_DATA_H

#include <ThostFtdcUserApiStruct.h>
#include <cstdlib>
#include <stdexcept>
#include <memory>
#include <map>

class market_data {
private:
    CThostFtdcDepthMarketDataField _data;
    std::string _key;

public:
    market_data(const CThostFtdcDepthMarketDataField &data) : _data(data) {
        _key = _data.ExchangeID;
        _key.push_back('.');
        _key.append(_data.InstrumentID);
    }

    void update_data(const CThostFtdcDepthMarketDataField &data);

    const std::string &get_key(void) const{
        return _key;
    }
    ///交易日
    const char* get_trading_day(void) const{
        return _data.TradingDay;
    }
    ///合约代码
    const char* get_instrument_id(void) const{
        return _data.InstrumentID;
    }
    ///交易所代码
    const char* get_exchange_id(void) const{
        return _data.ExchangeID;
    }
    ///合约在交易所的代码
    const char* get_instrument_id_from_exchange(void) const{
        return _data.ExchangeInstID;
    }
    ///最新价
    const double get_last_price(void) const{
        return _data.LastPrice;
    }
    ///上次结算价
    const double get_pre_settlement_price(void) const{
        return _data.PreSettlementPrice;
    }
    ///昨收盘
    const double get_pre_close_price(void) const{
        return _data.PreClosePrice;
    }
    ///昨持仓量
    const double get_pre_open_interest(void) const{
        return _data.PreOpenInterest;
    }
    ///今开盘
    const double get_open_price(void) const{
        return _data.OpenPrice;
    }
    ///最高价
    const double get_highest_price(void) const{
        return _data.HighestPrice;
    }
    ///最低价
    const double get_lowest_price(void) const{
        return _data.LowestPrice;
    }
    ///数量
    const long long get_volume(void) const{
        return _data.Volume;
    }
    ///成交金额
    const double get_turnover(void) const{
        return _data.Turnover;
    }
    ///持仓量
    const double get_open_interest(void) const{
        return _data.OpenInterest;
    }
    ///今收盘
    const double get_close_price(void) const{
        return _data.ClosePrice;
    }
    ///本次结算价
    const double get_settlement_price(void) const{
        return  _data.SettlementPrice;
    }
    ///涨停板价
    const double get_upper_limit_price(void) const{
        return  _data.UpperLimitPrice;
    }
    ///跌停板价
    const double get_lower_limit_price(void) const{
        return  _data.LowerLimitPrice;
    }
    ///昨虚实度
    const double get_pre_delta(void) const{
        return  _data.PreDelta;
    }
    ///今虚实度
    const double get_curr_delta(void) const{
        return  _data.CurrDelta;
    }
    ///最后修改时间
    const long get_update_time(int &hour, int &minute, int &second) const{
        hour = 0;
        minute = 0;
        second = 0;
        char item[10];
        int j = 0;
        int nitem = 0;
        for(int i=0;i<sizeof( _data.UpdateTime);i++) {
            char t = _data.UpdateTime[i];
            if(t==':' || t == '\0' ){
                if(j>10) break;
                item[j] = 0;
                j = 0;
                switch(nitem++){
                    case 0:
                        hour = std::atoi(item);
                        break;
                    case 1:
                        minute = std::atoi(item);
                        break;
                    case 2:
                        second = std::atoi(item);
                        break;
                }
            }
            else{
                item[j++] = t;
            }
            _data.UpdateTime;
        }
        return  hour*10000 + minute*100 + second;
    }
    ///最后修改毫秒
    const int get_update_millisec(void) const{
        return  _data.UpdateMillisec;
    }

    const double get_bid_price(int level) const{
        switch (level){
            case 1:
                return _data.BidPrice1;
            case 2:
                return _data.BidPrice2;
            case 3:
                return _data.BidPrice3;
            case 4:
                return _data.BidPrice4;
            case 5:
                return _data.BidPrice5;
            default:
                throw std::runtime_error("Unsupported level!");
        }
    }

    const double get_ask_price(int level) const{
        switch (level){
            case 1:
                return _data.AskPrice1;
            case 2:
                return _data.AskPrice2;
            case 3:
                return _data.AskPrice3;
            case 4:
                return _data.AskPrice4;
            case 5:
                return _data.AskPrice5;
            default:
                throw std::runtime_error("Unsupported level!");
        }
    }

    const int get_bid_volume(int level) const{
        switch (level){
            case 1:
                return _data.BidVolume1;
            case 2:
                return _data.BidVolume2;
            case 3:
                return _data.BidVolume3;
            case 4:
                return _data.BidVolume4;
            case 5:
                return _data.BidVolume5;
            default:
                throw std::runtime_error("Unsupported level!");
        }
    }

    const int get_ask_volume(int level) const{
        switch (level){
            case 1:
                return _data.AskVolume1;
            case 2:
                return _data.AskVolume2;
            case 3:
                return _data.AskVolume3;
            case 4:
                return _data.AskVolume4;
            case 5:
                return _data.AskVolume5;
            default:
                throw std::runtime_error("Unsupported level!");
        }
    }

    ///当日均价
    const double get_average_price(void) const{
        return _data.AveragePrice;
    }
};

typedef std::shared_ptr<market_data> PTR_MARKET_DATA;
typedef std::map<std::string, PTR_MARKET_DATA> MARKET_DATA_MAP;

#endif //NB2016_MARKET_DATA_H
