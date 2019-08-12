// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: proto/debug/other.proto

#include "proto/debug/other.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// This is a temporary google only hack
#ifdef GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
#include "third_party/protobuf/version.h"
#endif
// @@protoc_insertion_point(includes)

namespace debug {
class MessageThingDefaultTypeInternal {
public:
  ::google::protobuf::internal::ExplicitlyConstructed<MessageThing> _instance;
} _MessageThing_default_instance_;
} // namespace debug
namespace protobuf_proto_2fdebug_2fother_2eproto {
static void InitDefaultsMessageThing() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void *ptr = &::debug::_MessageThing_default_instance_;
    new (ptr)::debug::MessageThing();
    ::google::protobuf::internal::OnShutdownDestroyMessage(ptr);
  }
  ::debug::MessageThing::InitAsDefaultInstance();
}

::google::protobuf::internal::SCCInfo<0> scc_info_MessageThing = {
    {ATOMIC_VAR_INIT(::google::protobuf::internal::SCCInfoBase::kUninitialized),
     0, InitDefaultsMessageThing},
    {}};

void InitDefaults() {
  ::google::protobuf::internal::InitSCC(&scc_info_MessageThing.base);
}

::google::protobuf::Metadata file_level_metadata[1];

const ::google::protobuf::uint32
    TableStruct::offsets[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(
        protodesc_cold) = {
        GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::debug::MessageThing,
                                                       _has_bits_),
        GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::debug::MessageThing,
                                                       _internal_metadata_),
        ~0u, // no _extensions_
        ~0u, // no _oneof_case_
        ~0u, // no _weak_field_map_
        GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::debug::MessageThing,
                                                       name_),
        GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::debug::MessageThing,
                                                       id_),
        GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::debug::MessageThing,
                                                       extra_),
        0,
        2,
        1,
};
static const ::google::protobuf::internal::MigrationSchema
    schemas[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
        {0, 8, sizeof(::debug::MessageThing)},
};

static ::google::protobuf::Message const *const file_default_instances[] = {
    reinterpret_cast<const ::google::protobuf::Message *>(
        &::debug::_MessageThing_default_instance_),
};

void protobuf_AssignDescriptors() {
  AddDescriptors();
  AssignDescriptors("proto/debug/other.proto", schemas, file_default_instances,
                    TableStruct::offsets, file_level_metadata, NULL, NULL);
}

void protobuf_AssignDescriptorsOnce() {
  static ::google::protobuf::internal::once_flag once;
  ::google::protobuf::internal::call_once(once, protobuf_AssignDescriptors);
}

void protobuf_RegisterTypes(const ::std::string &)
    GOOGLE_PROTOBUF_ATTRIBUTE_COLD;
void protobuf_RegisterTypes(const ::std::string &) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::internal::RegisterAllTypes(file_level_metadata, 1);
}

void AddDescriptorsImpl() {
  InitDefaults();
  static const char descriptor[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(
      protodesc_cold) = {
      "\n\027proto/debug/other.proto\022\005debug\"7\n\014Mess"
      "ageThing\022\014\n\004name\030\001 \002(\t\022\n\n\002id\030\002 "
      "\002(\005\022\r\n\005ex"
      "tra\030\003 \001(\t"};
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(descriptor, 89);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
      "proto/debug/other.proto", &protobuf_RegisterTypes);
}

