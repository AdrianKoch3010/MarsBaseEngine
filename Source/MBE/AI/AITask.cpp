#include <MBE/AI/AITask.h>

using namespace mbe;


AITask::AITask(float utility) :
	utility(utility),
	aborting(false),
	completed(false),
	failed(false)
{

}