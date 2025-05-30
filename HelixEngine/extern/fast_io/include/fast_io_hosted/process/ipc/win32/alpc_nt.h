﻿#pragma once

namespace fast_io
{
namespace win32::nt::details
{
// https://hfiref0x.github.io/X86_64/NT6_syscalls.html
// nt6x alpc

using nt_alpc_char_type = char16_t;

using nt_alpc_internal_char_type = char16_t;

using nt_alpc_internal_str = ::fast_io::containers::basic_string<nt_alpc_internal_char_type, ::fast_io::native_global_allocator>;

using nt_alpc_internal_tlc_str = ::fast_io::containers::basic_string<nt_alpc_internal_char_type, ::fast_io::native_thread_local_allocator>;

using nt_alpc_internal_strvw = ::fast_io::containers::basic_string_view<nt_alpc_internal_char_type>;

template <::std::integral ch_type>
using nt_alpc_communication_tlc_strvw = ::fast_io::containers::basic_string_view<ch_type>;

template <typename... Args>
constexpr inline nt_alpc_internal_str concat_nt_alpc_internal_str(Args &&...args)
{
	constexpr bool type_error{::fast_io::operations::defines::print_freestanding_okay<::fast_io::details::dummy_buffer_output_stream<nt_alpc_internal_char_type>, Args...>};
	if constexpr (type_error)
	{
		return ::fast_io::basic_general_concat<false, nt_alpc_internal_char_type, nt_alpc_internal_str>(
			::fast_io::io_print_forward<nt_alpc_internal_char_type>(::fast_io::io_print_alias(args))...);
	}
	else
	{
		static_assert(type_error, "some types are not printable, so we cannot concat ::fast_io::win32::nt::details::nt_alpc_internal_str");
		return {};
	}
}

template <typename... Args>
constexpr inline nt_alpc_internal_tlc_str concat_nt_alpc_internal_tlc_str(Args &&...args)
{
	constexpr bool type_error{::fast_io::operations::defines::print_freestanding_okay<::fast_io::details::dummy_buffer_output_stream<nt_alpc_internal_char_type>, Args...>};
	if constexpr (type_error)
	{
		return ::fast_io::basic_general_concat<false, nt_alpc_internal_char_type, nt_alpc_internal_tlc_str>(
			::fast_io::io_print_forward<nt_alpc_internal_char_type>(::fast_io::io_print_alias(args))...);
	}
	else
	{
		static_assert(type_error, "some types are not printable, so we cannot concat ::fast_io::win32::nt::details::nt_alpc_internal_tlc_str");
		return {};
	}
}

template <nt_family family>
struct nt_alpc_handle FAST_IO_TRIVIALLY_RELOCATABLE_IF_ELIGIBLE
{
	using alpc_message_alloc = ::fast_io::native_thread_local_allocator;

	void *port_handle{};
	void *section_handle{};
	::fast_io::win32::nt::alpc_message_attributes *message_attribute{};
	// view section
	::std::byte *view_begin{};
	::std::byte *view_end{};
	// id
	void *pid{};
	void *tid{};

	nt_alpc_handle() noexcept = default;

	nt_alpc_handle(nt_alpc_handle const &) = delete;
	nt_alpc_handle &operator=(nt_alpc_handle const &) = delete;

	inline constexpr nt_alpc_handle(nt_alpc_handle &&other) noexcept
	{
		port_handle = other.port_handle;
		other.port_handle = nullptr;
		section_handle = other.section_handle;
		other.section_handle = nullptr;
		message_attribute = other.message_attribute;
		other.message_attribute = nullptr;
		view_begin = other.view_begin;
		other.view_begin = nullptr;
		view_end = other.view_end;
		other.view_end = nullptr;
		pid = other.pid;
		other.pid = nullptr;
		tid = other.tid;
		other.tid = nullptr;
	}

	inline constexpr nt_alpc_handle &operator=(nt_alpc_handle &&other) noexcept
	{
		constexpr bool zw{family == nt_family::zw};

		if (section_handle)
		{
			::fast_io::win32::nt::nt_alpc_disconnect_port<zw>(section_handle, 0);
		}
		section_handle = other.section_handle;
		other.section_handle = nullptr;

		if (port_handle)
		{
			::fast_io::win32::nt::nt_alpc_disconnect_port<zw>(port_handle, 0);
		}
		port_handle = other.port_handle;
		other.port_handle = nullptr;

		if (message_attribute)
		{
			alpc_message_alloc::deallocate(message_attribute);
		}
		message_attribute = other.message_attribute;
		other.message_attribute = nullptr;

		view_begin = other.view_begin;
		other.view_begin = nullptr;

		view_end = other.view_end;
		other.view_end = nullptr;

		pid = other.pid;
		other.pid = nullptr;

		tid = other.tid;
		other.tid = nullptr;
	}

