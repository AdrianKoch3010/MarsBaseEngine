#include <MBE/Core/ParallelTask.h>

using mbe::ParallelTask;

ParallelTask::ParallelTask() : completed(false)
{
}

void ParallelTask::Execute()
{
	// When executing the same task multiple times, completed must be reset
	this->completed = false;

	// Launch the thread
	//sf::Clock clock;
	this->thread = std::thread(threadFunction);
	//std::cout << std::endl << "The pathfinder insertion took " << clock.restart().asMicroseconds() << " us";
}