// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: proto/debug/other.proto

#ifndef PROTOBUF_INCLUDED_proto_2fdebug_2fother_2eproto
#define PROTOBUF_INCLUDED_proto_2fdebug_2fother_2eproto

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3006001
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h> // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#define PROTOBUF_INTERNAL_EXPORT_protobuf_proto_2fdebug_2fother_2eproto

namespace protobuf_proto_2fdebug_2fother_2eproto {
// Internal implementation detail -- do not use these members.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[1];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable
      serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void AddDescriptors();
} // namespace protobuf_proto_2fdebug_2fother_2eproto
namespace debug {
class MessageThing;
class MessageThingDefaultTypeInternal;
extern MessageThingDefaultTypeInternal _MessageThing_default_instance_;
} // namespace debug
namespace google {
namespace protobuf {
template <>
::debug::MessageThing *
Arena::CreateMaybeMessage<::debug::MessageThing>(Arena *);
} // namespace protobuf
} // namespace google
namespace debug {

// ===================================================================

class MessageThing : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:debug.MessageThing)
                                                         */
{
public:
  MessageThing();
  virtual ~MessageThing();

  MessageThing(const MessageThing &from);

  inline MessageThing &operator=(const MessageThing &from) {
    CopyFrom(from);
    return *this;
  }
#if LANG_CXX11
  MessageThing(MessageThing &&from) noexcept : MessageThing() {
    *this = ::std::move(from);
  }

  inline MessageThing &operator=(MessageThing &&from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from)
        InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
#endif
  inline const ::google::protobuf::UnknownFieldSet &unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }
  inline ::google::protobuf::UnknownFieldSet *mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor *descriptor();
  static const MessageThing &default_instance();

  static void InitAsDefaultInstance(); // FOR INTERNAL USE ONLY
  static inline const MessageThing *internal_default_instance() {
    return reinterpret_cast<const MessageThing *>(
        &_MessageThing_default_instance_);
  }
  static constexpr int kIndexInFileMessages = 0;

  void Swap(MessageThing *other);
  friend void swap(MessageThing &a, MessageThing &b) { a.Swap(&b); }

  // implements Message ----------------------------------------------

  inline MessageThing *New() const final {
    return CreateMaybeMessage<MessageThing>(NULL);
  }

  MessageThing *New(::google::protobuf::Arena *arena) const final {
    return CreateMaybeMessage<MessageThing>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message &from) final;
  void MergeFrom(const ::google::protobuf::Message &from) final;
  void CopyFrom(const MessageThing &from);
  void MergeFrom(const MessageThing &from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream *input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream *output) const final;
  ::google::protobuf::uint8 *InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8 *target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(MessageThing *other);

private:
  inline ::google::protobuf::Arena *GetArenaNoVirtual() const { return NULL; }
  inline void *MaybeArenaPtr() const { return NULL; }

public:
  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required string name = 1;
  bool has_name() const;
  void clear_name();
  static const int kNameFieldNumber = 1;
  const ::std::string &name() const;
  void set_name(const ::std::string &value);
#if LANG_CXX11
  void set_name(::std::string &&value);
#endif
  void set_name(const char *value);
  void set_name(const char *value, size_t size);
  ::std::string *mutable_name();
  ::std::string *release_name();
  void set_allocated_name(::std::string *name);

  // optional string extra = 3;
  bool has_extra() const;
  void clear_extra();
  static const int kExtraFieldNumber = 3;
  const ::std::string &extra() const;
  void set_extra(const ::std::string &value);
#if LANG_CXX11
  void set_extra(::std::string &&value);
#endif
  void set_extra(const char *value);
  void set_extra(const char *value, size_t size);
  ::std::string *mutable_extra();
  ::std::string *release_extra();
  void set_allocated_extra(::std::string *extra);

  // required int32 id = 2;
  bool has_id() const;
  void clear_id();
  static const int kIdFieldNumber = 2;
  ::google::protobuf::int32 id() const;
  void set_id(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:debug.MessageThing)
private:
  void set_has_name();
  void clear_has_name();
  void set_has_id();
  void clear_has_id();
  void set_has_extra();
  void clear_has_extra();

  // helper for ByteSizeLong()
  size_t RequiredFieldsByteSizeFallback() const;

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  ::google::protobuf::internal::ArenaStringPtr name_;
  ::google::protobuf::internal::ArenaStringPtr extra_;
  ::google::protobuf::int32 id_;
  friend struct ::protobuf_proto_2fdebug_2fother_2eproto::TableStruct;
};
// ===================================================================

// ===================================================================

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif // __GNUC__
// MessageThing