	inline ~nt_alpc_handle()
	{
		close_noexcept();
	}

	inline void close_noexcept() noexcept
	{
		constexpr bool zw{family == nt_family::zw};

		if (section_handle)
		{
			::fast_io::win32::nt::nt_alpc_disconnect_port<zw>(section_handle, 0);
			section_handle = nullptr;
		}

		if (port_handle)
		{
			::fast_io::win32::nt::nt_alpc_disconnect_port<zw>(port_handle, 0);
			port_handle = nullptr;
		}

		if (message_attribute)
		{
			alpc_message_alloc::deallocate(message_attribute);
			message_attribute = nullptr;
		}

		view_begin = nullptr;

		view_end = nullptr;

		pid = nullptr;

		tid = nullptr;
	}

	inline void close()
	{
		constexpr bool zw{family == nt_family::zw};

		::std::uint_least32_t status; // No initialization required

		if (section_handle)
		{
			status = ::fast_io::win32::nt::nt_alpc_disconnect_port<zw>(section_handle, 0);
			if (status) [[unlikely]]
			{
				throw_nt_error(status);
			}
			section_handle = nullptr;
		}

		if (port_handle)
		{
			status = ::fast_io::win32::nt::nt_alpc_disconnect_port<zw>(port_handle, 0);
			if (status) [[unlikely]]
			{
				throw_nt_error(status);
			}
			port_handle = nullptr;
		}

		if (message_attribute)
		{
			alpc_message_alloc::deallocate(message_attribute);
			message_attribute = nullptr;
		}

		view_begin = nullptr;

		view_end = nullptr;

		pid = nullptr;

		tid = nullptr;
	}
};

struct nt_ipc_alpc_thread_local_heap_allocate_guard
{
	using alloc = ::fast_io::native_thread_local_allocator;

	void *ptr{};
	inline constexpr nt_ipc_alpc_thread_local_heap_allocate_guard() noexcept = default;
	inline constexpr nt_ipc_alpc_thread_local_heap_allocate_guard(void *o_ptr) noexcept
		: ptr{o_ptr} {};