void AddDescriptors() {
  static ::google::protobuf::internal::once_flag once;
  ::google::protobuf::internal::call_once(once, AddDescriptorsImpl);
}
// Force AddDescriptors() to be called at dynamic initialization time.
struct StaticDescriptorInitializer {
  StaticDescriptorInitializer() { AddDescriptors(); }
} static_descriptor_initializer;
} // namespace protobuf_proto_2fdebug_2fother_2eproto
namespace debug {

// ===================================================================

void MessageThing::InitAsDefaultInstance() {}
#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int MessageThing::kNameFieldNumber;
const int MessageThing::kIdFieldNumber;
const int MessageThing::kExtraFieldNumber;
#endif // !defined(_MSC_VER) || _MSC_VER >= 1900

MessageThing::MessageThing()
    : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  ::google::protobuf::internal::InitSCC(
      &protobuf_proto_2fdebug_2fother_2eproto::scc_info_MessageThing.base);
  SharedCtor();
  // @@protoc_insertion_point(constructor:debug.MessageThing)
}
MessageThing::MessageThing(const MessageThing &from)
    : ::google::protobuf::Message(), _internal_metadata_(NULL),
      _has_bits_(from._has_bits_) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  name_.UnsafeSetDefault(
      &::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (from.has_name()) {
    name_.AssignWithDefault(
        &::google::protobuf::internal::GetEmptyStringAlreadyInited(),
        from.name_);
  }
  extra_.UnsafeSetDefault(
      &::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (from.has_extra()) {
    extra_.AssignWithDefault(
        &::google::protobuf::internal::GetEmptyStringAlreadyInited(),
        from.extra_);
  }
  id_ = from.id_;
  // @@protoc_insertion_point(copy_constructor:debug.MessageThing)
}

void MessageThing::SharedCtor() {
  name_.UnsafeSetDefault(
      &::google::protobuf::internal::GetEmptyStringAlreadyInited());
  extra_.UnsafeSetDefault(
      &::google::protobuf::internal::GetEmptyStringAlreadyInited());
  id_ = 0;
}

MessageThing::~MessageThing() {
  // @@protoc_insertion_point(destructor:debug.MessageThing)
  SharedDtor();
}

void MessageThing::SharedDtor() {
  name_.DestroyNoArena(
      &::google::protobuf::internal::GetEmptyStringAlreadyInited());
  extra_.DestroyNoArena(
      &::google::protobuf::internal::GetEmptyStringAlreadyInited());
}

void MessageThing::SetCachedSize(int size) const { _cached_size_.Set(size); }
const ::google::protobuf::Descriptor *MessageThing::descriptor() {
  ::protobuf_proto_2fdebug_2fother_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_proto_2fdebug_2fother_2eproto::file_level_metadata
      [kIndexInFileMessages]
          .descriptor;
}

const MessageThing &MessageThing::default_instance() {
  ::google::protobuf::internal::InitSCC(
      &protobuf_proto_2fdebug_2fother_2eproto::scc_info_MessageThing.base);
  return *internal_default_instance();
}

void MessageThing::Clear() {
  // @@protoc_insertion_point(message_clear_start:debug.MessageThing)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void)cached_has_bits;

  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 3u) {
    if (cached_has_bits & 0x00000001u) {
      name_.ClearNonDefaultToEmptyNoArena();
    }
    if (cached_has_bits & 0x00000002u) {
      extra_.ClearNonDefaultToEmptyNoArena();
    }
  }
  id_ = 0;
  _has_bits_.Clear();
  _internal_metadata_.Clear();
}

bool MessageThing::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream *input) {
#define DO_(EXPRESSION)                                                        \
  if (!GOOGLE_PREDICT_TRUE(EXPRESSION))                                        \
  goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:debug.MessageThing)
  for (;;) {
    ::std::pair<::google::protobuf::uint32, bool> p =
        input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second)
      goto handle_unusual;
    switch (
        ::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
    // required string name = 1;
    case 1: {
      if (static_cast<::google::protobuf::uint8>(tag) ==
          static_cast<::google::protobuf::uint8>(10u /* 10 & 0xFF */)) {
        DO_(::google::protobuf::internal::WireFormatLite::ReadString(
            input, this->mutable_name()));
        ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
            this->name().data(), static_cast<int>(this->name().length()),
            ::google::protobuf::internal::WireFormat::PARSE,
            "debug.MessageThing.name");
      } else {
        goto handle_unusual;
      }
      break;
    }

    // required int32 id = 2;
    case 2: {
      if (static_cast<::google::protobuf::uint8>(tag) ==
          static_cast<::google::protobuf::uint8>(16u /* 16 & 0xFF */)) {
        set_has_id();
        DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
             ::google::protobuf::int32,
             ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(input,
                                                                       &id_)));
      } else {
        goto handle_unusual;
      }
      break;
    }

    // optional string extra = 3;
    case 3: {
      if (static_cast<::google::protobuf::uint8>(tag) ==
          static_cast<::google::protobuf::uint8>(26u /* 26 & 0xFF */)) {
        DO_(::google::protobuf::internal::WireFormatLite::ReadString(
            input, this->mutable_extra()));
        ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
            this->extra().data(), static_cast<int>(this->extra().length()),
            ::google::protobuf::internal::WireFormat::PARSE,
            "debug.MessageThing.extra");
      } else {
        goto handle_unusual;
      }
      break;
    }

    default: {
    handle_unusual:
      if (tag == 0) {
        goto success;
      }
      DO_(::google::protobuf::internal::WireFormat::SkipField(
          input, tag, _internal_metadata_.mutable_unknown_fields()));
      break;
    }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:debug.MessageThing)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:debug.MessageThing)
  return false;
