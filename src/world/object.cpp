#include "apsis/world/object.h"

#include "apsis/registry/event.h"

const Apsis::World::Value& Apsis::World::Object::get(const char* key) const {
  return this->get(Apsis::Registry::Property::id(key));
}

const Apsis::World::Value& Apsis::World::Object::get(unsigned int key) const {
  if (this->_properties.count(key) == 0) {
    throw "Key not found";
  }

  const Apsis::World::Value& value = this->_properties.at(key);
  return value;
}

bool Apsis::World::Object::has(const char* key) const {
  return this->has(Apsis::Registry::Property::id(key));
}

bool Apsis::World::Object::has(unsigned int key) const {
  return this->_properties.count(key) != 0;
}

void Apsis::World::Object::set(const char* key, double value) {
  this->set(Apsis::Registry::Property::id(key), value);
}

void Apsis::World::Object::set(unsigned int key, double value) {
  this->_properties[key] = World::Value(value);
}

void Apsis::World::Object::set(const char* key, long value) {
  this->set(Apsis::Registry::Property::id(key), value);
}

void Apsis::World::Object::set(unsigned int key, long value) {
  this->_properties[key] = World::Value(value);
}

void Apsis::World::Object::set(const char* key, const char* value) {
  this->set(Apsis::Registry::Property::id(key), value);
}

void Apsis::World::Object::set(unsigned int key, const char* value) {
  this->_properties[key] = World::Value(value);
}

bool Apsis::World::Object::isEnabled(unsigned int state) const {
  return this->_states.count(state) > 0;
}

void Apsis::World::Object::enableState(unsigned int state) {
  this->_states.insert(state);
}

void Apsis::World::Object::disableState(unsigned int state) {
  this->_states.erase(state);
}

bool Apsis::World::Object::toggleState(unsigned int state) {
  if (this->isEnabled(state)) {
    this->disableState(state);
    return false;
  }
  else {
    this->enableState(state);
    return true;
  }
}

void Apsis::World::Object::enqueueEvent(unsigned int event_id) {
  _events.push_back(event_id);
}

void Apsis::World::Object::enqueueEvent(const char* event) {
  enqueueEvent(Apsis::Registry::Event::id(event));
}

void Apsis::World::Object::respondTo(unsigned int event_id) {
  _responds_to.insert(event_id);
}

void Apsis::World::Object::respondTo(const char* event) {
  respondTo(Apsis::Registry::Event::id(event));
}

void Apsis::World::Object::ignore(unsigned int event_id) {
  _responds_to.erase(event_id);
}

void Apsis::World::Object::ignore(const char* event) {
  ignore(Apsis::Registry::Event::id(event));
}

bool Apsis::World::Object::respondsTo(unsigned int event_id) const {
  return _responds_to.count(event_id) == 1;
}

bool Apsis::World::Object::respondsTo(const char* event) const {
  return respondsTo(Apsis::Registry::Event::id(event));
}

unsigned int Apsis::World::Object::dequeueEvent() {
  // TODO: Typical std::deque race condition
  unsigned int ret = _events.front();
  _events.pop_front();
  return ret;
}

bool Apsis::World::Object::hasEvents() const {
  return _events.size() > 0;
}


const Apsis::World::Value& Apsis::World::Object::getForEvent(unsigned int event_id,
                                                             const char* key) const {
  return this->getForEvent(event_id, Apsis::Registry::Property::id(key));
}

const Apsis::World::Value& Apsis::World::Object::getForEvent(unsigned int event_id,
                                                             unsigned int key) const {
  if (this->_event_properties.count(event_id) == 0) {
    throw "Event not available";
  }

  if (this->_event_properties.at(event_id).count(key) == 0) {
    throw "Key not found";
  }

  const Apsis::World::Value& value = this->_event_properties.at(event_id).at(key);
  return value;
}

bool Apsis::World::Object::hasForEvent(unsigned int event_id,
                                       const char* key) const {
  return this->hasForEvent(event_id,
                           Apsis::Registry::Property::id(key));
}

bool Apsis::World::Object::hasForEvent(unsigned int event_id,
                                       unsigned int key) const {
  return this->_properties.count(key) != 0;
}

void Apsis::World::Object::setForEvent(unsigned int event_id,
                                       const char* key,
                                       double value) {
  this->setForEvent(event_id,
                    Apsis::Registry::Property::id(key),
                    value);
}

void Apsis::World::Object::setForEvent(unsigned int event_id,
                                       unsigned int key,
                                       double value) {
  this->_event_properties[event_id][key] = World::Value(value);
}

void Apsis::World::Object::setForEvent(unsigned int event_id,
                                       const char* key,
                                       long value) {
  this->setForEvent(event_id,
                    Apsis::Registry::Property::id(key),
                    value);
}

void Apsis::World::Object::setForEvent(unsigned int event_id,
                                       unsigned int key,
                                       long value) {
  this->_event_properties[event_id][key] = World::Value(value);
}

void Apsis::World::Object::setForEvent(unsigned int event_id,
                                       const char* key,
                                       const char* value) {
  this->setForEvent(event_id,
                    Apsis::Registry::Property::id(key),
                    value);
}

void Apsis::World::Object::setForEvent(unsigned int event_id,
                                       unsigned int key,
                                       const char* value) {
  this->_event_properties[event_id][key] = World::Value(value);
}

bool Apsis::World::Object::isMe(const Apsis::World::Object& reference) const {
  return &reference == this;
}