	nt_ipc_alpc_thread_local_heap_allocate_guard(nt_ipc_alpc_thread_local_heap_allocate_guard const &) = delete;
	nt_ipc_alpc_thread_local_heap_allocate_guard &operator=(nt_ipc_alpc_thread_local_heap_allocate_guard const &) = delete;
	inline constexpr ~nt_ipc_alpc_thread_local_heap_allocate_guard()
	{
		clear();
	};
	inline constexpr void clear() noexcept
	{
		if (ptr) [[likely]]
		{
			alloc::deallocate(ptr);
			ptr = nullptr;
		}
	}
};

// SERVER
template <nt_family family>
inline void *nt_family_create_alpc_ipc_server_port_impl(nt_alpc_char_type const *server_name, ::std::size_t server_name_size, ::fast_io::ipc_mode mode)
{
	constexpr bool zw{family == nt_family::zw};

	if (::fast_io::details::is_invalid_dos_filename_with_size(server_name, server_name_size)) [[unlikely]]
	{
		throw_nt_error(3221225524);
	}

	auto temp_ipc_name_tlc_str{concat_nt_alpc_internal_tlc_str(u"\\RPC Control\\fast_io_ipc_", ::fast_io::mnp::os_c_str_with_known_size(server_name, server_name_size))};

	::fast_io::win32::nt::unicode_string us{};
	us.Buffer = const_cast<char16_t *>(temp_ipc_name_tlc_str.c_str());
	auto const temp_ipc_name_tlc_str_size_bytes{temp_ipc_name_tlc_str.size_bytes()};
	us.Length = static_cast<::std::uint_least16_t>(temp_ipc_name_tlc_str_size_bytes);
	us.MaximumLength = static_cast<::std::uint_least16_t>(temp_ipc_name_tlc_str_size_bytes + sizeof(char16_t));

	::fast_io::win32::nt::object_attributes oa{};
	oa.Length = sizeof(::fast_io::win32::nt::object_attributes);
	oa.ObjectName = __builtin_addressof(us);

	::fast_io::win32::nt::alpc_port_attributes apa{};
	apa.Flags = 0x80000 /*ALPC_PORTFLG_ALLOW_DUP_OBJECT*/;
	apa.MaxMessageLength = ::fast_io::win32::nt::AlpcMaxAllowedMessageLength();

	void *server_port_handle;

	check_nt_status(::fast_io::win32::nt::nt_alpc_create_port<zw>(
		__builtin_addressof(server_port_handle),
		__builtin_addressof(oa),
		__builtin_addressof(apa)));

	return server_port_handle;
}

template <nt_family family>
struct nt_family_create_alpc_ipc_server_paramenter
{
	using family_char_type = char16_t;
	ipc_mode im{};
	inline void *operator()(family_char_type const *filename, ::std::size_t filename_size)
	{
		return nt_family_create_alpc_ipc_server_port_impl<family>(filename, filename_size, im);
	}
};

template <nt_family family, typename T>
	requires(::fast_io::constructible_to_os_c_str<T>)
inline void *nt_create_alpc_ipc_server_impl(T const &t, ipc_mode im)
{
	return ::fast_io::nt_api_common(t, nt_family_create_alpc_ipc_server_paramenter<family>{im});
}

template <nt_family family>
inline ::fast_io::win32::nt::alpc_message_attributes *nt_family_create_alpc_ipc_server_message_attribute_view_impl(void *server_port)
{
	constexpr bool zw{family == nt_family::zw};

	constexpr ::std::uint_least32_t message_attribute{
		0x80000000 /*ALPC_MESSAGE_SECURITY_ATTRIBUTE*/ |
		0x40000000 /*ALPC_MESSAGE_VIEW_ATTRIBUTE*/};

	auto const header_size{::fast_io::win32::nt::AlpcGetHeaderSize(message_attribute)};

	if (header_size == 0) [[unlikely]]
	{
		throw_nt_error(3221225485);
	}

	auto message_stroge{nt_alpc_handle<family>::alpc_message_alloc::allocate(header_size)}; // This function does not recycle

	using alpc_message_attributes_may_alias_ptr
#if __has_cpp_attribute(__gnu__::__may_alias__)
		[[__gnu__::__may_alias__]]
#endif
		= ::fast_io::win32::nt::alpc_message_attributes *;

	auto p_msg_attr_aend{reinterpret_cast<alpc_message_attributes_may_alias_ptr>(message_stroge)};

	::std::size_t real_buf_size;

	check_nt_status(::fast_io::win32::nt::AlpcInitializeMessageAttribute(
		message_attribute,                 // the MessageAttribute
		p_msg_attr_aend,                   // pointer to allocated buffer that is used to holf attributes structures
		header_size,                       // buffer that has been allocated
		__builtin_addressof(real_buf_size) // the size that would be needed (in case of the buffer allocated was too small)
		));

	::std::size_t iNextMsgAttrBufferOffset{sizeof(::fast_io::win32::nt::alpc_message_attributes)}; // 4 bytes allocated attributes + 4 bytes valid attributes

	// ALPC_MESSAGE_SECURITY_ATTRIBUTE
	::fast_io::win32::nt::security_quality_of_service SecurityQos{};
	::fast_io::win32::nt::alpc_security_attr securityAttr{};

	SecurityQos.ImpersonationLevel = ::fast_io::win32::nt::security_impersonation_level::SecurityAnonymous; // SecurityIdentification;
	SecurityQos.ContextTrackingMode = 0 /*SECURITY_STATIC_TRACKING*/;
	SecurityQos.EffectiveOnly = 0;
	SecurityQos.Length = sizeof(SecurityQos);
	securityAttr.pQOS = __builtin_addressof(SecurityQos);
	securityAttr.Flags = 0; // 0x10000;
	check_nt_status(::fast_io::win32::nt::nt_alpc_create_security_context<zw>(server_port, 0, __builtin_addressof(securityAttr)));
	::fast_io::freestanding::my_memmove(reinterpret_cast<::std::byte *>(p_msg_attr_aend) + iNextMsgAttrBufferOffset, __builtin_addressof(securityAttr), sizeof(securityAttr));
	iNextMsgAttrBufferOffset += sizeof(securityAttr);

	// ALPC_MESSAGE_VIEW_ATTRIBUTE
	::fast_io::win32::nt::alpc_data_view_attr viewAttr{};
	viewAttr.Flags = 0;         // unknown
	viewAttr.SectionHandle = 0; // Future allocation
	viewAttr.ViewBase = 0;      // Automatic assign
	viewAttr.ViewSize = 0;      // Future allocation

	// place ALPC_MESSAGE_VIEW_ATTRIBUTE structure
	::fast_io::freestanding::my_memmove(reinterpret_cast<::std::byte *>(p_msg_attr_aend) + iNextMsgAttrBufferOffset, __builtin_addressof(viewAttr), sizeof(viewAttr));
	iNextMsgAttrBufferOffset += sizeof(viewAttr);

	return p_msg_attr_aend;
}

template <nt_family family>
inline void *nt_family_add_section_view_to_alpc_ipc_server_message_attribute_view_impl(
	void *__restrict server_port_handle, ::fast_io::win32::nt::alpc_message_attributes *ama, ::std::size_t view_size)
{
	constexpr bool zw{family == nt_family::zw};

	void *section_handle;
	::std::size_t server_section_size;

	check_nt_status(::fast_io::win32::nt::nt_alpc_create_port_section<zw>(
		server_port_handle,                      //_In_ HANDLE PortHandle,
		0,                                       //_In_ ULONG Flags, unknown, 0x40000 found in rpcrt4.dll
		nullptr,                                 //_In_opt_ HANDLE SectionHandle,
		0x1000,                                  // _In_ SIZE_T SectionSize,
		__builtin_addressof(section_handle),     //_Out_ HANDLE AlpcSectionHandle,
		__builtin_addressof(server_section_size) //_Out_ PSIZE_T ActualSectionSize
		));

	::fast_io::win32::nt::alpc_data_view_attr viewAttr{};
	viewAttr.Flags = 0; // unknown
	viewAttr.SectionHandle = section_handle;
	viewAttr.ViewBase = 0; // Automatic assign
	viewAttr.ViewSize = view_size;
	check_nt_status(::fast_io::win32::nt::nt_alpc_create_section_view<zw>(
		server_port_handle,           //_In_ HANDLE PortHandle,
		0,                            // _Reserved_ ULONG Flags, unknown
		__builtin_addressof(viewAttr) //_Inout_ PALPC_DATA_VIEW_ATTR ViewAttributes
		));

	::std::size_t iNextMsgAttrBufferOffset{sizeof(::fast_io::win32::nt::alpc_message_attributes)};
	iNextMsgAttrBufferOffset += sizeof(::fast_io::win32::nt::security_quality_of_service);
	::fast_io::freestanding::my_memmove(reinterpret_cast<::std::byte *>(ama) + iNextMsgAttrBufferOffset, __builtin_addressof(viewAttr), sizeof(viewAttr));

	return section_handle;
}

template <nt_family family>
inline ::std::byte *nt_family_alpc_ipc_server_wait_for_connect_impl(
	void *server_pipe_handle, ::fast_io::win32::nt::alpc_message_attributes *ama,
	::std::byte *handshake_msg_beg, ::std::byte *handshake_msg_end)
{
	constexpr bool zw{family == nt_family::zw};

	::std::size_t receive_size{sizeof(::fast_io::win32::nt::port_message) + static_cast<::std::size_t>(handshake_msg_end - handshake_msg_beg)};

	auto tmp{static_cast<::fast_io::win32::nt::alpc_message *>(nt_ipc_alpc_thread_local_heap_allocate_guard::alloc::allocate(receive_size))};
	nt_ipc_alpc_thread_local_heap_allocate_guard tmp_guard{tmp};

	using port_message_may_alias_ptr
#if __has_cpp_attribute(__gnu__::__may_alias__)
		[[__gnu__::__may_alias__]]
#endif
		= ::fast_io::win32::nt::port_message *;

	auto port_message_p{reinterpret_cast<port_message_may_alias_ptr>(tmp)};

	check_nt_status(::fast_io::win32::nt::nt_alpc_send_wait_receive_port<zw>(
		server_pipe_handle,
		0,                                 // no flags
		nullptr,                           // SendMessage
		nullptr,                           // SendMessageAttributes
		port_message_p,                    // ReceiveBuffer
		__builtin_addressof(receive_size), // BufferLength
		ama,                               // ReceiveMessageAttributes
		nullptr                            // no timeout
		));

	auto const actual_receive_size{static_cast<::std::size_t>(port_message_p->u1.s1.DataLength)};

	return static_cast<::std::byte *>(::fast_io::freestanding::my_memcpy(handshake_msg_beg, tmp->PortMessage, actual_receive_size));
}

template <nt_family family>
inline void nt_family_alpc_ipc_server_disconnect_impl(void *client_pipe_handle)
{
	constexpr bool zw{family == nt_family::zw};

	check_nt_status(::fast_io::win32::nt::nt_alpc_disconnect_port<zw>(client_pipe_handle, 0));
}

// CLIENT
template <nt_family family>
inline ::fast_io::win32::nt::alpc_message_attributes *nt_family_create_alpc_ipc_client_message_attribute_view_impl()
{
	constexpr bool zw{family == nt_family::zw};

	constexpr ::std::uint_least32_t message_attribute{
		0x80000000 /*ALPC_MESSAGE_SECURITY_ATTRIBUTE*/ |
		0x40000000 /*ALPC_MESSAGE_VIEW_ATTRIBUTE*/
	};

	auto const header_size{::fast_io::win32::nt::AlpcGetHeaderSize(message_attribute)};

	if (header_size == 0) [[unlikely]]
	{
		throw_nt_error(3221225485);
	}

	auto message_stroge{nt_alpc_handle<family>::alpc_message_alloc::allocate(header_size)}; // This function does not recycle

	using alpc_message_attributes_may_alias_ptr
#if __has_cpp_attribute(__gnu__::__may_alias__)
		[[__gnu__::__may_alias__]]
#endif
		= ::fast_io::win32::nt::alpc_message_attributes *;

	auto p_msg_attr_aend{reinterpret_cast<alpc_message_attributes_may_alias_ptr>(message_stroge)};

	::std::size_t real_buf_size;

	check_nt_status(::fast_io::win32::nt::AlpcInitializeMessageAttribute(
		message_attribute,                 // the MessageAttribute
		p_msg_attr_aend,                   // pointer to allocated buffer that is used to holf attributes structures
		header_size,                       // buffer that has been allocated
		__builtin_addressof(real_buf_size) // the size that would be needed (in case of the buffer allocated was too small)
		));

	return p_msg_attr_aend;
}

template <nt_family family>
inline void *nt_family_ipc_alpc_client_connect_impl(nt_alpc_char_type const *server_name, ::std::size_t server_name_size, ::fast_io::ipc_mode mode, ::std::byte const *message_begin, ::std::byte const *message_end, ::fast_io::win32::nt::alpc_message_attributes *message_attribute)
{
	constexpr bool zw{family == nt_family::zw};

	if (::fast_io::details::is_invalid_dos_filename_with_size(server_name, server_name_size)) [[unlikely]]
	{
		throw_nt_error(3221225524);
	}

	auto temp_ipc_name_tlc_str{concat_nt_alpc_internal_tlc_str(u"\\RPC Control\\fast_io_ipc_", ::fast_io::mnp::os_c_str_with_known_size(server_name, server_name_size))};

	::fast_io::win32::nt::unicode_string us{};
	us.Buffer = const_cast<char16_t *>(temp_ipc_name_tlc_str.c_str());
	auto const temp_ipc_name_tlc_str_size_bytes{temp_ipc_name_tlc_str.size_bytes()};
	us.Length = static_cast<::std::uint_least16_t>(temp_ipc_name_tlc_str_size_bytes);
	us.MaximumLength = static_cast<::std::uint_least16_t>(temp_ipc_name_tlc_str_size_bytes + sizeof(char16_t));

	::fast_io::win32::nt::security_quality_of_service SecurityQos{};
	SecurityQos.ImpersonationLevel = ::fast_io::win32::nt::security_impersonation_level::SecurityImpersonation;
	SecurityQos.ContextTrackingMode = 0 /*SECURITY_STATIC_TRACKING*/;
	SecurityQos.EffectiveOnly = 0;
	SecurityQos.Length = sizeof(SecurityQos);

	::fast_io::win32::nt::alpc_port_attributes apa{};
	apa.Flags = 0x80000 /*ALPC_PORTFLG_ALLOW_DUP_OBJECT*/;
	apa.MaxMessageLength = ::fast_io::win32::nt::AlpcMaxAllowedMessageLength();
	apa.SecurityQos = SecurityQos;

	::std::size_t const message_size{static_cast<::std::size_t>(message_end - message_begin)};
	::std::size_t receive_size{sizeof(::fast_io::win32::nt::port_message) + message_size};

	auto tmp{static_cast<::fast_io::win32::nt::alpc_message *>(nt_ipc_alpc_thread_local_heap_allocate_guard::alloc::allocate(receive_size))};
	nt_ipc_alpc_thread_local_heap_allocate_guard tmp_guard{tmp};

	::fast_io::freestanding::my_memset(tmp, 0, sizeof(::fast_io::win32::nt::port_message));

	tmp->PortHeader.u1.s1.DataLength = static_cast<::std::uint_least16_t>(message_size);
	tmp->PortHeader.u1.s1.TotalLength = static_cast<::std::uint_least16_t>(receive_size);

	if (message_begin)
	{
		::fast_io::freestanding::my_memcpy(tmp->PortMessage, message_begin, message_size);
	}

	using port_message_may_alias_ptr
#if __has_cpp_attribute(__gnu__::__may_alias__)
		[[__gnu__::__may_alias__]]
#endif
		= ::fast_io::win32::nt::port_message *;

	auto port_message_p{reinterpret_cast<port_message_may_alias_ptr>(tmp)};

	void *srv_common_port;

	check_nt_status(::fast_io::win32::nt::nt_alpc_connect_port<zw>(
		__builtin_addressof(srv_common_port), // REQUIRED: empty Communication port handle, fill be set by kernel
		__builtin_addressof(us),              // REQUIRED: Server Connect port name to connect to
		nullptr,                              // OPTIONAL: Object Attributes, none in this case
		__builtin_addressof(apa),             // OPTIONAL: PortAtrributes, used to set various port connection attributes, most imporatnly port flags
		0x20000 /*ALPC_SYNC_CONNECTION*/,     // OPTOONAL: Message Flags, no Flags
		nullptr,                              // OPTIONAL: Server SID
		port_message_p,                       // connection message
		__builtin_addressof(receive_size),    // connection message size
		nullptr,                              // pMsgAttrSend,		// out messages attribtus
		message_attribute,                    // in message attributes
		nullptr                               //&timeout				// OPTIONAL: Timeout, none in this case
		));

	return srv_common_port;
}

template <nt_family family>
struct nt_family_connect_alpc_ipc_server_paramenter
{
	using family_char_type = char16_t;
	ipc_mode im{};
	::std::byte const *mb{};
	::std::byte const *me{};
	::fast_io::win32::nt::alpc_message_attributes *ma{};

