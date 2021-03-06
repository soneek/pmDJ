#ifndef __SWDFILE_HPP
#define __SWDFILE_HPP
class swdFile;
class swdFileChunk;
class swdChunkEOD;
class swdChunkKGRP;
class swdChunkPCMD;
class swdChunkPRGI;
class swdChunkWAVI;
#include <fstream>
#include <ostream>
#include <stdint.h>
#include <string>
#include <vector>

unsigned readByte(const char*);
unsigned readWord(const char*);
unsigned readDWord(const char*);

class swdFile {
	private:
		std::string	intFilename;
		size_t		pcmdLength;
		size_t		waviLength;
		std::vector< swdFileChunk* >	chunks;
	public:
					 swdFile	(std::ifstream&);
					 swdFile	(const swdFile&);
					~swdFile	();
		swdFile&		operator=	(swdFile);
		friend std::ostream&	operator<<	(std::ostream&,const swdFile&);
		std::string		GetFilename	() const;
		size_t			GetPcmdLength	() const;
		size_t			GetWaviLength	() const;
		int			ChunkCount	() const;
		const swdFileChunk&	operator[]	(int) const;
		const std::vector< swdFileChunk* >& Chunks() const;
};

class swdFileChunk {
	public:
		typedef enum {	UNKNOWN_CHUNK,
				CHUNK_EOD,
				CHUNK_KGRP,
				CHUNK_PCMD,
				CHUNK_PRGI,
				CHUNK_WAVI
		} ChunkType;
	protected:
		char		label[5];
		off_t		chunkOffset;
		size_t		dataSize;
		char*		dataPtr;
		virtual std::ostream&	AdvancedInfo(std::ostream&) const;
	public:
					 swdFileChunk	(std::ifstream&);
					 swdFileChunk	(const swdFileChunk&);
					~swdFileChunk	();
		friend std::ostream&	operator<<	(std::ostream&,const swdFileChunk&);
		swdFileChunk&		operator=	(swdFileChunk);

		std::string		GetLabel	() const;
		size_t			GetSize		() const;
		const char*		GetDataPtr	() const;
		virtual ChunkType	GetType		() const;
		static ChunkType	GetType		(std::ifstream&);
};

class swdChunkEOD : public swdFileChunk {
	public:
					 swdChunkEOD	(std::ifstream&);
		swdFileChunk::ChunkType	GetType		() const;
};

class swdChunkKGRP : public swdFileChunk {
	public:
					 swdChunkKGRP	(std::ifstream&);
		swdFileChunk::ChunkType	GetType		() const;
};

class swdChunkPCMD : public swdFileChunk {
	public:
					 swdChunkPCMD	(std::ifstream&);
		swdFileChunk::ChunkType	GetType		() const;
};

class swdChunkPRGI : public swdFileChunk {
	public:
					 swdChunkPRGI	(std::ifstream&);
		swdFileChunk::ChunkType	GetType		() const;
};

class swdChunkWAVI : public swdFileChunk {
	public:
		typedef struct {
			int		indexNumber;
			uint16_t	unk_04;
			char		unk_12[0x20-0x12];
			uint16_t	sampleRate;
			char		unk_22[0x40-0x22];
		} Entry;
	private:
		std::vector< Entry > dataEntry;
		void	AddEntry(off_t);
		std::ostream& AdvancedInfo(std::ostream&) const;
	public:
					 swdChunkWAVI	(std::ifstream&);
		swdFileChunk::ChunkType	GetType		() const;
};
#endif