#undef DO_
}

void MessageThing::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream *output) const {
  // @@protoc_insertion_point(serialize_start:debug.MessageThing)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void)cached_has_bits;

  cached_has_bits = _has_bits_[0];
  // required string name = 1;
  if (cached_has_bits & 0x00000001u) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
        this->name().data(), static_cast<int>(this->name().length()),
        ::google::protobuf::internal::WireFormat::SERIALIZE,
        "debug.MessageThing.name");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
        1, this->name(), output);
  }

  // required int32 id = 2;
  if (cached_has_bits & 0x00000004u) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(2, this->id(),
                                                             output);
  }

  // optional string extra = 3;
  if (cached_has_bits & 0x00000002u) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
        this->extra().data(), static_cast<int>(this->extra().length()),
        ::google::protobuf::internal::WireFormat::SERIALIZE,
        "debug.MessageThing.extra");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
        3, this->extra(), output);
  }

  if (_internal_metadata_.have_unknown_fields()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        _internal_metadata_.unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:debug.MessageThing)
}

::google::protobuf::uint8 *
MessageThing::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8 *target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:debug.MessageThing)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void)cached_has_bits;

  cached_has_bits = _has_bits_[0];
  // required string name = 1;
  if (cached_has_bits & 0x00000001u) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
        this->name().data(), static_cast<int>(this->name().length()),
        ::google::protobuf::internal::WireFormat::SERIALIZE,
        "debug.MessageThing.name");
    target = ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->name(), target);
  }

  // required int32 id = 2;
  if (cached_has_bits & 0x00000004u) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(
        2, this->id(), target);
  }

  // optional string extra = 3;
  if (cached_has_bits & 0x00000002u) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
        this->extra().data(), static_cast<int>(this->extra().length()),
        ::google::protobuf::internal::WireFormat::SERIALIZE,
        "debug.MessageThing.extra");
    target = ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        3, this->extra(), target);
  }

  if (_internal_metadata_.have_unknown_fields()) {
    target =
        ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
            _internal_metadata_.unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:debug.MessageThing)
  return target;
}

