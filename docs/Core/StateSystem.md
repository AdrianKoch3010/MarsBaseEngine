# State System

Any software consists of multiple states. Typically, in a game those states will include the menus, the actual game state and perhaps some loading when transitioning between them. The mars base engine provides a simple interface for creating and managing such states and passing data to them.

### The Game Loop
Any game will most likely have some form of game loop that is run for the duration of the program running. It consists out of:

- Handling user input
- Updating the game logic
- Rendering the scene on screen

The State and StateManager provide the following functions respectivly:
- HandleSFMLEvents()
- Update()
- Render()

The StateManager takes care of calling these functions for the currently active states. All that is left in the main is to call these mehtods on the StateManager instance which may look something like this.

```cpp
while (window.isOpen())
{
    // Handle input events
    while (window.pollEvent(event))
	{
		// pass the event to the state manager
		stateManager.HandleSFMLEvents(event);
	}

    // Update the current states
    stateManager.Update(frameTime);

    // Quit the game if the state stack is empty
    if (stateManager.IsStateStackEmpty())
        window.close();

    // Render
    window.clear();
	stateManager.Render();
	window.display();

    // Get the time between frames
    frameTime = clock.restart();
}
```

## State Mashine Plus
The simplest way to implement such a system is by making use of some form of finite state mashine. This works well with the exception of one significant caveat - no more than a single state can be active at a time. Although this might seem reasonable at first, there are scenarios where one state needs to be overlayed another.

A good example of this would be a pause screen. The game is no longer updated and doesn't take any user input but it is still drawn (with the pause screen possibly drawn on top). Most importantly, the game content must not be stored and reloaded.

### The State Stack



## State Context
Every state has a context. The state context is global information that can be used by every state. It consists out of references to the

- Render window
- Texture wrappers
- Sounds
- Music
- Fonts

