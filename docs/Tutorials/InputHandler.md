# Handling User Input

## Input Types

Currently, the engine supports two main types of input; Mouse and Keyboard.

### Using SFML types

The engine uses the same key (and mouse) types as SFML which makes interfacing easier and avoids an unecessary duplication of code.

## Allowing for Key Bindings

A big part of a good input system is to allow the user to assign the keys themselves. This can be achieved by decoupling the actual physical input from the corresponding logical action.