	inline void *operator()(family_char_type const *filename, ::std::size_t filename_size)
	{
		return nt_family_ipc_alpc_client_connect_impl<family>(filename, filename_size, im, mb, me, ma);
	}
};

template <nt_family family, typename T>
	requires(::fast_io::constructible_to_os_c_str<T>)
inline void *nt_connect_alpc_ipc_server_impl(T const &t, ipc_mode im, ::std::byte const *mb, ::std::byte const *me, ::fast_io::win32::nt::alpc_message_attributes *ma)
{
	return ::fast_io::nt_api_common(t, nt_family_connect_alpc_ipc_server_paramenter<family>{im, mb, me, ma});
}

} // namespace win32::nt::details

template <nt_family family, ::std::integral ch_type>
class basic_nt_family_alpc_ipc_universal_observer
{
public:
	using native_handle_type = ::fast_io::win32::nt::details::nt_alpc_handle<family> *;
	using char_type = ch_type;
	using input_char_type = char_type;
	using output_char_type = char_type;
	native_handle_type handle{};
	inline constexpr native_handle_type native_handle() const noexcept
	{
		return handle;
	}
	inline explicit operator bool() const noexcept
	{
		return handle != nullptr;
	}
	inline constexpr native_handle_type release() noexcept
	{
		auto temp{handle};
		handle = nullptr;
		return temp;
	}
};

template <nt_family family, ::std::integral ch_type>
using basic_nt_family_alpc_ipc_server_observer = basic_nt_family_alpc_ipc_universal_observer<family, ch_type>;

template <nt_family family, ::std::integral ch_type>
class basic_nt_family_alpc_ipc_server FAST_IO_TRIVIALLY_RELOCATABLE_IF_ELIGIBLE : public basic_nt_family_alpc_ipc_server_observer<family, ch_type>
{
public:
	using typename basic_nt_family_alpc_ipc_server_observer<family, ch_type>::char_type;
	using typename basic_nt_family_alpc_ipc_server_observer<family, ch_type>::input_char_type;
	using typename basic_nt_family_alpc_ipc_server_observer<family, ch_type>::output_char_type;
	using typename basic_nt_family_alpc_ipc_server_observer<family, ch_type>::native_handle_type;
	using basic_nt_family_alpc_ipc_server_observer<family, ch_type>::native_handle;

