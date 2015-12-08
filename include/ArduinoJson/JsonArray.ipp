// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "JsonArray.hpp"
#include "JsonArraySubscript.hpp"

namespace ArduinoJson {

inline JsonArraySubscript JsonArray::operator[](size_t index) {
  return JsonArraySubscript(*this, index);
}

inline JsonVariant JsonArray::operator[](size_t index) const {
  return get(index);
}

template <typename TValue>
inline bool JsonArray::addNode(TValue value) {
  node_type *node = addNewNode();
  if (node == NULL) return false;
  setNodeValue<TValue>(node, value);
  return true;
}

template <typename TValue>
inline void JsonArray::setNodeAt(size_t index, TValue value) {
  node_type *node = getNodeAt(index);
  if (node == NULL) return;
  setNodeValue<TValue>(node, value);
}

template <typename TValue>
inline void JsonArray::setNodeValue(node_type *node, TValue value) {
  node->content = value;
}

template <>
inline void JsonArray::setNodeValue(node_type *node, const String &value) {
  node->content = _buffer->strdup(value);
}

inline JsonVariant JsonArray::get(size_t index) const {
  node_type *node = getNodeAt(index);
  return node ? node->content : JsonVariant();
}

template <typename T>
inline T JsonArray::get(size_t index) const {
  node_type *node = getNodeAt(index);
  return node ? node->content.as<T>() : JsonVariant::invalid<T>();
}

template <typename T>
inline bool JsonArray::is(size_t index) const {
  node_type *node = getNodeAt(index);
  return node ? node->content.is<T>() : false;
}

template <typename TImplem>
inline const JsonArraySubscript JsonVariantBase<TImplem>::operator[](
    int index) const {
  return asArray()[index];
}

template <>
inline JsonArray &JsonVariant::invalid<JsonArray &>() {
  return JsonArray::invalid();
}

template <>
inline JsonArray const &JsonVariant::invalid<JsonArray const &>() {
  return JsonArray::invalid();
}

inline JsonArray &JsonVariant::asArray() const {
  if (_type == Internals::JSON_ARRAY) return *_content.asArray;
  return JsonArray::invalid();
}
}
