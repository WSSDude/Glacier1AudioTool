//
// Created by Andrej Redeky.
// Copyright © 2015-2023 Feldarian Softworks. All rights reserved.
// SPDX-License-Identifier: EUPL-1.2
//
// TODO - verify null-terminated flag for std::string

#pragma once

namespace UTF
{
template <typename UTFCharType, bool CaseSensitive, typename UTFCharTypeTraits, typename UTFAllocator>
requires IsUTFCharType<UTFCharType>
class StringWrapper;

template <typename UTFCharType, bool CaseSensitive = true, typename UTFCharTypeTraits = std::char_traits<UTFCharType>>
requires IsUTFCharType<UTFCharType>
class StringViewWrapper
{
public:
  inline static constexpr auto npos{ std::basic_string_view<UTFCharType, UTFCharTypeTraits>::npos };

  constexpr StringViewWrapper() = default;

  template <typename UTFCharTypeInput, typename UTFCharTypeTraitsInput = std::char_traits<UTFCharTypeInput>>
  requires IsSameUTFCharType<UTFCharType, UTFCharTypeInput>
  constexpr StringViewWrapper(const std::basic_string_view<UTFCharTypeInput, UTFCharTypeTraitsInput> other, const bool nullTerminated = false)
    : utfData{ other.empty() ? reinterpret_cast<const UTFCharType *>("") : reinterpret_cast<const UTFCharType *>(other.data()), other.size() }
    , nullTerminated{ nullTerminated }
  {}

  template <typename UTFCharTypeInput, typename UTFCharTypeTraitsInput = std::char_traits<UTFCharTypeInput>, typename UTFAllocatorInput = std::allocator<UTFCharTypeInput>>
  requires IsSameUTFCharType<UTFCharType, UTFCharTypeInput>
  constexpr StringViewWrapper(const std::basic_string<UTFCharTypeInput, UTFCharTypeTraitsInput, UTFAllocatorInput> &other)
    : StringViewWrapper{ std::basic_string_view<UTFCharTypeInput, UTFCharTypeTraitsInput>(other), true }
  {}

  template <typename UTFCharTypeInput, size_t UTFSizeInput>
  requires IsSameUTFCharType<UTFCharType, UTFCharTypeInput>
  constexpr StringViewWrapper(const UTFCharTypeInput (&other)[UTFSizeInput])
    : StringViewWrapper{ std::basic_string_view<UTFCharTypeInput>(other, other[UTFSizeInput - 1] == UTFCharTypeInput(0) ? UTFSizeInput - 1 : UTFSizeInput), other[UTFSizeInput - 1] == UTFCharTypeInput(0) }
  {}

  template <typename UTFCharTypeInput>
  requires IsSameUTFCharType<UTFCharType, UTFCharTypeInput>
  constexpr StringViewWrapper(const UTFCharTypeInput *other, const size_t length)
    : StringViewWrapper{ std::basic_string_view<UTFCharTypeInput>(other, (length - 1 < length) && other[length - 1] == UTFCharTypeInput(0) ? length - 1 : length), (length - 1 < length) && other[length - 1] == UTFCharTypeInput(0) }
  {}

  constexpr StringViewWrapper(const std::filesystem::path &other)
    requires IsSameUTFCharType<UTFCharType, wchar_t>
    : StringViewWrapper{ other.native() }
  {}

  template <typename UTFCharTypeInput, bool CaseSensitiveInput = CaseSensitive, typename UTFCharTypeTraitsInput = std::char_traits<UTFCharTypeInput>, typename UTFAllocatorInput = std::allocator<UTFCharTypeInput>>
  requires IsSameUTFCharType<UTFCharType, UTFCharTypeInput>
  constexpr StringViewWrapper(const StringWrapper<UTFCharTypeInput, CaseSensitiveInput, UTFCharTypeTraitsInput, UTFAllocatorInput>& other);

  template <typename UTFCharTypeInput, bool CaseSensitiveInput = CaseSensitive, typename UTFCharTypeTraitsInput = std::char_traits<UTFCharTypeInput>>
  constexpr StringViewWrapper(const StringViewWrapper<UTFCharTypeInput, CaseSensitiveInput, UTFCharTypeTraitsInput> other)
    requires IsSameUTFCharType<UTFCharType, UTFCharTypeInput>
    : StringViewWrapper{ other.native(), other.IsNullTerminated() }
  {}