size_t MessageThing::RequiredFieldsByteSizeFallback() const {
  // @@protoc_insertion_point(required_fields_byte_size_fallback_start:debug.MessageThing)
  size_t total_size = 0;

  if (has_name()) {
    // required string name = 1;
    total_size += 1 + ::google::protobuf::internal::WireFormatLite::StringSize(
                          this->name());
  }

  if (has_id()) {
    // required int32 id = 2;
    total_size +=
        1 + ::google::protobuf::internal::WireFormatLite::Int32Size(this->id());
  }

  return total_size;
}
size_t MessageThing::ByteSizeLong() const {
  // @@protoc_insertion_point(message_byte_size_start:debug.MessageThing)
  size_t total_size = 0;

  if (_internal_metadata_.have_unknown_fields()) {
    total_size +=
        ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
            _internal_metadata_.unknown_fields());
  }
  if (((_has_bits_[0] & 0x00000005) ^ 0x00000005) ==
      0) { // All required fields are present.
    // required string name = 1;
    total_size += 1 + ::google::protobuf::internal::WireFormatLite::StringSize(
                          this->name());

    // required int32 id = 2;
    total_size +=
        1 + ::google::protobuf::internal::WireFormatLite::Int32Size(this->id());

  } else {
    total_size += RequiredFieldsByteSizeFallback();
  }
  // optional string extra = 3;
  if (has_extra()) {
    total_size += 1 + ::google::protobuf::internal::WireFormatLite::StringSize(
                          this->extra());
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void MessageThing::MergeFrom(const ::google::protobuf::Message &from) {
  // @@protoc_insertion_point(generalized_merge_from_start:debug.MessageThing)
  GOOGLE_DCHECK_NE(&from, this);
  const MessageThing *source =
      ::google::protobuf::internal::DynamicCastToGenerated<const MessageThing>(
          &from);
  if (source == NULL) {
    // @@protoc_insertion_point(generalized_merge_from_cast_fail:debug.MessageThing)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    // @@protoc_insertion_point(generalized_merge_from_cast_success:debug.MessageThing)
    MergeFrom(*source);
  }
}

void MessageThing::MergeFrom(const MessageThing &from) {
  // @@protoc_insertion_point(class_specific_merge_from_start:debug.MessageThing)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void)cached_has_bits;

  cached_has_bits = from._has_bits_[0];
  if (cached_has_bits & 7u) {
    if (cached_has_bits & 0x00000001u) {
      set_has_name();
      name_.AssignWithDefault(
          &::google::protobuf::internal::GetEmptyStringAlreadyInited(),
          from.name_);
    }
    if (cached_has_bits & 0x00000002u) {
      set_has_extra();
      extra_.AssignWithDefault(
          &::google::protobuf::internal::GetEmptyStringAlreadyInited(),
          from.extra_);
    }
    if (cached_has_bits & 0x00000004u) {
      id_ = from.id_;
    }
    _has_bits_[0] |= cached_has_bits;
  }
}

void MessageThing::CopyFrom(const ::google::protobuf::Message &from) {
  // @@protoc_insertion_point(generalized_copy_from_start:debug.MessageThing)
  if (&from == this)
    return;
  Clear();
  MergeFrom(from);
}

void MessageThing::CopyFrom(const MessageThing &from) {
  // @@protoc_insertion_point(class_specific_copy_from_start:debug.MessageThing)
  if (&from == this)
    return;
  Clear();
  MergeFrom(from);
}

bool MessageThing::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000005) != 0x00000005)
    return false;
  return true;
}

void MessageThing::Swap(MessageThing *other) {
  if (other == this)
    return;
  InternalSwap(other);
}
void MessageThing::InternalSwap(MessageThing *other) {
  using std::swap;
  name_.Swap(&other->name_,
             &::google::protobuf::internal::GetEmptyStringAlreadyInited(),
             GetArenaNoVirtual());
  extra_.Swap(&other->extra_,
              &::google::protobuf::internal::GetEmptyStringAlreadyInited(),
              GetArenaNoVirtual());
  swap(id_, other->id_);
  swap(_has_bits_[0], other->_has_bits_[0]);
  _internal_metadata_.Swap(&other->_internal_metadata_);
}

::google::protobuf::Metadata MessageThing::GetMetadata() const {
  protobuf_proto_2fdebug_2fother_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_proto_2fdebug_2fother_2eproto::file_level_metadata
      [kIndexInFileMessages];
}

// @@protoc_insertion_point(namespace_scope)
} // namespace debug
namespace google {
namespace protobuf {
template <>
GOOGLE_PROTOBUF_ATTRIBUTE_NOINLINE ::debug::MessageThing *
Arena::CreateMaybeMessage<::debug::MessageThing>(Arena *arena) {
  return Arena::CreateInternal<::debug::MessageThing>(arena);
}
} // namespace protobuf
} // namespace google

// @@protoc_insertion_point(global_scope)
