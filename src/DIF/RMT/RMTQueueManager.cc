//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "RMTQueueManager.h"

Define_Module(RMTQueueManager);

RMTQueueManager::RMTQueueManager()
{
}

RMTQueueManager::~RMTQueueManager()
{

}

void RMTQueueManager::initialize()
{
    qMonPolicy = ModuleAccess<RMTQMonitorBase>("queueMonitorPolicy").get();
    WATCH_PTRVECTOR(queues);
    WATCH_PTRMAP(queueToPort);
}


RMTQueue* RMTQueueManager::addQueue(RMTQueue::queueType type, RMTPort* port, unsigned short qosId)
{
    // find factory object
    cModuleType *moduleType = cModuleType::get("rina.DIF.RMT.RMTQueue");

    // generate a name
    std::ostringstream queueName;
    const char* strType = (type == RMTQueue::INPUT ? "in" : "out");
    for (int i = 0; ; i++)
    {
        queueName << strType << i;
        if (lookup(queueName.str().c_str(), type) != NULL)
        {
            queueName.str(std::string());
            queueName.clear();
            continue;
        }
        else
        {
            break;
        }
    }

    cModule *module_f = moduleType->createScheduleInit(queueName.str().c_str(), this->getParentModule());
    RMTQueue* module = dynamic_cast<RMTQueue*>(module_f);

    // modify the position a little
    cDisplayString& disp = module->getDisplayString();

    if (port != NULL)
    {
        disp.setTagArg("p", 0, atoi(port->getDisplayString().getTagArg("p", 0)) - 40);
    }
    else
    {
        disp.setTagArg("p", 0, 30);
    }

    disp.setTagArg("p", 1, 130 + (portQueueCount[port] * 40));
    portQueueCount[port] += 1;

    // connect to RMT submodule
    cModule* rmt = getParentModule()->getModuleByPath(".rmt");
    if (type == RMTQueue::OUTPUT)
    {
        cGate* rmtOut = rmt->addGate(queueName.str().c_str(), cGate::OUTPUT, false);
        rmtOut->connectTo(module->getInputGate());
        module->setRmtAccessGate(rmtOut);
    }
    else if (type == RMTQueue::INPUT)
    {
        cGate* rmtIn = rmt->addGate(queueName.str().c_str(), cGate::INPUT, false);
        module->getOutputGate()->connectTo(rmtIn);
    }

    module->setType(type);
    module->setQosId(qosId);
    qMonPolicy->postQueueCreation(module);
    queues.push_back(module);
    queueToPort[module] = port;

    return module;
}



void RMTQueueManager::removeQueue(RMTQueue* queue)
{
    cModule* rmt = getParentModule()->getModuleByPath(".rmt");

    if (queue->getType() == RMTQueue::OUTPUT)
    {
        queue->getRmtAccessGate()->disconnect();
    }
    else
    {
        queue->getOutputGate()->disconnect();
    }

    rmt->deleteGate(queue->getName());

    // remove item from table
    RMTQueuesIter i = queues.begin();
    while (i != queues.end())
    {
        if (*i == queue)
        {
            i = queues.erase(i);
        }
        else
        {
            ++i;
        }
    }

    qMonPolicy->preQueueRemoval(queue);

    queue->deleteModule();
}

RMTQueue* RMTQueueManager::lookup(const char* queueName, RMTQueue::queueType type)
{
    for(std::vector<RMTQueue*>::iterator it = queues.begin(); it != queues.end(); ++it )
    {
        RMTQueue* a = *it;
        if (!opp_strcmp(a->getName(), queueName) && (a->getType() == type))
        {
            return a;
        }
    }
    return NULL;
}

RMTQueue* RMTQueueManager::lookup(short qosId, RMTQueue::queueType type)
{
    for(std::vector<RMTQueue*>::iterator it = queues.begin(); it != queues.end(); ++it )
    {
        RMTQueue* a = *it;
        if (qosId == a->getQosId())
        {
            return a;
        }
    }
    return NULL;
}

RMTQueue* RMTQueueManager::getFirst(RMTPort* port, RMTQueue::queueType type)
{
    for(RMTQueues::iterator it = queues.begin(); it != queues.end(); ++it )
    {
        RMTQueue* a = *it;
        if (a->getType() == type)
        {
            return a;
        }
    }
    return NULL;
}

RMTQueue* RMTQueueManager::getLongest(RMTPort* port, RMTQueue::queueType type)
{
    int longest = 0;
    RMTQueue* result = NULL;

    for(RMTQueues::iterator it = queues.begin(); it != queues.end(); ++it)
    {
        RMTQueue* a = *it;
        if (a->getLength() > longest)
        {
            longest = a->getLength();
            result = a;
        }
    }

    return result;
}

RMTQueueManager::iterator RMTQueueManager::begin()
{
    return this->queues.begin();
}

RMTQueueManager::iterator RMTQueueManager::end()
{
    return this->queues.end();
}

