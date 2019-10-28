# Animation
On an abstract level, animation means changing an entities state over time in some defined way. The animation system provides a very general interface for this purpose as well as functionaliy for managaing any animation on an entity bases through the AnimationComponent.

## Defining an Animation
An animation is defined as a function object of the following type: 
```c++
typedef std::function<void(mbe::Entity &, float)> AnimationFunction;
```

The float parameter represents the progress of the animation and is a value between 0 and 1.

## Useful Animations
As described above, an animation can be literally anything. However, there are a couple useful animations provided by the engine itself that are listed below.

### Frame Animation
This animation will presumably be used the most. It allows for animating a sprite by changing the part of the [sprite sheet](https://www.codeandweb.com/what-is-a-sprite-sheet) drawn on screen over some specified amount of time.

## AnimationComponent and Animators

An animator stores an object's animations and their progress. It takes care of multiple animations which are registered by an ID. The animations can be played at any time but only **one** animation per animator can be played at a time.

The AnimationComponent provides an interface for creating and accessing animators. It also provides an additional layer of abstraction meaning that the animator must not necessarily be known to play, pause or stop an animation. This can be useful, for example, when two animators have an animation with the same id and one wants to play both of them using the PlayAnimation() function. Differentiating between them is possible by passing the animator's id as an argument. The complete documentation can be found [here](Doxygen/classmbe_1_1_animation_component.html).

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
animationComponent.PlayAnimation("walkUp", true);
```

In the last line one should notice two things. Firstly, the fact that the id of the animator (WalkAnimator) is not needed to play the animation. Note, however, that if the animation component happend to have another animator containing an animation with the same id (WalkUp), that animation will also be played.

The second thing to note is the supposed misspelling of the animation id. This is perfectly valid since all string ids get normalised (converted to lower case) by the engine. This is true for the engine in general but also applies here. This is something to keep in mind when using the GetPlayingAnimation() function. Therefore, it is recommended use one of the overloads of the IsPlayingAnimation() function to check whether a certain animation / animator is currently playing. If this is not applicible, mbe::NormaliseIdString() should be used as shown below.

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


## Animation and Child Entities


## Extra - Dealing with Animators
The engine allows accessing the low level animation interface through declaring user specific animators. In fact, an animation can act on any object (not just entities). Further, any sutable type can be chosen as an unique identifier for the animations. However, there is a template overload for std::string that normalises the string id, which is in turn the one used by the AnimationComponent.
