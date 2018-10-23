#pragma once
#include <sstream>
#include <fnd/IFile.h>
#include <fnd/SharedPtr.h>
#include <fnd/Vec.h>
#include <fnd/List.h>
#include <fnd/LayeredIntegrityMetadata.h>

namespace fnd
{
	class LayeredIntegrityWrappedIFile : public fnd::IFile
	{
	public:
		LayeredIntegrityWrappedIFile(const fnd::SharedPtr<fnd::IFile>& file, const LayeredIntegrityMetadata& hdr);

		size_t size();
		void seek(size_t offset);
		void read(byte_t* out, size_t len);
		void read(byte_t* out, size_t offset, size_t len);
		void write(const byte_t* out, size_t len);
		void write(const byte_t* out, size_t offset, size_t len);
	private:
		const std::string kModuleName = "LayeredIntegrityWrappedIFile";
		static const size_t kDefaultCacheSize = 0x10000;
		std::stringstream mErrorSs;

		fnd::SharedPtr<fnd::IFile> mFile;

		// data file
		fnd::SharedPtr<fnd::IFile> mData;
		size_t mDataOffset;
		size_t mDataBlockSize;
		fnd::List<fnd::sha::sSha256Hash> mDataHashLayer;
		bool mAlignHashCalcToBlock;

		fnd::Vec<byte_t> mCache;
		size_t mCacheBlockNum;

		inline size_t getOffsetBlock(size_t offset) const { return offset / mDataBlockSize; }
		inline size_t getOffsetInBlock(size_t offset) const { return offset % mDataBlockSize; }
		inline size_t getRemanderBlockReadSize(size_t total_size) const { return total_size % mDataBlockSize; }
		inline size_t getBlockNum(size_t total_size) const { return (total_size / mDataBlockSize) + (getRemanderBlockReadSize(total_size) > 0); }

		void initialiseDataLayer(const LayeredIntegrityMetadata& hdr);
		void readData(size_t block_offset, size_t block_num);
	};
}