// required string name = 1;
inline bool MessageThing::has_name() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void MessageThing::set_has_name() { _has_bits_[0] |= 0x00000001u; }
inline void MessageThing::clear_has_name() { _has_bits_[0] &= ~0x00000001u; }
inline void MessageThing::clear_name() {
  name_.ClearToEmptyNoArena(
      &::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_name();
}
inline const ::std::string &MessageThing::name() const {
  // @@protoc_insertion_point(field_get:debug.MessageThing.name)
  return name_.GetNoArena();
}
inline void MessageThing::set_name(const ::std::string &value) {
  set_has_name();
  name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
                   value);
  // @@protoc_insertion_point(field_set:debug.MessageThing.name)
}
#if LANG_CXX11
inline void MessageThing::set_name(::std::string &&value) {
  set_has_name();
  name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
                   ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:debug.MessageThing.name)
}
#endif
inline void MessageThing::set_name(const char *value) {
  GOOGLE_DCHECK(value != NULL);
  set_has_name();
  name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
                   ::std::string(value));
  // @@protoc_insertion_point(field_set_char:debug.MessageThing.name)
}
inline void MessageThing::set_name(const char *value, size_t size) {
  set_has_name();
  name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
                   ::std::string(reinterpret_cast<const char *>(value), size));
  // @@protoc_insertion_point(field_set_pointer:debug.MessageThing.name)
}
inline ::std::string *MessageThing::mutable_name() {
  set_has_name();
  // @@protoc_insertion_point(field_mutable:debug.MessageThing.name)
  return name_.MutableNoArena(
      &::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string *MessageThing::release_name() {
  // @@protoc_insertion_point(field_release:debug.MessageThing.name)
  if (!has_name()) {
    return NULL;
  }
  clear_has_name();
  return name_.ReleaseNonDefaultNoArena(
      &::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void MessageThing::set_allocated_name(::std::string *name) {
  if (name != NULL) {
    set_has_name();
  } else {
    clear_has_name();
  }
  name_.SetAllocatedNoArena(
      &::google::protobuf::internal::GetEmptyStringAlreadyInited(), name);
  // @@protoc_insertion_point(field_set_allocated:debug.MessageThing.name)
}

// required int32 id = 2;
inline bool MessageThing::has_id() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void MessageThing::set_has_id() { _has_bits_[0] |= 0x00000004u; }
inline void MessageThing::clear_has_id() { _has_bits_[0] &= ~0x00000004u; }
inline void MessageThing::clear_id() {
  id_ = 0;
  clear_has_id();
}
inline ::google::protobuf::int32 MessageThing::id() const {
  // @@protoc_insertion_point(field_get:debug.MessageThing.id)
  return id_;
}
inline void MessageThing::set_id(::google::protobuf::int32 value) {
  set_has_id();
  id_ = value;
  // @@protoc_insertion_point(field_set:debug.MessageThing.id)
}

// optional string extra = 3;
inline bool MessageThing::has_extra() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void MessageThing::set_has_extra() { _has_bits_[0] |= 0x00000002u; }
inline void MessageThing::clear_has_extra() { _has_bits_[0] &= ~0x00000002u; }
inline void MessageThing::clear_extra() {
  extra_.ClearToEmptyNoArena(
      &::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_extra();
}
inline const ::std::string &MessageThing::extra() const {
  // @@protoc_insertion_point(field_get:debug.MessageThing.extra)
  return extra_.GetNoArena();
}
inline void MessageThing::set_extra(const ::std::string &value) {
  set_has_extra();
  extra_.SetNoArena(
      &::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:debug.MessageThing.extra)
}
#if LANG_CXX11
inline void MessageThing::set_extra(::std::string &&value) {
  set_has_extra();
  extra_.SetNoArena(
      &::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:debug.MessageThing.extra)
}
#endif
inline void MessageThing::set_extra(const char *value) {
  GOOGLE_DCHECK(value != NULL);
  set_has_extra();
  extra_.SetNoArena(
      &::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(value));
  // @@protoc_insertion_point(field_set_char:debug.MessageThing.extra)
}
inline void MessageThing::set_extra(const char *value, size_t size) {
  set_has_extra();
  extra_.SetNoArena(
      &::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char *>(value), size));
  // @@protoc_insertion_point(field_set_pointer:debug.MessageThing.extra)
}
inline ::std::string *MessageThing::mutable_extra() {
  set_has_extra();
  // @@protoc_insertion_point(field_mutable:debug.MessageThing.extra)
  return extra_.MutableNoArena(
      &::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string *MessageThing::release_extra() {
  // @@protoc_insertion_point(field_release:debug.MessageThing.extra)
  if (!has_extra()) {
    return NULL;
  }
  clear_has_extra();
  return extra_.ReleaseNonDefaultNoArena(
      &::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void MessageThing::set_allocated_extra(::std::string *extra) {
  if (extra != NULL) {
    set_has_extra();
  } else {
    clear_has_extra();
  }
  extra_.SetAllocatedNoArena(
      &::google::protobuf::internal::GetEmptyStringAlreadyInited(), extra);
  // @@protoc_insertion_point(field_set_allocated:debug.MessageThing.extra)
}

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif // __GNUC__

// @@protoc_insertion_point(namespace_scope)

} // namespace debug

// @@protoc_insertion_point(global_scope)

#endif // PROTOBUF_INCLUDED_proto_2fdebug_2fother_2eproto
