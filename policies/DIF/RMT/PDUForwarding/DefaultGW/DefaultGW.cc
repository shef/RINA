// The MIT License (MIT)
//
// Copyright (c) 2014-2016 Brno University of Technology, PRISTINE project
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include <DefaultGW.h>
#include <iostream>
#include <sstream>

Register_Class(DefaultGW);

using namespace std;


void DefaultGW::onPolicyInit() { }

void DefaultGW::setGW(RMTPort * port) {
    ret.clear();
    if(port!=nullptr) {
        ret.push_back(port);
    }
}

vector<RMTPort *> DefaultGW::lookup(const PDU * pdu){
    return ret;
}
vector<RMTPort *> DefaultGW::lookup(const Address &dst, const std::string&qos) {
    return ret;
}

string DefaultGW::toString() {
    return this->getName();
}


void DefaultGW::finish()
{
    if (par("printAtEnd").boolValue())
    {
        EV << toString() << endl;
    }
}