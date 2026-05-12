#pragma once

#include <iostream>
#include <array>
#include <vector>
#include <algorithm>
#include <numeric_limits>

namespace stock
{

struct tick
{
    typedef std::array<double, 90> mas_t;
    typedef std::array<double, 10> signals_t;
    
    double     low,
               high,
               open,
               close;
    uint64_t   volume,
               seconds;
    mas_t      ema;
    signals_t  signal;
    
    double macd()        const { return ema[12] - ema[26]; }
    double macd_signal() const { return signal[9]; }

    tick() : seconds(0), volume(0), low(0), high(0), open(0), close(0), ema({ 0 }), signal({ 0 }) {}

    bool operator<(const tick &u) const { return seconds < u.seconds; }
};

struct frame
{
    uint64_t          available_shares;
    std::vector<tick> data;
    
    frame &add(const tick &s) { data.push_back(s); return *this; }
    frame &prepare()
    {
        std::sort(data.begin(), data.end());
        for (size_t i = 2, sz = data[0].ema.size(); i < sz; i++) {
            calculate_ma(i);
        }
        //for (size_t i = 2, sz = data[0].signal.size(); i < sz; i++) {
            calculate_signal(12, 26, 9);
        //}
        return *this;
    }

    frame(uint64_t available_shares) : available_shares(available_shares) {}
    
    double ema(size_t count, size_t index) { return data[index].ema[count]; }

private:
    
    double ema_difference(size_t count_a, size_t count_b, size_t index)
    {
        return data[index].ema[count_a] - data[index].ema[count_b];
    }

    void calculate_ma(size_t count)
    {
        double alpha = 2. / (count + 1), prev = data[0].close;
        for (size_t i = 1; i < count; i++) {
            double v     = data[i].close,
                   value = alpha * v + (1. - alpha) * prev;
            data[i].ema[count] = value;
            prev = value;
        }
    }
    
    void calculate_signal(size_t count_a, size_t count_b, size_t count_c)
    {
        double alpha = 2. / (count_c + 1), prev = ema_difference(count_a, count_b, 0);
        for (size_t i = 1; i < count_c; i++) {
            double v     = ema_difference(count_a, count_b, i),
                   value = alpha * v + (1. - alpha) * prev;
            data[i].signal[count_c] = value;
            prev = value;
        }
    }
};

struct order
{
    enum Action { NONE, BUY, SELL };

    Action   action;
    uint64_t date_time;
    double   shares;
    double   price;
    
    order(uint64_t date_time = 0, Action action = NONE, double shares = 0., double price = 0.) :
        action(action), shares(shares), price(price), date_time(date_time) { validate(); }
    
    double value()                      const { return shares * price; }
    double shares_for_cash(double cash) const { return price > 0 ? cash / price : 0; }
    order &clear()                            { action = NONE; shares = price = 0; date_time = 0; return *this; }
    order &validate()                         { if (value() <= 0) clear(); return *this; }
};

class account
{
    double              _initial_cash,
                        _cash,
                        _shares,
                        _current_price,
                        _stock_value,
                        _high,
                        _low;
    std::vector<order>  _orders;
    uint64_t            _date_time;
    
public:

    account(double cash = 0) : _cash(cash), _initial_cash(cash), _shares(0.), _high(-std::numeric_limits<double>::max()), _low(std::numeric_limits<double>::max()), _date_time(0), _current_price(0), _stock_value(0) { _update(); }
    
    account &set_date_time(uint64_t date_time)       { _date_time = date_time; return *this; }
    uint64_t get_date_time()                   const { return _date_time; }
    
    account &set_cash(double cash)                   { _initial_cash = _cash = cash; _update(); return *this; }
    account &add_cash(double cash)                   { _initial_cash += cash; _cash += cash; _update(); return *this; }
    double   get_cash()                        const { return _cash; }
    
    account &set_shares(double shares)               { _shares = shares; _update(); return *this; }
    double   get_shares()                      const { return _shares; }
    
    account &set_current_price(double price)         { _current_price = price; _update(); return *this; }
    double   get_current_price(double price)   const { return _current_price; }

    double   get_stock_value()                 const { return get_current_price() * _shares; }

    double   value()                           const { return get_stock_value() + _cash; }
    
    account &set_tick(const tick &t)                 { return set_date_time(t.seconds).set_current_price(t.close); }