	using native_handle_rmptr_type = ::std::remove_pointer_t<native_handle_type>;
	using tls_native_handle_rmptr_type_alloc = ::fast_io::native_typed_thread_local_allocator<native_handle_rmptr_type>;

	inline explicit constexpr basic_nt_family_alpc_ipc_server() noexcept = default;

	inline constexpr basic_nt_family_alpc_ipc_server(basic_nt_family_alpc_ipc_server_observer<family, ch_type>) noexcept = delete;
	inline constexpr basic_nt_family_alpc_ipc_server &operator=(basic_nt_family_alpc_ipc_server_observer<family, ch_type>) noexcept = delete;

	inline basic_nt_family_alpc_ipc_server(basic_nt_family_alpc_ipc_server const &) = delete;
	inline basic_nt_family_alpc_ipc_server &operator=(basic_nt_family_alpc_ipc_server const &) = delete;

	inline basic_nt_family_alpc_ipc_server(basic_nt_family_alpc_ipc_server &&__restrict b) noexcept
		: basic_nt_family_alpc_ipc_server_observer<family, ch_type>{b.release()}
	{
	}
	inline basic_nt_family_alpc_ipc_server &operator=(basic_nt_family_alpc_ipc_server &&__restrict b) noexcept
	{
		if (__builtin_addressof(b) == this) [[unlikely]]
		{
			return *this;
		}
		if (*this) [[likely]]
		{
			this->handle->close_noexcept();
		}
		this->handle = b.handle;
		b.handle = nullptr;
		return *this;
	}
	inline void reset(native_handle_type newhandle = {}) noexcept
	{
		if (*this) [[likely]]
		{
			this->handle->close_noexcept();
		}
		this->handle = newhandle;
	}
	inline void close()
	{
		if (*this) [[likely]]
		{
			this->handle->close();
			tls_native_handle_rmptr_type_alloc::deallocate_n(this->handle, 1);
			this->handle = nullptr; // POSIX standard says we should never call close(2) again even close syscall fails
		}
	}

