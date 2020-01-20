# Animation
On an abstract level, animation means changing an entity's state over time in some defined way. The animation system provides a very general interface for this purpose as well as functionaliy for managaing any animation on an entity bases through the AnimationComponent.

## Definition of an Animation
An animation is defined as a function object of the following type: 
```c++
typedef std::function<void(mbe::Entity&, float)> AnimationFunction;
```

The float parameter represents the progress of the animation and is a value between 0 and 1. This means that animations must not be constrained to graphics. It is perfectly valid to define 'animations' acting on pitch or volume of an audio source, for instance.

## Useful Animations
As described above, an animation can be literally anything. However, there are a couple useful animations provided by the engine itself that are listed below.

### Frame Animation
This animation will presumably be used the most. It allows for animating a sprite by changing the part of the [sprite sheet](https://www.codeandweb.com/what-is-a-sprite-sheet) drawn on screen over some specified amount of time.

Frames corresponding to an animation are assumed to be stored from left to right in one or more rows of the sprite sheet. Adding frames is done using the AddFrames() function that allows for adding frames in a row. If the animation spannes multiple rows, this function can be called multiple times. There is no requirement for the frames to be equal in size although they most likely will be in practise.

```c++
// Create a new FrameAnimation
mbe::FrameAnimation animation;

// Add the first 4 frames stored in the second row of the sprite sheet
// The individual frames are 32 by 64 pixels big. Therefore, the second row will start at (0, 64)
animation.AddFrames({ 32u, 64u }, { 0u, 64u }, 4);
// Add another two frames stored in the next row
animation.AddFrames({ 32u, 64u }, { 0u, 128u }, 2);
```

## AnimationComponent and Animators

An animator stores an object's animations and their progress. It takes care of multiple animations which are registered by an ID. The animations can be played at any time but only **one** animation per animator can be played at once.

The AnimationComponent provides an interface for creating and accessing animators. It also provides an additional layer of abstraction meaning that the animator must not necessarily be known to play, pause or stop an animation. This can be useful when two animators have an animation with the same id and one wants to play both of them using the PlayAnimation() function. Differentiating between them is possible by passing the animator's id as an argument. The complete documentation can be found [here](Doxygen/classmbe_1_1_animation_component.html).

>Example

```c++
// Create a new FrameAnimation
mbe::FrameAnimation walkUpAnimation;
// Add 9 64 by 64 pixel frames (from left to right) starting from position (0, 0)
walkUpAnimation.AddFrames({ 64u, 64u }, { 0u, 0u }, 9);

// Create an animator and add the animation with a duration of one second
// In practice, more animation would be added at this stage
auto & animator = animationComponent.CreateAnimator("WalkAnimator");
animator.AddAnimation("WalkUp", walkUpAnimation, sf::seconds(1.f));

// Play the animation in a loop
animationComponent.PlayAnimation("walkuP", true);
```

In the last line one should notice two things. Firstly, the fact that the id of the animator (WalkAnimator) is not needed to play the animation. Note, however, that if the animation component happend to have another animator containing an animation with the same id (WalkUp), that animation will also be played.

The second thing to note is the supposed misspelling of the animation id. This is perfectly valid since all string ids get normalised (converted to lower case) by the engine. This is true for the engine in general but also applies here. This is something to keep in mind when using the GetPlayingAnimation() function. Therefore, it is recommended to use one of the overloads of the IsPlayingAnimation() function to check whether a certain animation / animator is currently playing. If this is not applicible, mbe::NormaliseIdString() should be used as shown below.

```c++
bool isPlaying;

// Check whether any animation is playing
isPlaying = animationComponent.IsPlayingAnimation();
// Check whetehr a specific animator is playing any animation
isPlaying = animationComponent.IsPlayingAnimation("AnimatorID");
// Check whether a specific animator is playing a specific aniamtion
isPlaying = animationComponent.IsPlayingAnimation("AnimatorID", "AnimationID");

// This should generally be avoided, but this is how to do it anyways
// Note that this list could contain the same animation id twice as they are only unique per animator
std::vector<std::string> animationList = animationComponent.GetPlayingAnimations();
for (const auto & animationId : animationList)
{
    // Using NormaliseStringID is important here since the animationList contains normalised ids
    if (mbe::NormaliseIDString("AnimationID") == animationId)
    {
        // The animation is playing
    }
}
```


## Animations and Child Entities
The section on the [entity component system](EntityComponentSystem.md) descirbes how a game object may be constructed using multiple entities. This makes it possible for the following scenario to arise:

A game character might be comprised of an entity with components like TileMapComponent, MovementComponent and so on as well as a child entity for rendering with the TransformComponent, SpriteRenderComponent, AnimationComponent etc. needed specifically for displaying it on screen. In fact, it is likely to have multiple child entities for rendering; one for each body part for instance. Now, most of the time we will be dealing with the parent entity as its the main entity describing most of the character. Hence, when we call ```characterEntity.PlayAnimation("AnimationId")``` we expect this to work - despite the animations being stored in a different entity.

The engine provides this behaviour by recursivly trying to play the requested animation on all child entities that also have an AnimationComponent. Therefore, if there happen to be more than one child entity with the same animation id, all the respective animations will be played. Similar to how the PlayAnimation() function doesn't require the animator to be known, this functionality allows for some very flexible behaviour when dealing with animations. Consider the case where the character's legs are rendered as two seperate entities; playing the walk animation will act on both legs, provided that they both have an animation registered under the same id.

The underlaying principle can be compared to how the [EntityClickedEvent](Events.md) 'bubbles up' through the parent entities. Contrasting the EntityClickedEvent, however, the logic travells from the parent to the children.

## Extra - Dealing with Animators
The engine allows accessing the low level animation interface through declaring user specific animators. In fact, an animation can act on any object (not just entities). Further, any sutable type can be chosen as an unique identifier for the animations. However, there is a template overload for std::string that normalises the string id, which is in turn the one used by the AnimationComponent. Using this functionality is not recommended as it may raise compatibility issues with other systems.
