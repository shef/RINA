#include "Addons/DataInjectors/FlowsSimulation/PDUandData/PDU_Req.h"
#include "./Flow_t.h"
#include "Addons/DataInjectors/FlowsSimulation/PDUandData/Flow_Data.h"

PDU_Req::PDU_Req () :
    f(nullptr), data(nullptr) {}

PDU_Req::PDU_Req (Flow_t * _f, Flow_Data * _data ) :
    f(_f), data(_data) {}