	template <typename native_hd>
		requires ::std::same_as<native_handle_type, ::std::remove_cvref_t<native_hd>>
	inline explicit constexpr basic_nt_family_alpc_ipc_server(native_hd handle1) noexcept
		: basic_nt_family_alpc_ipc_server_observer<family, ch_type>{handle1}
	{}

	template <::fast_io::constructible_to_os_c_str T>
	inline explicit basic_nt_family_alpc_ipc_server(T const &server_name, ipc_mode im)
	{
		this->handle = tls_native_handle_rmptr_type_alloc::allocate_zero(1);
		this->handle->port_handle = ::fast_io::win32::nt::details::nt_create_alpc_ipc_server_impl<family>(server_name, im);
		this->handle->message_attribute = ::fast_io::win32::nt::details::nt_family_create_alpc_ipc_server_message_attribute_view_impl<family>(this->handle->port_handle);
	}

	inline ~basic_nt_family_alpc_ipc_server()
	{
		if (*this) [[likely]]
		{
			this->handle->close_noexcept();
			tls_native_handle_rmptr_type_alloc::deallocate_n(this->handle, 1);
			this->handle = nullptr; // POSIX standard says we should never call close(2) again even close syscall fails
		}
	}
};

template <nt_family family, ::std::integral ch_type>
using basic_nt_family_alpc_ipc_client_observer = basic_nt_family_alpc_ipc_universal_observer<family, ch_type>;

template <nt_family family, ::std::integral ch_type>
class basic_nt_family_alpc_ipc_client FAST_IO_TRIVIALLY_RELOCATABLE_IF_ELIGIBLE : public basic_nt_family_alpc_ipc_client_observer<family, ch_type>
{
public:
	using typename basic_nt_family_alpc_ipc_client_observer<family, ch_type>::char_type;
	using typename basic_nt_family_alpc_ipc_client_observer<family, ch_type>::input_char_type;
	using typename basic_nt_family_alpc_ipc_client_observer<family, ch_type>::output_char_type;
	using typename basic_nt_family_alpc_ipc_client_observer<family, ch_type>::native_handle_type;
	using basic_nt_family_alpc_ipc_client_observer<family, ch_type>::native_handle;

