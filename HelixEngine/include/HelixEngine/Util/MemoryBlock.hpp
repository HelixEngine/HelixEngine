#pragma once
#include <fstream>
#include <functional>
#include <HelixEngine/Util/Object.hpp>
#include <HelixEngine/Util/Logger.hpp>

namespace helix
{
	/**
	 * @brief 内存块
	 */
	class MemoryBlock final : public Object
	{
	public:
		using Allocator = std::function<void*(size_t)>;

		explicit MemoryBlock(Allocator allocator = [](size_t size) { return malloc(size); }) :
			allocator(std::move(allocator))
		{
		}

		/**
		 * @brief 以内存块形式管理内存，默认不释放内存
		 * @param memory 内存指针
		 * @param size 内存大小
		 * @param isRelease 是否自动释放内存，默认不释放
		 */
		MemoryBlock(void* memory, size_t size, bool isRelease = false) :
			ptr(static_cast<std::byte*>(memory)), size(size), isAutoRelease(isRelease)
		{
		}

		MemoryBlock(const MemoryBlock&) = delete;
		MemoryBlock(MemoryBlock&& block) = default;

		MemoryBlock& operator=(const MemoryBlock&) = delete;
		MemoryBlock& operator=(MemoryBlock&&) = default;

		~MemoryBlock() override
		{
			if (isAutoRelease)
				free();
		}

		/**
		 * @brief 复制内存
		 * @param src 要复制的数据
		 * @param size 要复制的大小
		 */
		void copy(const void* src, size_t size) const
		{
			memcpy(ptr, src, size);
		}

		/**
		 * @brief 复制内存
		 * @note 复制的数据大小以当前内存块大小为准
		 * @param src 要复制的数据
		 */
		void copy(const void* src) const
		{
			memcpy(ptr, src, size);
		}

		/**
		 * @brief 复制内存块
		 * @param block 要复制的内存块
		 */
		void copy(const MemoryBlock& block) const
		{
			copy(block.getPointer(), block.getSize());
		}

		void copy(std::ifstream& file) const
		{
			file.read(static_cast<char*>(ptr), static_cast<std::streamsize>(size));
		}

		/**
		 * @brief 重新分配内存块大小
		 * @param size 要分配的字节数
		 */
		void allocate(size_t size)
		{
			free();
			ptr = malloc(size);
			size = size;
		}

		void allocate(const std::string& path)
		{
			struct stat info{};
			if (stat(path.c_str(), &info))
			{
				Logger::error(u8"内存块读取文件信息错误！");
				return;
			}

			std::ifstream file;
			file.open(path, std::ios::binary | std::ios::in);

			if (!file.is_open())
			{
				Logger::error(u8"内存块打开文件错误！");
				return;
			}

			allocate(static_cast<size_t>(info.st_size));
			copy(file);
			file.close();
		}

		void allocate(const void* src, size_t size)
		{
			allocate(size);
			copy(src, size);
		}

		void allocate(const MemoryBlock& block)
		{
			allocate(block.getSize());
			copy(block);
		}

		void setAllocator(Allocator allocator)
		{
			this->allocator = std::move(allocator);
		}

		/**
		 * @brief 将内存块移动到新地方
		 * @param dst 目标指针
		 */
		void move(void* dst)
		{
			memcpy(dst, ptr, size);
			free();
		}

		/**
		 * @brief 重置，但不释放内存
		 * @return 原来内存块的指针
		 */
		void* reset()
		{
			void* result = ptr;
			ptr = nullptr;
			size = 0;
			return result;
		}

		/**
		 * @brief 重置，不释放内存，并管理新内存，新内存默认不释放
		 * @param pointer 内存指针
		 * @param size 内存大小
		 * @param autoRelease 是否自动释放内存，默认不释放
		 * @return 原来内存块的指针
		 */
		void* reset(void* pointer, size_t size, bool autoRelease = false)
		{
			void* result = ptr;
			ptr = nullptr;
			this->size = 0;
			isAutoRelease = autoRelease;
			return result;
		}

		/**
		 * @brief 释放内存块
		 */
		void free()
		{
			if (ptr)
			{
				::free(ptr);
				ptr = nullptr;
			}
		}

		/**
		 * @brief 设置是否自动释放
		 * @param autoRelease 是否自动释放
		 */
		void setAutoRelease(bool autoRelease)
		{
			isAutoRelease = autoRelease;
		}

		/**
		 * @brief 获取内存块大小
		 * @return 内存块的大小，字节数
		 */
		[[nodiscard]] size_t getSize() const
		{
			return size;
		}

		/**
		 * @brief 获取指向内存的指针
		 * @tparam ToType 内存指针要转换成的类型
		 * @return 指向内存块的指针
		 */
		template<typename ToType>
		ToType getPointer() const
		{
			return reinterpret_cast<ToType>(ptr);
		}

		/**
		 * @brief 获取指向内存的指针
		 * @return 指向内存块的指针
		 */
		[[nodiscard]] void* getPointer() const
		{
			return ptr;
		}
	private:
		void* ptr = nullptr;
		size_t size = 0;
		bool isAutoRelease = true;
		Allocator allocator;
	};
}
