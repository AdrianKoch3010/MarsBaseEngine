# Serialisation
Serialisation is the process of translating data structures or object state into a format that can be stored. The Mars Base Engine takes the approach of using a human-readable format, namely XML for this task. Any set of entities can be saved to and loaded from an XML file (or string containing valid xml) using the ```mbe::EntitySerialiser```.

Further, serialisation is seperated into its own module. Rather than forcing each component or entity to provide an interface for loading and storing, there exist seperate classes for this purpose.

## Entity Serialiser
The first of those classes is the entity serialiser. 

## Component Serialiser


There are two component serialisers that themself provide an interface for attaching lower level serialisers; the animation and ai components.

### Animations

### AI Tasks

## Writing Custom Serialisers
The engine doesn't provide its own XML library. However, the library used internally is embedded into the source code and, therefore, ready for use. Documentation on how to it can be found [here](http://www.grinninglizard.com/tinyxml2/).

#### Writing custom component serialiser class

To write your own component serialiser you have to inherit from ```mbe::ComponentSerialiser``` and overwrite the ```LoadComponent()``` and ```StoreComponent()``` methods.

``` c++
class CustomComponentSerialiser : public mbe::ComponentSerialiser´
{
public:
    typedef std::shared_ptr<CustomComponentSerialiser> Ptr;
    typedef std::weak_ptr<CustomComponentSerialiser> WPtr;
    typedef std::unique_ptr<CustomComponentSerialiser> UPtr;

public:
    CustomComponentSerialiser(/* possible arguments*/);
    ~CustomComponentSerialiser();

public:
    void LoadComponent(mbe::Entity& entity, const tinyxml2::XMLElement& componentData) override;

    void StoreComponent(const mbe::Entity& entity, tinyxml2::XMLDocument& document, tinyxml2::XMLElement& componentData) override;
}
```

It is entirely up to you, what to load and store, but the convention is to store **everything** (or at least everything that is required to recreate the component in the exact same state). Its also good practice to follow the engine convention of defining all smart pointer types as typedefs.

#### Registering the component serialiser
Don't forget to register the newly created component serialiser to the entity serialiser used. This is done by using the ```AddComponentSerialiser()``` method provided by the entity serialiser.

``` c++
// The two template parameters are the type of the serialiser and the component this serialiser is added for
// The first string argument will be the name used for the component's type attribute in the XML file
entitySerialiser.AddComponentSerialiser<CustomComponentSerialiser, CustomComponent>("CustomComponent", /*possible arguments*/);
```

Once added, the component serialiser can be retrieved by its type using the ```GetComponentSerialiser<TComponentSerialiser>()``` method.