	using native_handle_rmptr_type = ::std::remove_pointer_t<native_handle_type>;
	using tls_native_handle_rmptr_type_alloc = ::fast_io::native_typed_thread_local_allocator<native_handle_rmptr_type>;

	inline explicit constexpr basic_nt_family_alpc_ipc_client() noexcept = default;

	inline constexpr basic_nt_family_alpc_ipc_client(basic_nt_family_alpc_ipc_client_observer<family, ch_type>) noexcept = delete;
	inline constexpr basic_nt_family_alpc_ipc_client &operator=(basic_nt_family_alpc_ipc_client_observer<family, ch_type>) noexcept = delete;

	inline basic_nt_family_alpc_ipc_client(basic_nt_family_alpc_ipc_client const &) = delete;
	inline basic_nt_family_alpc_ipc_client &operator=(basic_nt_family_alpc_ipc_client const &) = delete;

	inline basic_nt_family_alpc_ipc_client(basic_nt_family_alpc_ipc_client &&__restrict b) noexcept
		: basic_nt_family_alpc_ipc_client_observer<family, ch_type>{b.release()}
	{
	}
	inline basic_nt_family_alpc_ipc_client &operator=(basic_nt_family_alpc_ipc_client &&__restrict b) noexcept
	{
		if (__builtin_addressof(b) == this) [[unlikely]]
		{
			return *this;
		}
		if (*this) [[likely]]
		{
			this->handle->close_noexcept();
		}
		this->handle = b.handle;
		b.handle = nullptr;
		return *this;
	}
	inline void reset(native_handle_type newhandle = {}) noexcept
	{
		if (*this) [[likely]]
		{
			this->handle->close_noexcept();
		}
		this->handle = newhandle;
	}
	inline void close()
	{
		if (*this) [[likely]]
		{
			this->handle->close();
			tls_native_handle_rmptr_type_alloc::deallocate_n(this->handle, 1);
			this->handle = nullptr; // POSIX standard says we should never call close(2) again even close syscall fails
		}
	}

