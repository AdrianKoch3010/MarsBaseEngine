#include "ParallelTask.h"

using mbe::ParallelTask;

ParallelTask::ParallelTask() : completed(false)
{
}

void ParallelTask::Execute()
{
	// When executing the same task multiple times, completed must be reset
	this->completed = false;

	// Launch the thread
	this->thread = std::thread(threadFunction);
}