/* 
 * Copyright (C) 2016 iCub Facility - Istituto Italiano di Tecnologia
 * Author: Ugo Pattacini <ugo.pattacini@iit.it>
 * CopyPolicy: Released under the terms of the GNU GPL v3.0.
*/

#include <mutex>
#include <condition_variable>
#include <chrono>
#include <string>

#include <yarp/robottestingframework/TestCase.h>
#include <robottestingframework/dll/Plugin.h>
#include <robottestingframework/TestAssert.h>

#include <yarp/os/all.h>
#include <yarp/dev/all.h>
#include <yarp/sig/all.h>
#include <yarp/math/Math.h>

using namespace std;
using namespace robottestingframework;
using namespace yarp::os;
using namespace yarp::dev;
using namespace yarp::sig;
using namespace yarp::math;

#define TEST_MESSAGE    "Hello Tutorial"

class BottleReader : public BufferedPort<Bottle> {
    std::string msg;
    mutex mtx_semaphore;
    condition_variable cv_semaphore;

public:
    BottleReader(): msg("timeout") { }

  virtual void onRead(Bottle &bt) {
        if(bt.size() && bt.get(0).isString())
            msg = bt.get(0).asString();
        else
            msg = "wrong data";
        cv_semaphore.notify_all();
    }

    const std::string getMessage() {
        unique_lock<mutex> lck(mtx_semaphore);
        cv_semaphore.wait_for(lck, chrono::seconds(5));
        return msg;
    }
};

/**********************************************************************/
class TestTutorialRFModule : public yarp::robottestingframework::TestCase
{
private:
        BufferedPort<Bottle> portOut;
        BottleReader portIn;
public:
    /******************************************************************/
    TestTutorialRFModule() :
        yarp::robottestingframework::TestCase("TestTutorialRFModule") { }
    
    /******************************************************************/
    virtual ~TestTutorialRFModule() {
    }
    
    /******************************************************************/
    virtual bool setup(yarp::os::Property& property) {       

        ROBOTTESTINGFRAMEWORK_ASSERT_ERROR_IF_FALSE(portOut.open("/TestTutorialRFModule/o"), "Cannot open the output port");
        ROBOTTESTINGFRAMEWORK_ASSERT_ERROR_IF_FALSE(portIn.open("/TestTutorialRFModule/i"), "Cannot open the output port");

        ROBOTTESTINGFRAMEWORK_ASSERT_ERROR_IF_FALSE(NetworkBase::connect("/decoder/Codec/out", portIn.getName()),
                                  "Cannot connect to /decoder/Codec/out");

        ROBOTTESTINGFRAMEWORK_ASSERT_ERROR_IF_FALSE(NetworkBase::connect(portOut.getName(), "/coder/Codec/in"),
                                  "Cannot connect to /coder/Codec/in");
        portIn.useCallback();
        return true;
    }
    
    /******************************************************************/
    virtual void tearDown() {
        ROBOTTESTINGFRAMEWORK_TEST_REPORT("Tearing down TestTutorialRFModule");
        NetworkBase::disconnect("/decoder/Codec/in", portIn.getName());
        NetworkBase::disconnect(portOut.getName(), "/coder/Codec/out");
        portIn.interrupt();
        portIn.close();
        portOut.close();
    }
    
    /******************************************************************/
    virtual void run()
    {   
        ROBOTTESTINGFRAMEWORK_TEST_REPORT(Asserter::format("Writing test message %s", TEST_MESSAGE));
        Bottle& msg = portOut.prepare();
        msg.clear();
        msg.addString(TEST_MESSAGE);
        portOut.write(true);
        ROBOTTESTINGFRAMEWORK_TEST_REPORT("Reading decoded test message");
        string str = portIn.getMessage();
        ROBOTTESTINGFRAMEWORK_TEST_CHECK(str == string(TEST_MESSAGE),
                       Asserter::format("encoded/decoded message (%s == %s)", str.c_str(), TEST_MESSAGE) );
    }

    /******************************************************************/
    virtual void interrupt() {
        portIn.interrupt();
    }
};

ROBOTTESTINGFRAMEWORK_PREPARE_PLUGIN(TestTutorialRFModule)