    std::ostream &print(std::ostream &os) const
    {
        os << "date time:           " << _date_time         << std::endl
           << "cash:                " << _cash              << std::endl
           << "initial cash:        " << _initial_cash      << std::endl
           << "shares:              " << _shares            << std::endl
           << "current share price: " << _current_price     << std::endl
           << "stock value:         " << get_stock_value()  << std::endl
           << "total value:         " << value()            << std::endl;
        return os;
    }

    order buy_order(uint64_t date_time, double shares)
    {
        order ord(date_time, order::BUY, shares, get_current_price());
        if (ord.value() >= _cash) {
            ord.shares = order::shares_for_currency(_cash);
        }
        return ord.validate();
    }
    
    order sell_order(uint64_t date_time, double shares)
    {
        return order(date_time, order::SELL, _shares > shares ? shares : _shares, get_current_price());
    }

    order execute(order ord)
    {
        ord.validate();
        if (ord.action == order::NONE) {
            return ord;
        }
        double sign = ord.action == order::BUY ? 1. : -1.;
        _shares += sign * ord.shares;
        _cash   -= sign * ord.value();
        orders.push_back(ord);
        _update();
        return ord;
    }

private:

    double _update()
    {
        _stock_value = _current_price * _shares;
        double value = _stock_value + _cash;
        _high = std::max(_high, value);
        _low  = std::min(_low,  value);
        return value;
    }
};

struct context
{
    size_t               index;
    const frame         &data;
    double               current_shares;
    double               current_value;
    double               account_high;
    double               account_low;

    context(const frame &data) : data(data), index(0), current_shares(0), account_high(-std::numeric_limits<double>::max()), account_low(std::numeric_limits<double>::max()), current_value(0.) {}

    const tick &current() const { return data.data[index]; }
    
    bool done() { return index >= data.data.size(); }

    context &advance()
    {
        index++;
        return *this;
    }
    
    double ema(size_t count, int id = -1)    const { return data.data[id == -1 ? index : id].ema[count]; }
    double signal(size_t count, int id = -1) const { return data.data[id == -1 ? index : id].signal[count]; }
    
    double macd(int id = -1)                 const { return ema(12, id) - ema(26, id); }
    
    double macd_signal(int id = -1)          const { return signal(9, id); }

    bool  macd_pos(int id = -1)             const { return macd_signal(id) >= macd(id); }
    bool  macd_neg(int id = -1)             const { return macd_signal(id) <= macd(id); }
    
    bool  macd_cross_up(int id = -1)        const { int i = (id == -1 ? static_cast<int>(index) : id); return macd_pos(i) && !macd_pos(i - 1); }
    bool  macd_cross_down(int id = -1)      const { int i = (id == -1 ? static_cast<int>(index) : id); return !macd_pos(i) && macd_pos(i - 1); }
};

void fill_with_random_motion(frame &frame, size_t count, double base_price, double increment, double volatility)
{
    size_t time = 0;
    tick t;
    for (size_t i = 0; i < count; i++) {
        double offset = (rand() & 1 ? -1. : 1.) * rand() * volatility / RAND_MAX + increment,
               variation = (rand() & 1 ? -1. : 1.) * rand() / RAND_MAX;
        t.open = base_price;
        t.close = t.open + offset;
        double a = t.close + variation, b = t.open + variation;
        t.low = std::min(std::min(t.open, t.close), std::min(a, b));
        t.high = std::max(std::max(t.open, t.close), std::max(a, b));
        base_price = t.close;
        time += 60;
        t.seconds = time;
        frame.add(t);
        if (i == 0) {
            std::cout << std::endl << "start " << t.open;
        }
    }
    frame.prepare();
    std::cout << std::endl << "end   " << t.close << std::endl;
}

struct strategy
{
    void execute(context &c)
    {
        double money = 100000.0;
        while (!c.done()) {
            if (money && c.macd_cross_up()) {
                money -= c.buy_all(money);
            } else if (c.macd_cross_down()) {
                money += c.sell_all();
            }
            c.advance();
        }
        std::cout << "money " << money << std::endl;
    }
};

void test_stock()
{
    frame fr(100000000);
    fill_with_random_motion(fr, 10000, 15.00, 0.001, 1.0);
    context cont(fr);
    strategy strat;
    strat.execute(cont);
}

}