  constexpr StringViewWrapper(const StringViewWrapper& other)
    : utfData{ other.utfData }
    , nullTerminated{ other.nullTerminated }
  {}

  constexpr StringViewWrapper(StringViewWrapper&& other) noexcept
    : utfData{ std::move(other.utfData) }
    , nullTerminated{ other.nullTerminated }
  {}

  StringViewWrapper& operator=(const StringViewWrapper& other)
  {
    if ((static_cast<const void *>(data()) == static_cast<const void *>(other.data())) && (size() == other.size()))
      return *this;

    utfData = other.utfData;
    nullTerminated = other.nullTerminated;

    return *this;
  }

  StringViewWrapper& operator=(StringViewWrapper&& other) noexcept
  {
    if ((static_cast<const void *>(data()) == static_cast<const void *>(other.data())) && (size() == other.size()))
      return *this;

    utfData = std::move(other.utfData);
    nullTerminated = other.nullTerminated;

    return *this;
  }

  [[nodiscard]] constexpr UTFCharType& operator[](const size_t index)
  {
    return utfData[index];
  }

  [[nodiscard]] constexpr UTFCharType operator[](const size_t index) const
  {
    return utfData[index];
  }

  [[nodiscard]] constexpr auto begin() const
  {
    return utfData.cbegin();
  }

  [[nodiscard]] constexpr auto cbegin() const
  {
    return utfData.cbegin();
  }

  [[nodiscard]] constexpr auto end() const
  {
    return utfData.cend();
  }

  [[nodiscard]] constexpr auto cend() const
  {
    return utfData.cend();
  }

  [[nodiscard]] constexpr const UTFCharType* data() const
  {
    return utfData.data();
  }

  [[nodiscard]] constexpr const UTFCharType* c_str() const
  {
    if (nullTerminated)
    return utfData.data();

    utfDataSource = std::make_unique<std::basic_string<UTFCharType>>();
    utfDataSource->assign(utfData.data(), utfData.size());
    utfData = *utfDataSource;
    nullTerminated = true;

    return utfData.data();
  }

  [[nodiscard]] constexpr const UTFCharType& front() const
  {
    return utfData.front();
  }

  [[nodiscard]] constexpr const UTFCharType& back() const
  {
    return utfData.back();
  }

  [[nodiscard]] constexpr bool empty() const
  {
    return utfData.empty();
  }

  [[nodiscard]] constexpr size_t size() const
  {
    return utfData.size();
  }

  [[nodiscard]] constexpr size_t length() const
  {
    return utfData.length();
  }

  [[nodiscard]] constexpr auto& native()
  {
    return utfData;
  }

  [[nodiscard]] constexpr const auto& native() const
  {
    return utfData;
  }

  [[nodiscard]] constexpr std::filesystem::path path() const;

  [[nodiscard]] constexpr bool IsNullTerminated() const
  {
    return nullTerminated;
  }

private:
  mutable std::basic_string_view<UTFCharType, UTFCharTypeTraits> utfData;
  mutable std::unique_ptr<std::basic_string<UTFCharType, UTFCharTypeTraits>> utfDataSource;
  mutable bool nullTerminated = false;
};

template <typename ...Args>
concept StringView8Constructible = std::constructible_from<StringViewWrapper<char>, Args...>;

template <typename ...Args>
concept StringView16Constructible = std::constructible_from<StringViewWrapper<UChar>, Args...>;

template <typename ...Args>
concept StringView32Constructible = std::constructible_from<StringViewWrapper<UChar32>, Args...>;

template <typename ...Args>
concept StringViewNativeConstructible = std::constructible_from<StringViewWrapper<char>, Args...>
                                    || std::constructible_from<StringViewWrapper<wchar_t>, Args...>;

template <typename ...Args>
concept StringViewConstructible = StringView8Constructible<Args...>
                                    || StringView16Constructible<Args...>
                                    || StringView32Constructible<Args...>;

using StringView8 = StringViewWrapper<char, true>;
using StringView16 = StringViewWrapper<UChar, true>;
using StringView32 = StringViewWrapper<UChar32, true>;

using StringViewW = StringViewWrapper<wchar_t, true>;

using StringView8CI = StringViewWrapper<char, false>;
using StringView16CI = StringViewWrapper<UChar, false>;
using StringView32CI = StringViewWrapper<UChar32, false>;

using StringViewWCI = StringViewWrapper<wchar_t, false>;

}
