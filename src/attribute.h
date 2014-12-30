#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include "entity.h"

namespace herbstluft {

class Object;

class Attribute : public Entity {
public:
    Attribute() {}
    Attribute(const std::string &name,
              bool readable, bool writeable)
        : Entity(name),
          readable_(readable), writeable_(writeable) {}
    // set the owner after object creation (when pointer is available)
    void setOwner(std::weak_ptr<Object> owner) { owner_ = owner; }
    virtual ~Attribute() {};

    virtual Type type() { return Type::ATTRIBUTE; }

    std::shared_ptr<Object> owner();

    // all access to the payload is delegated to owner!
    bool readable();
    std::string read();
    bool writeable();
    void write(const std::string &value);

    // accessors only to be used by owner!
    // TODO for both: programming error when reached, thrown an exception.
    virtual std::string str() { return {}; }
    virtual void change(const std::string &payload) {}

protected:
    std::weak_ptr<Object> owner_;
    bool readable_, writeable_;
};

/* attributes that don't hold reference a data field (no templating), but are
 * rather a shallow interface to the owner's getter and setter doing magic. */
class DynamicAttribute : public Attribute {
public:
    DynamicAttribute() {}
    DynamicAttribute(const std::string &name, Type type,
                     bool readable, bool writeable)
        : Attribute(name, readable, writeable), type_(type) {}

    Type type() { return type_; }

protected:
    Type type_;
};

class Action : public Entity {
public:
    Action() {}
    Action(const std::string &name)
        : Entity(name) {}
    void setOwner(std::weak_ptr<Object> owner) { owner_ = owner; }

    Type type() { return Type::ACTION; }

    void trigger(const std::string &args);
private:
    std::weak_ptr<Object> owner_;
};

} // close namespace before further includes!

#include "attribute_.h" // template and specializations

#endif // ATTRIBUTE_H
