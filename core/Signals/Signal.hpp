#ifndef SIGNAL_HPP
#define SIGNAL_HPP

#include<map>
#include<functional>

template <typename... Args>
class Signal
{
public:
    Signal() = default;
    ~Signal() = default;

    Signal(Signal const&) {}

    Signal& operator = (Signal const& other)
    {
        if(this != &other){
            disconnect_all();
        }
        return *this;
    }

    Signal(Signal&& other) noexcept:
        _slots(std::move(other._slots)),
        _current_id(other._current_id) {}

    Signal& operator = (Signal&& other) noexcept
    {
        if(this != &other)
        {
            _slots = std::move(other._slots);
            _current_id = other.current_id;
        }
        return *this;
    } 

    int connect(std::function<void(Args...)> const& slot) const
    {
        _slots.insert(std::make_pair(++_current_id, slot));
        return _current_id;
    }

    template<typename T>
    int connect_member(T *inst, void(T::*func)(Args...))
    {
        return connect([=](Args... args){
            (inst ->*func)(args...);
        });
    }

    template<typename T>
    int connect_member(T *inst, void(T::*func)(Args...) const)
    {
        return connect([=](Args... args){
            (inst ->*func)(args...);
        });
    }

    void disconnect(int id) const
    {
        _slots.erase(id);
    }

    void disconnect_all() const
    {
        _slots.clear();
    }

    void emit(Args... p)
    {
        for(auto& it : _slots)
            it.second(p...);
    }

    void emit_for_all_but_one(int excludedConnectionID, Args... p)
    {
        for(auto& it : _slots)
        {
            if( it.first != excludedConnectionID )
                it.second(p...);
        }
    }

    void emit_for(int connectionID, Args... p)
    {
        auto const& it = _slots.find(connectionID);
        if( it != _slots.end() )
            it -> second(p...);
    }


    private:
    mutable std::map<int, std::function<void(Args...)> > _slots;
    mutable int _current_id {0};
};
#endif