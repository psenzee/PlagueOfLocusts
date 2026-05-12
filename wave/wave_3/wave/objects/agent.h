#pragma once

#include "serialize.h"
#include "collection.h"

struct agent : public object
{
    enum AgentType : uint16_t { AGENT_NONE = 0 };
    
    inline agent(AgentType agent_type = AGENT_NONE)
        : object(AGENT), _agent_type(AGENT_NONE), _inventory(std::unique_ptr<collection>()) {}
    
    inline AgentType agent_type() const { return _agent_type; }
    
    collection &inventory() { return *_inventory; }
    
    std::size_t write(binary_out_stream &os) const
    {
        std::size_t bytes = os.write(type());
        bytes += os.write(agent_type());
        // TODO other stuff
        bytes += _inventory->write(os);
        return bytes;
    }

    std::size_t read(binary_in_stream &is, bool read_type = true)
    {
        std::size_t bytes = 0;
        if (read_type) {
            Type this_type = NONE;
            bytes += is.read(this_type);
            if (this_type != type()) {
                throw std::runtime_error("expected agent type");
            }
        }
        AgentType agent = AGENT_NONE;
        bytes += is.read(agent);
        _agent_type = agent;
        // TODO other stuff
        bytes += _inventory->read(is, true);
        return bytes;
    }
    
private:
    
    AgentType                   _agent_type;
    std::unique_ptr<collection> _inventory;
};