	template <typename native_hd>
		requires ::std::same_as<native_handle_type, ::std::remove_cvref_t<native_hd>>
	inline explicit constexpr basic_nt_family_alpc_ipc_client(native_hd handle1) noexcept
		: basic_nt_family_alpc_ipc_client_observer<family, ch_type>{handle1}
	{}

	template <::fast_io::constructible_to_os_c_str T>
	inline explicit basic_nt_family_alpc_ipc_client(T const &client_name, ipc_mode im)
	{
		this->handle = tls_native_handle_rmptr_type_alloc::allocate_zero(1);
		this->handle->message_attribute = ::fast_io::win32::nt::details::nt_family_create_alpc_ipc_client_message_attribute_view_impl<family>();
		this->handle->port_handle = ::fast_io::win32::nt::details::nt_connect_alpc_ipc_server_impl<family>(client_name, im, nullptr, nullptr, this->handle->message_attribute);
	}

	template <::fast_io::constructible_to_os_c_str T>
	inline explicit basic_nt_family_alpc_ipc_client(T const &client_name, ipc_mode im, ::fast_io::win32::nt::details::nt_alpc_communication_tlc_strvw<ch_type> message)
	{
		auto const str_begin{reinterpret_cast<::std::byte const *>(message.data())};
		auto const str_size{message.size_bytes()};

		this->handle = tls_native_handle_rmptr_type_alloc::allocate_zero(1);
		this->handle->message_attribute = ::fast_io::win32::nt::details::nt_family_create_alpc_ipc_client_message_attribute_view_impl<family>();
		this->handle->port_handle = ::fast_io::win32::nt::details::nt_connect_alpc_ipc_server_impl<family>(client_name, im, str_begin, str_begin + str_size, this->handle->message_attribute);
	}

	inline ~basic_nt_family_alpc_ipc_client()
	{
		if (*this) [[likely]]
		{
			this->handle->close_noexcept();
			tls_native_handle_rmptr_type_alloc::deallocate_n(this->handle, 1);
			this->handle = nullptr; // POSIX standard says we should never call close(2) again even close syscall fails
		}
	}
};

// todo struct

template <nt_family server_family, ::std::integral server_ch_type, nt_family client_family = nt_family::nt, ::std::integral client_ch_type = char>
inline basic_nt_family_alpc_ipc_client_observer<client_family, client_ch_type> wait_for_connect_and_recv(
	basic_nt_family_alpc_ipc_server_observer<server_family, server_ch_type> server,
	server_ch_type *received_message_begin, server_ch_type *received_message_end)
{
	win32::details::win32_family_named_pipe_ipc_server_wait_for_connect_impl(server.handle);
	return {reinterpret_cast<void *>(static_cast<::std::ptrdiff_t>(-1))};
}

namespace freestanding
{
template <nt_family fm>
struct is_trivially_copyable_or_relocatable<win32::nt::details::nt_alpc_handle<fm>>
{
	inline static constexpr bool value = true;
};

template <nt_family fm>
struct is_zero_default_constructible<win32::nt::details::nt_alpc_handle<fm>>
{
	inline static constexpr bool value = true;
};

template <nt_family fm, ::std::integral ch_type>
struct is_zero_default_constructible<basic_nt_family_alpc_ipc_universal_observer<fm, ch_type>>
{
	inline static constexpr bool value = true;
};

template <nt_family fm, ::std::integral ch_type>
struct is_trivially_copyable_or_relocatable<basic_nt_family_alpc_ipc_server<fm, ch_type>>
{
	inline static constexpr bool value = true;
};

template <nt_family fm, ::std::integral ch_type>
struct is_zero_default_constructible<basic_nt_family_alpc_ipc_server<fm, ch_type>>
{
	inline static constexpr bool value = true;
};

template <nt_family fm, ::std::integral ch_type>
struct is_trivially_copyable_or_relocatable<basic_nt_family_alpc_ipc_client<fm, ch_type>>
{
	inline static constexpr bool value = true;
};

template <nt_family fm, ::std::integral ch_type>
struct is_zero_default_constructible<basic_nt_family_alpc_ipc_client<fm, ch_type>>
{
	inline static constexpr bool value = true;
};
} // namespace freestanding

} // namespace fast_io
